# Setting up a raspberry pi for building pingnoo.

## Install Prerequisites.
```
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

### Build git from source as debian version is too old.
```
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

### Install cmake from snap

```
sudo snap install core &&
sudo snap install cmake --classic
```

### Fix /etc/ld.so.conf

Raspbian attempts to support older architecture pi boards, for us we are only interested in Pi 3 or 4.

```
sudo echo /usr/lib/arm-linux-gnueabihf/libarmmem-v7l.so > /etc/ld.so.preload
```