/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt Purchasing module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.2

Item {
    PageHeader {
        id: header
        title: "How to Play"
    }

    Flickable {
        anchors.top: header.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.margins: 8
        clip: true

        contentHeight: helpContent.height + 16

        Item {
            id: helpContent
            width: parent.width
            height: contentColumn.height
            Column {
                id: contentColumn
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.margins: 8
                spacing: 8
                Text {
                    height: contentHeight
                    width: parent.width
                    wrapMode: Text.Wrap
                    font.family: ".Helvetica Neue Interface -M3"
                    color: "white"
                    font.pointSize: 16
                    text: "\
Hangman is a classic word game where the objective is to guess a given word \
before you make too many mistakes and the hangman gets hung.\n"
                }

                Word {
                    anchors.margins: 8
                    height: 32
                    width: parent.width * .8
                    text: "HANGMAN"
                }

                Text {
                    height: contentHeight
                    width: parent.width
                    wrapMode: Text.Wrap
                    font.family: ".Helvetica Neue Interface -M3"
                    color: "white"
                    font.pointSize: 16
                    text: "\
\nYou play by guessing letters. If you guess a letter that is part of the word \
it will be shown in any locations in the word it is located. If however it is \
not part of the word, another piece will be added and the hangman will be one \
step closer to death. \n"
                }

                Hangman {
                    height: 150
                    width: 150
                    errorCount: 9
                }

                Text {
                    height: contentHeight
                    width: parent.width
                    wrapMode: Text.Wrap
                    font.family: ".Helvetica Neue Interface -M3"
                    color: "white"
                    font.pointSize: 16
                    text: "\
\nVowels must be purchased, unlocked or earned to be used.  If you guess a word, \
any vowels that have not been guess already will be added to your vowel pool."
                }

                ScoreItem {
                    anchors.margins: 8
                    height: 48
                }

                Text {
                    height: contentHeight
                    width: parent.width
                    wrapMode: Text.Wrap
                    font.family: ".Helvetica Neue Interface -M3"
                    color: "white"
                    font.pointSize: 16
                    text: "\
When you guess a word you are rewarded points.  You receive a point for each \
consonant that was guessed as well as a point for any remaining parts of the \
hangman.  You can not do anything with points, they just show how awesome you are.
"
                }
            }
        }
    }
}
