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
# MemoryNotification CachedFile Watcher EventfdWatcher InitifyWatcher WatcherBuilder Platform
SOURCES += cached_file.cpp cgroups_watcher.cpp debug_watcher.cpp memory_notification.cpp platform.cpp poller.cpp watcher_builder.cpp watcher.cpp
DEFINES += QT_DLL MEMNOTIFY_SHARED MEMNOTIFY_VERSION=\\\"$${VERSION}\\\"
CONFIG  += qt warn_on debug_and_release build_all
QT -= gui

