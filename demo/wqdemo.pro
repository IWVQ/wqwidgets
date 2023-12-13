QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp\
    ../src/wqcolorbutton.cpp \
    ../src/wqfontbutton.cpp \
    ../src/wqledlabel.cpp \
    ../src/wqledcodec.cpp \
    ../src/wqoptionbox.cpp\
    ../src/wqslider.cpp\
    ../src/wqspinbox.cpp\
    ../src/wqswitch.cpp\
    ../src/wqnotifier.cpp\
    ../src/wqmenubox.cpp

HEADERS += \
    mainwindow.h\
    ../src/wqcolorbutton.h \
    ../src/wqfontbutton.h \
    ../src/wqledlabel.h \
    ../src/wqledcodec.h \
    ../src/wqoptionbox.h\
    ../src/wqslider.h\
    ../src/wqspinbox.h\
    ../src/wqswitch.h\
    ../src/wqnotifier.h\
    ../src/wqmenubox.h

FORMS += \
    mainwindow.ui

INCLUDEPATH += $$PWD/../src

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    img.qrc
