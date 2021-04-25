# Docker Images

This folder contains the docker image definitions used by my CI/CD system (TeamCity) for building native packages.

Each platform may consist of 2 main images, which is for convenience when test building the images.  By having the system packages installed in a "base" image, we do not have to re-build everything should compilation fail on Git or Python.

Base only ever contains packages installed using the operating systems native a package manager (rpm, apt, gem, pip and so on).

-   *no prefix*, this is the base OS image. Typically, not required unless building the OS image from scratch (this is the case for raspbian)
-   *-base* is the base OS image with the required apt packages installed for building & deployment
-   *-builder* contains extra tools (if needed) not from native packages, either from source or a pre-built binary (git, python, cmake etc.).  This image builds the redistributable.

I self-host my a docker registry, so that transfer quota restrictions on DockerHub do not limit me.

Each folder may include a python script named init.py, the user may run this directly, but it is typically invoked by the build.py script if it exists.  The script's purpose is to do any initialisation before building the image, such as downloading any required files.  Additionally, it may be used to prevent an image from being built on the wrong architecture, i.e. the raspbian image builders can only be executed on a raspberry pi device.

## TeamCity Agents

A TeamCity docker agent spawns the actual docker container (on demand) used to build the software. Unfortunately, the TeamCity docker agent image has a hardcoded path which means you can only run one instance of the agent on a host.

### Building custom agents

However, you can re-build the agent image and replace the path with a  suffix, allowing multiple agents to run on a single host.

```bash
git clone https://github.com/JetBrains/teamcity-docker-images.git
cd teamcity-docker-images
wget -c https://download.jetbrains.com/teamcity/TeamCity-2020.2.4.tar.gz -O - | tar -xz -C context
```

Edit the files:

```text
context/run-agent.sh
context/generated/linux/MinimalAgent/Ubuntu/20.04/Dockerfile
```

Replace all occurrences of the following.

```bash
/opt/build agent
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
## Pushing images to a local registry

Once built, the user can push the images with the following terminal commands.

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
