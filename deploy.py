#!/usr/bin/env python3

# Copyright (C) 2021 Adrian Carpenter, et al
#
# This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
#
# An open-source cross-platform traceroute analyser.
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

import argparse
import codecs
import datetime
import glob
import logging
import os
import platform
import re
import shutil
import string
import sys
import tempfile
import time

from pingnoo_support_python.common import *
from pingnoo_support_python.makedeb import debCreate
from pingnoo_support_python.makerpm import rpm_create
from pingnoo_support_python.msg_printer import msg_printer, MsgPrinterException

if sys.hexversion < 0x030600f0:
    raise RuntimeError('requires python >= 3.6')

if sys.hexversion < 0x030800f0:
    # Older python versions not liking our checkboxes and color
    sys.stdout = codecs.getwriter("utf-8")(sys.stdout.detach())


def notarize_file(filetonotarize, username, password):
    # TODO: Move to new execute
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


def find_qt():
    """ Common code between Linux and Darwin """
    with msg_printer('Checking qtdir...'):
        if platform.system() == "Windows":
            raise MsgPrinterException("Windows shouldn't have called find_qt()!?!")
        if args.qtdir and os.path.isfile(args.qtdir + '/bin/qmake'):
            qtdir = args.qtdir
        else:
            qtdir = None

        if qtdir is None:
            qmake = which('qmake')
            if qmake:
                qtdir = parent(os.path.normpath(os.path.dirname(qmake)))
            else:
                qtdir = None

        if (qtdir is None) or (not os.path.isdir(qtdir)):
            raise MsgPrinterException('qt directory could not be found. (see --qtdir).')
    return qtdir


# application entry point

parser = argparse.ArgumentParser(description='Pingnoo deployment tool')

parser.add_argument('--qtdir', type=str, nargs='?', help='path to qt')
parser.add_argument('--curlbin', type=str, nargs='?', help='path to curl binary')

if platform.system() == "Darwin":
    parser.add_argument('--arch',
                        choices=['x86_64', 'arm64', 'universal'],
                        type=str,
                        default='x86_64',
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

    parser.add_argument('--deb',
                        action='store_true',
                        help='generate deb package')

    parser.add_argument('--rpm',
                        action='store_true',
                        help='generate rpm package')

    parser.add_argument('--appimage',
                        action='store_true',
                        help='generate AppImage package')

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

    parser.add_argument('--pin',
                        type=str,
                        nargs='?',
                        default='',
                        help='pin when using scsigntool')

    parser.add_argument('--portable',
                        action='store_true',
                        help='create portable zip')

if platform.system() == "Darwin":
    parser.add_argument('--appleid', type=str, nargs='?', help='apple id to use for notarization')
    parser.add_argument('--password', type=str, nargs='?', help='password for apple id')

parser.add_argument('--version', type=str, nargs='?', help='version string', required=True)
parser.add_argument('--debugoutput', type=str, nargs='?', help='debugoutput string', required=False)

args = parser.parse_args()

build_arch = args.arch
build_type = args.type.capitalize()
build_version = args.version


def _do_darwin():
    """ MacOS / Darwin version """

    def mac_sign_binary(filetosign, cert):
        # TODO: Move to new execute
        return execute(f'codesign --verify --timestamp -o runtime --force --sign "{cert}" "{filetosign}"')

    # check for qt installation
    qtdir = find_qt()

    # remove previous deployment files and copy current binaries
    with msg_printer('Setting up deployment directory...'):
        rm_path('deployment')
        os.makedirs('deployment')
        rm_path(f'bin/{build_arch}/Deploy')
        os.makedirs(f'bin/{build_arch}/Deploy')

    if not os.path.isfile('tools/macdeployqtfix/macdeployqtfix.py'):
        rm_path('tools/macdeployqtfix')

        with msg_printer('Cloning macdeployqtfix...'):
            execute('cd tools; git clone https://github.com/nedrysoft/macdeployqtfix.git',
                    fail_msg='unable to clone macdeployqtfix.')

    if not os.path.isfile('tools/create-dmg/create-dmg'):
        rm_path('tools/create-dmg')

        with msg_printer('Cloning create-dmg...'):
            execute('cd tools;git clone https://github.com/andreyvit/create-dmg.git',
                    fail_msg='unable to clone create-dmg.')

    if not build_arch == "universal":
        shutil.copytree(f'bin/{build_arch}/{build_type}/Pingnoo.app',
                        f'bin/{build_arch}/Deploy/Pingnoo.app', symlinks=True)
    else:
        if not os.path.isfile('tools/makeuniversal/makeuniversal'):
            rm_path('tools/makeuniversal')

            with msg_printer('Cloning makeuniversal...'):
                execute('cd tools;git clone https://github.com/nedrysoft/makeuniversal.git',
                        fail_msg='unable to clone makeuniversal.')

            with msg_printer('Building makeuniversal...'):
                execute(f'cd tools/makeuniversal;{qtdir}/bin/qmake;make', fail_msg='error building makeuniversal.')

        with msg_printer('Running makeuniversal...'):

            execute(f'tools/makeuniversal/makeuniversal bin/universal/Deploy/Pingnoo.app '
                    f'bin/x86_64/{build_type}/Pingnoo.app bin/arm64/{build_type}/Pingnoo.app',
                    fail_msg='error building makeuniversal.')

    # run standard qt deployment tool
    with msg_printer('Running macdeployqt...'):
        execute(f'{qtdir}/bin/macdeployqt bin/{build_arch}/Deploy/Pingnoo.app -no-strip',
                fail_msg='there was a problem running macdeployqt.')

    # remove the sql drivers that we don't use
    with msg_printer('Removing unwanted qt plugins...'):
        rm_file(f'bin/{build_arch}/Deploy/Pingnoo.app/Contents/PlugIns/sqldrivers/libqsqlodbc.dylib')
        rm_file(f'bin/{build_arch}/Deploy/Pingnoo.app/Contents/PlugIns/sqldrivers/libqsqlpsql.dylib')

    # run fixed qt deployment tool
    if platform.system() == "Darwin":
        with msg_printer('Running macdeployqtfix...'):
            sys.path.insert(1, 'tools/macdeployqtfix')

            import macdeployqtfix as fixdeploy

            fixdeploy.GlobalConfig.qtpath = os.path.normpath(f'{qtdir}/bin')
            fixdeploy.GlobalConfig.exepath = f'bin/{build_arch}/Deploy/Pingnoo.app'
            fixdeploy.GlobalConfig.logger = logging.getLogger()
            fixdeploy.GlobalConfig.logger.addHandler(logging.NullHandler())

            if not fixdeploy.fix_main_binaries():
                raise MsgPrinterException('there was a problem running macdeployqtfix.')

        # sign the application
        with msg_printer('Signing binaries...'):
            for file in glob.glob(f'bin/{build_arch}/Deploy/Pingnoo.app/**/*.framework', recursive=True):
                result_code, result_output = mac_sign_binary(file, args.cert)
                if result_code:
                    raise MsgPrinterException(f'there was a problem signing a file ({file}).\r\n\r\n{result_output}\r\n')

            for file in glob.glob(f'bin/{build_arch}/Deploy/Pingnoo.app/**/*.dylib', recursive=True):
                result_code, result_output = mac_sign_binary(file, args.cert)
                if result_code:
                    raise MsgPrinterException(f'there was a problem signing a file ({file}).\r\n\r\n{result_output}\r\n')

            result_code, result_output = mac_sign_binary(f'bin/{build_arch}/Deploy/Pingnoo.app', args.cert)

            if result_code:
                raise MsgPrinterException(f'there was a problem signing a file (bin/{build_arch}/Deploy/Pingnoo.app).'
                                          f'\r\n\r\n{result_output}\r\n')

    # package the application into a zip file and notarize the application
    with msg_printer('Creating zip archive...'):
        execute(f'ditto '
                f'-ck '
                f'--sequesterRsrc '
                f'--keepParent bin/{build_arch}/Deploy/Pingnoo.app '
                f'bin/{build_arch}/Deploy/Pingnoo.zip', fail_msg='there was a problem generating the application zip.')

    with msg_printer('Performing notarization of application binary...'):
        status = notarize_file(f'bin/{build_arch}/Deploy/Pingnoo.zip', args.appleid, args.password)

        if not status == "success":
            raise MsgPrinterException(f'there was a problem notarizing the application ({status}).')

    with msg_printer('Stapling notarization ticket to binary...'):
        execute(f'xcrun stapler staple bin/{build_arch}/Deploy/Pingnoo.app',
                'there was a problem stapling the ticket to application.')

    with msg_printer('Creating installation dmg...'):
        execute('tiffutil '
                '-cat '
                'artwork/background.tiff artwork/background@2x.tiff '
                '-out '
                'artwork/pingnoo_background.tiff',
                fail_msg='there was a problem creating the combined tiff.')
        execute(f'tools/create-dmg/create-dmg '
                f'--volname "Pingnoo" '
                f'--background ./artwork/pingnoo_background.tiff '
                f'--window-size 768 534 '
                f'--icon-size 160 '
                f'--icon Pingnoo.app 199 276 -'
                f'-app-drop-link 569 276 '
                f'./bin/{build_arch}/Deploy/Pingnoo.dmg '
                f'bin/{build_arch}/Deploy/Pingnoo.app',
                fail_msg='there was a problem creating the dmg.')

    # sign the dmg and notarize it
    with msg_printer('Signing dmg...'):
        result_code, result_output = mac_sign_binary(f'./bin/{build_arch}/Deploy/Pingnoo.dmg', args.cert)

        if result_code:
            raise MsgPrinterException(f'there was a problem signing the dmg.\r\n\r\n{result_output}\r\n')

    with msg_printer('Performing notarization of installation dmg...'):
        status = notarize_file(f'bin/{build_arch}/Deploy/Pingnoo.dmg', args.appleid, args.password)

        if not status == "success":
            raise MsgPrinterException(f'there was a problem notarizing the dmg ({status}).')

    with msg_printer('Stapling notarization ticket to dmg...'):
        execute(f'xcrun stapler staple bin/{build_arch}/Deploy/Pingnoo.dmg',
                fail_msg='there was a problem stapling the ticket to dmg.')

    with msg_printer('Copying dmg to deployment directory...'):
        build_filename = f'deployment/Pingnoo.{build_version}.{build_arch}.dmg'
        shutil.copy2(f'bin/{build_arch}/Deploy/Pingnoo.dmg', build_filename)

    print(f'\r\n' + Style.BRIGHT + Fore.CYAN + f'Disk Image at \"deployment/{build_filename}\" is ' +
          Fore.GREEN + 'ready' + Fore.CYAN + ' for distribution.', flush=True)


def _do_linux():
    """ Linux version """
    if not any([args.appimage, args.deb, args.rpm]):
        print('You must select at least one type of output: appimage, rpm, deb')
        sys.exit(1)

    with msg_printer('Checking for curl...'):
        if args.curlbin and os.path.isfile(args.curlbin):
            curl = args.curlbin
        else:
            curl = which('curl')

        if not curl:
            raise MsgPrinterException('curl could not be found. (see --curlbin).')

    if args.appimage or args.deb:
        # RPM building has its own Qt macros
        qtdir = find_qt()

    deployed_message = ""

    linux_deploy_qt = args.linuxdeployqt

    if args.appimage:
        # here we check if we need linuxdeploy qt for the deployment, if so we check if it's been supplied or whether
        # we need to download it.  This if can be expanded to any other builds which require linuxdeployqt
        if not linux_deploy_qt or not os.path.isfile(linux_deploy_qt):
            with msg_printer('Downloading linuxdeployqt...'):
                if os.path.exists('tools/linuxdeployqt'):
                    rm_path('tools/linuxdeployqt')

                os.mkdir('tools/linuxdeployqt')

                execute('cd tools/linuxdeployqt; '
                        'curl -LJO '
                        'https://github.com/probonopd/linuxdeployqt/releases/download/6/'
                        'linuxdeployqt-6-x86_64.AppImage',
                        fail_msg='unable to download linuxdeployqt.')

                execute('chmod +x tools/linuxdeployqt/linuxdeployqt-6-x86_64.AppImage',
                        fail_msg='unable to set permissions on linuxdeployqt.')

            linux_deploy_qt = 'tools/linuxdeployqt/linuxdeployqt-6-x86_64.AppImage'

        if not os.path.isfile(linux_deploy_qt):
            bad_msg("> No valid linuxdeployqt could be found.")

        _, result_output = execute('ldd --version')

        ldd_regex = re.compile(r"^ldd\s\(.*\)\s(?P<version>.*)$", re.MULTILINE)

        match_result = ldd_regex.match(result_output)

        if not match_result:
            bad_msg("> Skipping AppImage deployment, unable to get glibc version.")
        else:
            glibc_version = float(match_result.group("version"))

            if glibc_version > 2.23:
                args.appimage = False  # TODO: Set a flag and return errorlevel != 0?
                bad_msg("> Skipping AppImage deployment, glibc is too new..")

    if args.appimage:
        appimage_tool = args.appimagetool

        if not appimage_tool or not os.path.isfile(appimage_tool):
            with msg_printer('Downloading appimagetool...'):
                if not os.path.exists('tools/appimagetool'):
                    rm_path('tools/appimagetool')

                os.mkdir('tools/appimagetool')

                execute('cd tools/appimagetool; '
                        'curl -LJO '
                        'https://github.com/AppImage/AppImageKit/releases/download/continuous/'
                        'appimagetool-x86_64.AppImage', fail_msg='unable to download appimagetool.')

                execute('chmod +x tools/appimagetool/appimagetool-x86_64.AppImage',
                        fail_msg='unable to set permissions on appimagetool.')
                appimage_tool = 'tools/appimagetool/appimagetool-x86_64.AppImage'

        if not os.path.isfile(appimage_tool):
            bad_msg("> No valid appimagetool could be found.")

        # remove previous deployment files and copy current binaries
        with msg_printer('Setting up deployment directory...'):
            rm_path(f'bin/{build_arch}/Deploy/AppImage/')
            rm_path('deployment')

            os.makedirs('deployment')

            os.makedirs(f'bin/{build_arch}/Deploy/AppImage/usr/bin')
            os.makedirs(f'bin/{build_arch}/Deploy/AppImage/usr/lib')
            os.makedirs(f'bin/{build_arch}/Deploy/AppImage/usr/share/icons/hicolor/128x128/apps')
            os.makedirs(f'bin/{build_arch}/Deploy/AppImage/usr/share/applications')

            shutil.copy2(f'bin/{build_arch}/{build_type}/Pingnoo',
                         f'bin/{build_arch}/Deploy/AppImage/usr/bin')
            shutil.copy2('installer/Pingnoo.png',
                         f'bin/{build_arch}/Deploy/AppImage/usr/share/icons/hicolor/128x128/apps')
            shutil.copy2('installer/Pingnoo.desktop',
                         f'bin/{build_arch}/Deploy/AppImage/usr/share/applications')
            shutil.copy2('installer/AppRun', f'bin/{build_arch}/Deploy/AppImage/')
            shutil.copytree(f'bin/{build_arch}/{build_type}/Components',
                            f'bin/{build_arch}/Deploy/AppImage/Components', symlinks=True)

            for file in glob.glob(f'bin/{build_arch}/{build_type}/*.so'):
                shutil.copy2(file, f'bin/{build_arch}/Deploy/AppImage/usr/lib')

        # create the app dir
        with msg_printer('Running linuxdeployqt...'):
            execute(f'{linux_deploy_qt} '
                    f'\'bin/{build_arch}/Deploy/AppImage/usr/share/applications/Pingnoo.desktop\' '
                    f'-qmake=\'{qtdir}/bin/qmake\' '
                    f'-bundle-non-qt-libs '
                    f'-exclude-libs=\'libqsqlodbc,libqsqlpsql\'',
                    fail_msg='there was a problem running linuxdeployqt.')

        # create the AppImage
        sign_parameters = ''

        if args.cert:
            sign_parameters = f'-s --sign-key={args.cert} '

        with msg_printer('Creating AppImage...'):
            build_filename = f'Pingnoo.{build_version}.{build_arch}.AppImage'

            execute(f'{appimage_tool} -g {sign_parameters} '
                    f'bin/{build_arch}/Deploy/AppImage \"deployment/{build_filename}\"',
                    fail_msg='there was a problem creating the AppImage.')

            deployed_message += '\r\n' + Style.BRIGHT + Fore.CYAN + \
                              f'AppImage at \"deployment/{build_filename}\" is ' + Fore.GREEN + 'ready' + \
                              Fore.CYAN + ' for distribution.'

    if args.deb:
        write_msg('> Creating deb package...')
        deb_arch = "all"

        if args.arch == 'x86_64':
            deb_arch = "amd64"

        deb_version = build_version.replace('/', '.')

        issue_parts = open('/etc/issue').readline().lower().strip().split(' ')

        distro = issue_parts[0]

        if distro == "ubuntu":
            release_parts = issue_parts[1].split('.')

            major = release_parts[0]
            minor = release_parts[1]

            deb_distro = f'{distro}{major}.{minor}'
        elif distro == "debian":
            release = issue_parts[2]

            deb_distro = f'{distro}{release}'
        else:
            deb_distro = 'unknown'

        version_parts = deb_version.split('-', 1)
        build_filename = f'deployment/pingnoo_{deb_version}-{deb_distro}_{deb_arch}.deb'

        if len(version_parts) == 2:
            deb_version = version_parts[0][2:]

        try:
            if debCreate(build_arch, build_type, deb_version, build_filename, args.cert):
                raise RuntimeError("deb creation unknown error")

            deployed_message += '\r\n' + Style.BRIGHT + Fore.CYAN + \
                            f'deb package at \"{build_filename}\" is ' + Fore.GREEN + 'ready' + Fore.CYAN + \
                            ' for distribution.'
        except Exception as err:
            print(f"Failed building deb: {type(err).__name__}: {err}")

    if args.rpm:
        write_msg('> Creating rpm package...')

        rpm_version = build_version.replace('/', '.')
        rpm_release = 1  # TODO: CLI argument

        rpm_name = rpm_create(build_arch, build_type, rpm_version, rpm_release, args.cert)

        build_filename = f'bin/{build_arch}/Deploy/rpm/{rpm_name}'
        deployed_message += '\r\n' + Style.BRIGHT + Fore.CYAN + \
                          f'rpm package at \"{build_filename}\" is ' + Fore.GREEN + 'ready' + Fore.CYAN + \
                          ' for distribution.'

    print(deployed_message, flush=True)


def _do_windows():
    """ Windows version """

    def win_sign_binary(signingtool, filetosign, cert, timeserver, pin=None):
        # TODO: Move to new execute
        # FIXME: cert parameter was unused?
        if pin:
            pin = "-pin " + pin

        if args.debugoutput:
            print(f'sign command {signingtool} {pin} sign /fd sha256 /t {timeserver} /n {cert} {filetosign}')

        return execute(f'{signingtool} {pin} sign /fd sha256 /t {timeserver} /n {cert} {filetosign}')

    with msg_printer('Checking for curl...'):
        if args.curlbin and os.path.isfile(args.curlbin):
            curl = args.curlbin
        else:
            curl = which('curl.exe')
        if not curl:
            raise MsgPrinterException('curl could not be found. (see --curlbin).')

    # check for qt installation
    with msg_printer('Checking qtdir...'):
        if args.qtdir and os.path.isfile(f'{args.qtdir}\\bin\\windeployqt.exe'):
            windeployqt = f'{args.qtdir}\\bin\\windeployqt.exe'
        else:
            windeployqt = which('windeployqt')

        if not windeployqt:
            raise MsgPrinterException('qt could not be found. (see --qtdir).')

    pin_code = None

    if args.pin:
        pin_code = args.pin
    else:
        if os.environ.get('PINGNOO_CERTIFICATE_PIN'):
            pin_code = os.environ.get('PINGNOO_CERTIFICATE_PIN')

    cert = None

    if args.cert:
        cert = args.cert;
    else:
        if os.environ.get('PINGNOO_DEVELOPER_CERTIFICATE'):
            cert = os.environ.get('PINGNOO_DEVELOPER_CERTIFICATE')

    tempdir = os.path.normpath(tempfile.mkdtemp())
    signtool = args.signtool

    if cert:
        if not os.path.exists(signtool):
            with msg_printer('Downloading SmartCardTools...'):
                execute(f'cd \"{tempdir}\" && \"{curl}\"'
                        f' -LJO https://www.mgtek.com/files/smartcardtools.zip',
                        fail_msg='unable to download SmartCardTools.')
                execute(f'cd \"{tempdir}\" && \"{curl}\"'
                        f' -LJO ftp://ftp.info-zip.org/pub/infozip/win32/unz600xn.exe'
                        f' & unz600xn -jo unzip.exe',
                        fail_msg='unable to download info-zip tools.')
                execute(f'\"{tempdir}\\unzip\" \"{tempdir}'
                        f'\\smartcardtools.zip\" -d tools\\smartcardtools',
                        fail_msg='unable to unzip SmartCardTools.')
                signtool = 'tools\\smartcardtools\\x64\\ScSignTool.exe'

    # remove previous deployment files and copy current binaries
    with msg_printer('Setting up deployment directory...'):
        deploy_dir = f'bin\\{build_arch}\\Deploy'
        binary_dir = f'bin\\{build_arch}\\{build_type}'
        extensions = ['.exe', '.dll']

        rm_path('deployment')
        rm_path(deploy_dir)

        sign_list = []
        os.makedirs(deploy_dir)
        os.makedirs('deployment')

        # TODO: Refactor with os.walk
        for file in glob.glob(f'{binary_dir}\\**\\*', recursive=True):
            _, extension = os.path.splitext(file)

            if os.path.isdir(file):
                os.makedirs(file.replace(binary_dir, deploy_dir, 1))

            if extension in extensions:
                dest_file = file.replace(binary_dir, deploy_dir, 1)
                shutil.copy2(file, dest_file)

                sign_list.append(dest_file)

        files = []

        for extension in extensions:
            files += glob.glob(f'{deploy_dir}\\*{extension}')

        if not files:
            raise MsgPrinterException('no files could be found to deploy.')

    # sign the application binaries
    if cert:
        with msg_printer('Signing binaries...'):
            for file in sign_list:
                result_code, result_output = win_sign_binary(signtool, file, cert, args.timeserver, pin_code)

                if result_code:
                    raise MsgPrinterException(f'there was a problem signing a file ({file}).\r\n\r\n{result_output}\r\n')

    files_string = ' '.join(files)

    # run windeplotqt
    with msg_printer('Running windeployqt...'):
        execute(f'{windeployqt} --dir {deploy_dir} {files_string} -sql --{args.type}',
                fail_msg='there was a problem running windeployqt.')

    # use powershell to create a portable zip
    if args.portable:
        with msg_printer('Creating portable edition...'):
            execute(f'powershell Compress-Archive "bin\\{build_arch}\\Deploy\\*" ".\\deployment\\Pingnoo.Portable.{build_version}.{build_arch}.zip" -Force',
                    fail_msg='there was a problem creating the portable archive.')

    # run advanced installer
    with msg_printer('Creating installer...'):
        rm_file('installer\\PingnooBuild.aip')

        # use python templating to set the pin in the aip file as it can't lookup an environment variable directly
        with open("installer\\Pingnoo.aip", 'r') as installer_file:
            installer_template = string.Template(installer_file.read())

        installer_file_content = installer_template.substitute(pinCode=f'{pin_code}')
        with open('installer\\PingnooBuild.aip', 'w') as out_installer_file:
            out_installer_file.write(installer_file_content)

        build_parts = args.version.split('-', 1)

        if len(build_parts) != 2:
            win_build_version = "0.0.0"
        else:
            win_build_version = build_parts[0][2:]

        build_filename = f'Pingnoo.{build_version}.{build_arch}.exe'

        execute(f'AdvancedInstaller.com /edit installer\\PingnooBuild.aip /SetVersion {win_build_version}',
                fail_msg='there was a problem creating the installer.')
        execute(
            f'AdvancedInstaller.com /edit installer\\PingnooBuild.aip /SetPackageName "{build_filename}" -buildname MsiBuild',
            fail_msg='there was a problem creating the installer.')
        execute(f'AdvancedInstaller.com /build installer\\PingnooBuild.aip',
                fail_msg='there was a problem creating the installer.')

    if args.cert:
        with msg_printer('Signing installer...'):
            result_code, result_output = win_sign_binary(signtool, f'deployment\\{build_filename}', args.cert,
                                                         args.timeserver,
                                                         args.pin)

            if result_code:
                raise MsgPrinterException(f'there was a problem signing the installer.\r\n\r\n{result_output}\r\n')
    print(f'\r\n' + Style.BRIGHT + Fore.CYAN + f'Installer at \"deployment\\{build_filename}\" is ' +
          Fore.GREEN + 'ready' + Fore.CYAN + ' for distribution.', flush=True)


print(Style.BRIGHT + 'Deployment process started at ' + str(datetime.datetime.now()) + '\r\n', flush=True)
start_time = time.time()
if platform.system() == "Windows":
    _do_windows()
elif platform.system() == "Linux":
    _do_linux()
elif platform.system() == "Darwin":
    _do_darwin()
else:
    raise RuntimeError("Unknown OS!")
end_time = time.time()

print(Style.BRIGHT + f'\r\nTotal time taken to perform deployment was ' +
      timedelta(end_time - start_time) + '.', flush=True)

sys.exit(0)
