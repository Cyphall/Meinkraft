cmake_minimum_required(VERSION 3.17)

set(CMAKE_TOOLCHAIN_FILE "vcpkg/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_TARGET_TRIPLET "x64-windows-static")

cmake_policy(SET CMP0091 NEW)

project(Meinkraft)

add_executable(Meinkraft vendor/imgui-1.79/include/imgui.cpp vendor/imgui-1.79/include/imgui_draw.cpp vendor/imgui-1.79/include/imgui_widgets.cpp vendor/imgui-1.79/include/imgui_stdlib.cpp  vendor/imgui-1.79/include/imgui_demo.cpp src/Meinkraft/Main.cpp src/Meinkraft/Renderer.cpp src/Meinkraft/Renderer.h src/Meinkraft/World.cpp src/Meinkraft/World.h src/Meinkraft/Toolbox.cpp src/Meinkraft/Toolbox.h src/Meinkraft/Camera.cpp src/Meinkraft/Camera.h src/Meinkraft/Chunk.cpp src/Meinkraft/Chunk.h src/Meinkraft/BlockId.h src/Meinkraft/WorldGenerator.cpp src/Meinkraft/WorldGenerator.h src/Meinkraft/MountainGenerator.cpp src/Meinkraft/MountainGenerator.h src/Meinkraft/NoiseGenerator.cpp src/Meinkraft/NoiseGenerator.h src/Meinkraft/Helper/MathHelper.h src/Meinkraft/GL/ShaderProgram.cpp src/Meinkraft/GL/ShaderProgram_setUniform.cpp src/Meinkraft/GL/ShaderProgram.h src/Meinkraft/Helper/FileHelper.h src/Meinkraft/Helper/ShaderHelper.h src/Meinkraft/Helper/StringHelper.h src/Meinkraft/Texture/Block/BlockTexture.cpp src/Meinkraft/Texture/Block/BlockTexture.h src/Meinkraft/GL/Uniforms.h src/Meinkraft/BlockContainer.cpp src/Meinkraft/BlockContainer.h src/Meinkraft/ChunkBuffer.cpp src/Meinkraft/ChunkBuffer.h src/Meinkraft/ChunkBufferSegment.cpp src/Meinkraft/ChunkBufferSegment.h src/Meinkraft/GL/VertexData.h src/Meinkraft/ChunkBufferManager.cpp src/Meinkraft/ChunkBufferManager.h src/Meinkraft/Block/BlockDefinition.h src/Meinkraft/Block/BlockDefinition.cpp src/Meinkraft/Block/BlockType.h src/Meinkraft/Block/BlockDefinition/Air.cpp src/Meinkraft/Block/BlockDefinition/Air.h src/Meinkraft/Block/BlockDefinition/Stone.cpp src/Meinkraft/Block/BlockDefinition/Stone.h src/Meinkraft/Texture/Block/BlockTextureManager.cpp src/Meinkraft/Texture/Block/BlockTextureManager.h src/Meinkraft/Texture/Block/BlockTextureId.h src/Meinkraft/Block/BlockDefinition/Grass.cpp src/Meinkraft/Block/BlockDefinition/Grass.h src/Meinkraft/Block/BlockDefinition/Dirt.cpp src/Meinkraft/Block/BlockDefinition/Dirt.h src/Meinkraft/GL/ShaderProgramCreateInfo.h src/Meinkraft/ConcurrentChunkTaskQueue.cpp src/Meinkraft/ConcurrentChunkTaskQueue.h src/Meinkraft/MainThreadChunkTaskQueue.cpp src/Meinkraft/MainThreadChunkTaskQueue.h src/Meinkraft/Helper/SpecHelper.cpp src/Meinkraft/Helper/SpecHelper.h)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC") # MSVC
	set_property(TARGET Meinkraft PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC") # clang-cl
	set_property(TARGET Meinkraft PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
	target_compile_options(Meinkraft PRIVATE /EHsc)
endif()

target_compile_features(Meinkraft PRIVATE cxx_std_20)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(Meinkraft PRIVATE glfw)

find_package(glm CONFIG REQUIRED)
target_link_libraries(Meinkraft PRIVATE glm::glm)

find_path(STB_INCLUDE_DIRS "stb_c_lexer.h")
target_include_directories(Meinkraft PRIVATE ${STB_INCLUDE_DIRS})

find_package(magic_enum CONFIG REQUIRED)
target_link_libraries(Meinkraft PRIVATE magic_enum::magic_enum)

target_include_directories(Meinkraft PRIVATE "src")
target_include_directories(Meinkraft SYSTEM PRIVATE "vendor/glad/include")
target_include_directories(Meinkraft SYSTEM PRIVATE "vendor/imgui-1.79/include")
target_include_directories(Meinkraft SYSTEM PRIVATE "vendor/fast_noise_lite/include")

add_custom_command(TARGET Meinkraft POST_BUILD
		COMMAND robocopy ${PROJECT_SOURCE_DIR}/resources/ ${PROJECT_BINARY_DIR}/resources /E /NS /NC /NFL /NDL /NP /NJH /NJS || cmd /c "exit /b 0")