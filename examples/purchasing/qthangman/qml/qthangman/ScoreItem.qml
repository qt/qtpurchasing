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
import QtQuick.Controls 1.1

Rectangle {
    id: borderRect
    color: "transparent"
    border.color: "white"
    width: scoreText.contentWidth + (topLevel.globalMargin * 2)
    radius: 10
    Label {
        id: scoreText
        anchors.fill: parent
        anchors.topMargin: topLevel.globalMargin
        anchors.bottomMargin: topLevel.globalMargin
        anchors.rightMargin: topLevel.globalMargin
        anchors.leftMargin: topLevel.globalMargin
        horizontalAlignment: Text.AlignRight
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: parent.height
        font.family: "Helvetica Neue"
        font.weight: Font.Light
        text: applicationData.score
        color: "white"
    }
}
