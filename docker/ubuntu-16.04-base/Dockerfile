FROM ubuntu:16.04
ENV DEBIAN_FRONTEND noninteractive
RUN apt-get update && apt-get install -y && \
    apt-get install -y software-properties-common --no-install-recommends && \
    add-apt-repository -y ppa:ubuntu-toolchain-r/test && \
    apt-get update && \
    apt-get install -y --no-install-recommends \
    curl \
    python3 \
    p7zip \
    p7zip-full && \
    curl https://bootstrap.pypa.io/pip/3.5/get-pip.py -o get-pip.py && \
    python3 get-pip.py && \
    python3 -m pip install aqtinstall && \
    apt-get install -y --no-install-recommends \
    gcc-6 \
    g++-6 \
    curl \
    build-essential \
    wget \
    unzip \
    vim \
    mesa-common-dev \
    libdbus-1-dev \
    libxcb-xinerama0-dev \
    qttools5-dev-tools \
    qtdeclarative5-dev \
    qml-module-qtquick-controls \
    qt5-default \
    qttools5-dev \
    dpkg-sig \
    autoconf \
    dh-autoreconf \
    libcurl4-gnutls-dev \
    libexpat1-dev \
    gettext \
    libz-dev \
    libssl-dev \
    asciidoc \
    xmlto \
    docbook2x \
    install-info \
    zlib1g-dev \
    libncurses5-dev \
    libgdbm-dev \
    libnss3-dev \
    libssl-dev \
    libsqlite3-dev \
    libreadline-dev \
    libffi-dev \
    libbz2-dev \
    ruby \
    ruby-dev && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

