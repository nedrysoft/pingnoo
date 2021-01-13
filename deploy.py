#!/usr/bin/env python3

# Copyright (C) 2019 Adrian Carpenter
#
# This file is part of Pingnoo (https://github.com/fizzyade/pingnoo)
#
# An open-source cross-platform traceroute analyser.
#
# Created by Adrian Carpenter on 27/03/2020.
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

if platform.python_version_tuple() < ('3', '6', '0'):
    print('requires python >= 3.6', flush=True)
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


def timedelta(seconds):
    seconds = abs(int(seconds))
    
    days, seconds = divmod(seconds, 86400)
    hours, seconds = divmod(seconds, 3600)
    minutes, seconds = divmod(seconds, 60)

    if days > 0:
        return f'{days}d{hours}h{minutes}m{seconds}s'
    elif hours > 0:
        return f'{hours}h{minutes}m{seconds}s'
    elif minutes > 0:
        return f'{minutes}m{seconds}s'
    else:
        return f'{seconds}s'


def startmessage(message):
    sys.stdout.write(Style.BRIGHT+f'> {message} ')
    sys.stdout.flush()


def endmessage(state, message=None):
    if state:
        if platform.system() == "Windows":
            sys.stdout.write(Style.BRIGHT+'['+Fore.GREEN+'Y'+Fore.RESET+']\r\n')
        else:
            sys.stdout.write(Style.BRIGHT+'['+Fore.GREEN+'✓'+Fore.RESET+']\r\n')
    else:
        if platform.system() == "Windows":
            sys.stdout.write(Style.BRIGHT+'['+Fore.RED+'N'+Fore.RESET+']\r\n')
        else:
            sys.stdout.write(Style.BRIGHT+'['+Fore.RED+'✘'+Fore.RESET+']\r\n')

    if not state and message:
        print('\r\n'+Fore.RED+'ERROR: '+Fore.RESET+message, flush=True)

    sys.stdout.flush()


def parent(path):
    return os.path.normpath(os.path.join(path, os.pardir))


def execute(command):
    output = subprocess.run(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    return output.returncode, output.stdout.decode('utf-8')+output.stderr.decode('utf-8')


def which(appname):
    if platform.system() == "Windows":
        command = 'where'
    else:
        command = 'which'

    whichstatusresult, output = execute(f'{command} {appname}')

    if whichstatusresult and output:
        return output.split()[0]


def run(command):
    stream = os.popen(command)

    return stream.read()


def macsignbinary(filetosign, cert):
    return execute(f'codesign --verify --timestamp -o runtime --force --sign "{cert}" "{filetosign}"')


def winsignbinary(signingtool, filetosign, cert, timeserver):
    return execute(f'{signingtool} sign /n "{cert}" /t {timeserver} /fd sha256 /v "{filetosign}"')


def notarizefile(filetonotarize, username, password):
    uuidpattern = re.compile(r'RequestUUID\s=\s(?P<requestUUID>[a-f|0-9]{8}-[a-f|0-9]{4}-[a-f|0-9]{4}-[a-f|0-9]{4}-['
                             r'a-f|0-9]{12})\n')
    statuspattern = re.compile(r'\s*Status:\s(?P<status>\w+)\n')

    uploadid = str(int(time.time()))

    resultcode, result = execute(f'xcrun altool --notarize-app --primary-bundle-id "com.nedrysoft.pingnoo.{uploadid}" '
                                 f'-u {username} --password {password} --file {filetonotarize}')

    if resultcode:
        return False

    match = uuidpattern.search(result)

    if not match:
        return False

    requestid = match.groupdict()["requestUUID"]

    if not requestid:
        return False

    while True:
        resultcode, result = execute(f'xcrun altool --notarization-info {requestid} -u {username}'
                                     f' --password {password}')

        if not resultcode:
            match = statuspattern.search(result)

            if match:
                currentstatus = match.groupdict()["status"]

                if currentstatus == "in progress":
                    time.sleep(10)
                    continue

                if currentstatus == "success":
                    break

                if currentstatus == "invalid":
                    break

    return currentstatus


# application entry point

parser = argparse.ArgumentParser(description='Pingnoo deployment tool')

parser.add_argument('--qtdir', type=str, nargs='?', help='path to qt')
parser.add_argument('--curlbin', type=str, nargs='?', help='path to curl binary')

if platform.system() == "Darwin":
    parser.add_argument('--arch',
                        choices=['x86_64', 'arm64', 'universal'],
                        type=str,
                        default='x64_64',
                        nargs='?',
                        help='architecture type to deploy')
elif platform.system() == "Linux":
    parser.add_argument('--arch',
                        choices=['x86', 'x86_64'],
                        type=str,
                        default='x86_64',
                        nargs='?',
                        help='architecture type to deploy')
else:
    parser.add_argument('--arch',
                        choices=['x86', 'x86_64'],
                        type=str,
                        default='x86_64',
                        nargs='?',
                        help='architecture type to deploy')

parser.add_argument('--type',
                    choices=['release', 'debug'],
                    default='release',
                    type=str,
                    nargs='?',
                    help='type of build to deploy')

parser.add_argument('--cert', type=str, nargs='?', help='certificate id to sign with')

if platform.system() == "Linux":
    parser.add_argument('--linuxdeployqt',
                        type=str,
                        default='tools/linuxdeployqt/linuxdeployqt-6-x86_64.AppImage',
                        nargs='?',
                        help='path to linuxdeployqt')

    parser.add_argument('--appimagetool',
                        type=str,
                        default='tools/appimagetool/appimagetool-x86_64.AppImage',
                        nargs='?',
                        help='path to appimagetool')

if platform.system() == "Windows":
    parser.add_argument('--timeserver',
                        type=str,
                        default='http://time.certum.pl/',
                        nargs='?',
                        help='time server to use for signing')

    parser.add_argument('--signtool',
                        type=str,
                        nargs='?',
                        default='tools\\smartcardtools\\x64\\ScSignTool.exe',
                        help='path to signing binary')

if platform.system() == "Darwin":
    parser.add_argument('--appleid', type=str, nargs='?', help='apple id to use for notarization')
    parser.add_argument('--password', type=str, nargs='?', help='password for apple id')

parser.add_argument('--version', type=str, nargs='?', help='version string')

args = parser.parse_args()

buildArch = args.arch
buildType = args.type.capitalize()
buildVersion = args.version

if platform.system() == "Windows":
    print(Style.BRIGHT+'Deployment process started at '+str(datetime.datetime.now())+'\r\n', flush=True)

    startTime = time.time()

    # check curl is available

    startmessage('Checking for curl...')

    if args.curlbin:
        if os.path.isfile(args.curlbin):
            curl = args.curlbin
        else:
            curl = None
    else:
        curl = which('curl.exe')

    if not curl:
        endmessage(False, 'curl could not be found. (see --curlbin).')
        exit(1)

    endmessage(True)

    # check for qt installation

    startmessage('Checking qtdir...')

    if args.qtdir:
        if os.path.isfile(f'{args.qtdir}\\bin\\windeployqt.exe'):
            windeployqt = f'{args.qtdir}\\bin\\windeployqt.exe'
        else:
            windeployqt = None
    else:
        windeployqt = which('windeployqt')

    if not windeployqt:
        endmessage(False, 'qt could not be found. (see --qtdir).')
        exit(1)

    endmessage(True)

    tempdir = os.path.normpath(tempfile.mkdtemp())

    signtool = args.signtool

    if args.cert:
        if not os.path.exists(signtool):
            startmessage('Downloading SmartCardTools...')

            resultCode, resultOutput = execute(f'cd \"{tempdir}\" && \"{curl}\"'
                                               f' -LJO https://www.mgtek.com/files/smartcardtools.zip')

            if resultCode:
                endmessage(False, f'unable to download SmartCardTools.\r\n\r\n{resultOutput}\r\n')
                exit(1)

            resultCode, resultOutput = execute(f'cd \"{tempdir}\" && \"{curl}\"'
                                               f' -LJO ftp://ftp.info-zip.org/pub/infozip/win32/unz600xn.exe'
                                               f' & unz600xn -jo unzip.exe')

            if resultCode:
                endmessage(False, f'unable to download info-zip tools.\r\n\r\n{resultOutput}\r\n')
                exit(1)

            resultCode, resultOutput = execute(f'\"{tempdir}\\unzip\" \"{tempdir}'
                                               f'\\smartcardtools.zip\" -d tools\\smartcardtools')

            if resultCode:
                endmessage(False, f'unable to unzip SmartCardTools.\r\n\r\n{resultOutput}\r\n')
                exit(1)

            signtool = 'tools\\smartcardtools\\x64\\ScSignTool.exe'

            endmessage(True)

    # remove previous deployment files and copy current binaries

    startmessage('Setting up deployment directory...')

    deployDir = f'bin\\{buildArch}\\Deploy'
    binaryDir = f'bin\\{buildArch}\\{buildType}'

    extensions = ['.exe', '.dll']

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
        endmessage(False, 'no files could be found to deploy.')
        exit(1)

    endmessage(True)

    # sign the application binaries

    if args.cert:
        startmessage('Signing binaries...')

        for file in signList:
            resultCode, resultOutput = winsignbinary(signtool, file, args.cert, args.timeserver)

            if resultCode:
                endmessage(False, f'there was a problem signing a file ({file}).\r\n\r\n{resultOutput}\r\n')
                exit(1)

        endmessage(True)

    filesString = ''

    for file in files:
        filesString += f' {file}'

    # run windeplotqt

    startmessage('Running windeployqt...')

    resultCode, resultOutput = execute(f'{windeployqt} --dir {deployDir} {filesString}')

    if resultCode:
        endmessage(False, f'there was a problem running windeployqt.\r\n\r\n{resultOutput}\r\n')
        exit(1)

    endmessage(True)

    # run advanced installer

    startmessage('Creating installer...')

    if os.path.exists('installer\\PingnooBuild.aip'):
        os.remove('installer\\PingnooBuild.aip')

    shutil.copy2('installer\\Pingnoo.aip', 'installer\\PingnooBuild.aip')

    buildParts = args.version.split('-', 1)

    if len(buildParts) != 2:
        winBuildVersion = "0.0.0"
    else:
        winBuildVersion = buildParts[0][2:]

    buildFilename = f'Pingnoo Setup [{buildVersion}] ({buildArch}).exe'

    resultCode, resultOutput = execute(f'AdvancedInstaller.com /edit installer\\PingnooBuild.aip'
                                       f' /SetVersion {winBuildVersion}')

    if resultCode:
        endmessage(False, f'there was a problem creating the installer.\r\n\r\n{resultOutput}\r\n')
        exit(1)

    resultCode, resultOutput = execute(f'AdvancedInstaller.com /edit installer\\PingnooBuild.aip'
                                       f' /SetPackageName "{buildFilename}" -buildname MsiBuild')

    if resultCode:
        endmessage(False, f'there was a problem creating the installer.\r\n\r\n{resultOutput}\r\n')
        exit(1)

    resultCode, resultOutput = execute(f'AdvancedInstaller.com /build installer\\PingnooBuild.aip')

    if resultCode:
        endmessage(False, f'there was a problem creating the installer.\r\n\r\n{resultOutput}\r\n')
        exit(1)

    endmessage(True)

    # sign the installer file

    if args.cert:
        startmessage('Signing installer...')

        resultCode, resultOutput = winsignbinary(signtool, f'deployment\\{buildFilename}', args.cert, args.timeserver)

        if resultCode:
            endmessage(False, f'there was a problem signing the installer.\r\n\r\n{resultOutput}\r\n')
            exit(1)

        endmessage(True)

    endTime = time.time()

    # done!

    print(f'\r\n'+Style.BRIGHT+Fore.CYAN+f'Finished! Installer at \"deployment\\{buildFilename}\" is ' +
          Fore.GREEN+'ready'+Fore.CYAN+' for distribution.', flush=True)

    print(Style.BRIGHT + f'\r\nTotal time taken to perform deployment was ' +
          timedelta(endTime - startTime) + '.', flush=True)

    exit(0)

if platform.system() == "Linux":
    print(Style.BRIGHT+'Deployment process started at '+str(datetime.datetime.now())+'\r\n', flush=True)

    startTime = time.time()

    # check curl is available

    startmessage('Checking for curl...')

    if args.curlbin:
        if os.path.isfile(args.curlbin):
            curl = args.curlbin
        else:
            curl = None
    else:
        curl = which('curl')

    if not curl:
        endmessage(False, 'curl could not be found. (see --curlbin).')
        exit(1)

    endmessage(True)

    # check for qt installation

    startmessage('Checking qtdir...')

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
        endmessage(False, 'qt directory could not be found. (see --qtdir).')
        exit(1)
    else:
        if not os.path.isdir(qtdir):
            endmessage(False, 'qt directory could not be found. (see --qtdir).')
            exit(1)

    endmessage(True)

    # download linuxdeployqt

    linuxdeployqt = args.linuxdeployqt

    if not os.path.isfile(linuxdeployqt):
        startmessage('Downloading linuxdeployqt...')

        if not os.path.exists('tools/linuxdeployqt'):
            os.mkdir('tools/linuxdeployqt')

        resultCode, resultOutput = execute('cd tools/linuxdeployqt; '
                                           'curl -LJO '
                                           'https://github.com/probonopd/linuxdeployqt/releases/download/6/'
                                           'linuxdeployqt-6-x86_64.AppImage')

        if resultCode:
            endmessage(False, f'unable to download linuxdeployqt.\r\n\r\n{resultOutput}\r\n')
            exit(1)

        resultCode, resultOutput = execute('chmod +x tools/linuxdeployqt/linuxdeployqt-6-x86_64.AppImage')

        if resultCode:
            endmessage(False, f'unable to set permissions on linuxdeployqt.\r\n\r\n{resultOutput}\r\n')
            exit(1)

        linuxdeployqt = 'tools/linuxdeployqt/linuxdeployqt-6-x86_64.AppImage'

        endmessage(True)

    appimagetool = args.appimagetool

    if not os.path.isfile(appimagetool):
        startmessage('Downloading appimagetool...')

        if not os.path.exists('tools/appimagetool'):
            os.mkdir('tools/appimagetool')

        resultCode, resultOutput = execute('cd tools/appimagetool; '
                                           'curl -LJO '
                                           'https://github.com/AppImage/AppImageKit/releases/download/continuous/'
                                           'appimagetool-x86_64.AppImage')

        if resultCode:
            endmessage(False, f'unable to download appimagetool.\r\n\r\n{resultOutput}\r\n')
            exit(1)

        resultCode, resultOutput = execute('chmod +x tools/appimagetool/appimagetool-x86_64.AppImage')

        if resultCode:
            endmessage(False, f'unable to set permissions on appimagetool.\r\n\r\n{resultOutput}\r\n')
            exit(1)

        appimagetool = 'tools/appimagetool/appimagetool-x86_64.AppImage'

        endmessage(True)

    # remove previous deployment files and copy current binaries

    startmessage('Setting up deployment directory...')

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

    for file in glob.glob(f'bin/{buildArch}/{buildType}/*.so'):
        shutil.copy2(file, f'bin/{buildArch}/Deploy/usr/lib')

    endmessage(True)

    # create the app dir

    startmessage('Running linuxdeployqt...')

    resultCode, resultOutput = execute(f'{linuxdeployqt} '
                                       f'\'bin/{buildArch}/Deploy/usr/share/applications/Pingnoo.desktop\' '
                                       f'-qmake=\'{qtdir}/bin/qmake\' '
                                       f'-bundle-non-qt-libs '
                                       f'-exclude-libs=\'libqsqlodbc,libqsqlpsql\'')

    if resultCode:
        endmessage(False, f'there was a problem running linuxdeployqt.\r\n\r\n{resultCode}\r\n{resultOutput}\r\n')
        exit(1)

    endmessage(True)

    # create the AppImage

    signParameters = ''

    if args.cert:
        signParameters = f'-s --sign-key={args.cert} '

    startmessage('Creating AppImage...')

    buildFilename = f'Pingnoo [{buildVersion}] (x86_64).AppImage'

    resultCode, resultOutput = execute(f'{appimagetool} -g {signParameters} '
                                       f'bin/{buildArch}/Deploy \"deployment/{buildFilename}\"')

    if resultCode:
        endmessage(False, f'there was a problem creating the AppImage.\r\n\r\n{resultCode}\r\n')
        exit(1)   

    endmessage(True)

    endTime = time.time()

    # done!

    print(f'\r\n' + Style.BRIGHT + Fore.CYAN + f'Finished! AppImage at \"deployment/{buildFilename}\" is '
          + Fore.GREEN+'ready' + Fore.CYAN + ' for distribution.', flush=True)

    print(Style.BRIGHT + f'\r\nTotal time taken to perform deployment was ' +
          timedelta(endTime - startTime) + '.', flush=True)

    exit(0)

if platform.system() == "Darwin":
    print(Style.BRIGHT+'Deployment process started at '+str(datetime.datetime.now())+'\r\n', flush=True)

    startTime = time.time()

    # check for qt installation

    startmessage('Checking qtdir...')

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
        endmessage(False, 'qt directory could not be found. (see --qtdir).')
        exit(1)
    else:
        if not os.path.isdir(qtdir):
            endmessage(False, 'qt directory could not be found. (see --qtdir).')
            exit(1)

    endmessage(True)

    # remove previous deployment files and copy current binaries

    startmessage('Setting up deployment directory...')

    if os.path.exists(f'deployment'):
        shutil.rmtree(f'deployment')
    
    os.makedirs(f'deployment')

    if os.path.exists(f'bin/{buildArch}/Deploy'):
        shutil.rmtree(f'bin/{buildArch}/Deploy')
    
    os.makedirs(f'bin/{buildArch}/Deploy')

    endmessage(True)

    if not os.path.isfile('tools/macdeployqtfix/macdeployqtfix.py'):
        if os.path.exists('tools/macdeployqtfix'):
            shutil.rmtree(f'tools/macdeployqtfix')

        startmessage('Cloning macdeployqtfix...')

        resultCode, resultOutput = execute('cd tools;git clone https://github.com/fizzyade/macdeployqtfix.git')

        if resultCode:
            endmessage(False, f'unable to clone macdeployqtfix.\r\n\r\n{resultOutput}\r\n')
            exit(1)

        endmessage(True)

    if not os.path.isfile('tools/create-dmg/create-dmg'):
        if os.path.exists('tools/create-dmg'):
            shutil.rmtree(f'tools/create-dmg')

        startmessage('Cloning create-dmg...')

        resultCode, resultOutput = execute('cd tools;git clone https://github.com/andreyvit/create-dmg.git')

        if resultCode:
            endmessage(False, f'unable to clone create-dmg.\r\n\r\n{resultOutput}\r\n')
            exit(1)

        endmessage(True)

    if not buildArch == "universal":
        shutil.copytree(f'bin/{buildArch}/{buildType}/Pingnoo.app',
                        f'bin/{buildArch}/Deploy/Pingnoo.app', symlinks=True)
    else:
        if not os.path.isfile('tools/makeuniversal/makeuniversal'):
            if os.path.exists('tools/makeuniversal'):
                shutil.rmtree(f'tools/makeuniversal')

            startmessage('Cloning makeuniversal...')

            resultCode, resultOutput = execute('cd tools;git clone https://github.com/fizzyade/makeuniversal.git')

            if resultCode:
                endmessage(False, f'unable to clone makeuniversal.\r\n\r\n{resultOutput}\r\n')
                exit(1)

            endmessage(True)

            startmessage('Building makeuniversal...')

            resultCode, resultOutput = execute(f'cd tools/makeuniversal;{qtdir}/bin/qmake;make')

            if resultCode:
                endmessage(False, f'error building makeuniversal.\r\n\r\n{resultOutput}\r\n')
                exit(1)

            endmessage(True)

        startmessage('Running makeuniversal...')

        resultCode, resultOutput = execute(f'tools/makeuniversal/makeuniversal bin/universal/Deploy/Pingnoo.app '
                                           f'bin/x86_64/{buildType}/Pingnoo.app bin/arm64/{buildType}/Pingnoo.app')

        if resultCode:
            endmessage(False, f'error building makeuniversal.\r\n\r\n{resultOutput}\r\n')
            exit(1)

        endmessage(True)

    # run standard qt deployment tool

    startmessage('Running macdeployqt...')

    resultCode, resultOutput = execute(f'{qtdir}/bin/macdeployqt bin/{buildArch}/Deploy/Pingnoo.app -no-strip')

    if resultCode:
        endmessage(False, f'there was a problem running macdeployqt.\r\n\r\n{resultOutput}\r\n')
        exit(1)

    endmessage(True)

    # remove the sql drivers that we don't use

    startmessage('Removing unwanted qt plugins...')

    if os.path.isfile(f'bin/{buildArch}/Deploy/Pingnoo.app/Contents/PlugIns/sqldrivers/libqsqlodbc.dylib'):
        os.remove(f'bin/{buildArch}/Deploy/Pingnoo.app/Contents/PlugIns/sqldrivers/libqsqlodbc.dylib')

    if os.path.isfile(f'bin/{buildArch}/Deploy/Pingnoo.app/Contents/PlugIns/sqldrivers/libqsqlpsql.dylib'):
        os.remove(f'bin/{buildArch}/Deploy/Pingnoo.app/Contents/PlugIns/sqldrivers/libqsqlpsql.dylib')

    endmessage(True)

    # run fixed qt deployment tool

    startmessage('Running macdeployqtfix...')

    sys.path.insert(1, 'tools/macdeployqtfix')

    import macdeployqtfix as fixdeploy

    fixdeploy.GlobalConfig.qtpath = os.path.normpath(f'{qtdir}/bin')
    fixdeploy.GlobalConfig.exepath = f'bin/{buildArch}/Deploy/Pingnoo.app'
    fixdeploy.GlobalConfig.logger = logging.getLogger()
    fixdeploy.GlobalConfig.logger.addHandler(logging.NullHandler())

    if not fixdeploy.fix_main_binaries():
        endmessage(False, 'there was a problem running macdeployqtfix.')
        exit(1)

    endmessage(True)

    # sign the application

    startmessage('Signing binaries...')

    for file in glob.glob(f'bin/{buildArch}/Deploy/Pingnoo.app/**/*.framework', recursive=True):
        resultCode, resultOutput = macsignbinary(file, args.cert)
        if resultCode:
            endmessage(False, f'there was a problem signing a file ({file}).\r\n\r\n{resultOutput}\r\n')
            exit(1)

    for file in glob.glob(f'bin/{buildArch}/Deploy/Pingnoo.app/**/*.dylib', recursive=True):
        resultCode, resultOutput = macsignbinary(file, args.cert)
        if resultCode:
            endmessage(False, f'there was a problem signing a file ({file}).\r\n\r\n{resultOutput}\r\n')
            exit(1)

    resultCode, resultOutput = macsignbinary(f'bin/{buildArch}/Deploy/Pingnoo.app', args.cert)

    if resultCode:
        endmessage(False, f'there was a problem signing a file (bin/{buildArch}/Deploy/Pingnoo.app).'
                          f'\r\n\r\n{resultOutput}\r\n')
        exit(1)

    endmessage(True)

    # package the application into a zip file and notarize the application
    
    startmessage('Creating zip archive...')

    resultCode, resultOutput = execute(f'ditto '
                                       f'-ck '
                                       f'--sequesterRsrc '
                                       f'--keepParent bin/{buildArch}/Deploy/Pingnoo.app '
                                       f'bin/{buildArch}/Deploy/Pingnoo.zip')

    if resultCode:
        endmessage(False, f'there was a problem generating the application zip.\r\n\r\n{resultOutput}\r\n')
        exit(1)

    endmessage(True)

    startmessage('Performing notarization of application binary...')

    status = notarizefile(f'bin/{buildArch}/Deploy/Pingnoo.zip', args.appleid, args.password)

    if not status == "success":
        endmessage(False, f'there was a problem notarizing the application ({status}).')
        exit(1)

    endmessage(True)

    startmessage('Stapling notarization ticket to binary...')

    resultCode, resultOutput = execute(f'xcrun stapler staple bin/{buildArch}/Deploy/Pingnoo.app')

    if resultCode:
        endmessage(False, f'there was a problem stapling the ticket to application.\r\n\r\n{resultOutput}\r\n')
        exit(1)

    endmessage(True)

    # create dmg    

    startmessage('Creating installation dmg...')

    resultCode, resultOutput = execute('tiffutil '
                                       '-cat '
                                       'artwork/background.tiff artwork/background@2x.tiff '
                                       '-out '
                                       'artwork/pingnoo_background.tiff')

    if resultCode:
        endmessage(False, f'there was a problem creating the combined tiff.\r\n\r\n{resultOutput}\r\n')
        exit(1)

    resultCode, resultOutput = execute(f'tools/create-dmg/create-dmg '
                                       f'--volname "Pingnoo" '
                                       f'--background ./artwork/pingnoo_background.tiff '
                                       f'--window-size 768 534 '
                                       f'--icon-size 160 '
                                       f'--icon Pingnoo.app 199 276 -'
                                       f'-app-drop-link 569 276 '
                                       f'./bin/{buildArch}/Deploy/Pingnoo.dmg '
                                       f'bin/{buildArch}/Deploy/Pingnoo.app')

    if resultCode:
        endmessage(False, f'there was a problem creating the dmg.\r\n\r\n{resultOutput}\r\n')
        exit(1)

    endmessage(True)

    # sign the dmg and notarize it

    startmessage('Signing dmg...')

    resultCode, resultOutput = macsignbinary(f'./bin/{buildArch}/Deploy/Pingnoo.dmg', args.cert)

    if resultCode:
        endmessage(False, f'there was a problem signing the dmg.\r\n\r\n{resultOutput}\r\n')
        exit(1)

    endmessage(True)

    startmessage('Performing notarization of installation dmg...')

    status = notarizefile(f'bin/{buildArch}/Deploy/Pingnoo.dmg', args.appleid, args.password)

    if not status == "success":
        endmessage(False, f'there was a problem notarizing the dmg ({status}).')
        exit(1)

    endmessage(True)

    startmessage('Stapling notarization ticket to dmg...')

    resultCode, resultOutput = execute(f'xcrun stapler staple bin/{buildArch}/Deploy/Pingnoo.dmg')

    if resultCode:
        endmessage(False, f'there was a problem stapling the ticket to dmg.\r\n\r\n{resultOutput}\r\n')
        exit(1)

    endmessage(True)

    startmessage('Copying dmg to deployment directory...')

    buildFilename = f'deployment/Pingnoo [{buildVersion}] ({buildArch}).dmg'

    shutil.copy2(f'bin/{buildArch}/Deploy/Pingnoo.dmg', buildFilename)

    endmessage(True)

    endTime = time.time()

    # done!

    print(f'\r\n' + Style.BRIGHT + Fore.CYAN + f'Finished! Disk Image at \"deployment/{buildFilename}\" is ' +
          Fore.GREEN+'ready' + Fore.CYAN+' for distribution.', flush=True)

    print(Style.BRIGHT + f'\r\nTotal time taken to perform deployment was ' +
          timedelta(endTime - startTime) + '.', flush=True)

    exit(0)
