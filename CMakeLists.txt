# CMakeList.txt: проект CMake для ProgaProject; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

include(LocalConfig.cmake)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("ProgaProject")
set(GLEW_USE_STATIC_LIBS TRUE)

find_package(GLEW REQUIRED)
find_package(OPENGL REQUIRED)

include_directories("./include")

add_executable (ProgaProject "src/ProgaProject.cpp" "src/Shader.cpp" "src/ShaderProgram.cpp" "src/Camera.cpp" "include/Keyboard.h" "src/Keyboard.cpp" "include/Mouse.h" "src/Mouse.cpp")

target_compile_definitions(ProgaProject PUBLIC GLEW_STATIC)

target_include_directories(ProgaProject PUBLIC ${GLEW_INCLUED_DIRS})
target_include_directories(ProgaProject PUBLIC ${GLFW_INCLUDE_DIRS})
target_include_directories(ProgaProject PUBLIC ${GLM_INCLUDE_DIRS})

target_link_libraries(ProgaProject PUBLIC OpenGL::GL)
target_link_libraries(ProgaProject PUBLIC GLEW::glew_s)
target_link_libraries(ProgaProject PUBLIC ${GLFW_STATIC_LIBRARIES})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ProgaProject PROPERTY CXX_STANDARD 20)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
