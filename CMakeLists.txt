cmake_minimum_required(VERSION 3.17)

set(CMAKE_TOOLCHAIN_FILE "vcpkg/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_TARGET_TRIPLET "x64-windows-static")

cmake_policy(SET CMP0091 NEW)

project(Meinkraft)

add_executable(Meinkraft vendor/imgui-1.79/include/imgui.cpp vendor/imgui-1.79/include/imgui_draw.cpp vendor/imgui-1.79/include/imgui_widgets.cpp vendor/imgui-1.79/include/imgui_stdlib.cpp  vendor/imgui-1.79/include/imgui_demo.cpp src/Main.cpp src/Renderer.cpp src/Renderer.h src/World.cpp src/World.h src/Toolbox.cpp src/Toolbox.h src/Camera.cpp src/Camera.h src/Chunk.cpp src/Chunk.h src/BlockType.h src/WorldGenerator.cpp src/WorldGenerator.h src/MountainGenerator.cpp src/MountainGenerator.h src/NoiseGenerator.cpp src/NoiseGenerator.h src/Helper/MathHelper.h src/GL/ShaderProgram.cpp src/GL/ShaderProgram_setUniform.cpp src/GL/ShaderProgram.h src/Helper/FileHelper.h src/Helper/ShaderHelper.h src/Helper/StringHelper.h src/GL/Texture.cpp src/GL/Texture.h src/GL/Uniforms.h src/BlockTextures.h src/BlockTextures.cpp src/BlockContainer.cpp src/BlockContainer.h src/ChunkBuffer.cpp src/ChunkBuffer.h src/ChunkBufferSegment.cpp src/ChunkBufferSegment.h src/GL/VertexData.h src/ChunkBufferManager.cpp src/ChunkBufferManager.h)

set_property(TARGET Meinkraft PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_compile_features(Meinkraft PRIVATE cxx_std_20)
target_compile_options(Meinkraft PRIVATE /Zc:__cplusplus)
target_compile_options(Meinkraft PRIVATE $<$<CONFIG:Release>:-O2>)

find_path(STB_INCLUDE_DIRS "stb.h")
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(TBB CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

target_include_directories(Meinkraft PRIVATE
        "vendor/glad/include"
		"vendor/imgui-1.79/include"
		"vendor/fast_noise_lite/include"
		${STB_INCLUDE_DIRS})

target_link_libraries(Meinkraft PRIVATE
		glfw
		glm::glm
		TBB::tbb
		fmt::fmt)

add_custom_command(TARGET Meinkraft PRE_BUILD
		COMMAND robocopy ${PROJECT_SOURCE_DIR}/resources/ ${PROJECT_BINARY_DIR}/resources /E /NS /NC /NFL /NDL /NP /NJH /NJS  || cmd /c "exit /b 0")
