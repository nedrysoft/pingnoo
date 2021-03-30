# Instructions for TeamCity build agent on a linux system.

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

## Configure the teamcity build agent.

```
cp /opt/teamcity-agent/conf/buildAgent.dist.properties /opt/teamcity-agent/conf/buildAgent.properties &
vi /opt/teamcity-agent/conf/buildAgent.properties
```
then set the following configuration values.
```
serverURL=http://<teamcity server ip>>:8111

system.linuxDistribution=<linux operating system + version>
```

The linux operating system + version should be one of the following:

* ubuntu X.Y
* fedora rX
* debian X

***(where X is the major os release and Y is the minor)***

## Enable and start the teamcity build agent.
```
systemctl enable teamcity-agent &
service teamcity-agent start
```