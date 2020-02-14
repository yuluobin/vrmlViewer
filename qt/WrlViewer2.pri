#
# vim:filetype=qmake sw=4 ts=4 expandtab nospell
#

FORMS = \
        $$FORMSDIR/WrlMainWindow.ui \
        $$FORMSDIR/WrlToolsWidget.ui \
        $$(NULL)

# RESOURCES = $$ASSETSDIR/assets.qrc

VIEWER_DIR = $$SOURCEDIR/viewer
IO_DIR     = $$SOURCEDIR/io
UTIL_DIR   = $$SOURCEDIR/util
WRL_DIR    = $$SOURCEDIR/wrl

HEADERS += \
        $$SOURCEDIR/io/AppLoader.hpp \
        $$SOURCEDIR/io/AppSaver.hpp \
        $$SOURCEDIR/io/Loader.hpp \
        $$SOURCEDIR/io/LoaderWrl.hpp \
        $$SOURCEDIR/io/Saver.hpp \
        $$SOURCEDIR/io/SaverWrl.hpp \
        $$SOURCEDIR/io/StrException.hpp \
        $$SOURCEDIR/io/Tokenizer.hpp \
        $$SOURCEDIR/util/BBox.hpp \
        $$SOURCEDIR/util/StaticRotation.hpp \
        $$SOURCEDIR/viewer/WrlAboutDialog.hpp \
        $$SOURCEDIR/viewer/WrlGLBuffer.hpp \
        $$SOURCEDIR/viewer/WrlGLHandles.hpp \
        $$SOURCEDIR/viewer/WrlGLShader.hpp \
        $$SOURCEDIR/viewer/WrlGLWidget.hpp \
        $$SOURCEDIR/viewer/WrlMainWindow.hpp \
        $$SOURCEDIR/viewer/WrlQtLogo.hpp \
        $$SOURCEDIR/viewer/WrlToolsWidget.hpp \
        $$SOURCEDIR/viewer/WrlViewerData.hpp \
        $$SOURCEDIR/wrl/Appearance.hpp \
        $$SOURCEDIR/wrl/Group.hpp \
        $$SOURCEDIR/wrl/ImageTexture.hpp \
        $$SOURCEDIR/wrl/IndexedFaceSet.hpp \
        $$SOURCEDIR/wrl/IndexedLineSet.hpp \
        $$SOURCEDIR/wrl/IsoSurf.hpp \
        $$SOURCEDIR/wrl/Material.hpp \
        $$SOURCEDIR/wrl/Node.hpp \
        $$SOURCEDIR/wrl/PixelTexture.hpp \
        $$SOURCEDIR/wrl/Rotation.hpp \
        $$SOURCEDIR/wrl/SceneGraph.hpp \
        $$SOURCEDIR/wrl/SceneGraphProcessor.hpp \
        $$SOURCEDIR/wrl/SceneGraphTraversal.hpp \
        $$SOURCEDIR/wrl/Shape.hpp \
        $$SOURCEDIR/wrl/Transform.hpp \
        $$(NULL)

SOURCES += \
        $$SOURCEDIR/io/AppLoader.cpp \
        $$SOURCEDIR/io/AppSaver.cpp \
        $$SOURCEDIR/io/LoaderWrl.cpp \
        $$SOURCEDIR/io/SaverWrl.cpp \
        $$SOURCEDIR/io/Tokenizer.cpp \
        $$SOURCEDIR/util/BBox.cpp \
        $$SOURCEDIR/util/StaticRotation.cpp \
        $$SOURCEDIR/viewer/WrlAboutDialog.cpp \
        $$SOURCEDIR/viewer/WrlGLBuffer.cpp \
        $$SOURCEDIR/viewer/WrlGLHandles.cpp \
        $$SOURCEDIR/viewer/WrlGLShader.cpp \
        $$SOURCEDIR/viewer/WrlGLWidget.cpp \
        $$SOURCEDIR/viewer/WrlMainWindow.cpp \
        $$SOURCEDIR/viewer/WrlQtLogo.cpp \
        $$SOURCEDIR/viewer/WrlToolsWidget.cpp \
        $$SOURCEDIR/viewer/WrlViewerApp.cpp \
        $$SOURCEDIR/viewer/WrlViewerData.cpp \
        $$SOURCEDIR/wrl/Appearance.cpp \
        $$SOURCEDIR/wrl/Group.cpp \
        $$SOURCEDIR/wrl/ImageTexture.cpp \
        $$SOURCEDIR/wrl/IndexedFaceSet.cpp \
        $$SOURCEDIR/wrl/IndexedLineSet.cpp \
        $$SOURCEDIR/wrl/IsoSurf.cpp \
        $$SOURCEDIR/wrl/Material.cpp \
        $$SOURCEDIR/wrl/Node.cpp \
        $$SOURCEDIR/wrl/PixelTexture.cpp \
        $$SOURCEDIR/wrl/Rotation.cpp \
        $$SOURCEDIR/wrl/SceneGraph.cpp \
        $$SOURCEDIR/wrl/SceneGraphProcessor.cpp \
        $$SOURCEDIR/wrl/SceneGraphTraversal.cpp \
        $$SOURCEDIR/wrl/Shape.cpp \
        $$SOURCEDIR/wrl/Transform.cpp \
        $$(NULL)
	
INCLUDEPATH += $$SOURCEDIR $$EIGEN_DIR $$VIEWER_DIR $$IO_DIR $$UTIL_DIR $$WRL_DIR

message(INCLUDEPATH = $$INCLUDEPATH)

