/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of pingnoo (https://github.com/fizzyade/pingnoo)
 * An open source ping path analyser
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

#ifndef NEDRYSOFT_RIBBON_RIBBONWIDGETPLUGIN_H
#define NEDRYSOFT_RIBBON_RIBBONWIDGETPLUGIN_H

#include <QtUiPlugin/QDesignerCustomWidgetInterface>

class RibbonWidgetPlugin :
        public QObject, public QDesignerCustomWidgetInterface {
    Q_OBJECT
        Q_INTERFACES(QDesignerCustomWidgetInterface)

    public:
        explicit RibbonWidgetPlugin(QObject *parent = nullptr);

        bool isContainer() const override;

        bool isInitialized() const override;

        QIcon icon() const override;

        QString domXml() const override;

        QString group() const override;

        QString includeFile() const override;

        QString name() const override;

        QString toolTip() const override;

        QString whatsThis() const override;

        QWidget *createWidget(QWidget *parent) override;

        void initialize(QDesignerFormEditorInterface *core) override;

    private:
        bool initialized = false;
};

#endif // NEDRYSOFT_RIBBON_RIBBONWIDGETPLUGIN_H
