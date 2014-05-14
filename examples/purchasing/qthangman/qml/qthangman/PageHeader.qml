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
    id: header

    property alias title: titleText.text
    property int buttonHeight: topLevel.buttonHeight
    signal clicked()


    anchors.top: parent.top
    anchors.left: parent.left
    anchors.right: parent.right
    height: buttonHeight + (topLevel.globalMargin * 2)
    SimpleButton {
        id: backButton
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: topLevel.globalMargin
        height: buttonHeight
        width: parent.width * 0.25
        text: "Back"
        onClicked: {
            pageStack.pop();
            header.clicked();
        }
    }
    Text {
        id: titleText
        anchors.left: backButton.right
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: topLevel.globalMargin
        font.family: ".Helvetica Neue Interface -M3"
        color: "white"
        font.pointSize: 64
        fontSizeMode: Text.Fit
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        minimumPointSize: 8
    }
}
