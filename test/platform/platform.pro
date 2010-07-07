VERSION = 0.1.0
TEMPLATE = app
SRC = ../../src
INCLUDEPATH=$$SRC
SOURCES += $$SRC/cached_file.cpp
SOURCES += $$SRC/platform.cpp
SOURCES += test-platform.cpp
DEFINES += MEMNOTIFY_VERSION=\\\"$${VERSION}\\\"
CONFIG  += qt warn_on debug build_all
QT -= gui

target.path = /usr/lib/libmemnotifyqt/tests

INSTALLS += target
