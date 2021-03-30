# Setting up Fedora for building pingnoo.

## Install Prerequisites.

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

ln -s /usr/lib64/qt5/bin/lrelease /bin/lrelease
```

### r33 or later.
```
  dnf -y install qt5-qtbase-devel
```

### r32 or earlier.
```
  dnf -y install qt5-devel
```
