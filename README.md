# Pingnoo

/pɪŋ ɡəˈnuː/

---


<a href="https://www.travis-ci.com/github/nedrysoft/pingnoo"><img src="https://img.shields.io/travis/com/nedrysoft/pingnoo"/></a>
<a href="https://packagecloud.io/nedrysoft/pingnoo"><img src="https://img.shields.io/badge/deb-packagecloud.io-844fec.svg"/></a>
<a href="https://packagecloud.io/nedrysoft/pingnoo"><img src="https://img.shields.io/badge/rpm-packagecloud.io-844fec.svg"/></a>
<br/>
Pingnoo is an open-source combined traceroute and ping application for analysing network connections.  It runs under Windows, Mac OS and Linux.

<img src="https://user-images.githubusercontent.com/55795671/103213760-96965c80-4906-11eb-98ee-34658db89b4c.gif"/>

## Installation

[The binaries are available under the releases page.](https://github.com/nedrysoft/pingnoo/releases)

- **Windows**
  
  ***Installer***
  
  Download the installer executable and run to install the application.  This will create a shortcut which is used launch the software.

  ***Portable Edition***

  The portable edition is supplied as a zip file which can be unzipped onto a memory stick, the application can be launched directly from the memory stick without the need to install the software.

- **Mac OS**  
  
  The application is a dmg disk image.  Download and open the disk image and drag the Pingnoo icon into the Applications folder, the application can then be launched by double-clicking on the Pingnoo icon in Applications.

- **Linux**
  
  ***AppImage***
  
  The application is provided as an AppImage, an AppImage bundles all dependencies and therefore will run on any modern Linux distribution.

  After downloading the AppImage, you then need to do the following:

  `chmod +x <downloaded filename>`

  The application requires RAW socket access, and this means some limitations affect the operation of the application.  You will need to follow the instructions outlined in the Linux Notes section regarding setuid; this will allow you to run the application directly.  Alternatively, you can launch from the terminal by using:

  `sudo ./<downloaded filename>`

  ***Native Packages***

  Native packages for popular Linux distributions are available, and these install with the correct permissions, which allow the application to run without root permissions.

    - Fedora 32 & 33
    - Ubuntu 18.04, 20.04, 20.10
    - Debian 10

  **Installing on Debian/Ubuntu**
    
  To install on Ubuntu or Debian, you should download the appropriate deb package for your OS, and then you can install it with the following commands:

  ```
  dpkg -i <downloaded file>.deb
  apt -f install
  ```

  **Installing on Fedora**
  
  To install on Fedora, you should download the appropriate rpm package for your OS, and then you can install it with the following commands:

  ```
  dnf install <downloaded file>.rpm
  ```

## Documentation

Documentation can be found on https://pingnoo.readthedocs.io/en/latest/

## Requirements (Development)

- Qt 5 libraries (Requirement for development)
- CMake for building the application
- Qt Creator (Optional development environment)
- Linux has unique requirements; please read the Linux Notes section.

## Development

Pingnoo uses a modular design architecture based on components (aka plugins); the software can be extended easily by third parties by creating new components or modifying existing ones.

### General Information

The following compiler configurations are known to work for development.

- ***Linux*** - 32 & 64-bit using GNU g++
- ***Windows*** - 32 & 64-bit using MSVC 2019 Community Edition
- ***Windows*** - 32 & 64-bit using mingw-g++
- ***Mac OS*** - 64-bit using clang from the xcode command line tools

### Building

Building the project will require certain libraries and toolchairs to be installed, the instructions below should be used as a guide for setting up a build environment.

The source can be obtained using Git.

```bash
git clone https://github.com/nedrysoft/pingnoo.git
cd pingnoo
git submodule update --init --recursive
```

#### Build dependencies

The [build documentation](docs/build/README.md) contains information about packages that are required to build the software from source.

#### Building the application

Change to the folder that contains the source code, and then execute the following commands.

```bash
mkdir build
cd build
cmake ../
make
```

The software will be configured and the resulting binaries can be found in ../bin/<arch>/<build type>

For example, an x86_64 Release build would be located at ,,/bin/x86_64/Release

Change to that folder and execute ./Pingnoo to run the application.

### Deployment

The deploy.py python script creates runnable/installable binaries for the given operating system.

- ***Linux*** - Generates a stand-alone AppImage for easy use.
- ***Windows*** - Generates an installer for easy deployment.
- ***Mac OS*** - Generates a DMG file for easy deployment.

The script requires Python 3.6 or later, curl and the [colorama](https://github.com/tartley/colorama) python module to enhance the console output. You can install the Colorama module with the following command.

*The CMake script provides an install target which uses the deploy script and will generate the appropriate deployable image for the platform being used.*

`pip3 install colorama`

The script provides the following parameters:

- `--qtdir="<path to qt>"` - the path to the version of qt used.
- `--curlbin="<path to curl binary>"` - the path to the curl binary, i.e `"/usr/bin/curl"`.
- `--arch="<x64|x86>"` - the architecture to deploy
- `--type=<release|debug>` - the build type to deploy
- `--cert="<cert>"` - the value of this is dependent on the platform, for Windows and Mac OS it is the name of the certificate to be used, for Linux it is the gpg2 key identifier.
- `--timeserver="<name>"` - windows only, the name of the time server to be used during signing
- `--appleid="<email>"` - *Mac OS only*, the Apple ID of the developer account used to sign the binary
- `--password="<password>"` - *Mac OS only*, the password of the Apple ID.  The password can be stored in the keychain and accessed using `"@keychain:<identifier>"` instead of passing the actual password to the script.
- `--appimage` - *Linux only*, will generate an AppImage.
- `--rpm` - *Linux only*, will generate an rpm package.
- `--deb` - *Linux only*, will generate a deb package.
- `--appiamagetool="<path to appimage tool"` - *Linux only*, the path to the appimagetool binary.
- `--linuxdeployqy="<path to linuxdeployqt tool"` - *Linux only*, the path to the linuxdeployqt binary.
- `--signtool="<path to sign tool>"` - *Windows only*, the path to the signing tool.
- `--pin=<pin>` - *Windows only*, if using scsigntool, the pin to use.
- `--portable` - *Windows only*, will generate a portable zip file for windows.
- `--version` - the version of the deployment tool.
- `--debugoutput` - produce debug output.

The script will use the curl binary to obtain any tools required for the deployment process.

The script stores the deployable asset in the deployment folder.

### Linux Notes

The PingCommandPingEngine ping engine is an experimental ping engine that uses the linux system ping command, this allows the software to run without any special privileges.

If the ICMPPingEngine is selected, then the application will require RAW socket privileges and therefore must be run as root or given raw socket access using the setcap* command.

The setcap comamnd will not work on the AppImage linux release as the privileges will not propagate down into the AppImage when it is mounted via fuse. 

The CMake configuration for Linux has the following post link command, this command is optional, and you can control this with the option `Pingnoo_SetRawCapabilities`.

`sudo -n /usr/sbin/setcap cap_net_raw,cap_net_admin=eip $${APPLICATION_BINARY_FOLDER}/$${TARGET}`

This command will automatically apply the privileges to the executable after linking by running setcap via sudo, the flag `-n` is required to stop sudo prompting for a password if necessary if a password is needed then sudo will fail.

To stop sudo requesting a password for the `/usr/sbin/setcap` command create the following entry in `/etc/sudoers`(replacing <username> with the name of your user account):

`<username> ALL = (root) NOPASSWD: /usr/sbin/setcap`

Note that in some linux distributions `setcap` is located in `/sbin`.

*Please be aware of any security issues by doing this.*

Also, it's not possible to debug the application due to the requirement of RAW sockets, one way of solving this is to create a script in `/usr/bin/gdb-sudo` with the following content:

`#!/bin/sh`
`sudo -n /usr/bin/gdb $@`

Create an entry in `/etc/sudoers` to allow gdb to run as root without a password:

`<username> ALL = (root) NOPASSWD: /usr/bin/gdb`

The final step is to configure Qt Creator to use this proxy script.   Create a new debugger configuration in Qt Creator selecting `/usr/bin/gdb-sudo` as the gdb binary.  You can choose this script as the active debugger in any Qt kit that you wish to use for debugging.

*Please be aware of any security issues by doing this.*

#### Valgrind

Debugging memory leaks can be done with Valgrind, but as above the application requires raw socket privileges and Valgrind cannot run the executable without being run as root.  The following command can be executed from the application binary folder to run Valgrind and produce an XML debug log.  In QtCreator you can load this log for viewing. (For convenience a `valgrind.sh` script has been provided)

`sudo /usr/bin/valgrind --child-silent-after-fork=yes --xml=yes --smc-check=all-non-file --tool=memcheck --gen-suppressions=all --track-origins=yes --leak-check=summary --num-callers=25 --xml-file=./debug.xml ./Pingnoo_debug`

#### Deleaker

The primary mechanism of finding memory leaks is done with the Deleaker tool, it's currently Windows only, but works incredibly well for finding leaks.

The user interface is simple and allows viewing the stack trace of the leak which is invaluable when figuring out which level of code is responsible for the leak.

During development of modules, leaks often occur as code changes can be fluet, a tool like deleaker is essential for tracking these and it's a time saver not to worry about leaks during the intial coding phase knowing that you can easily and quickly fix them after development of the feature is complete.

#### Unit Tests

Set the `Pingnoo_Build_Tests` option to `ON` to generate a binary which performs unit tests.

#### Ribbon Bar

The user interface uses a Ribbon style toolbar (as seen in applications such as Microsoft Office).  To build the designer plugin set the CMake option `Pingnoo_Build_RibbonDesignerPlugin` to `ON`, this will create the plugin in the `bin` folder which you will then need to copy to the Qt `plugins` folder.

#### Mac OS

The gatekeeper requirements in Mac OS mean that the application needs to be signed and notarised to run without user interaction for allowing the application to run.  You will require a full developer account to obtain a code signing certificate that is valid for gatekeeper authentication.

# Credits

This project was created and maintained by [Adrian Carpenter](https://github.com/fizzyade).

I thank the following people for contributing to the project:

- [Aaron D. Marasco](https://github.com/AaronDMarasco)
- [Paul Varghese](https://github.com/Paulls20)
- [Stepan Michalek](https://github.com/Ste-Mich)
- [Nick Boultonn](https://github.com/boultonn)

Pingnoo uses the following third-party libraries/assets/tools/services in the development of Pingnoo.

- [Qt](https://www.qt.io/download) - cross platform framework, licensed under the [GPLv3 licence](https://www.gnu.org/licenses/gpl-3.0.en.html).
- [cmake](www.cmake.org) - cross platform project build system, licensed under [BSD licence](https://gitlab.kitware.com/cmake/cmake/raw/master/Copyright.txt).
- [Catch2](https://github.com/catchorg/Catch2) - unit testing framework, licensed under the [BSL-1.0 licence](https://github.com/catchorg/Catch2/blob/master/LICENSE.txt).
- [GSL](https://github.com/Microsoft/GSL) - guidelines support library for C++, licensed under the [MIT licence](https://github.com/microsoft/GSL/blob/master/LICENSE).
- [Font Awesome](https://fontawesome.com) - glyph based icon font, licensed under the [SIL OFL 1.1 licence](https://scripts.sil.org/OFL).
- [QCustomPlot](https://www.qcustomplot.com/) - charting library for Qt, licensed under the [GPLv3 licence](https://www.gnu.org/licenses/gpl-3.0.en.html).
- [macdeployqtfix](https://github.com/arl/macdeployqtfix) - finishes the job that macdeployqt starts, licensed under the [MIT licence](https://github.com/arl/macdeployqtfix/blob/master/LICENSE).
- [linuxdeployqt](https://github.com/probonopd/linuxdeployqt) - creates deployable linux binaries, licensed under the [GPLv3 licence](https://github.com/probonopd/linuxdeployqt/blob/master/LICENSE.GPLv3).
- [create-dmg](https://github.com/andreyvit/create-dmg) - automates the creation of Mac OS DMG files, licensed under the [MIT licence](https://github.com/andreyvit/create-dmg/blob/master/LICENSE).
- [colorama](https://github.com/tartley/colorama) - python module for terminal colour, licensed under the [BSD licence](https://github.com/tartley/colorama/blob/master/LICENSE.txt).
- [rapidfuzz-cpp](https://github.com/maxbachmann/rapidfuzz-cpp) - an implementation for fuzzy string matching, licensed under the [MIT licence](https://github.com/maxbachmann/rapidfuzz-cpp/blob/master/LICENSE).
- [gren](https://github-tools.github.io/github-release-notes/) - a tool for automating release notes, licensed under the [GPLv3 licence](https://github.com/github-tools/github-release-notes/blob/master/LICENSE).

Also, Pingnoo was developed using the following commercially licensed tools/services.

- [SmartCard Tools](https://www.mgtek.com/smartcard) - code-signing tool, allows automation of signing using a smartcard token.
- [Certum Code Signing Certificate](https://en.sklep.certum.pl/data-safety/code-signing-certificates/open-source-code-signing-1022.html) - open-source code signing certificate for signing Windows binaries.
- [AdvancedInstaller](https://www.advancedinstaller.com/) - Installer creator for windows, License kindly provided by them for free.
- [Affinity Designer](https://www.serif.com/designer) - Vector artwork design application.
- [Affinity Photo](https://www.serif.com/photo) - Bitmap artwork design application.
- [CLion](https://www.jetbrains.com/clion/) - The C++ IDE Pingnoo is developed in, the license was kindly provided for free for the project.
- [Deleaker](https://deleaker.com) - A memory leak detector for Windows, the license was kindly provided for free for the project. 
- [Icofx](https://icofx.ro) - A tool for producing icon & icns files from source images.
- [packagecloud](https://packagecloud.io) - packagecloud kindly host the package repositories for debian, ubuntu and fedora that make installing on those operating systems very easy.

## Thanks

This project is a grateful recipient of a micro-grant from [icculus](https://icculus.org/microgrant/2020/) who provide micro-grants each year to open-source projects.

# License

Pingnoo is open source and released under the GPLv3 License

Distributed as-is; no warranty is given or implied.

