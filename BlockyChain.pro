TEMPLATE = app
CONFIG += c++11 console
QT += core
CONFIG -= app_bundle

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    Block.cpp \
    Blockchain.cpp \
    JSON.cpp \
    Main.cpp \
    Transaction.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Block.h \
    Blockchain.h \
    Transaction.h

DISTFILES += \
    README.md \
    V2_Images/afterGenesisBlock.jpg \
    V2_Images/binaryFile.jpg \
    V2_Images/jsonFormattedChain.jpg \
    V2_Images/settingUpMiner.jpg \
    V2_Images/validationOfChain.jpg
