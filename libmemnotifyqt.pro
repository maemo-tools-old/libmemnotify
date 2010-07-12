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

policy.files = default.mn
policy.path = /etc/memnotify

INSTALLS += policy
