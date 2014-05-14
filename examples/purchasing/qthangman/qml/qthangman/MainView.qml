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
import com.digia.qthangman 1.0
import QtPurchasing 1.0

Item {
    id: topLevel

    height: 480
    width: 320
    property real buttonHeight: 0
    property real globalMargin: width * 0.025

    Component.onCompleted: forceActiveFocus()
    Keys.onBackPressed: {
        if (pageStack.depth === 1) {
            Qt.quit()
        } else {
            pageStack.pop()
            event.accepted = true
            forceActiveFocus()
        }
    }
    focus: true

    HangmanGame {
        id: applicationData
        onWordChanged: {
            if (word.length > 0)
                splashLoader.source = "";
        }
    }

    StackView {
        id: pageStack
        anchors.fill: topLevel
        initialItem: Qt.resolvedUrl("GameView.qml")
    }
    // ![0]
    Store {
        id: iapStore
    }
    // ![0]
}
