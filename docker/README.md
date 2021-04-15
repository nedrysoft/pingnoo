# Docker Images

This folder contains the docker image definitions used by my CI/CD system (TeamCity) for building native packages.

Each platform is split into 2 separate images.

- -base is the base OS image with the required apt packages installed for building & deployment
- -builder contains extra tools (if needed) that are built from source (or available as a binary) (git, python, cmake).  This image is used to build the distributable packages.

I self host my own docker registry so that I am not limited by transfer quota restrictions on DockerHub.

## TeamCity Agents

A TeamCity agent docker image is used to spawn the actual docker container used to build the software, unfortunately the TeamCity docker agent image has a hardcoded path which means you can only run 1 instance of the agent on a host.

### Building the Agents

However, you can re-build the agent image and replace the path with your own suffix while will allow multiple agents to run on a single host.

```bash
git clone https://github.com/JetBrains/teamcity-docker-images.git
cd teamcity-docker-images
wget -c https://download.jetbrains.com/teamcity/TeamCity-2020.2.4.tar.gz -O - | tar -xz -C context
```

Edit the files:

```
context/run-agent.sh
context/generated/linux/MinimalAgent/Ubuntu/20.04/Dockerfile
```

And replace all occurrences of:

```bash
/opt/buildagent
```

With (for example):

```bash
/opt/buildagent-<suffix>
```

To build the image:

```bash
cd generated
sh ./teamcity-agent-local-linux.sh
docker tag teamcity-agent:local-linux registry.fizzyade.com/teamcity-agent-<suffix>
docker push registry.fizzyade.com/teamcity-agent-<suffix>
```

### Using the Agents

```bash
docker run \
    -it 
    -e SERVER_URL="teamcity.fizzyade.com"  \
    -u 0 \
    --rm \
    --name="teamcity-buildagent-<suffix>" \
    -v /mnt/user/appdata/teamcity-buildagent-<suffix>/config:/data/teamcity_agent/conf \
    -v /var/run/docker.sock:/var/run/docker.sock  \
    -v /opt/buildagent-<suffix>/work:/opt/buildagent-<suffix>/work \
    -v /opt/buildagent-<suffix>/temp:/opt/buildagent-<suffix>/temp \
    -v /opt/buildagent-<suffix>/tools:/opt/buildagent-<suffix>/tools \
    -v /opt/buildagent-<suffix>/plugins:/opt/buildagent-<suffix>/plugins \
    -v /opt/buildagent-<suffix>/system:/opt/buildagent-<suffix>/system \
    registry.fizzyade.com/teamcity-agent-<suffix>
```

## Building images

From the ```docker``` folder, execute the following command sequence to build the images.

```bash
(cd arch-builder && docker build -f Dockerfile -t registry.fizzyade.com/arch-builder .) && \
(cd debian-buster-base && docker build -f Dockerfile -t registry.fizzyade.com/debian-buster-base .) && \
(cd debian-buster-builder && docker build -f Dockerfile -t registry.fizzyade.com/debian-buster-builder .) && \
(cd ubuntu-18.04-base && docker build -f Dockerfile -t registry.fizzyade.com/ubuntu-18.04-base .) && \
(cd ubuntu-20.04-base && docker build -f Dockerfile -t registry.fizzyade.com/ubuntu-20.04-base .) && \
(cd ubuntu-20.10-base && docker build -f Dockerfile -t registry.fizzyade.com/ubuntu-20.10-base .) && \
(cd ubuntu-18.04-builder && docker build -f Dockerfile -t registry.fizzyade.com/ubuntu-18.04-builder .) && \
(cd ubuntu-20.04-builder && docker build -f Dockerfile -t registry.fizzyade.com/ubuntu-20.04-builder .) && \
(cd ubuntu-20.10-builder && docker build -f Dockerfile -t registry.fizzyade.com/ubuntu-20.10-builder .) && \
(cd fedora-32-builder && docker build -f Dockerfile -t registry.fizzyade.com/fedora-32-builder .) && \
(cd fedora-33-builder && docker build -f Dockerfile -t registry.fizzyade.com/fedora-33-builder .)
```
## Pushing images to local registry

Once built, the images can be pushed 

```bash
docker push registry.fizzyade.com/ubuntu-18.04-base && \
docker push registry.fizzyade.com/ubuntu-20.04-base && \
docker push registry.fizzyade.com/ubuntu-20.10-base && \
docker push registry.fizzyade.com/debian-buster-base && \
docker push registry.fizzyade.com/debian-buster-builder && \
docker push registry.fizzyade.com/ubuntu-18.04-builder && \
docker push registry.fizzyade.com/ubuntu-20.04-builder && \
docker push registry.fizzyade.com/ubuntu-20.10-builder && \
docker push registry.fizzyade.com/fedora-33-builder && \
docker push registry.fizzyade.com/fedora-32-builder && \
docker push registry.fizzyade.com/arch-builder
```