CXX_MODULE = mobileextras
TARGET  = declarative_mobileextras
TARGETPATH = QtMobileExtras
IMPORT_VERSION = 1.0

QT += qml quick mobileextras mobileextras-private
SOURCES += \
    $$PWD/inapppurchase.cpp \
    $$PWD/qinappproductconsumable.cpp \
    $$PWD/qinappproductqmltype.cpp \
    $$PWD/qinappproductunlockable.cpp \
    $$PWD/qinappstoreqmltype.cpp

load(qml_plugin)

HEADERS += \
    $$PWD/qinappproductconsumable_p.h \
    $$PWD/qinappproductqmltype_p.h \
    $$PWD/qinappproductunlockable_p.h \
    $$PWD/qinappstoreqmltype_p.h

OTHER_FILES += qmldir
