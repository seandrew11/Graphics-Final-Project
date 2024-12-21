cmake_minimum_required(VERSION 3.1)
project(ModernCelt)

find_package(OpenGL REQUIRED)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

add_subdirectory(external)

include_directories(
		external/glfw-3.1.2/include/
		external/glm-0.9.7.1/
		external/glad-opengl-3.3/include/  # Match the working project's path
		project/
		project/include/
		external/stb/
)

add_executable(main
		project/main.cpp
		project/Terrain.cpp
		project/Terrain.h
		project/render/shader.cpp
		project/Building.h
		project/Building.cpp
		project/Skybox.h
		project/Skybox.cpp
		project/Character.h
		project/Character.cpp
)

target_link_libraries(main
		${OPENGL_LIBRARY}
		glfw
		glad  # Add this line to link against glad
)
