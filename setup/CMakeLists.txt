set(PROJECT_NAME "setup")

include("${PROJECT_SOURCE_DIR}/CMake/exe.cmake")

if(${PROJECT_DOC} AND Doxygen_FOUND)
set(DOXYGEN_TAGFILES "${STD_TAG}")
include("${PROJECT_SOURCE_DIR}/CMake/dox.cmake")
endif()
