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
    id: button
    property string text: ""
    property color buttonColor: "white"
    property color textColor: "black"
    property bool available: true
    property alias fontPointSize: buttonText.font.pointSize

    signal clicked()

    state: "NORMAL"

    Rectangle {
        id: buttonRect
        anchors.fill: parent
        radius: 10
        color: buttonColor
        visible: button.available
        Text {
            id: buttonText
            anchors.fill: parent
            anchors.rightMargin: parent.width * 0.05
            anchors.leftMargin: parent.width * 0.05
            anchors.bottomMargin: parent.height * 0.20
            anchors.topMargin: parent.height * 0.20
            text: button.text
            color: textColor
            fontSizeMode: Text.Fit
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            minimumPointSize: 8
            font.pointSize: 64
            font.family: "Helvetica Neue"
            font.weight: Font.Light
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            button.clicked();
        }
        onPressed: {
            button.state = "PRESSED";
        }
        onReleased: {
            button.state = "NORMAL";
        }
    }

    states: [
        State {
            name: "NORMAL"
            PropertyChanges {
                target: buttonRect
                color: button.buttonColor
                border.color: "transparent"
            }
            PropertyChanges {
                target: buttonText
                color: button.textColor
            }
        },
        State {
            name: "PRESSED"
            PropertyChanges {
                target: buttonRect
                color: "transparent"
                border.color: button.buttonColor
            }
            PropertyChanges {
                target: buttonText
                color: button.buttonColor
            }
        }
    ]
}
