VERSION = 1.0.0
TEMPLATE = lib
TARGET = memnotifyqt
INCLUDEPATH=.
HEADERS += memnotify/definitions.hpp 
HEADERS += memnotify/cached_file.hpp 
HEADERS += memnotify/platform.hpp 
HEADERS += memnotify/watcher.hpp 
HEADERS += memnotify/watcher_builder.hpp
HEADERS += memnotify/memory_notification.hpp
HEADERS += poller.hpp 
SOURCES += cached_file.cpp 
SOURCES += platform.cpp 
SOURCES += watcher.cpp
SOURCES += watcher_builder.cpp 
SOURCES += cgroups_watcher.cpp 
SOURCES += debug_watcher.cpp 
SOURCES += poller.cpp 
SOURCES += memory_notification.cpp 
DEFINES += QT_DLL MEMNOTIFY_SHARED MEMNOTIFY_VERSION=\\\"$${VERSION}\\\"
CONFIG  += qt warn_on release build_all
QT -= gui

target.path = /usr/lib

includes.path = /usr/include
includes.files = memnotify

INSTALLS += includes target
