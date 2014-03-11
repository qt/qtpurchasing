LIBS += -framework StoreKit -framework Foundation
INCLUDEPATH += $$PWD
HEADERS += \
    inapppurchase/ios/qiosinapppurchaseproduct_p.h \
    inapppurchase/ios/qiosinapppurchasebackend_p.h \
    inapppurchase/ios/qiosinapppurchasetransaction_p.h

OBJECTIVE_SOURCES += \
    inapppurchase/ios/qiosinapppurchasebackend.mm \
    inapppurchase/ios/qiosinapppurchaseproduct.mm \
    inapppurchase/ios/qiosinapppurchasetransaction.mm
