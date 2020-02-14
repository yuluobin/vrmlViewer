set(NAME util)

set(HEADERS
  BBox.hpp
  StaticRotation.hpp
  Tokenizer.h
) # HEADERS    

set(SOURCES
  BBox.cpp
  StaticRotation.cpp
  Tokenizer.cpp
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



