VERSION = 0.1.0
TEMPLATE = lib
TARGET = memnotifyqt
INCLUDEPATH=.
HEADERS += memnotify/definitions.hpp 
HEADERS += memnotify/cached_file.hpp 
HEADERS += memnotify/platform.hpp 
HEADERS += memnotify/poller.hpp 
HEADERS += memnotify/watcher.hpp 
HEADERS += memnotify/watcher_builder.hpp
HEADERS += memnotify/memory_notification.hpp
SOURCES += cached_file.cpp 
SOURCES += platform.cpp 
SOURCES += watcher.cpp
SOURCES += watcher_builder.cpp 
SOURCES += cgroups_watcher.cpp 
SOURCES += debug_watcher.cpp 
SOURCES += poller.cpp 
SOURCES += memory_notification.cpp 
DEFINES += QT_DLL MEMNOTIFY_SHARED MEMNOTIFY_VERSION=\\\"$${VERSION}\\\"
CONFIG  += qt warn_on debug_and_release build_all
QT -= gui

