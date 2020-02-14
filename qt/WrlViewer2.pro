#
# vim:filetype=qmake sw=4 ts=4 expandtab nospell
#

win32:CONFIG += USE_DSHOW
CONFIG += static

unix:CONFIG += USE_UNIX_DAEMONIZE

##########################################################################

BASEDIR = ..
TOPDIR = $$BASEDIR/..
# EIGEN_DIR  = $$TOPDIR/../eigen
EIGEN_DIR  = $$TOPDIR/../../eigen-3-3-7
UI_DIR = GeneratedFiles
DESTDIR = $$BASEDIR/bin
FORMSDIR = $$BASEDIR/forms
SOURCEDIR = $$BASEDIR/src
ASSETSDIR = $$BASEDIR/assets

NAME = WrlViewer2

CONFIG += qt
QT += opengl
QT += gui

win32 {
    DEFINES += NOMINMAX _CRT_SECURE_NO_WARNINGS
    DEFINES += _SCL_SECURE_NO_WARNINGS _USE_MATH_DEFINES
    QMAKE_CXXFLAGS_WARN_ON += -W3 -wd4396 -wd4100 -wd4996
    QMAKE_LFLAGS += /INCREMENTAL:NO
    DSHOW_LIBS = -lStrmiids -lVfw32 -lOle32 -lOleAut32 -lopengl32
}

unix:!macx {
    QMAKE_LFLAGS += -Wl,-rpath=$$OPENCV_DIR/lib
    #QMAKE_CXXFLAGS += -g
}

macx {
    LIBS += -framework Foundation -framework QTKit 
    LIBS += -framework CoreFoundation -framework IOkit
}

CONFIG(release, debug|release) {
    TARGET = $$NAME
} else {
    TARGET = $${NAME}_d
}
CONFIG += console

# message(LIBS = $$LIBS)

include($${NAME}.pri)
