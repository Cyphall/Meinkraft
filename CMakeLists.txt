cmake_minimum_required(VERSION 3.17)

set(CMAKE_TOOLCHAIN_FILE "vcpkg/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_TARGET_TRIPLET "x64-windows-static")

cmake_policy(SET CMP0091 NEW)

project(Meinkraft)

add_executable(Meinkraft vendor/imgui-1.79/include/imgui.cpp vendor/imgui-1.79/include/imgui_draw.cpp vendor/imgui-1.79/include/imgui_widgets.cpp vendor/imgui-1.79/include/imgui_stdlib.cpp  vendor/imgui-1.79/include/imgui_demo.cpp src/Main.cpp src/Renderer.cpp src/Renderer.h src/World.cpp src/World.h src/Toolbox.cpp src/Toolbox.h src/Camera.cpp src/Camera.h src/Chunk.cpp src/Chunk.h src/BlockId.h src/WorldGenerator.cpp src/WorldGenerator.h src/MountainGenerator.cpp src/MountainGenerator.h src/NoiseGenerator.cpp src/NoiseGenerator.h src/Helper/MathHelper.h src/GL/ShaderProgram.cpp src/GL/ShaderProgram_setUniform.cpp src/GL/ShaderProgram.h src/Helper/FileHelper.h src/Helper/ShaderHelper.h src/Helper/StringHelper.h src/Texture/Block/BlockTexture.cpp src/Texture/Block/BlockTexture.h src/GL/Uniforms.h src/BlockContainer.cpp src/BlockContainer.h src/ChunkBuffer.cpp src/ChunkBuffer.h src/ChunkBufferSegment.cpp src/ChunkBufferSegment.h src/GL/VertexData.h src/ChunkBufferManager.cpp src/ChunkBufferManager.h src/Block/BlockDefinition.h src/Block/BlockDefinition.cpp src/Block/BlockType.h src/Block/BlockDefinition/Air.cpp src/Block/BlockDefinition/Air.h src/Block/BlockDefinition/Stone.cpp src/Block/BlockDefinition/Stone.h src/Texture/Block/BlockTextureManager.cpp src/Texture/Block/BlockTextureManager.h src/Texture/Block/BlockTextureId.h src/Block/BlockDefinition/Grass.cpp src/Block/BlockDefinition/Grass.h src/Block/BlockDefinition/Dirt.cpp src/Block/BlockDefinition/Dirt.h src/GL/ShaderProgramCreateInfo.h)

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

find_package(TBB CONFIG REQUIRED)
target_link_libraries(Meinkraft PRIVATE TBB::tbb)

target_include_directories(Meinkraft SYSTEM PRIVATE "vendor/glad/include")
target_include_directories(Meinkraft SYSTEM PRIVATE "vendor/imgui-1.79/include")
target_include_directories(Meinkraft SYSTEM PRIVATE "vendor/fast_noise_lite/include")

add_custom_command(TARGET Meinkraft POST_BUILD
		COMMAND robocopy ${PROJECT_SOURCE_DIR}/resources/ ${PROJECT_BINARY_DIR}/resources /E /NS /NC /NFL /NDL /NP /NJH /NJS  || cmd /c "exit /b 0")