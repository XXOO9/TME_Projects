INCLUDEPATH += $$PWD/../../bindCard/thirdPart/RFID1356/include

LIBS += -L$$PWD/../../bindCard/thirdPart/RFID1356/lib/x64/ -lRFID1356

HEADERS += \
    $$PWD/CIdentificDevNFC.h

SOURCES += \
    $$PWD/CIdentificDevNFC.cpp
