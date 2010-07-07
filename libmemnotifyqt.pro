TEMPLATE = subdirs

SUBDIRS = \
          doc \
          src \
          test

CONFIG += release

QMAKE_CLEAN += configure-stamp

pc.files = libmemnotifyqt.pc
pc.path = /usr/lib/pkgconfig

INSTALLS += pc
