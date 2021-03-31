# Setting up Arch for building pingnoo.

## Uncomment mirrors.
```
vi /etc/pacman.d/mirrorlist
```

## Install Prerequisites.
```
pacman-key --init &&
pacman-key --populate archlinux

pacman --noconfirm -Syu

pacman --noconfirm -S cmake &&
pacman --noconfirm -S wget &&
pacman --noconfirm -S git &&
pacman --noconfirm -S base-devel &&
pacman --noconfirm -S qt5-base &&
pacman --noconfirm -S qt5-tools &&
pacman --noconfirm -S qt5-quickcontrols &&
pacman --noconfirm -S dbus &&
pacman --noconfirm -S python3
```

