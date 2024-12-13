cmake_minimum_required(VERSION 3.1)
project(lab1)

find_package(OpenGL REQUIRED)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

add_subdirectory(external)

include_directories(
	external/glfw-3.1.2/include/
	external/glm-0.9.7.1/
	external/glad-opengl-3.3/include/
	lab1/
)

add_executable(main
	project/main.cpp
		project/main.cpp
		project/main.cpp
)
target_link_libraries(main
	${OPENGL_LIBRARY}
	glfw
)


