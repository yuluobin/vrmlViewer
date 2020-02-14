PROJECT(wrltest)
cmake_minimum_required(VERSION 2.8)

# list of source files
set(test_files wrltest.cpp)

# define the executable
# if(WIN32)
  add_executable(wrltest WIN32 ${test_files})
# else()
#   add_executable(wrltest ${test_files})
# endif()

# in Windows + Visual Studio we need this to make it a console application
if(WIN32)
  if(MSVC)
    set_target_properties(wrltest PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
  endif(MSVC)
endif(WIN32)

# install application
set(BIN_DIR ${CMAKE_INSTALL_PREFIX}/bin)

target_link_libraries(wrltest io util wrl)

install(TARGETS wrltest DESTINATION ${BIN_DIR})
