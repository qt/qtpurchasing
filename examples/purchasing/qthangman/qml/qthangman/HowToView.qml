/****************************************************************************
**
** Copyright (C) 2014 Digia Plc
** All rights reserved.
** For any questions to Digia, please use contact form at http://qt.digia.com
**
** This file is part of the Qt Purchasing Add-on.
**
** $QT_BEGIN_LICENSE$
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.
**
** If you have questions regarding the use of this file, please use
** contact form at http://qt.digia.com
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
        id: helpFlickable
        anchors.top: header.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.margins: topLevel.globalMargin
        clip: true

        contentHeight: helpContent.height + (topLevel.globalMargin * 2)

        Item {
            id: helpContent
            width: parent.width
            height: contentColumn.height
            Column {
                id: contentColumn
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.margins: topLevel.globalMargin
                spacing: topLevel.globalMargin
                Text {
                    height: contentHeight
                    width: parent.width
                    wrapMode: Text.Wrap
                    font.family: ".Helvetica Neue Interface -M3"
                    color: "white"
                    font.pixelSize: helpFlickable.height * 0.04
                    text: "\
Hangman is a classic word game where the objective is to guess a given word \
before you make too many mistakes and the hangman gets hung.\n"
                }

                Word {
                    anchors.margins: topLevel.globalMargin
                    height: topLevel.buttonHeight
                    width: parent.width * .8
                    text: "HANGMAN"
                }

                Text {
                    height: contentHeight
                    width: parent.width
                    wrapMode: Text.Wrap
                    font.family: ".Helvetica Neue Interface -M3"
                    color: "white"
                    font.pixelSize: helpFlickable.height * 0.04
                    text: "\
\nYou play by guessing letters. If you guess a letter that is part of the word \
it will be shown in any locations in the word it is located. If however it is \
not part of the word, another piece will be added and the hangman will be one \
step closer to death. \n"
                }

                Hangman {
                    height: width
                    width: parent.width / 2
                    errorCount: 9
                }

                Text {
                    height: contentHeight
                    width: parent.width
                    wrapMode: Text.Wrap
                    font.family: ".Helvetica Neue Interface -M3"
                    color: "white"
                    font.pixelSize: helpFlickable.height * 0.04
                    text: "\
\nVowels must be purchased, unlocked or earned to be used.  If you guess a word, \
any vowels that have not been guess already will be added to your vowel pool."
                }

                ScoreItem {
                    anchors.margins: topLevel.globalMargin
                    height: topLevel.buttonHeight
                }

                Text {
                    height: contentHeight
                    width: parent.width
                    wrapMode: Text.Wrap
                    font.family: ".Helvetica Neue Interface -M3"
                    color: "white"
                    font.pixelSize: helpFlickable.height * 0.04
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
