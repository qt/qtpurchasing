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

Rectangle {
    id: topLevel
    gradient: Gradient {
        GradientStop {
            position: 0.0
            color: "#87E0FD"
        }
        GradientStop {
            position: 0.4
            color: "#53CBF1"
        }
        GradientStop {
            position: 1.0
            color: "#05ABE0"
        }
    }

    Hangman {
        id: logo
        anchors.centerIn: parent
        height: 268
        width: 268
        errorCount: 9
    }

    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: logo.bottom
        anchors.topMargin: 10
        text: "Qt Hangman"
        font.family: ".Helvetica Neue Interface -M3"
        color: "white"
        font.pointSize: 24
    }
}
