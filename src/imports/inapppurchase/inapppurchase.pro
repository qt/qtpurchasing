CXX_MODULE = mobileextras
TARGET  = declarative_mobileextras
TARGETPATH = QtMobileExtras
IMPORT_VERSION = 1.0

QT += qml quick mobileextras
SOURCES += inapppurchase.cpp \
    qinappproductconsumable.cpp \
    qinappproductqmltype.cpp \
    qinappproductunlockable.cpp \
    qinappstoreqmltype.cpp

load(qml_plugin)

HEADERS += \
    qinappproductconsumable_p.h \
    qinappproductqmltype_p.h \
    qinappproductunlockable_p.h \
    qinappstoreqmltype_p.h

OTHER_FILES += qmldir
