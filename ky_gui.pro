#-------------------------------------------------
#
# Project created by QtCreator 2019-06-22T17:19:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ky_gui
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++17
CONFIG += static

QMAKE_CXXFLAGS += "-fno-sized-deallocation"

SOURCES += \
        src/main.cpp \
        src/mainwindow.cpp \
        src/client.cpp \
        src/instagram_task.cpp\
        src/argdialog.cpp \
        src/consoledialog.cpp \
        src/connection_indicator.cpp \
        src/kfiledialog.cpp \

        src/connection_indicator.cpp
HEADERS += \
        include/ui/argdialog.h \
        include/ui/consoledialog.h \
        include/ui/mainwindow.h \
        include/ui/connection_indicator.h \
        include/ui/kfiledialog.h \
        include/client/client.hpp \
        include/task/task.hpp \
        include/task/instagram_task.hpp\
        headers/kmessage_codec.hpp \
        headers/instatask_generated.h \
        headers/json.hpp \
        headers/util.hpp \
        headers/kiq_types.hpp \
        headers/rapidjson/writer.h \
        headers/rapidjson/stringbuffer.h \
        headers/rapidjson/document.h \


FORMS += \
        argdialog.ui \
        consoledialog.ui \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    kres.qrc
