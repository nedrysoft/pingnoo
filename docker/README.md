# Docker Images

This folder contains the docker image definitions used by my CI/CD system (TeamCity) for building native packages.

Each platform is split into 2 separate images.

- -base is the base OS image with the required apt packages installed for building & deployment
- -builder contains extra tools (if needed) that are built from source (or availalbe as a binary) (git, python, cmake).  This image is used to build the distributable packages.
