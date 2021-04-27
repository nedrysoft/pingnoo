# Building pingnoo for macOS

This file documents the steps required for building pingnoo on macOS.

## Building Qt from source

Pingnoo can be compiled for both Intel and Apple Silicon and can be deployed as a universal binary which produces a single application that runs natively under Intel or macOS.

We build both the Intel and Apple Silicon builds to ensure both architectures are configured the same.

### Prerequisites

-  Xcode ([App Store](https://apps.apple.com/gb/app/xcode/id497799835?mt=12))

### Building Qt for Apple Silicon

```bash
mkdir ~/qt-5.15.2-arm64-build
cd ~/qt-5.15.2-arm64-build
../qt5/configure -nomake examples -nomake tests -skip qt3d -prefix /usr/local/qt/5.15.2/arm64 -device-option QMAKE_APPLE_DEVICE_ARCHS=arm64
make -j$(sysctl -n hw.ncpu)
make install
```

### Building Qt for Intel

```bash
mkdir ~/qt-5.15.2-x86_64-build
cd ~/qt-5.15.2-x86_64-build
../qt5/configure -nomake examples -nomake tests -skip qt3d -prefix /usr/local/qt/5.15.2/x86_64 -device-option QMAKE_APPLE_DEVICE_ARCHS=x86_64
make -j$(sysctl -n hw.ncpu)
make install
```

## Creating a universal version of Qt

The 2 installed  built architectures to build universal versions of the qt libraries so that we spuply a single version of our application.  We will do something similar to this for the actual application binary.

You will need the makeuniversal tool which can be found at [https://github.com/nedrysoft/makeuniversal](https://github.com/nedrysoft/makeuniversal), this tool does the work on combining the x86_64 and apple silicon versions of the qt libraries and is also used for the same purpose with the pingnoo binaries.

```bash
makeuniversal /usr/local/qt/5.15.2/universal /usr/local/qt/5.15.2/x86_64 /usr/local/qt/5.15.2/arm64
```

## Creating a universal version of pingnoo

To create the universal binary of pingnoo, from the root of the repository execute the following.

```bash
makeuniversal ./bin/Universal/Release ./bin/x86_64/Release ./bin/arm64Release
```

## Finishing up

After creating the universal binary for the application, macdeployqt and macdeployqtfix should be used to add the qt dependencies to the application bundle, once the bundling is complete the resulting app bundle can be signed.