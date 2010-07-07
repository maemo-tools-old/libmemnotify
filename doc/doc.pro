dox.target = doc
dox.commands = doxygen
dox.files = html/* 
dox.path  = /usr/share/doc/libmemnotifyqt-doc/html
dox.depends =

texts.files = design.txt environment.txt
texts.path = /usr/share/doc/libmemnotifyqt-doc
texts.depends = 

QMAKE_EXTRA_TARGETS += dox
INSTALLS += dox texts
