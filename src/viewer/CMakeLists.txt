set(NAME WrlViewer2)

set(HEADERS
  WrlAboutDialog.hpp
  WrlGLBuffer.hpp
  WrlGLHandles.hpp
  WrlGLShader.hpp
  WrlGLWidget.hpp
  WrlMainWindow.hpp
  WrlQtLogo.hpp
  WrlToolsWidget.hpp
  WrlViewerData.hpp
) # HEADERS    

set(SOURCES
  WrlAboutDialog.cpp
  WrlGLBuffer.cpp
  WrlGLHandles.cpp
  WrlGLShader.cpp
  WrlGLWidget.cpp
  WrlMainWindow.cpp
  WrlQtLogo.cpp
  WrlToolsWidget.cpp
  WrlViewerApp.cpp
  WrlViewerData.cpp
) # SOURCES

qt5_wrap_ui(FORMS
 ../../forms/WrlMainWindow.ui
 ../../forms/WrlToolsWidget.ui
) # FORMS

qt5_add_resources(ASSETS
) # ASSETS

if (APPLE)
  set(APP_ICON ${ASSETS_DIR}/WrlViewer.icns)
  set_source_files_properties(${APP_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
  add_executable(${NAME} MACOSX_BUNDLE ${APP_ICON}
      ${HEADERS}
      ${SOURCES}
      ${FORMS}
      ${ASSETS}
  ) # add_executable
  set(MACOSX_BUNDLE_ICON_FILE "WrlViewer.icns" )
else(APPLE)
  add_executable(${NAME}
      ${HEADERS}
      ${SOURCES}
      ${FORMS}
      ${ASSETS}
  ) # add_executable
endif(APPLE)

target_compile_features(${NAME} PRIVATE cxx_right_angle_brackets cxx_lambdas)

target_link_libraries(${NAME} ${LIB_LIST})

install(TARGETS ${NAME} DESTINATION ${BIN_DIR})
