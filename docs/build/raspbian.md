# Setting up a raspberry pi for building pingnoo.

It is not possible to cross compile pingnoo easily, therefore builds are performed on an actual raspberry pi.  Due to the limited resources, it is recommended that you get the most powerful pi that is available. (at this time it is the Raspberry Pi 4 with 8GB of RAM)

The image building is still done in a docker container, each container contains the rootfs for the specific version of raspbian and contains all of the tools required to produce a deployable image from source.

Because of the limited resources, TeamCity must set a shared resource with a limit of 1 for each raspberry pi agent, this will prevent multiple builds happening in parallel on the pi which would result in out of memory and the kernel will kill off the compiler tasks and the build will fail.

## Docker Images

The docker templates are located in the docker folder at the root of the source tree, the script build.py can be used to build an image for a specific version of raspbian.

```bash
python3 ./build.py --all raspbian-stretch
python3 ./build.py --all raspbian-stretch
```

### Manual Setup for Development

The following here list the required packages, tools and configuration for compiling pingnoo in the target distribution.

#### Prerequisite Packages

```bash
apt update -y &&
apt install software-properties-common -y &&
apt update -y &&
apt install curl -y &&
apt install build-essential -y &&
apt install unzip -y &&
apt install vim -y &&
apt install mesa-common-dev -y &&
apt install libdbus-1-dev -y &&
apt install libxcb-xinerama0-dev -y &&
apt install qttools5-dev-tools -y &&
apt install qtdeclarative5-dev -y &&
apt install qml-module-qtquick-controls -y &&
apt install qt5-default -y &&
apt install qttools5-dev -y &&
apt install default-jdk -y &&
apt install dpkg-sig -y &&
apt install ruby -y &&
apt install autoconf -y &&
apt install ruby-dev -y
```

#### GIT

The version of GIT supplied in the distributions is too old, so we need to build GIT from source.

```bash
apt install dh-autoreconf libcurl4-gnutls-dev libexpat1-dev gettext libz-dev libssl-dev asciidoc xmlto docbook2x install-info -y &&
cd /tmp &&
wget https://github.com/git/git/archive/refs/tags/v2.31.0.tar.gz &&
tar -zxf v2.31.0.tar.gz &&
cd git-2.31.0 &&
make configure &&
./configure --prefix=/usr &&
make -j 4 all doc info &&
make install install-doc install-html install-info
```

#### CMake

The version of CMake suppied in the distributions is too old, so we need to build CMake from source.

```bash
sudo snap install core &&
sudo snap install cmake --classic
```

#### Fix /etc/ld.so.conf

Raspbian attempts to support older architecture pi boards, for us we are only interested in Pi 3 or 4.

```bash
sudo echo /usr/lib/arm-linux-gnueabihf/libarmmem-v7l.so > /etc/ld.so.preload
```

## Building pingnoo from source

Pingnoo uses c++17 and requires gcc 5 or later.

### Raspbian stretch (9)

The version of gcc in stretch is too old to support C++17.  The builder image builds 8.3.0 and installs it to `/usr/local/gcc-8.3.0`, it exists side by side with the original gcc version.  To use the 8.3 the `CC` and `CXX` environment variables must be set to the location of the replacement versions.

```bash
cd /tmp
git clone https://github.com/nedrysoft/pingnoo
git submodule update --init --recursive
mkdir build
cd build
CC=/usr/local/gcc-8.3.0/bin/gcc-8.3 CXX=/usr/local/gcc-8.3.0/bin/g++-8.3 cmake ..
```

### Raspbian buster (10)

Buster provides a gcc version which has c++17 support.

```bash
cd /tmp
git clone https://github.com/nedrysoft/pingnoo
git submodule update --init --recursive
mkdir build
cd build
cmake ..
```