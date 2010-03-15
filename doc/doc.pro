dox.target = doc
contains(CONFIG, nodoc) {
dox.commands = mkdir -p $$PWD/html && touch $$PWD/html/not_docs_here_yet
}
else {
dox.commands = doxygen $$PWD/Doxyfile
}
dox.depends =
dox.files = html
dox.path  = $$ASD_INSTALL_ROOT/share/doc/libmemnotifyqt
QMAKE_EXTRA_TARGETS += dox

INSTALLS += dox
