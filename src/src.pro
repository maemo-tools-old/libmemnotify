VERSION = 0.1.0
TEMPLATE = lib
INCLUDEPATH=.
HEADERS += memnotify/definitions.hpp
# MemoryNotification CachedFile Watcher EventfdWatcher InitifyWatcher WatcherBuilder Platform
#SOURCES += MemoryNotification.cpp CachedFile.cpp Watcher.cpp EventfdWatcher.cpp InitifyWatcher.cpp WatcherBuilder.cpp Platform.cpp
SOURCES +=
DEFINES += QT_DLL MEMNOTIFY_SHARED MEMNOTIFY_VERSION=\\\"$${VERSION}\\\"
CONFIG  += qt warn_on debug_and_release build_all
QT -= gui
