TEMPLATE = subdirs

SUBDIRS = \
          doc \
          src \
#          test

CONFIG += ordered

# Make it so projects can find our specific features
system(echo QMAKEFEATURES+=$$PWD/features > $$OUT_PWD/.qmake.cache)

QMAKE_CLEAN += configure-stamp

