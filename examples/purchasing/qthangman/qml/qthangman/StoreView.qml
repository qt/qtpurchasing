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
import QtQuick.Layouts 1.1
import QtPurchasing 1.0

Item {
    PageHeader {
        id: header
        title: "Hangman Store"
    }

    Column {
        anchors.top: header.bottom
        anchors.bottom: restoreButton.top
        anchors.margins: topLevel.globalMargin
        anchors.right: parent.right
        anchors.left: parent.left
        spacing: topLevel.globalMargin
        // ![2]
        StoreItem {
            product: product100Vowels
            width: parent.width
        }

        StoreItem {
            product: productUnlockVowels
            width: parent.width
        }
        // ![2]
    }

    SimpleButton {
        id: restoreButton
        anchors.bottom: parent.bottom
        anchors.margins: topLevel.globalMargin
        anchors.horizontalCenter: parent.horizontalCenter
        height: topLevel.buttonHeight
        width: parent.width * .5
        text: "Restore Purchases"
        onClicked: {
            iapStore.restorePurchases();
        }
    }

    // ![0]
    Product {
        id: product100Vowels
        store: iapStore
        type: Product.Consumable
        identifier: "org.qtproject.qthangman.100vowels"

        onPurchaseSucceeded: {
            console.log(identifier + " purchase successful");
            applicationData.vowelsAvailable += 100;
            transaction.finalize();
            pageStack.pop();
        }

        onPurchaseFailed: {
            console.log(identifier + " purchase failed");
            console.log("reason: "
                        + transaction.failureReason === Transaction.CanceledByUser ? "Canceled" : transaction.errorString);
            transaction.finalize();
        }
    }
    // ![0]
    // ![1]
    Product {
        id: productUnlockVowels
        type: Product.Unlockable
        store: iapStore
        identifier: "org.qtproject.qthangman.unlockvowels"

        onPurchaseSucceeded: {
            console.log(identifier + " purchase successful");
            applicationData.vowelsUnlocked = true;
            transaction.finalize();
            pageStack.pop();
        }

        onPurchaseFailed: {
            console.log(identifier + " purchase failed");
            console.log("reason: "
                        + transaction.failureReason === Transaction.CanceledByUser ? "Canceled" : transaction.errorString);
            transaction.finalize();
        }

        onPurchaseRestored: {
            console.log(identifier + "purchase restored");
            applicationData.vowelsUnlocked = true;
            console.log("timestamp: " + transaction.timestamp);
            transaction.finalize();
            pageStack.pop();
        }
    }
    // ![1]

}
