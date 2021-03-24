# Ubuntu Build Container Setup Guide

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


cd /tmp &&
apt install  zlib1g-dev libncurses5-dev libgdbm-dev libnss3-dev libssl-dev libsqlite3-dev libreadline-dev libffi-dev curl libbz2-dev -y &&
wget https://www.python.org/ftp/python/3.9.1/Python-3.9.1.tgz &&
tar -xf Python-3.9.1.tgz &&
cd Python-3.9.1 &&
./configure --enable-optimizations &&
make -j 4 &&
make install

gem install rake &&
gem install package_cloud

update-alternatives --install /usr/bin/python3 python3 /usr/bin/python3.9 2

cd /tmp  &&
wget https://github.com/Kitware/CMake/releases/download/v3.20.0-rc2/cmake-3.20.0-rc2-linux-x86_64.sh &&
chmod +x cmake-3.20.0-rc2-linux-x86_64.sh &&
./cmake-3.20.0-rc2-linux-x86_64.sh --skip-license --prefix=/usr/local

cd /tmp &&
wget http://172.29.13.9:8111/update/buildAgentFull.zip &&
mkdir /opt/teamcity-agent &&
cd /opt/teamcity-agent &&
unzip /tmp/buildAgentFull.zip &&
ln /usr/lib/x86_64-linux-gnu/qt5/bin/lrelease /bin/lrelease
```

## Create the /etc/systemd/system/teamcity-agent.service file.

```
[Unit]
Description=TeamCity Agent
After=network.target

[Install]
WantedBy=multi-user.target

[Service]
ExecStart=/opt/teamcity-agent/bin/agent.sh start
Type=forking
RemainAfterExit=yes
SyslogIdentifier=teamcity_agent
PrivateTmp=yes
PIDFile=/opt/teamcity-agent/logs/buildAgent.pid
ExecStop=/opt/teamcity-agent/bin/agent.sh stop
RestartSec=5
Restart=on-failure
```

## Configure the teamcity build agent

```
cp /opt/teamcity-agent/conf/buildAgent.dist.properties /opt/teamcity-agent/conf/buildAgent.properties &&
vi /opt/teamcity-agent/conf/buildAgent.properties
```
then set the following configuration values.
```
serverURL=http://http://172.29.13.9:8111

system.linuxDistribution=ubuntu xx.yy
```

## Enable and start the teamcity build agent
```
systemctl enable teamcity-agent &&
service teamcity-agent start
```
