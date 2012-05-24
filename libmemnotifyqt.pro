TEMPLATE = subdirs

SUBDIRS = src test

CONFIG += release

QMAKE_CLEAN += configure-stamp

pc.files = libmemnotifyqt.pc
pc.path = /usr/lib/pkgconfig
INSTALLS += pc

texts.files = doc/FAQ.txt doc/design.txt doc/environment.txt
texts.path  = /usr/share/doc/packages/libmemnotifyqt-doc
INSTALLS   += texts

policy.files = default.mn
policy.path = /etc/memnotify
INSTALLS += policy
