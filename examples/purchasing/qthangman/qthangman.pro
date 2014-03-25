QT += quick qml
!android: !ios: !blackberry: qtHaveModule(widgets): QT += widgets

SOURCES += main.cpp \
    hangmangame.cpp

HEADERS += \
    hangmangame.h

RESOURCES += \
    resources.qrc

QT += purchasing

OTHER_FILES += \
    qml/qthangman/GameView.qml \
    qml/qthangman/GuessWordView.qml \
    qml/qthangman/Hangman.qml \
    qml/qthangman/HowToView.qml \
    qml/qthangman/Key.qml \
    qml/qthangman/Letter.qml \
    qml/qthangman/LetterSelector.qml \
    qml/qthangman/main.qml \
    qml/qthangman/MainView.qml \
    qml/qthangman/PageHeader.qml \
    qml/qthangman/ScoreItem.qml \
    qml/qthangman/SimpleButton.qml \
    qml/qthangman/SplashScreen.qml \
    qml/qthangman/StoreItem.qml \
    qml/qthangman/StoreView.qml \
    qml/qthangman/Word.qml \
    enable2.txt

ios {
    QMAKE_TARGET_BUNDLE_PREFIX = "com.digia.qt.iosteam"
}
