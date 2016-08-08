TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += main.cpp \
    tcpserver.cpp

RESOURCES += qml.qrc

LIBS        += -lboost_system -lpthread -lboost_regex

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    tcpserver.h
