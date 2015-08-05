INCLUDEPATH += $$PWD

ANDROID_PERMISSIONS += \
    com.android.vending.BILLING

HEADERS += \
    $$PWD/qinappstore.h \
    $$PWD/qinappproduct.h \
    $$PWD/qinapptransaction.h \
    $$PWD/qinappstore_p.h \
    $$PWD/qinapppurchasebackend_p.h \
    $$PWD/qinapppurchasebackendfactory_p.h

SOURCES += \
    $$PWD/qinappproduct.cpp \
    $$PWD/qinapptransaction.cpp \
    $$PWD/qinappstore.cpp \
    $$PWD/qinapppurchasebackend.cpp \
    $$PWD/qinapppurchasebackendfactory.cpp

android {
    QT += androidextras
    include ($$PWD/android/android.pri)
}

mac {
    include ($$PWD/mac/mac.pri)
}
