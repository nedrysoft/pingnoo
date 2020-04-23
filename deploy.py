#!/usr/bin/env python3
# Copyright (C) 2019 Adrian Carpenter
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

import platform
import os
import fnmatch
import shutil
import glob
import tempfile
import argparse
import re
import time
import subprocess
import sys
import logging
import datetime
import enum

if platform.python_version_tuple()<('3','6','0'):
    print('requires python >= 3.6')
    quit(1)

try:
    from colorama import Fore, Back, Style, init

    init(autoreset=True)

except ModuleNotFoundError:
    class Style:
        BRIGHT = ''
        RESET = ''

    class Fore:
        GREEN = ''
        RED = ''
        CYAN = ''
        RESET = ''

def timeDelta(seconds):
    seconds = abs(int(seconds))
    
    days, seconds = divmod(seconds, 86400)
    hours, seconds = divmod(seconds, 3600)
    minutes, seconds = divmod(seconds, 60)

    if days > 0:
        return(f'{days}d{hours}h{minutes}m{seconds}s')
    elif hours > 0:
        return(f'{hours}h{minutes}m{seconds}s')
    elif minutes > 0:
        return(f'{minutes}m{seconds}s')
    else:
        return(f'{seconds}s')

def startMessage(message):
    sys.stdout.write(Style.BRIGHT+f'> {message} ')

def endMessage(state, message=None):
    if state:
        if platform.system()=="Windows":
            sys.stdout.write(Style.BRIGHT+'['+Fore.GREEN+'Y'+Fore.RESET+']\r\n')
        else:
            sys.stdout.write(Style.BRIGHT+'['+Fore.GREEN+'✓'+Fore.RESET+']\r\n')
    else:
        if platform.system()=="Windows":
            sys.stdout.write(Style.BRIGHT+'['+Fore.RED+'N'+Fore.RESET+']\r\n')
        else:
            sys.stdout.write(Style.BRIGHT+'['+Fore.RED+'✘'+Fore.RESET+']\r\n')


    if not state and message:
        print('\r\n'+Fore.RED+'ERROR: '+Fore.RESET+message)

def parent(path):
    return(os.path.normpath(os.path.join(path, os.pardir)))

def execute(command):
    output = subprocess.run(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    return(output.returncode, output.stdout.decode('utf-8'))

def which(appname):
    if platform.system()=="Windows" :
        command = 'where'
    else:
        command = 'which'

    status, output = execute(f'{command} {appname}')

    if status and output:
        return(output.split()[0])

def run(command) :
    stream = os.popen(command)

    return stream.read()

def macSignBinary(file, cert):
    return(execute(f'codesign --verify --timestamp -o runtime --force --sign "{cert}" "{file}"')[0])

def winSignBinary(file, cert, timeserver):
    return(execute(f'tools\\smartcardtools\\x64\\scsigntool sign /n "{cert}" /t {timeserver} /fd sha256 /v "{file}"')[0])

def notarizeFile(file, username, password):
    uuidPattern = re.compile(r'RequestUUID\s=\s(?P<requestUUID>[a-f|0-9]{8}-[a-f|0-9]{4}-[a-f|0-9]{4}-[a-f|0-9]{4}-[a-f|0-9]{12})\n')
    statusPattern = re.compile(r'\s*Status:\s(?P<status>\w+)\n')

    filename = os.path.basename(file)

    uploadId = str(int(time.time()))

    resultCode,result = execute(f'xcrun altool --notarize-app --primary-bundle-id "com.fizzyade.pingnoo.{uploadId}" -u {args.appleid} --password {args.password} --file {file}')

    if resultCode:
        return(False)

    match = uuidPattern.search(result)

    if not match:
        return(False)

    requestId = match.groupdict()["requestUUID"]

    if not requestId:
        return(False)

    while True:
        resultCode,result = execute(f'xcrun altool --notarization-info {requestId} -u {username} --password {password}')

        if not resultCode:
            match = statusPattern.search(result)

            if match:
                status = match.groupdict()["status"]

                if status=="in progress":
                    time.sleep(10)
                    continue

                if status=="success":
                    break

                if status=="invalid":
                    break

    return(status)

# application entry point

parser = argparse.ArgumentParser(description='Pingnoo deployment tool')

parser.add_argument('--qtdir', type=str, nargs='?', help='path to qt')
parser.add_argument('--curlbin', type=str, nargs='?', help='path to curl binary')
parser.add_argument('--arch', choices=['x86', 'x64'], type=str, default='x64', nargs='?', help='architecture type to deploy')
parser.add_argument('--type', choices=['release', 'debug'], default='release', type=str, nargs='?', help='type of build to deploy')
parser.add_argument('--cert', type=str, nargs='?', help='certificate id to sign with')

if platform.system()=="Windows":
    parser.add_argument('--timeserver', type=str, default='http://time.certum.pl/', nargs='?', help='time server to use for signing')
    parser.add_argument('--scsigntool', type=str, nargs='?', help='path to scsigntool root')

if platform.system()=="Darwin":
    parser.add_argument('--appleid', type=str, nargs='?', help='apple id to use for notarization')
    parser.add_argument('--password', type=str, nargs='?', help='password for apple id')

args = parser.parse_args()

buildArch = args.arch
buildType = args.type.capitalize()

if platform.system()=="Windows":
    print(Style.BRIGHT+'Deployment process started at '+str(datetime.datetime.now())+'\r\n')

    startTime = time.time()

    # check curl is available

    startMessage('Checking for curl...')

    if args.curlbin:
        if os.path.isfile(args.curlbin):
            curl = args.curlbin
        else:
            curl = None
    else:
        curl = which('curl.exe')

    if not curl:
        endMessage(False, 'curl could not be found. (see --curlbin).')
        exit(1)

    endMessage(True)

    # check for qt installATION

    startMessage('Checking qtdir...')

    if args.qtdir:
        if os.path.isfile(f'{args.qtdir}\\bin\\windeployqt.exe'):
            windeployqt = f'{args.qtdir}\\bin\\windeployqt.exe'
        else:
            windeployqt = None
    else:
        windeployqt = which('windeployqt')

    if not windeployqt:
        endMessage(False, 'qt could not be found. (see --qtdir).')
        exit(1)

    endMessage(True)

    tempdir = os.path.normpath(tempfile.mkdtemp())

    if args.cert:
        if not os.path.exists('tools\\smartcardtools\\x86\\ScSignTool.exe'):
            startMessage('Downloading SmartCardTools...')

            if execute(f'cd \"{tempdir}\" && \"{curl}\" -LJO https://www.mgtek.com/files/smartcardtools.zip')[0]:
                endMessage(False, 'unable to download SmartCardTools.')
                exit(1)

            if execute(f'cd \"{tempdir}\" && \"{curl}\" -LJO ftp://ftp.info-zip.org/pub/infozip/win32/unz600xn.exe & unz600xn -jo unzip.exe')[0]:
                endMessage(False, 'unable to download info-zip tools.')
                exit(1)

            if execute(f'\"{tempdir}\\unzip\" \"{tempdir}\\smartcardtools.zip\" -d tools\\smartcardtools')[0]:
                endMessage(False, 'unable to unzip SmartCardTools.')
                exit(1)

            endMessage(True)

    # remove previous deployment files and copy current binaries

    startMessage('Setting up deployment directory...')

    deployDir = f'bin\\{buildArch}\\Deploy'
    binaryDir = f'bin\\{buildArch}\\{buildType}'

    extensions=['.exe', '.dll']

    if os.path.exists(deployDir):
        shutil.rmtree(deployDir)

    signList = []

    os.makedirs(deployDir)

    for file in glob.glob(f'{binaryDir}\\**\\*', recursive=True):
        basename, extension = os.path.splitext(file)

        if os.path.isdir(file):
            os.makedirs(file.replace(binaryDir, deployDir, 1))

        if extension in extensions:
            destFile = file.replace(binaryDir, deployDir, 1)
            shutil.copy2(file, destFile)

            signList.append(destFile)

    files = []

    for extension in extensions:
        files += glob.glob(f'{deployDir}\\*{extension}')

    if not files:
        endMessage(False, 'no files could be found to deploy.')
        exit(1)

    endMessage(True)

    # sign the application binaries

    if args.cert:
        startMessage('Signing binaries...')

        for file in signList:
            if winSignBinary(file, args.cert, args.timeserver):
                endMessage(False, f'there was a problem signing a file ({file}).')
                exit(1)

        endMessage(True)

    filesString = ''

    for file in files:
        filesString += f' {file}'

    # run windeplotqt

    startMessage('Running windeployqt...')

    if execute(f'{windeployqt} --dir {deployDir} {filesString}')[0]:
        endMessage(False, 'there was a problem running windeployqt.')
        exit(1)

    endMessage(True)    

    # run advanced installer

    startMessage('Creating installer...')

    if execute(f'AdvancedInstaller.com /build installer\\Pingnoo.aip')[0]:
        endMessage(False, 'there was a problem creating the installer.')
        exit(1)

    endMessage(True)  

    # sign the installer file

    if args.cert:
        startMessage('Signing installer...')

        if winSignBinary('deployment\\Pingnoo.exe', args.cert, args.timeserver):
            endMessage(False, f'there was a problem signing the installer.')
            exit(1)

        endMessage(True)

    endTime = time.time()

    # done!

    print(f'\r\n'+Style.BRIGHT+Fore.CYAN+f'Finished! Installer at \"deployment\\Pingnoo.exe\" is '+Fore.GREEN+'ready'+Fore.CYAN+' for distribution.')

    print(Style.BRIGHT+f'\r\nTotal time taken to perform deployment was '+timeDelta(endTime-startTime)+'.')

    exit(0)

if platform.system()=="Linux" :
    print(Style.BRIGHT+'Deployment process started at '+str(datetime.datetime.now())+'\r\n')

    startTime = time.time()

    # check curl is available

    startMessage('Checking for curl...')

    if args.curlbin:
        if os.path.isfile(args.curlbin):
            curl = args.curlbin
        else:
            curl = None
    else:
        curl = which('curl')

    if not curl:
        endMessage(False, 'curl could not be found. (see --curlbin).')
        exit(1)

    endMessage(True)

    # check for qt installation

    startMessage('Checking qtdir...')

    if args.qtdir:
        if os.path.isfile(args.qtdir+'/bin/qmake'):
            qtdir = args.qtdir
        else:
            qtdir = None
    else:
        qmake = which('qmake')

        if qmake:
            qtdir = parent(os.path.normpath(os.path.dirname(qmake)))
        else:
            qtdir = None

    if not qtdir:
        endMessage(False, 'qt directory could not be found. (see --qtdir).')
        exit(1)
    else:
        if not os.path.isdir(qtdir):
            endMessage(False, 'qt directory could not be found. (see --qtdir).')
            exit(1)

    endMessage(True)

    # download linuxdeployqt

    startMessage('Downloading linuxdeployqt...')

    if not os.path.isfile('tools/linuxdeployqt/linuxdeployqt-6-x86_64.AppImage') :
        if not os.path.exists('tools/linuxdeployqt'):
            os.mkdir('tools/linuxdeployqt')

        if execute('cd tools/linuxdeployqt; curl -LJO https://github.com/probonopd/linuxdeployqt/releases/download/6/linuxdeployqt-6-x86_64.AppImage')[0]:
            endMessage(False, 'unable to download linuxdeployqt.')
            exit(1)

        if execute('chmod +x tools/linuxdeployqt/linuxdeployqt-6-x86_64.AppImage')[0]:
            endMessage(False, 'unable to set permissions on linuxdeployqt.')
            exit(1)

    endMessage(True)

    startMessage('Downloading appimagetool...')

    if not os.path.isfile('tools/appimagetool/appimagetool-x86_64.AppImage') :
        if not os.path.exists('tools/appimagetool'):
            os.mkdir('tools/appimagetool')

        if execute('cd tools/appimagetool; curl -LJO https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage')[0]:
            endMessage(False, 'unable to download appimagetool.')
            exit(1)

        if execute('chmod +x tools/appimagetool/appimagetool-x86_64.AppImage')[0]:
            endMessage(False, 'unable to set permissions on appimagetool.')
            exit(1)

    endMessage(True)

    # remove previous deployment files and copy current binaries

    startMessage('Setting up deployment directory...')

    if os.path.exists(f'bin/{buildArch}/Deploy/'):
        shutil.rmtree(f'bin/{buildArch}/Deploy/')

    if os.path.exists(f'deployment'):
        shutil.rmtree(f'deployment')

    os.makedirs(f'deployment')

    os.makedirs(f'bin/{buildArch}/Deploy/usr/bin')
    os.makedirs(f'bin/{buildArch}/Deploy/usr/lib')
    os.makedirs(f'bin/{buildArch}/Deploy/usr/share/icons/hicolor/128x128/apps')
    os.makedirs(f'bin/{buildArch}/Deploy/usr/share/applications')

    shutil.copy2(f'bin/{buildArch}/{buildType}/Pingnoo', f'bin/{buildArch}/Deploy/usr/bin')
    shutil.copy2(f'installer/Pingnoo.png', f'bin/{buildArch}/Deploy/usr/share/icons/hicolor/128x128/apps')
    shutil.copy2(f'installer/Pingnoo.desktop', f'bin/{buildArch}/Deploy/usr/share/applications')
    shutil.copy2(f'installer/AppRun', f'bin/{buildArch}/Deploy/')
    shutil.copytree(f'bin/{buildArch}/{buildType}/Components', f'bin/{buildArch}/Deploy/Components', symlinks=True)

    for file in glob.glob(f'bin/{buildArch}/{buildType}/*.so') :
        shutil.copy2(file, f'bin/{buildArch}/Deploy/usr/lib')

    endMessage(True)

    # create the app dir

    startMessage('Running linuxdeployqt...')

    if execute(f'tools/linuxdeployqt/linuxdeployqt-6-x86_64.AppImage \'bin/{buildArch}/Deploy/usr/share/applications/Pingnoo.desktop\' -qmake=\'{qtdir}/bin/qmake\' -exclude-libs=\'libqsqlodbc,libqsqlpsql\'')[0]:
        endMessage(False, 'there was a problem running linuxdeployqt.')
        exit(1)

    endMessage(True)

    # create the AppImage

    signParameters = ''

    if args.cert:
        signParameters = f'-s --sign-key={args.cert} '

    startMessage('Creating AppImage...')

    if execute(f'cd deployment; ../tools/appimagetool/appimagetool-x86_64.AppImage -g {signParameters}../bin/{buildArch}/Deploy')[0]:
        endMessage(False, 'there was a problem creating the AppImage.')
        exit(1)   

    endMessage(True)

    endTime = time.time()

    # done!

    print(f'\r\n'+Style.BRIGHT+Fore.CYAN+f'Finished! AppImage at \"deployment/Pingnoo-x86_64.AppImage\" is '+Fore.GREEN+'ready'+Fore.CYAN+' for distribution.')

    print(Style.BRIGHT+f'\r\nTotal time taken to perform deployment was '+timeDelta(endTime-startTime)+'.')

    exit(0)

if platform.system()=="Darwin":
    print(Style.BRIGHT+'Deployment process started at '+str(datetime.datetime.now())+'\r\n')

    startTime = time.time()

    # check for qt installation

    startMessage('Checking qtdir...')

    if args.qtdir:
        if os.path.isfile(args.qtdir+'/bin/qmake'):
            qtdir = args.qtdir
        else:
            qtdir = None
    else:
        qmake = which('qmake')

        if qmake:
            qtdir = parent(os.path.normpath(os.path.dirname(qmake)))
        else:
            qtdir = None

    if not qtdir:
        endMessage(False, 'qt directory could not be found. (see --qtdir).')
        exit(1)
    else:
        if not os.path.isdir(qtdir):
            endMessage(False, 'qt directory could not be found. (see --qtdir).')
            exit(1)

    endMessage(True)

    # remove previous deployment files and copy current binaries

    startMessage('Setting up deployment directory...')

    if os.path.exists(f'deployment'):
        shutil.rmtree(f'deployment')
    
    os.makedirs(f'deployment')

    if os.path.exists(f'bin/{buildArch}/Deploy/'):
        shutil.rmtree(f'bin/{buildArch}/Deploy/')
    
    os.makedirs(f'bin/{buildArch}/Deploy')

    shutil.copytree(f'bin/{buildArch}/{buildType}/Pingnoo.app', f'bin/{buildArch}/Deploy/Pingnoo.app', symlinks=True)

    endMessage(True)

    # run standard qt deployment tool

    startMessage('Running macdeployqt...')

    if execute(f'{qtdir}/bin/macdeployqt bin/{buildArch}/Deploy/Pingnoo.app -no-strip')[0]:
        endMessage(False, 'there was a problem running macdeployqt.')
        exit(1)

    endMessage(True)

    # remove the sql drivers that we don't use

    startMessage('Removing unwanted qt plugins...')

    if os.path.isfile(f'bin/{buildArch}/Deploy/Pingnoo.app/Contents/PlugIns/sqldrivers/libqsqlodbc.dylib') :
        os.remove(f'bin/{buildArch}/Deploy/Pingnoo.app/Contents/PlugIns/sqldrivers/libqsqlodbc.dylib')

    if os.path.isfile(f'bin/{buildArch}/Deploy/Pingnoo.app/Contents/PlugIns/sqldrivers/libqsqlpsql.dylib') :
        os.remove(f'bin/{buildArch}/Deploy/Pingnoo.app/Contents/PlugIns/sqldrivers/libqsqlpsql.dylib')

    endMessage(True)

    # run fixed qt deployment tool

    startMessage('Running macdeployqtfix...')

    sys.path.insert(1, 'tools/macdeployqtfix')

    import macdeployqtfix as fixDeploy

    fixDeploy.GlobalConfig.qtpath = os.path.normpath(f'{qtdir}/bin')
    fixDeploy.GlobalConfig.exepath = f'bin/{buildArch}/Deploy/Pingnoo.app'
    fixDeploy.GlobalConfig.logger = logging.getLogger()
    fixDeploy.GlobalConfig.logger.addHandler(logging.NullHandler())

    if not fixDeploy.fix_main_binaries():
        endMessage(False, 'there was a problem running macdeployqtfix.')
        exit(1)

    endMessage(True)

    # sign the application

    startMessage('Signing binaries...')

    for file in glob.glob(f'bin/{buildArch}/Deploy/Pingnoo.app/**/*.framework', recursive=True):
        if macSignBinary(file, args.cert):
            endMessage(False, f'there was a problem signing a file ({file}).')
            exit(1)

    for file in glob.glob(f'bin/{buildArch}/Deploy/Pingnoo.app/**/*.dylib', recursive=True):
        if macSignBinary(file, args.cert):
            endMessage(False, f'there was a problem signing a file ({file}).')
            exit(1)

    if macSignBinary(f'bin/{buildArch}/Deploy/Pingnoo.app', args.cert):
        endMessage(False, f'there was a problem signing a file ({file}).')
        exit(1)

    endMessage(True)

    # package the application into a zip file and notarize the application
    
    startMessage('Creating zip archive...')

    if execute(f'ditto -ck --sequesterRsrc --keepParent bin/{buildArch}/Deploy/Pingnoo.app bin/{buildArch}/Deploy/Pingnoo.zip')[0]:
        endMessage(False, 'there was a problem generating the application zip.')
        exit(1)

    endMessage(True)

    startMessage('Performing notarization of application binary...')

    status = notarizeFile(f'bin/{buildArch}/Deploy/Pingnoo.zip', args.appleid, args.password)

    if not status=="success":
        endMessage(False, f'there was a problem notarizing the application ({status}).')
        exit(1)

    endMessage(True)

    startMessage('Stapling notarization ticket to binary...')

    if execute(f'xcrun stapler staple bin/{buildArch}/Deploy/Pingnoo.app')[0]:
        endMessage(False, f'there was a problem stapling the ticket to application.')
        exit(1)

    endMessage(True)

    # create dmg    

    startMessage('Creating installation dmg...')

    if execute('tiffutil -cat artwork/background.tiff artwork/background@2x.tiff -out artwork/pingnoo_background.tiff')[0]:
        endMessage(False, f'there was a problem creating the combined tiff.')
        exit(1)

    if execute(f'tools/create-dmg/create-dmg --volname "Pingnoo" --background ./artwork/pingnoo_background.tiff --window-size 768 534 --icon-size 160 --icon Pingnoo.app 199 276 --app-drop-link 569 276 ./bin/{buildArch}/Deploy/Pingnoo.dmg bin/{buildArch}/Deploy/Pingnoo.app')[0]:
        endMessage(False, f'there was a problem creating the dmg.')
        exit(1)

    endMessage(True)

    # sign the dmg and notarize it

    startMessage('Signing dmg...')

    if macSignBinary(f'./bin/{buildArch}/Deploy/Pingnoo.dmg', args.cert):
        endMessage(False, f'there was a problem signing the dmg.')
        exit(1)

    endMessage(True)

    startMessage('Performing notarization of installation dmg...')

    status = notarizeFile(f'bin/{buildArch}/Deploy/Pingnoo.dmg', args.appleid, args.password)

    if not status=="success":
        endMessage(False, f'there was a problem notarizing the dmg ({status}).')
        exit(1)

    endMessage(True)

    startMessage('Stapling notarization ticket to dmg...')

    if execute(f'xcrun stapler staple bin/{buildArch}/Deploy/Pingnoo.dmg')[0]:
        endMessage(False, f'there was a problem stapling the ticket to dmg.')
        exit(1)

    endMessage(True)

    startMessage('Copying dmg to deployment directory...')

    shutil.copy2(f'bin/{buildArch}/Deploy/Pingnoo.dmg', 'deployment/Pingnoo.dmg')

    endMessage(True)

    endTime = time.time()

    # done!

    print(f'\r\n'+Style.BRIGHT+Fore.CYAN+f'Finished! Installer at \"deployment/Pingnoo.dmg\" is '+Fore.GREEN+'ready'+Fore.CYAN+' for distribution.')

    print(Style.BRIGHT+f'\r\nTotal time taken to perform deployment was '+timeDelta(endTime-startTime)+'.')

    exit(0)
