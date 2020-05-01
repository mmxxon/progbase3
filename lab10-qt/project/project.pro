
QT += core gui sql xml widgets
CONFIG += c++1z

TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    fndradd.cpp \
    fndredit.cpp \
    orgadd.cpp \
    orgedit.cpp \
    storage.cpp \
    sqlite_storage.cpp \
    mainwindow.cpp \
    main.cpp \
    useredit.cpp

HEADERS += \
    fndradd.hpp \
    fndredit.hpp \
    founder.h \
    orgadd.hpp \
    organisation.h \
    orgedit.hpp \
    user.h \
    storage.h \
    sqlite_storage.h \
    mainwindow.h \
    useredit.hpp

FORMS += \
  fndradd.ui \
  fndredit.ui \
  mainwindow.ui \
  orgadd.ui \
  orgedit.ui \
  useredit.ui

