QT       += core gui widgets

TARGET = GitIHM
TEMPLATE = app

CONFIG += c++11

DESTDIR = ./../build

SOURCES += \
        src/gui/BranchWindow.cpp \
        src/gui/ErrorViewer.cpp \
        src/gui/TagsWindow.cpp \
        src/main.cpp \
        src/gui/MainWindow.cpp \
        src/tools/Context.cpp \
        src/tools/Logger.cpp

HEADERS += \
        inc/gui/BranchWindow.hpp \
        inc/gui/ErrorViewer.hpp \
        inc/gui/MainWindow.hpp \
        inc/gui/TagsWindow.hpp \
        inc/tools/Context.hpp \
        inc/tools/Logger.hpp

INCLUDEPATH += inc/gui \
        inc/tools

FORMS += \
        form/BranchWindow.ui \
        form/ErrorViewer.ui \
        form/MainWindow.ui \
        form/TagsWindow.ui

RESOURCES += \
    ressources/darkstyle.qrc
