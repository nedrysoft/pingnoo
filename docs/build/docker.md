# Docker

Docker can be used by TeamCity as build environments, this document lists the Linux distributions docker images.

## Ubuntu

-   18.04	bionic
-   20.04	focal
-   20.10	groovy
-   21.04	hirsute
-   14.04	trusty
-   16.04	xenial

docker run -it ubuntu:<tag> /bin/bash

## Debian

-   10	buster
-   9		stretch

docker run -it debian:<tag> /bin/bash

## Fedora

-   35
-   34
-   33
-   32
-   31

docker run -it fedora:<tag> /bin/bash

## Arch

docker run -it archlinux:latest /bin/bash

# Docker Images

We use the above image as a base, and then install the required prerequisites in the environment.

```dockerfile
FROM debian:buster
ENV DEBIAN_FRONTEND noninteractive
RUN apt-get update && apt-get install -y && \
    apt-get install software-properties-common -y --no-install-recommends && \
    apt update -y && \
    apt install -y --no-install-recommends curl \
    build-essential \
    unzip \
    vim \
    mesa-common-dev \
    libdbus-1-dev \
    libxcb-xinerama0-dev \
    git \
    qttools5-dev-tools \
    qtdeclarative5-dev \
    qml-module-qtquick-controls \
    qt5-default \
    qttools5-dev \
    default-jdk \
    dpkg-sig \
    ruby \
    ruby-dev && \
    echo "gem: --no-ri --no-rdoc" >/etc/gemrc && \
    gem install fpm -v 1.4.0 && \
    gem install fpm-cookery -v 0.29.0 && \
    gem install buildtasks -v 0.0.1 && \
    gem install bundler -v 1.10.0 && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*
```

