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
import QtPurchasing 1.0
import QtQuick.Controls 1.1

Rectangle {
    id: storeItem

    property Product product: undefined

    state: "NORMAL"

    visible: product.status == Product.Registered
    radius: 10
    color: "white"

    height: titleText.contentHeight + descriptionText.contentHeight + 16

    Text {
        id: titleText
        text: product.title
        font.bold: true
        anchors.right: priceText.left
        anchors.rightMargin: 8
        anchors.top: parent.top
        anchors.topMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 8
    }

    Text {
        id: descriptionText
        text: product.description
        anchors.right: priceText.left
        anchors.rightMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.top: titleText.bottom
        anchors.topMargin: 4
        wrapMode: Text.WordWrap
    }

    Text {
        id: priceText
        text: product.price
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.verticalCenter: parent.verticalCenter
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            pendingRect.visible = true;
            spinBox.visible = true;
            statusText.text = "Purchasing...";
            storeItem.state = "PURCHASING";
            product.purchase();
        }
        onPressed: {
            storeItem.state = "PRESSED";
        }
        onReleased: {
            storeItem.state = "NORMAL";
        }
    }

    Rectangle {
        id: pendingRect
        anchors.fill: parent
        opacity: 0.0
        color: "white"
        radius: parent.radius
        Text {
            id: statusText
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: spinBox.left
            anchors.margins: 8
            verticalAlignment: Text.AlignVCenter
        }
        BusyIndicator {
            id: spinBox
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.margins: 8
            width: height
        }

        Connections {
            target: product
            onPurchaseSucceeded: {
                statusText.text = "Purchase Succeeded";
                spinBox.visible = false;

            }
            onPurchaseFailed: {
                statusText.text = "Purchase Failed";
                spinBox.visible = false;
                storeItem.state = "NORMAL";
            }
        }
    }

    states: [
        State {
            name: "NORMAL"
            PropertyChanges {
                target: storeItem
                color: "white"
                border.color: "transparent"
            }
            PropertyChanges {
                target: pendingRect
                opacity: 0.0
            }
        },
        State {
            name: "PRESSED"
            PropertyChanges {
                target: storeItem
                color: "transparent"
                border.color: "white"
            }
        },
        State {
            name: "PURCHASING"
            PropertyChanges {
                target: pendingRect
                opacity: 1.0
            }
        }
    ]

    transitions: [
        Transition {
            from: "PURCHASING"
            to: "NORMAL"
            NumberAnimation { target: pendingRect; property: "opacity"; duration: 2000; easing.type: Easing.InExpo }
        }
    ]
}
