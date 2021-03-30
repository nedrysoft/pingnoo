# Setting up Ubuntu for building pingnoo.

## Install Prerequisites
```
apt update -y &&
apt install software-properties-common -y &&
add-apt-repository ppa:git-core/ppa -y &&
apt update -y &&
apt install curl -y &&
apt install build-essential -y &&
apt install unzip -y &&
apt install wget -y &&
apt install vim -y &&
apt install mesa-common-dev -y &&
apt install libdbus-1-dev -y &&
apt install libxcb-xinerama0-dev -y &&
apt install git -y &&
apt install qttools5-dev-tools -y &&
apt install qtdeclarative5-dev -y &&
apt install qml-module-qtquick-controls -y &&
apt install qt5-default -y &&
apt install qttools5-dev -y &&
apt install openjdk-8-jdk -y &&
apt install dpkg-sig -y &&
apt install ruby -y &&
apt install ruby-dev -y

ln /usr/lib/x86_64-linux-gnu/qt5/bin/lrelease /bin/lrelease
```

### Build python 3.9 from source
```
cd /tmp &&
apt install  zlib1g-dev libncurses5-dev libgdbm-dev libnss3-dev libssl-dev libsqlite3-dev libreadline-dev libffi-dev curl libbz2-dev -y &&
wget https://www.python.org/ftp/python/3.9.1/Python-3.9.1.tgz &&
tar -xf Python-3.9.1.tgz &&
cd Python-3.9.1 &&
./configure --enable-optimizations &&
make -j 4 &&
make install

update-alternatives --install /usr/bin/python3 python3 /usr/bin/python3.9 2
```

### Download and install CMake
```
cd /tmp  &&
wget https://github.com/Kitware/CMake/releases/download/v3.20.0-rc2/cmake-3.20.0-rc2-linux-x86_64.sh &&
chmod +x cmake-3.20.0-rc2-linux-x86_64.sh &&
./cmake-3.20.0-rc2-linux-x86_64.sh --skip-license --prefix=/usr/local
```
