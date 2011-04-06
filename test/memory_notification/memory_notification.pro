VERSION = 0.1.0
TEMPLATE = app
SRC = ../../src
INCLUDEPATH=$$SRC
HEADERS += $$SRC/memnotify/definitions.hpp
HEADERS += $$SRC/memnotify/platform.hpp
HEADERS += $$SRC/memnotify/cached_file.hpp
HEADERS += $$SRC/memnotify/watcher.hpp
HEADERS += $$SRC/memnotify/memory_notification.hpp
HEADERS += $$SRC/poller.hpp
HEADERS += test-memory_notification.hpp
SOURCES += $$SRC/cached_file.cpp
SOURCES += $$SRC/cgroups_watcher.cpp 
SOURCES += $$SRC/debug_watcher.cpp 
SOURCES += $$SRC/memory_notification.cpp
SOURCES += $$SRC/platform.cpp
SOURCES += $$SRC/poller.cpp 
SOURCES += $$SRC/watcher_builder.cpp 
SOURCES += $$SRC/watcher.cpp
SOURCES += test-memory_notification.cpp
DEFINES += UNIT_TEST MEMNOTIFY_VERSION=\\\"$${VERSION}\\\"
CONFIG  += qt warn_on debug build_all
QT -= gui

target.path = /usr/lib/libmemnotifyqt/tests

extra.files = debug.mn cgroup.nm sensor.txt
extra.path = /usr/lib/libmemnotifyqt/tests

INSTALLS += target extra
