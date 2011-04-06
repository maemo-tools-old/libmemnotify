VERSION = 0.1.0
TEMPLATE = app
SRC = ../../src
INCLUDEPATH=$$SRC
SOURCES += $$SRC/cached_file.cpp
SOURCES += $$SRC/cgroups_watcher.cpp 
SOURCES += $$SRC/debug_watcher.cpp 
SOURCES += $$SRC/platform.cpp
SOURCES += $$SRC/watcher_builder.cpp 
SOURCES += $$SRC/watcher.cpp
SOURCES += test-watcher_builder.cpp
DEFINES += UNIT_TEST MEMNOTIFY_VERSION=\\\"$${VERSION}\\\"
CONFIG  += qt warn_on debug build_all
QT -= gui

target.path = /usr/lib/libmemnotifyqt/tests

INSTALLS += target
