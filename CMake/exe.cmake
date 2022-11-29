file(GLOB PROJECT_SOURCE_FILES "Src/*.cpp")
file(GLOB INCLUDE_FILES "*.hpp" "Inline/*.hpp")
file(GLOB DOC_FILES "*.hpp")
add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_FILES} ${INCLUDE_FILES})

target_link_libraries(${PROJECT_NAME} test_all)

file(MAKE_DIRECTORY "${OUTPUT_DIR}/${PROJECT_NAME}")    

configure_file (
  "${PROJECT_SOURCE_DIR}/CMake/Output.hpp.in"
  "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/Output.hpp"
  )
