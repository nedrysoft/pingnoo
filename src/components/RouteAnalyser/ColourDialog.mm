/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 19/12/2020.
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

#include "ColourDialog.h"

#import <AppKit/NSColor.h>
#import <AppKit/NSWindowRestoration.h>

Nedrysoft::RouteAnalyser::ColourDialog::ColourDialog() :
        m_observer(nullptr),
        m_colourFunction(nullptr),
        m_colourPanel(nullptr) {

    m_colourPanel = [NSColorPanel sharedColorPanel];

    if( [m_colourPanel respondsToSelector:@selector(setRestorable:)] ) {
        NSWindow *window = static_cast<NSWindow <NSWindowRestoration> *>(m_colourPanel);

        [window setRestorable:NO];
    }
}

Nedrysoft::RouteAnalyser::ColourDialog::~ColourDialog() = default;

auto Nedrysoft::RouteAnalyser::ColourDialog::close() -> void {
    [m_colourPanel close];
}

auto Nedrysoft::RouteAnalyser::ColourDialog::getInstance() -> Nedrysoft::RouteAnalyser::ColourDialog * {
    static Nedrysoft::RouteAnalyser::ColourDialog *instance = nullptr;

    if (instance==nullptr) {
        instance = new  Nedrysoft::RouteAnalyser::ColourDialog;
    }

    return instance;
}

auto Nedrysoft::RouteAnalyser::ColourDialog::open(
        QColor initialColour,
        QString titleString,
        Nedrysoft::RouteAnalyser::ColourFunction colourFunction ) -> void {

    titleString = QString(
            QT_TR_NOOP("%1 Colour"))
            .arg(titleString.toLower().replace(0, 1, titleString.at(0).toUpper()).trimmed() );

    NSNotificationCenter *center = [NSNotificationCenter defaultCenter];
    NSOperationQueue *mainQueue = [NSOperationQueue mainQueue];

    if (m_observer) {
        [center removeObserver:m_observer];
    }

    m_observer = [center addObserverForName:NSColorPanelColorDidChangeNotification
            object:nil
            queue:mainQueue usingBlock:^(NSNotification *note) {
                auto colour = QColor::fromRgbF(
                        m_colourPanel.color.redComponent,
                        m_colourPanel.color.greenComponent,
                        m_colourPanel.color.blueComponent );

                if (colourFunction) {
                    colourFunction(colour);
                }
            }];

    [m_colourPanel setTitle: titleString.toNSString()];

    NSColor *colour = [NSColor
            colorWithRed:initialColour.redF()
            green:initialColour.greenF()
            blue:initialColour.blueF()
            alpha:initialColour.alphaF() ];

    [m_colourPanel setColor: colour];
    [m_colourPanel setFloatingPanel: true];
    [m_colourPanel makeKeyAndOrderFront: nil];
}
