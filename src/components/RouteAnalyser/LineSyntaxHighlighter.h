/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of Pingnoo (https://github.com/nedrysoft/pingnoo)
 *
 * An open-source cross-platform traceroute analyser.
 *
 * Created by Adrian Carpenter on 17/12/2020.
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

#ifndef NEDRYSOFT_LINESYNTAXHIGHLIGHTER_H
#define NEDRYSOFT_LINESYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextDocument>

namespace Nedrysoft::RouteAnalyser {
    using SyntaxFunction = std::function<bool(const QString &text)>;

    /**
     * @brief The LineSyntaxHighlighter provides a basic complete line syntax highlighter.
     */
    class LineSyntaxHighlighter :
            public QSyntaxHighlighter {

        public:
            /**
             * @brief       Constructs a new LineSyntaxHighlighter for the given document and syntax function.
             *
             * @param[in]   parent the QTextDocument this highlighter operates on.
             * @param[in]   function the syntax highlighter function, determines whether to highlight.
             */
            LineSyntaxHighlighter(QTextDocument *parent, SyntaxFunction function);

            /**
             * @brief       Reimplements QSyntaxHighlighter::highlightBlock(const QString &text)
             *
             * @param[in]   text the text to apply the syntax highlighting to.
             */
            auto highlightBlock(const QString &text) -> void override;

            /**
             * @brief       This function is used to force a re-highlight the document.
             */
            auto updateSyntax() -> void;

        private:
            SyntaxFunction m_syntaxFunction;
    };

}

#endif //NEDRYSOFT_LINESYNTAXHIGHLIGHTER_H
