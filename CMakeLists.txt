cmake_minimum_required (VERSION 3.15)

project(FC VERSION 1.0 LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-Wall")

set(OUTPUT_DIR "${PROJECT_BINARY_DIR}/output")
include_directories(${CMAKE_SOURCE_DIR})

set (PROJECT_DOC YES)
if(${PROJECT_DOC})
  find_package(Doxygen REQUIRED dot)
  if(${DOXYGEN_FOUND})
    set(DOXYGEN_EXTERNAL_GROUPS NO) 
    set(DOXYGEN_USE_MATHJAX YES)
    set(PROJECT_DOC_DIR "${PROJECT_BINARY_DIR}/doc")
    set(STD_TAG "${PROJECT_SOURCE_DIR}/CMake/cppreference-doxygen-web.tag.xml=http://en.cppreference.com/w/")
    set(CFL_TAG "${PROJECT_DOC_DIR}/cfl/cfl.tag=../../cfl/html")
  endif()
endif()

# GSL
find_package(GSL REQUIRED)
include_directories(${GSL_INCLUDE_DIRS})

# messages for testing
message("PROGRAMS AND LIBRARIES:")
message("CMake program: " ${CMAKE_MAKE_PROGRAM})
message("C++ compiler: " ${CMAKE_CXX_COMPILER})
message("Doxygen: " ${DOXYGEN_EXECUTABLE})
message("Doxygen dot: " ${DOXYGEN_DOT_EXECUTABLE})
message("pkg-config: " ${PKG_CONFIG_EXECUTABLE})
message("GSL include dir: " ${GSL_INCLUDE_DIRS})
message("GSL library: " ${GSL_LIBRARY})
message("BLAS library: " ${GSL_CBLAS_LIBRARY})

# write your Student ID in ""
set (STUDENT_ID "Vyacheslav Chekmenev")

# add subdicretories below, when you need them
add_subdirectory(test)
add_subdirectory(setup)

# Vega
#add_subdirectory(prep1)
#add_subdirectory(prep2)
add_subdirectory(prepExam)
