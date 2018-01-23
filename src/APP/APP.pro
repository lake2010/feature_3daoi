TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
QT += core
QT += xml

SOURCES += \
    main.cpp \
    AppService.cpp \
    AppSetting.cpp \
    LoginWnd.cpp \
    MainWnd.cpp \
    PathSetting.cpp \
    Sequence.cpp \
    StartupWnd.cpp \
    UIManager.cpp \
    DataGenerator.cpp \
    DataGeneratorSetting.cpp \
    GlobalVariants.cpp

HEADERS += \
    AppService.hpp \
    AppSetting.hpp \
    LoginWnd.hpp \
    MainWnd.hpp \
    PathSetting.hpp \
    Sequence.hpp \
    StartupWnd.hpp \
    UIManager.hpp \
    DataGenerator.hpp \
    DataGeneratorSetting.hpp \
    GlobalVariants.hpp

INCLUDEPATH += $$PWD/../../include/sqlite
INCLUDEPATH += $$PWD/../../include

unix::LIBS += -L$$PWD/../lib/ -lsqlite3

unix::LIBS += -L/usr/lib/x86_64-linux-gnu\
-ldl

include(dependencies.pri)
