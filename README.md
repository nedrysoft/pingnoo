# Pingnoo

/pɪŋ ɡəˈnuː/

---
<a href="https://github.com/nedrysoft/pingnoo/blob/develop/LICENSE"><img src="https://img.shields.io/github/license/nedrysoft/pingnoo"/></a>
<a href="https://www.travis-ci.com/github/nedrysoft/pingnoo"><img src="https://www.travis-ci.com/nedrysoft/pingnoo.svg?branch=develop"/></a>
<a href="https://packagecloud.io/nedrysoft/pingnoo"><img src="https://img.shields.io/badge/deb-packagecloud.io-844fec.svg"/></a>
<a href="https://packagecloud.io/nedrysoft/pingnoo"><img src="https://img.shields.io/badge/rpm-packagecloud.io-844fec.svg"/></a>
<a href="https://aur.archlinux.org/packages/pingnoo/"><img src="https://img.shields.io/aur/version/pingnoo"/></a>
<a href="https://github.com/nedrysoft/pingnoo/graphs/contributors"><img src="https://img.shields.io/github/contributors/nedrysoft/pingnoo"/></a>
<a href="https://github.com/nedrysoft/pingnoo/releases/latest"><img src="https://img.shields.io/badge/%F0%9F%A4%96-release%20notes-00B2EE.svg"/></a>
<a href="https://www.codacy.com/gh/nedrysoft/pingnoo/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=nedrysoft/pingnoo&amp;utm_campaign=Badge_Grade"><img src="https://app.codacy.com/project/badge/Grade/7a90ccfa21324fceb5f27a92b8edbac6"></a>
<br/>

Pingnoo is an open-source combined traceroute and ping application for analysing network connections.  It runs under Windows, macOS and Linux.

<img alt="the pingnoo user interface" src="https://user-images.githubusercontent.com/55795671/103213760-96965c80-4906-11eb-98ee-34658db89b4c.gif"/>

---
##Donations

Any donations are gratefully received. The kindness of some commercial software & services companies has reduced the costs I incur. I still have to pay for code signing certificates, developer accounts, hardware, electricity, development tools that aren't available for free (or free for OSS).

I am trying to expand the software as much as I can. I have a list of features I will implement, and I have received great feedback and suggestions from people actively using the software.

***There absolutely is no obligation for you to donate***, you are free to use or share this software or its source code under the terms of the GPLv3, but every little bit helps!

[![Donate with Bitcoin](https://en.cryptobadges.io/badge/big/1EDYnME2Ke4k1EThnHYJcpPk4DUzC8ALmc)](https://en.cryptobadges.io/donate/1EDYnME2Ke4k1EThnHYJcpPk4DUzC8ALmc)

---

## Installation

[The binaries are available under the releases page.](https://github.com/nedrysoft/pingnoo/releases)

---

### Windows Installer

Download the installer executable and run it to install the application.  The installer will (optionally) create a desktop and a start menu shortcut for launching the software.

---

### Windows Portable Edition

You can extract the portable edition zip file onto a memory stick. The application can then be launched directly from the memory stick without the need to install the software.

---

### MacOS

The application is a dmg disk image.

Download and open the disk image and drag the Pingnoo icon into the Applications folder. You can then launch the application by double-clicking on the Pingnoo icon in Applications.

From version 2021.04.30 pingnoo is built as a universal binary that will run natively on both Intel and Apple Silicon devices.

---

## Linux Native Packages

Native packages for popular Linux distributions are available, and these install with the correct permissions, which allow the application to run without root permissions.

### Debian/Ubuntu/Raspbian

Pingnoo is available pre-built for the following distributions.

-   Arch Linux (*64 bit binaries only*)
-   Debian 9, 10 (*64 bit binaries only*)
-   Ubuntu 18.04, 20.04, 20.10 (*64 bit binaries only*)
-   Raspbian 9, 10 (*32 bit binaries only*)

pingnoo is available for installation using apt. To do this, you will need to add our apt repository and gpg key, and You can use the following command to add the pingnoo repository to your operating system.

```bash
curl -s https://packagecloud.io/install/repositories/nedrysoft/pingnoo/script.deb.sh | sudo bash
```

You can then install pingnoo using the apt command.

```bash
sudo apt install pingnoo
```

By using the apt repository, you will be able to update pingnoo easily.

```bash
sudo apt update
```

Alternatively, you can download the appropriate deb file and install it manually.

```bash
dpkg -i <downloaded file>.deb
apt -f install
```

### Fedora

Pingnoo is available pre-built for the following versions.

-   Fedora r32
-   Fedora r33

The yum command can install pingnoo on a supported operating system.  To do this, you will need to add the rpm repository and gpg key to your operating system installation.

```bash
curl -s https://packagecloud.io/install/repositories/nedrysoft/pingnoo/script.rpm.sh | sudo bash
```

After adding the repository, yum is used to install pingnoo.

```bash
sudo yum install pingnoo
```

Using the yum repository will allow you to update pingnoo easily when a new version is released.

```bash
sudo yum update
```

Alternatively, you can download the appropriate rpm file and install it manually.

```bash
yum localinstall <downloaded file>.rpm
```

### Arch

Pingnoo is available in the AUR.

You can also clone the project from the AUR and build and install it using pacman.

```bash
git clone https://aur.archlinux.org/pingnoo.git
cd pingnoo
makepkg
sudo pacman -U <filename>.zst
```

Alternatively, You can also use a tool such as yay to install the pingnoo binary along with any required dependencies.

```bash
yay -S pingnoo
```

### AppImage

The AppImage file contains all dependencies and will run on any modern Linux distribution without installing additional packages.

After downloading the AppImage, you will need to set the execute attribute on the file.

```bash
chmod +x <downloaded filename>
```

The application requires RAW socket access, which means some limitations created by the AppImage process affect the operation of the application.  You will need to follow the instructions outlined in the Linux Notes section regarding setuid, allowing you to run the application directly.  Alternatively, you can launch from the terminal.

```bash
sudo ./<downloaded filename>`
```

---

## Documentation

The documentation for pingnoo is available on readthedocs.

https://pingnoo.readthedocs.io/en/latest/

---

## Development

You also have the option of building pingnoo from the source code. In addition to a compiler that supports the C++17 standard, you will also require the Qt 5 or 6 development libraries and CMake.

### Architecture

Pingnoo uses a modular design architecture based on components (aka plugins); the software can be extended easily by third parties by creating new components or modifying existing ones.

### General Information

The following compiler configurations are known to work for development.  (We do not supply binaries for 32-bit systems)

-   ***Linux*** - 32 & 64-bit using GNU g++
-   ***Windows*** - 32 & 64-bit using MSVC 2019 Community Edition
-   ***Windows*** - 32 & 64-bit using mingw-g++
-   ***macOS*** - 64-bit using clang from the xcode command line tools

### Building from source

Building the project will require specific libraries and toolchains to be installed.

You should use git to obtain the source code, pingnoo uses submodules, and GitHub does not bundle these in the source code tarball. If you use a tarball, you will have to download and place the other modules into the source tree.

```bash
git clone https://github.com/nedrysoft/pingnoo.git
cd pingnoo
git submodule update --init --recursive
```

#### Build dependencies

The [build documentation](docs/build/README.md) contains information about packages required to build the software from the source.

#### Building the application

Change to the folder that contains the source code, and then execute the following commands.

```bash
mkdir build
cd build
cmake ..
make
```

CMake will configure the build ready for building using make.  The resulting binaries are located in ../bin/<arch>/<build type>

For example, the folder ../bin/x86_64/Release would contain the compiled binaries for an x86_64 release build.

Change to that folder and execute ./Pingnoo to run the application.

### Docker

We use Docker containers to build pingnoo with our CI/CD system (TeamCity).  These images contain all the required dependencies and build tools pre-installed for a specific operating system version.

The docker folder in the repository contains python scripts that produce repeatable images for all Linux distribution targets.

### Deployment

The deploy.py python script creates runnable/installable binaries for the given operating system.

-   ***Linux*** - Generates a stand-alone AppImage and distribution specific packages.
-   ***Windows*** - Generates an installer and a portable zip file that can run from a USB stick.
-   ***macOS*** - Generates a DMG file.

The script requires Python 3.6 or later, curl and the [colorama](https://github.com/tartley/colorama) python module to enhance the console output. You can install the Colorama module with the following command.

(The CMake script provides an install target that uses the deploy.py script to generate a deployable image)

```bash
pip3 install colorama
```

The script provides the following parameters:

-   `--qtdir="<path to qt>"` - the path to the version of qt used.
-   `--curlbin="<path to curl binary>"` - the path to the curl binary, i.e `"/usr/bin/curl"`.
-   `--arch="<x64|x86|armv7l|universal>"` - the architecture to deploy. (universal is available on macOS and Windows)
-   `--type=<release|debug>` - the build type to deploy.
-   `--cert="<cert>"` - the value of this is dependent on the platform, for Windows and macOS OS it is the name of the certificate to be used, for Linux it is the gpg2 key identifier.
-   `--timeserver="<name>"` - windows only, the name of the time-server to be used during signing
-   `--appleid="<email>"` - *macOS only*, the Apple ID of the developer account used to sign the binary
-   `--password="<password>"` - *macOS only*, the password of the Apple ID.  The password can be stored in the keychain and accessed using `"@keychain:<identifier>"` instead of passing the actual password to the script.
-   `--appimage` - *Linux only*, will generate an AppImage.
-   `--rpm` - *Linux only*, will generate an rpm package.
-   `--deb` - *Linux only*, will generate a deb package.
-   `--pkg` - *Linux only*, will generate an arch package.
-   `--appiamagetool="<path to appimage tool"` - *Linux only*, the path to the appimagetool binary.
-   `--linuxdeployqy="<path to linuxdeployqt tool"` - *Linux only*, the path to the linuxdeployqt binary.
-   `--signtool="<path to sign tool>"` - *Windows only*, the path to the signing tool.
-   `--pin=<pin>` - *Windows only*, if using scsigntool, the pin to use.
-   `--portable` - *Windows only*, will generate a portable zip file for windows.
-   `--version` - the version of the deployment tool.
-   `--debugoutput` - produce debug output.

The script will use the curl binary to obtain any tools required for the deployment process.

The script stores the deployable asset in the deployment folder.

### Linux Notes

The PingCommandPingEngine ping engine is an experimental ping engine that uses the Linux system ping command, and this allows the software to run without any special privileges. Please note that the Linux ping executable does not provide timing information for TTL expired replies.

Pingnoo instead uses a precision timer to measure the time taken from invoking the ping executable to receiving the TTL expired reply.  Please note that this is not as accurate as the ICMP socket engine.

If the ICMPPingEngine is selected, then the application will require RAW socket privileges and must be run as root or given raw socket access using the setcap* command.

The setcap command will not work on the AppImage Linux release due to limitations with the AppImage architecture.

The CMake configuration for Linux has the following post link command, this command is optional, and you can control this with the option `Pingnoo_SetRawCapabilities`.

```bash
sudo -n /usr/sbin/setcap cap_net_raw,cap_net_admin=eip $${APPLICATION_BINARY_FOLDER}/$${TARGET}
```

This command will automatically apply the required privileges to the executable after linking.  By running setcap via sudo, the flag `-n` is needed to stop sudo prompting for a password. If a password is required, then sudo will fail.

To stop sudo from requesting a password for the `/usr/sbin/setcap` executable, create the following entry in `/etc/sudoers`(replacing <username> with the name of your user account).

`<username> ALL = (root) NOPASSWD: /usr/sbin/setcap`

In some Linux distributions, the `setcap` executable may reside in a different folder such as `/sbin`.

(*Please be aware of any security issues by doing this.*)

Also, it's not possible to debug the application due to the requirement of RAW sockets. One way of solving this is to create a script in `/usr/bin/gdb-sudo` with the following content.

```bash
#!/bin/sh
sudo -n /usr/bin/gdb $@
```

Create an entry in `/etc/sudoers` to allow gdb to run as root without a password.

`<username> ALL = (root) NOPASSWD: /usr/bin/gdb`

The final step is to configure Qt Creator to use this proxy script.   Create a new debugger configuration in Qt Creator, selecting `/usr/bin/gdb-sudo` as the gdb binary, repeating this for each kit you intend to use for debugging.

(*Please be aware of any security issues by doing this.*)

#### Valgrind

Debugging memory leaks can be done with Valgrind, but as above, the application requires raw socket privileges, and Valgrind cannot run the executable without being run as root.  The following command can be executed from the application binary folder to run Valgrind and produce an XML debug log.

```bash
sudo /usr/bin/valgrind --child-silent-after-fork=yes --xml=yes --smc-check=all-non-file --tool=memcheck --gen-suppressions=all --track-origins=yes --leak-check=summary --num-callers=25 --xml-file=./debug.xml ./Pingnoo_debug
```

#### Deleaker

The Deleaker tool is used under Windows to locate memory leaks.  It is currently Windows-only but works incredibly well for finding leaks.

The user interface is simple and allows viewing the leak trace of the leak, which is invaluable when figuring out which code level is responsible for the leak.

#### Unit Tests

Set the `Pingnoo_Build_Tests` option to `ON` to generate a binary that performs unit tests.

#### Ribbon Bar

The user interface uses a Ribbon style toolbar (as seen in applications such as Microsoft Office).  To build the designer plugin, set the CMake option `Pingnoo_Build_RibbonDesignerPlugin` to `ON`. Copy the generated plugin to the Qt plugins folder to make it available in designer.

#### Mac OS

The gatekeeper requirements in macOS mean that the application needs to be signed and notarised to run without user interaction for allowing the application to run.  You will require a paid developer account to obtain a code signing certificate that is valid for gatekeeper authentication.

---

## Credits

This project was created and maintained by [Adrian Carpenter](https://github.com/fizzyade).

I thank the following people for contributing to the project:

-   [Aaron D. Marasco](https://github.com/AaronDMarasco)
-   [Paul Varghese](https://github.com/Paulls20)
-   [Stepan Michalek](https://github.com/Ste-Mich)
-   [Nick Boultonn](https://github.com/boultonn)

Pingnoo uses the following third-party libraries/assets/tools/services in the development of Pingnoo.

-   [Qt](https://www.qt.io/download) - cross-platform framework, licensed under the [GPLv3 licence](https://www.gnu.org/licenses/gpl-3.0.en.html).
-   [cmake](https://www.cmake.org) - cross-platform project build system, licensed under [BSD licence](https://gitlab.kitware.com/cmake/cmake/raw/master/Copyright.txt).
-   [Catch2](https://github.com/catchorg/Catch2) - unit testing framework, licensed under the [BSL-1.0 licence](https://github.com/catchorg/Catch2/blob/master/LICENSE.txt).
-   [GSL](https://github.com/Microsoft/GSL) - guidelines support library for C++, licensed under the [MIT licence](https://github.com/microsoft/GSL/blob/master/LICENSE).
-   [Font Awesome](https://fontawesome.com) - glyph based icon font, licensed under the [SIL OFL 1.1 licence](https://scripts.sil.org/OFL).
-   [QCustomPlot](https://www.qcustomplot.com/) - charting library for Qt, licensed under the [GPLv3 licence](https://www.gnu.org/licenses/gpl-3.0.en.html).
-   [macdeployqtfix](https://github.com/arl/macdeployqtfix) - finishes the job that macdeployqt starts, licensed under the [MIT licence](https://github.com/arl/macdeployqtfix/blob/master/LICENSE).
-   [linuxdeployqt](https://github.com/probonopd/linuxdeployqt) - creates deployable linux binaries, licensed under the [GPLv3 licence](https://github.com/probonopd/linuxdeployqt/blob/master/LICENSE.GPLv3).
-   [dmgbuild](https://github.com/al45tair/dmgbuild.git) - automates the creation of macOS DMG files, licensed under the [MIT licence](https://github.com/al45tair/dmgbuild/blob/master/LICENSE).
-   [colorama](https://github.com/tartley/colorama) - python module for terminal colour, licensed under the [BSD licence](https://github.com/tartley/colorama/blob/master/LICENSE.txt).
-   [rapidfuzz-cpp](https://github.com/maxbachmann/rapidfuzz-cpp) - an implementation for fuzzy string matching, licensed under the [MIT licence](https://github.com/maxbachmann/rapidfuzz-cpp/blob/master/LICENSE).
-   [gren](https://github-tools.github.io/github-release-notes/) - a tool for automating release notes, licensed under the [GPLv3 licence](https://github.com/github-tools/github-release-notes/blob/master/LICENSE).

Also, the development of pingnoo is made possible with the following commercially licensed tools/services.

-   [SmartCard Tools](https://www.mgtek.com/smartcard) - code-signing tool, allows automation of signing using a smartcard token.
-   [Certum Code Signing Certificate](https://en.sklep.certum.pl/data-safety/code-signing-certificates/open-source-code-signing-1022.html) - open-source code signing certificate for signing Windows binaries.
-   [AdvancedInstaller](https://www.advancedinstaller.com/) - Installer creator for windows, License kindly provided by them for free.
-   [Affinity Designer](https://www.serif.com/designer) - Vector artwork design application.
-   [Affinity Photo](https://www.serif.com/photo) - Bitmap artwork design application.
-   [CLion](https://www.jetbrains.com/clion/) - The preferred C++ IDE for development, Jetbrains kindly provided the license for free for the project.
-   [Deleaker](https://deleaker.com) - A memory leak detector for Windows, Softanics kindly provided the license for free for the project.
-   [Icofx](https://icofx.ro) - A tool for producing icon & icns files from source images.
-   [packagecloud](https://packagecloud.io) - packagecloud kindly host the package repositories for Debian, Ubuntu and Fedora that make installing on those operating systems very easy.
-   [Araxis Merge](https://www.araxis.com/merge) - Araxis kindly provided the license for free for the project.
-   [Outline](https://www.getoutline.com/) - A note-taking app that supports rich content and based on Markdown, the application is open-source but hosted accounts are available.

## Thanks

This project is a grateful recipient of a micro-grant from [icculus](https://icculus.org/microgrant/2020/) who provide micro-grants each year for eligible open-source projects.

## License

Pingnoo is open source and released under the GPLv3 license.

Distributed as-is; no warranty is given or implied.
