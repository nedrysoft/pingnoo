/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of the Nedrysoft SettingsDialog. (https://github.com/nedrysoft/SettingsDialog)
 *
 * A cross-platform settings dialog
 *
 * Created by Adrian Carpenter on 07/12/2020.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "includes/MacHelper/MacHelper.h"

#include <QDialog>

#import <AppKit/AppKit.h>

constexpr auto bitsPerPixel = 8;
constexpr auto samplesPerPixel = 4;
constexpr auto systemFontSize = 12;

void Nedrysoft::MacHelper::enablePreferencesToolbar(QWidget *window) {
    if (@available(macOS 11, *)) {
        auto nativeView = reinterpret_cast<NSView *>(window->winId());

        if (!nativeView) {
            return;
        }

        Q_ASSERT_X([nativeView isKindOfClass:[NSView class]], static_cast<const char *>(__FUNCTION__),
                   "Object was not a NSView");

        auto nativeWindow = [nativeView window];

        if (nativeWindow == nil) {
            return;
        }

        Q_ASSERT_X([nativeWindow isKindOfClass:[NSWindow class]], static_cast<const char *>(__FUNCTION__),
                   "Object was not a NSWindow");

        if ([nativeWindow respondsToSelector:@selector(setToolbarStyle:)]) {
            [nativeWindow setToolbarStyle:NSWindowToolbarStylePreference];
        }
    }
}


auto Nedrysoft::MacHelper::standardImage(StandardImage::StandardImageName standardImage, QSize imageSize)->QPixmap {
    auto bitmapRepresentation = [[NSBitmapImageRep alloc]
            initWithBitmapDataPlanes: nullptr
                          pixelsWide: imageSize.width()
                          pixelsHigh: imageSize.height()
                       bitsPerSample: bitsPerPixel
                     samplesPerPixel: samplesPerPixel
                            hasAlpha: YES
                            isPlanar: NO
                      colorSpaceName: NSDeviceRGBColorSpace
                        bitmapFormat: NSBitmapFormatAlphaFirst
                         bytesPerRow: 0
                        bitsPerPixel: 0
    ];

    NSString *nativeImageName = nullptr;

    switch(standardImage) {
        case Nedrysoft::StandardImage::NSImageNamePreferencesGeneral: {
            nativeImageName = NSImageNamePreferencesGeneral;
            break;
        }

        case Nedrysoft::StandardImage::NSImageNameUserAccounts: {
            nativeImageName = NSImageNameUserAccounts;
            break;
        }

        case Nedrysoft::StandardImage::NSImageNameAdvanced: {
            nativeImageName = NSImageNameAdvanced;
            break;
        }
    }

    auto nsImage = [NSImage imageNamed: nativeImageName];

    [NSGraphicsContext saveGraphicsState];

    [NSGraphicsContext setCurrentContext: [NSGraphicsContext graphicsContextWithBitmapImageRep: bitmapRepresentation]];

    [nsImage drawInRect: NSMakeRect(0, 0, imageSize.width(), imageSize.height()) fromRect: NSZeroRect operation: NSCompositingOperationSourceOver fraction: 1];

    [NSGraphicsContext restoreGraphicsState];

    auto pixmap = QtMac::fromCGImageRef([bitmapRepresentation CGImage]);

    [bitmapRepresentation release];

    return pixmap;
}

auto Nedrysoft::MacHelper::nativeAlert(
        QWidget *parent,
        const QString &messageText,
        const QString &informativeText,
        const QStringList &buttons,
        std::function<void(Nedrysoft::AlertButton::AlertButtonResult)> alertFunction) -> void {

    Q_UNUSED(parent)

    // ok, this looks odd, but without the singleShot the dialog fails to display properly and flashes up on
    // screen briefly before closing itself, this only happens under certain circumstances but this fixes
    // the issue.

    QTimer::singleShot(0, [=]() {
        auto alert = [[NSAlert alloc] init];

        for (auto &button : buttons) {
            [alert addButtonWithTitle:button.toNSString()];
        }

        [alert setMessageText:messageText.toNSString()];
        [alert setInformativeText:informativeText.toNSString()];
        [alert setAlertStyle:NSAlertStyleInformational];

        auto returnValue = [alert runModal];

        [alert release];

        alertFunction(static_cast<Nedrysoft::AlertButton::AlertButtonResult>(returnValue));
    });
}

auto Nedrysoft::MacHelper::loadImage(const QString &filename, std::shared_ptr<char *> &data, unsigned int *length) -> bool {
    auto fileName = filename.toNSString();

    auto loadedImage = [[NSImage alloc] initWithContentsOfFile:fileName];

    if (loadedImage.isValid) {
        NSData *tiffData = [loadedImage TIFFRepresentation];

        data = std::make_shared<char *>(static_cast<char *>(malloc(tiffData.length)));

        *length = static_cast<unsigned int>(tiffData.length);

        memcpy(data.get(), tiffData.bytes, *length);

        [loadedImage release];

        return true;
    }

    [loadedImage release];

    return false;
}

auto Nedrysoft::MacHelper::imageForFile(const QString &filename, std::shared_ptr<char *> &data, unsigned int *length, int width, int height) -> bool {
    auto loadedImage = [[NSWorkspace sharedWorkspace] iconForFile:filename.toNSString()];

    [loadedImage setSize:NSMakeSize(width,height)];

    if (loadedImage.isValid) {
        NSData *tiffData = [loadedImage TIFFRepresentation];

        data = std::make_shared<char *>(static_cast<char *>(malloc(tiffData.length)));

        *length = static_cast<unsigned int>(tiffData.length);

        memcpy(data.get(), tiffData.bytes, *length);

        return true;
    }

    return false;
}

auto Nedrysoft::MacHelper::systemFontName() -> QString {
    auto font = [NSFont systemFontOfSize: systemFontSize];

    return QString([[font fontName] cStringUsingEncoding:[NSString defaultCStringEncoding]]);
}

auto Nedrysoft::MacHelper::fontFilename(const QString& fontName) ->QString {
    auto font = [NSFont fontWithName: [NSString stringWithCString: fontName.toLatin1().data() encoding: [NSString defaultCStringEncoding]] size: systemFontSize];

    if (font) {
        auto fontRef = CTFontDescriptorCreateWithNameAndSize(reinterpret_cast<CFStringRef>([font fontName]), [font pointSize]);

        auto url = static_cast<CFURLRef>(CTFontDescriptorCopyAttribute(fontRef, kCTFontURLAttribute));

        auto fontPath = [NSString stringWithString: [(NSURL *) CFBridgingRelease(url) path]];

        return QString([fontPath cStringUsingEncoding: [NSString defaultCStringEncoding]]);
    }

    return QString();
}
