set(NAME wrl)

set(HEADERS
  Appearance.hpp
  Group.hpp
  ImageTexture.hpp
  IndexedFaceSet.hpp
  IndexedLineSet.hpp
  IsoSurf.hpp
  Material.hpp
  Node.hpp
  PixelTexture.hpp
  Rotation.hpp
  SceneGraph.hpp
  SceneGraphProcessor.hpp
  SceneGraphTraversal.hpp
  Shape.hpp
  Transform.hpp
) # HEADERS    

set(SOURCES
  Appearance.cpp
  Group.cpp
  ImageTexture.cpp
  IndexedFaceSet.cpp
  IndexedLineSet.cpp
  IsoSurf.cpp
  Material.cpp
  Node.cpp
  PixelTexture.cpp
  Rotation.cpp
  SceneGraph.cpp
  SceneGraphProcessor.cpp
  SceneGraphTraversal.cpp
  Shape.cpp
  Transform.cpp
) # SOURCES

qt5_wrap_ui(FORMS
) # FORMS

qt5_add_resources(ASSETS
) # ASSETS

add_library(${NAME}
    ${HEADERS}
    ${SOURCES}
    ${FORMS}
    ${ASSETS}
) # add_library

target_compile_features(${NAME} PRIVATE cxx_lambdas)

target_link_libraries(${NAME} ${LIB_LIST})
