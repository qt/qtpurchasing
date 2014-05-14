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
    property alias text: label.text

    Text {
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        id: label
        color: "white"
        font.pixelSize: parent.height * 0.75
        font.family: "Helvetica Neue"

        opacity: applicationData.lettersOwned.indexOf(text) >= 0 ? 1.0 : 0.0
        visible: opacity > 0.0

        anchors.horizontalCenterOffset: visible ? 0 : -topLevel.width / 2

        Behavior on anchors.horizontalCenterOffset {
            NumberAnimation {
                duration: 500
                easing.type: Easing.OutQuad
            }
        }
    }

    Rectangle {
        color: "white"
        width: parent.width
        anchors.bottom: parent.bottom
        anchors.top: label.bottom
    }
}
