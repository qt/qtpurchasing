CXX_MODULE = purchasing
TARGET  = declarative_purchasing
TARGETPATH = QtPurchasing
IMPORT_VERSION = 1.1

QT += qml quick purchasing purchasing-private
SOURCES += \
    $$PWD/inapppurchase.cpp \
    $$PWD/qinappproductqmltype.cpp \
    $$PWD/qinappstoreqmltype.cpp

load(qml_plugin)

HEADERS += \
    $$PWD/qinappproductqmltype_p.h \
    $$PWD/qinappstoreqmltype_p.h

OTHER_FILES += qmldir
