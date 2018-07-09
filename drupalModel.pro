QT += quick network
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += leveldb/include


SOURCES += main.cpp \
    drupalmodel.cpp \


LIBS += -L/leveldb/build -lleveldb


RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    drupalmodel.h \









win32:CONFIG(release, debug|release): LIBS += -L$$PWD/leveldb/build/ -lleveldb
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/leveldb/build/ -lleveldbd
else:unix: LIBS += -L$$PWD/leveldb/build/ -lleveldb

INCLUDEPATH += $$PWD/leveldb/include/leveldb
DEPENDPATH += $$PWD/leveldb/include/leveldb

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/leveldb/build/libleveldb.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/leveldb/build/libleveldbd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/leveldb/build/leveldb.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/leveldb/build/leveldbd.lib
else:unix: PRE_TARGETDEPS += $$PWD/leveldb/build/libleveldb.a
