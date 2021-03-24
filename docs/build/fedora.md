# Fedora Build Container Setup Guide

## Install Prerequisites

```
dnf -y group install "C Development Tools and Libraries"  &&
dnf -y install cmake &&
dnf -y install git &&
dnf -y install python-qt5-rpm-macros &&
dnf -y install qt5-linguist &&
dnf -y install rpm-build &&
dnf -y install rpmdevtools &&
dnf -y install qt5-qtquickcontrols2-devel &&
dnf -y install dbus-devel &&
dnf -y install vim &&
dnf -y install wget &&
dnf -y install unzip &&
dnf -y install java-11-openjdk &&
dnf -y install rpm-sign && 
dnf -y install ruby &&
dnf -y install ruby-devel

- **r33 or later** 

  dnf -y install qt5-qtbase-devel
  
- **r32 or earlier** 

  dnf -y install qt5-devel

ln -s /usr/lib64/qt5/bin/lrelease /bin/lrelease

gem install rake &&
gem install package_cloud

cd /tmp &&
wget http://172.29.13.9:8111/update/buildAgentFull.zip &&
mkdir /opt/teamcity-agent &&
cd /opt/teamcity-agent &&
unzip /tmp/buildAgentFull.zip
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
cp /opt/teamcity-agent/conf/buildAgent.dist.properties /opt/teamcity-agent/conf/buildAgent.properties &
vi /opt/teamcity-agent/conf/buildAgent.properties
```
then set the following configuration values.
```
serverURL=http://http://172.29.13.9:8111

system.linuxDistribution=fedora rXX
```

## Enable and start the teamcity build agent
```
systemctl enable teamcity-agent &
service teamcity-agent start
```