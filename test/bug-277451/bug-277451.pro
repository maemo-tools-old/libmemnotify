TEMPLATE = app

SRC = ../../src
INCLUDEPATH=$$SRC
HEADERS += $$SRC/memnotify/definitions.hpp
HEADERS += $$SRC/memnotify/platform.hpp
HEADERS += $$SRC/memnotify/cached_file.hpp
HEADERS += $$SRC/memnotify/watcher.hpp
HEADERS += $$SRC/memnotify/memory_notification.hpp
HEADERS += $$SRC/poller.hpp
SOURCES += $$SRC/cached_file.cpp
SOURCES += $$SRC/platform.cpp
SOURCES += $$SRC/cgroups_watcher.cpp
SOURCES += $$SRC/debug_watcher.cpp
SOURCES += $$SRC/memnotify_watcher.cpp
SOURCES += $$SRC/memory_notification.cpp
SOURCES += $$SRC/poller.cpp
SOURCES += $$SRC/watcher_builder.cpp
SOURCES += $$SRC/watcher.cpp
SOURCES += main.cpp

CONFIG  += qt warn_on debug build_all

target.path = /usr/lib/libmemnotifyqt/tests

INSTALLS += target
