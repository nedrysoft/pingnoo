# Setting up Arch for building pingnoo.

## Uncomment mirrors.
```
vi /etc/pacman.d/mirrorlist
```

## Update arch
```
pacman-key --init &&
pacman-key --populate archlinux

pacman --noconfirm -Syu
```

## Install dhcpcd

```
pacman --noconfirm -S dhcpcd
```

And then add:

```nodev```

to /etc/dhcpcd.conf, without doing this the container will not have network access.

## Install Prerequisites.
```
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

## AUR Information

The source tarball is generated using:

```
git-archive-all <output file>
```

This creates a tarball that contains the submodules as well.

```
git clone https://aur.archlinux.org/pingnoo.git
cd pingnoo
cp <generated PKGBUILD> .
makepkg --printsrcinfo > .SRCINFO
git add .
git commit -m "repo: updated to <version>"
```

