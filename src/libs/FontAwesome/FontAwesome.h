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

#ifndef FIZZYADE_FONTAWSOME_FONTAWESOME_H
#define FIZZYADE_FONTAWSOME_FONTAWESOME_H

#include <QString>
#include <QMap>
#include <QIcon>

#if (defined(FIZZYADE_LIBRARY_FONTAWESOME_EXPORT))
#define FIZZYADE_FONTAWESOME_DLLSPEC Q_DECL_EXPORT
#else
#define FIZZYADE_FONTAWESOME_DLLSPEC Q_DECL_IMPORT
#endif

namespace FizzyAde::FontAwesome
{
    class FIZZYADE_FONTAWESOME_DLLSPEC FontAwesome
    {
        private:
            static FontAwesome *getInstance();
            FontAwesome();

        public:

            static QString brandsName();
            static QString regularName();
            static QString solidName();

            static QString richText(QString string);
            static QIcon icon(QString glpyhName, int pointSize, QColor colour);

        private:
            int m_brandsId;
            int m_regularId;
            int m_solidId;

            QString m_brandsName;
            QString m_regularName;
            QString m_solidName;

            QString m_styleString;

            QMap<QString, QString> m_glyphMap;
    };
}

#endif // FIZZYADE_FONTAWSOME_FONTAWESOME_H
