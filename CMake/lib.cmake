file(GLOB PROJECT_SOURCE_FILES "Src/*.cpp")
file(GLOB INCLUDE_FILES "*.hpp" "Inline/*.hpp")
file(GLOB DOC_FILES "*.hpp")
add_library(${PROJECT_NAME} STATIC ${PROJECT_SOURCE_FILES} ${INCLUDE_FILES})

