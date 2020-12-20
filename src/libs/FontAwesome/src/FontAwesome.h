/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of fontawesome for qt (https://github.com/fizzyade/fontawesome)
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

#ifndef NEDRYSOFT_FONTAWESOME_FONTAWESOME_H
#define NEDRYSOFT_FONTAWESOME_FONTAWESOME_H

#include <QIcon>
#include <QMap>
#include <QString>

#if ( defined(NEDRYSOFT_LIBRARY_FONTAWESOME_EXPORT))
#define NEDRYSOFT_FONTAWESOME_DLLSPEC Q_DECL_EXPORT
#else
#define NEDRYSOFT_FONTAWESOME_DLLSPEC Q_DECL_IMPORT
#endif

namespace Nedrysoft {
    /**
     * @brief           The FontAwesome class provides functions to use the FontAwesome library.
     */
    class NEDRYSOFT_FONTAWESOME_DLLSPEC FontAwesome {
        private:
            /**
             * @brief       Gets the singleton instance of the font awesome object.
             *
             * @return      the singleton object.
             */
            static FontAwesome *getInstance();

            /**
             * @brief       Constructs a new FontAwesome instance.
             *
             */
            FontAwesome();

        public:
            /**
              * @brief       Returns the name of the brands font.
              *
              * #returns     the name of the brands font.
              */
            static QString brandsName();

            /**
             * @brief       Returns the name of the regular font.
             *
             * @returns     the name of the regular font.
             *
             */
            static QString regularName();

            /**
             * @brief       Returns the name of the solid font.
             *
             * @returns     the name of the solid font.
             *
             */
            static QString solidName();

            /**
             * @brief       Returns a rich text with FontAwesome glyphs.
             *
             * @details     takes a QString with tags in [fas|fab|far <glyph name>] and produces a HTML rich text
             *              which then includes the respective font awesome glyhs in
             *
             * @param[in]   string the text to convert.
             *
             * @returns     the rich text.
             */
            static QString richText(QString string);

            /**
             * @brief       Returns a QIcon of a font anwesome glyph.
             *
             * @param[in]   glpyhName the name of the font awesome glyph,
             * @param[in]   pointSize the size in points of the icon.
             * @param[in]   colour the colour for the resulting icon.
             *
             * @return      the QIcon of the FontAwesome glyph.
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

#endif // NEDRYSOFT_FONTAWESOME_FONTAWESOME_H
