# Build Notes

The documents here contain notes for setting up build environments for different operating systems.

The information contained here is mainly notes for myself so that I can easily replicate containers for teamcity build agents without having to remember the exact steps.

## Linux Distribution Setup Instructions

The Linux builds require that the application is compiled on the target operating system.  For convenience, wherever possible builds are done in docker containers as these are easy to maintain and do not consume memory or resources like a virtual machine would. 

If there is no rootfs image for a distribution, then solutions other than docker may be needed, the specifics will be in the notes for the distribution.

* [Fedora](fedora.md)
* [Ubuntu](ubuntu.md)
* [Debian](debian.md)
* [Arch](arch.md)
* [Raspbian](raspbian.md)

## Services Setup Instructions

* [TeamCity](teamcity.md)
* [packagecloud](packagecloud.md)