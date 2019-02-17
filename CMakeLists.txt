cmake_minimum_required(VERSION 2.6)
project(RenderAM)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
find_package(SFML COMPONENTS system graphics window audio network )
include_directories(${SFML_INCLUDE_DIRS})
find_package(SFGUI REQUIRED COMPONENTS sfgui)
include_directories(${SFGUI_INCLUDE_DIRS})
set(LIBS ${LIBS} ${SFML_LIBRARIES})
set(LIBS ${LIBS} ${SFGUI_LIBRARIES})
add_executable( outfile.bin
                    ${CMAKE_SOURCE_DIR}/src/Main.cpp
                    ${CMAKE_SOURCE_DIR}/src/Application.hpp
                    ${CMAKE_SOURCE_DIR}/src/Renderer.hpp
                    ${CMAKE_SOURCE_DIR}/src/Uniform.hpp
                    ${CMAKE_SOURCE_DIR}/src/Material.hpp
                    ${CMAKE_SOURCE_DIR}/src/Texture.hpp
                    ${CMAKE_SOURCE_DIR}/src/Mesh.hpp
                    ${CMAKE_SOURCE_DIR}/src/Coords3f.hpp
                    ${CMAKE_SOURCE_DIR}/src/Matrices.hpp
                    ${CMAKE_SOURCE_DIR}/src/DecartVector.hpp
                    ${CMAKE_SOURCE_DIR}/src/Stb_image.h
                    ${CMAKE_SOURCE_DIR}/lib/Stb_image.o
)

target_link_libraries( outfile.bin
${SFML_LIBRARIES} -lGL -lGLU -lGLEW -lsfgui
)
