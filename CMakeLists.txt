cmake_minimum_required (VERSION 3.8)

project (native-webgl)

file (GLOB_RECURSE SOURCE_FILES "./Source/*.cpp" "./Source/*.h")
add_library (native-webgl SHARED ${SOURCE_FILES})
set_target_properties (native-webgl PROPERTIES PREFIX "" SUFFIX ".node")
set_target_properties (native-webgl PROPERTIES CXX_STANDARD 17)

## teresa
target_include_directories (native-webgl PRIVATE "./Source")

## node
get_filename_component (NODE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/External/node-v11.8.0/x64-windows/include" ABSOLUTE)
get_filename_component (NODE_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/External/node-v11.8.0/x64-windows/lib/node.lib" ABSOLUTE)
message ("${NODE_LIBRARIES}")
target_include_directories (native-webgl PRIVATE ${NODE_INCLUDE_DIRECTORIES})
target_link_libraries (native-webgl PRIVATE ${NODE_LIBRARIES})

## glfw
find_package(glfw3 REQUIRED)
target_include_directories (native-webgl PRIVATE glfw)
target_link_libraries (native-webgl PRIVATE glfw)

## glad
find_package(glad REQUIRED)
find_path (GLAD_INCLUDE_DIRECTORIES NAMES glad/glad.h)
target_include_directories (native-webgl PRIVATE ${GLAD_INCLUDE_DIRECTORIES})
find_library (GLAD_LIBRARIES NAMES glad.lib)
target_link_libraries (native-webgl PRIVATE ${GLAD_LIBRARIES})

add_custom_command (TARGET native-webgl 
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_LIST_DIR}/Test/test.js" $<TARGET_FILE_DIR:native-webgl>)
