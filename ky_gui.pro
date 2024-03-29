#-------------------------------------------------
#
# Project created by QtCreator 2019-06-22T17:19:11
#
#-------------------------------------------------

QT += core gui
QT += printsupport
QT += network
QT += svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ky_gui
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS SPDLOG_COMPILED_LIB DEBUG_CLIENT

CONFIG += c++17
CONFIG += static

QMAKE_CXXFLAGS += -fno-sized-deallocation

SOURCES += src/main.cpp \
           src/window/main/mainwindow.cpp \
           src/window/main/controller.cpp \
           src/window/document/documentwindow.cpp \
           src/window/document/helpers.cpp \
           src/window/research/researchwindow.cpp \
           src/util/util.cpp \
           src/client/client.cpp \
           src/client/console.cpp \
           src/task/instagram_task.cpp \
           src/task/generic_task.cpp \
           src/dialog/appdialog.cpp \
           src/dialog/argdialog.cpp \
           src/dialog/messagedialog.cpp \
           src/dialog/postdialog.cpp \
           src/dialog/scheduledialog.cpp \
           src/dialog/postmodel.cpp \
           src/component/connection_indicator.cpp \
           src/component/kfiledialog.cpp \
           src/component/status_delegate.cpp \
           src/component/button_delegate.cpp \
           third_party/klogger/src/logger.cpp

HEADERS += include/ui/documentwindow.hpp \
           include/ui/appdialog.hpp \
           include/ui/argdialog.h \
           include/ui/messagedialog.hpp \
           include/ui/postdialog.hpp \
           include/ui/scheduledialog.hpp \
           include/ui/mainwindow.h \
           include/ui/researchwindow.hpp \
           include/ui/connection_indicator.h \
           include/ui/kfiledialog.h \
           include/ui/status_delegate.hpp \
           include/ui/button_delegate.hpp \
           include/client/client.hpp \
           include/client/console.hpp \
           include/task/task.hpp \
           include/task/instagram_task.hpp\
           include/task/generic_task.hpp\
           src/window/document/helpers.hpp \
           third_party/kiqoder/kiqoder.hpp \
           third_party/kiqoder/file_iterator.hpp \
           third_party/klogger/src/logger.hpp \
           third_party/klogger/src/spdlog/spdlog.h \
           headers/kmessage_codec.hpp \
           headers/instatask_generated.h \
           headers/generictask_generated.h \
           src/util/util.hpp \
           headers/kiq_types.hpp \
           src/util/rapidjson/writer.h \
           src/util/rapidjson/stringbuffer.h \
           src/util/rapidjson/document.h

FORMS += appdialog.ui \
         argdialog.ui \
         consoledialog.ui \
         documentwindow.ui \
         mainwindow.ui \
         messagedialog.ui \
         postdialog.ui \
         researchwindow.ui \
         scheduledialog.ui

LIBS += third_party/klogger/src/libspdlog.a

INCLUDEPATH += third_party/klogger/src src third_party/kproto/include third_party/kutils/include

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    kres.qrc
