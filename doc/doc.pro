dox.target = doc
dox.commands = doxygen
dox.files = html/* 
dox.path  = /usr/share/doc/libmemnotifyqt-doc/html
dox.depends =

texts.target   = doc
texts.files    = *.txt
texts.path     = /usr/share/doc/libmemnotifyqt-doc

QMAKE_EXTRA_TARGETS += dox
QMAKE_EXTRA_TARGETS += texts
INSTALLS += texts dox
