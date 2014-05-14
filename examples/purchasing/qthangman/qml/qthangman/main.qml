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

ApplicationWindow {
    id: mainWindow
    height: 480
    width: 320
    visible: true;

    Rectangle {
        id: mainRect
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
        anchors.fill: parent

        Loader {
            id: gameLoader
            asynchronous: true
            visible: status == Loader.Ready
            anchors.fill: parent
        }

        Loader {
            id: splashLoader
            anchors.fill: parent
            source: "SplashScreen.qml"
            onLoaded: gameLoader.source = "MainView.qml";
        }
    }
}
