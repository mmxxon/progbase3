QT       -= gui

TARGET = csvlab
TEMPLATE = lib
CONFIG += staticlib
DEFINES += QT_DEPRECATED_WARNINGS
SOURCES +=               \
        csvlab.cpp       \
        string_table.cpp \

HEADERS +=               \
  csvlab.hpp \ \
  string_table.hpp

unix {
    target.path = /usr/lib
    INSTALLS += target
}
