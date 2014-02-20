TARGET = QtMobileExtras
QT = core

load(qt_module)

ANDROID_BUNDLED_JAR_DEPENDENCIES = \
    jar/QtMobileExtras-bundled.jar
ANDROID_JAR_DEPENDENCIES = \
    jar/QtMobileExtras.jar

HEADERS += \
    $$PWD/qmobileextrasglobal.h \
    $$PWD/qinapptransaction_p.h

include(inapppurchase/inapppurchase.pri)
