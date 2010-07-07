#############################################################################
# Makefile for building: libmemnotifyqt
# Generated by qmake (2.01a) (Qt 4.6.2) on: Wed Jun 30 16:40:20 2010
# Project:  libmemnotifyqt.pro
# Template: subdirs
# Command: /usr/bin/qmake -unix -o Makefile libmemnotifyqt.pro
#############################################################################

first: make_default
MAKEFILE      = Makefile
QMAKE         = /usr/bin/qmake
DEL_FILE      = rm -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p
COPY          = cp -f
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
INSTALL_FILE  = install -m 644 -p
INSTALL_PROGRAM = install -m 755 -p
INSTALL_DIR   = $(COPY_DIR)
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p
SUBTARGETS    =  \
		sub-doc \
		sub-src

doc/$(MAKEFILE): 
	@$(CHK_DIR_EXISTS) doc/ || $(MKDIR) doc/ 
	cd doc/ && $(QMAKE) /home/ldm/git/memnotify/libmemnotifyqt/doc/doc.pro -unix -o $(MAKEFILE)
sub-doc-qmake_all:  FORCE
	@$(CHK_DIR_EXISTS) doc/ || $(MKDIR) doc/ 
	cd doc/ && $(QMAKE) /home/ldm/git/memnotify/libmemnotifyqt/doc/doc.pro -unix -o $(MAKEFILE)
sub-doc: doc/$(MAKEFILE) FORCE
	cd doc/ && $(MAKE) -f $(MAKEFILE)
sub-doc-make_default-ordered: doc/$(MAKEFILE) FORCE
	cd doc/ && $(MAKE) -f $(MAKEFILE) 
sub-doc-make_default: doc/$(MAKEFILE) FORCE
	cd doc/ && $(MAKE) -f $(MAKEFILE) 
sub-doc-make_first-ordered: doc/$(MAKEFILE) FORCE
	cd doc/ && $(MAKE) -f $(MAKEFILE) first
sub-doc-make_first: doc/$(MAKEFILE) FORCE
	cd doc/ && $(MAKE) -f $(MAKEFILE) first
sub-doc-all-ordered: doc/$(MAKEFILE) FORCE
	cd doc/ && $(MAKE) -f $(MAKEFILE) all
sub-doc-all: doc/$(MAKEFILE) FORCE
	cd doc/ && $(MAKE) -f $(MAKEFILE) all
sub-doc-clean-ordered: doc/$(MAKEFILE) FORCE
	cd doc/ && $(MAKE) -f $(MAKEFILE) clean
sub-doc-clean: doc/$(MAKEFILE) FORCE
	cd doc/ && $(MAKE) -f $(MAKEFILE) clean
sub-doc-distclean-ordered: doc/$(MAKEFILE) FORCE
	cd doc/ && $(MAKE) -f $(MAKEFILE) distclean
sub-doc-distclean: doc/$(MAKEFILE) FORCE
	cd doc/ && $(MAKE) -f $(MAKEFILE) distclean
sub-doc-install_subtargets-ordered: doc/$(MAKEFILE) FORCE
	cd doc/ && $(MAKE) -f $(MAKEFILE) install
sub-doc-install_subtargets: doc/$(MAKEFILE) FORCE
	cd doc/ && $(MAKE) -f $(MAKEFILE) install
sub-doc-uninstall_subtargets-ordered: doc/$(MAKEFILE) FORCE
	cd doc/ && $(MAKE) -f $(MAKEFILE) uninstall
sub-doc-uninstall_subtargets: doc/$(MAKEFILE) FORCE
	cd doc/ && $(MAKE) -f $(MAKEFILE) uninstall
src/$(MAKEFILE): 
	@$(CHK_DIR_EXISTS) src/ || $(MKDIR) src/ 
	cd src/ && $(QMAKE) /home/ldm/git/memnotify/libmemnotifyqt/src/src.pro -unix -o $(MAKEFILE)
sub-src-qmake_all:  FORCE
	@$(CHK_DIR_EXISTS) src/ || $(MKDIR) src/ 
	cd src/ && $(QMAKE) /home/ldm/git/memnotify/libmemnotifyqt/src/src.pro -unix -o $(MAKEFILE)
sub-src: src/$(MAKEFILE) FORCE
	cd src/ && $(MAKE) -f $(MAKEFILE)
sub-src-make_default-ordered: src/$(MAKEFILE) sub-doc-make_default-ordered  FORCE
	cd src/ && $(MAKE) -f $(MAKEFILE) 
sub-src-make_default: src/$(MAKEFILE) FORCE
	cd src/ && $(MAKE) -f $(MAKEFILE) 
sub-src-make_first-ordered: src/$(MAKEFILE) sub-doc-make_first-ordered  FORCE
	cd src/ && $(MAKE) -f $(MAKEFILE) first
sub-src-make_first: src/$(MAKEFILE) FORCE
	cd src/ && $(MAKE) -f $(MAKEFILE) first
sub-src-all-ordered: src/$(MAKEFILE) sub-doc-all-ordered  FORCE
	cd src/ && $(MAKE) -f $(MAKEFILE) all
sub-src-all: src/$(MAKEFILE) FORCE
	cd src/ && $(MAKE) -f $(MAKEFILE) all
sub-src-clean-ordered: src/$(MAKEFILE) sub-doc-clean-ordered  FORCE
	cd src/ && $(MAKE) -f $(MAKEFILE) clean
sub-src-clean: src/$(MAKEFILE) FORCE
	cd src/ && $(MAKE) -f $(MAKEFILE) clean
sub-src-distclean-ordered: src/$(MAKEFILE) sub-doc-distclean-ordered  FORCE
	cd src/ && $(MAKE) -f $(MAKEFILE) distclean
sub-src-distclean: src/$(MAKEFILE) FORCE
	cd src/ && $(MAKE) -f $(MAKEFILE) distclean
sub-src-install_subtargets-ordered: src/$(MAKEFILE) sub-doc-install_subtargets-ordered  FORCE
	cd src/ && $(MAKE) -f $(MAKEFILE) install
sub-src-install_subtargets: src/$(MAKEFILE) FORCE
	cd src/ && $(MAKE) -f $(MAKEFILE) install
sub-src-uninstall_subtargets-ordered: src/$(MAKEFILE) sub-doc-uninstall_subtargets-ordered  FORCE
	cd src/ && $(MAKE) -f $(MAKEFILE) uninstall
sub-src-uninstall_subtargets: src/$(MAKEFILE) FORCE
	cd src/ && $(MAKE) -f $(MAKEFILE) uninstall

Makefile: libmemnotifyqt.pro .qmake.cache /targets/ff/usr/share/qt4/mkspecs/linux-g++-maemo5/qmake.conf /targets/ff/usr/share/qt4/mkspecs/common/unix.conf \
		/targets/ff/usr/share/qt4/mkspecs/common/linux.conf \
		/targets/ff/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/targets/ff/usr/share/qt4/mkspecs/features/default_pre.prf \
		.qmake.cache \
		/targets/ff/usr/share/qt4/mkspecs/qconfig.pri \
		/targets/ff/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/targets/ff/usr/share/qt4/mkspecs/features/qt_config.prf \
		/targets/ff/usr/share/qt4/mkspecs/features/release.prf \
		/targets/ff/usr/share/qt4/mkspecs/features/default_post.prf \
		/targets/ff/usr/share/qt4/mkspecs/features/warn_on.prf \
		/targets/ff/usr/share/qt4/mkspecs/features/qt.prf \
		/targets/ff/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/targets/ff/usr/share/qt4/mkspecs/features/moc.prf \
		/targets/ff/usr/share/qt4/mkspecs/features/resources.prf \
		/targets/ff/usr/share/qt4/mkspecs/features/uic.prf \
		/targets/ff/usr/share/qt4/mkspecs/features/yacc.prf \
		/targets/ff/usr/share/qt4/mkspecs/features/lex.prf \
		/targets/ff/usr/share/qt4/mkspecs/features/include_source_dir.prf
	$(QMAKE) -unix -o Makefile libmemnotifyqt.pro
/targets/ff/usr/share/qt4/mkspecs/common/unix.conf:
/targets/ff/usr/share/qt4/mkspecs/common/linux.conf:
/targets/ff/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/targets/ff/usr/share/qt4/mkspecs/features/default_pre.prf:
.qmake.cache:
/targets/ff/usr/share/qt4/mkspecs/qconfig.pri:
/targets/ff/usr/share/qt4/mkspecs/features/qt_functions.prf:
/targets/ff/usr/share/qt4/mkspecs/features/qt_config.prf:
/targets/ff/usr/share/qt4/mkspecs/features/release.prf:
/targets/ff/usr/share/qt4/mkspecs/features/default_post.prf:
/targets/ff/usr/share/qt4/mkspecs/features/warn_on.prf:
/targets/ff/usr/share/qt4/mkspecs/features/qt.prf:
/targets/ff/usr/share/qt4/mkspecs/features/unix/thread.prf:
/targets/ff/usr/share/qt4/mkspecs/features/moc.prf:
/targets/ff/usr/share/qt4/mkspecs/features/resources.prf:
/targets/ff/usr/share/qt4/mkspecs/features/uic.prf:
/targets/ff/usr/share/qt4/mkspecs/features/yacc.prf:
/targets/ff/usr/share/qt4/mkspecs/features/lex.prf:
/targets/ff/usr/share/qt4/mkspecs/features/include_source_dir.prf:
qmake: qmake_all FORCE
	@$(QMAKE) -unix -o Makefile libmemnotifyqt.pro

qmake_all: sub-doc-qmake_all sub-src-qmake_all FORCE

make_default: sub-doc-make_default-ordered sub-src-make_default-ordered FORCE
make_first: sub-doc-make_first-ordered sub-src-make_first-ordered FORCE
all: sub-doc-all-ordered sub-src-all-ordered FORCE
clean: sub-doc-clean-ordered sub-src-clean-ordered FORCE
	-$(DEL_FILE) configure-stamp
distclean: sub-doc-distclean-ordered sub-src-distclean-ordered FORCE
	-$(DEL_FILE) Makefile
install_subtargets: sub-doc-install_subtargets-ordered sub-src-install_subtargets-ordered FORCE
uninstall_subtargets: sub-doc-uninstall_subtargets-ordered sub-src-uninstall_subtargets-ordered FORCE

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all
install: install_subtargets  FORCE

uninstall:  uninstall_subtargets FORCE

FORCE:

