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

#ifndef NEDRYSOFT_FONTAWSOME_FONTAWESOME_H
#define NEDRYSOFT_FONTAWSOME_FONTAWESOME_H

#include <QString>
#include <QMap>
#include <QIcon>

#if ( defined(NEDRYSOFT_LIBRARY_FONTAWESOME_EXPORT))
#define NEDRYSOFT_FONTAWESOME_DLLSPEC Q_DECL_EXPORT
#else
#define NEDRYSOFT_FONTAWESOME_DLLSPEC Q_DECL_IMPORT
#endif

namespace Nedrysoft {
    /**
     * @brief           FontAwesome library
     *
     * @details         Provides helper functions to make use of the FontAwesome library
     *
     */
    class NEDRYSOFT_FONTAWESOME_DLLSPEC FontAwesome {
        private:
            /**
             * @brief       get the singleton instance of the font awesome object
             *
             * @return      the singleton object
             */
            static FontAwesome *getInstance();

            /**
             * @brief       Constructor
             *
             */
            FontAwesome();

        public:

            /**
             * @brief       brandsName
             *
             * @return      the name of the brands font
             *
             */
            static QString brandsName();

            /**
             * @brief       regularName
             *
             * @return      the name of the regular font
             *
             */
            static QString regularName();

            /**
             * @brief       solidName
             *
             * @return      the name of the solid font
             *
             */
            static QString solidName();

            /**
             * @brief       richText
             *
             * @details     takes a QString with tags in [fas|fab|far <glyph name>] and produces a HTML rich text
             *              which then includes the respective font awesome glyhs in
             *
             * @param[in]   string          text to convert
             *
             * @return      the resulting rich text
             *
             */
            static QString richText(QString string);

            /**
             * @brief       icon
             *
             * @details     creates q QIcon from the font-awesome glyph in the appropriate colour
             *
             * @param[in]   glpyhName       the name of the font awesome glpyh
             * @param[in]   pointSize       size in points
             * @param[in]   colour          the colour for the resulting glyph
             *
             * @return      the created QIcon
             */
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

#endif // NEDRYSOFT_FONTAWSOME_FONTAWESOME_H
