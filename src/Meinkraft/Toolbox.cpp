#include "Toolbox.h"

#include "Meinkraft/Camera.h"
#include "Meinkraft/ChunkBufferManager.h"
#include "Meinkraft/Texture/Block/BlockTextureManager.h"
#include "Meinkraft/Rendering/Renderer.h"
#include "Meinkraft/World.h"

GLFWwindow* Toolbox::window = nullptr;
std::unique_ptr<Camera> Toolbox::camera;
std::unique_ptr<ChunkBufferManager> Toolbox::chunkBufferManager;
std::unique_ptr<BlockTextureManager> Toolbox::blockTextureManager;
std::unique_ptr<Renderer> Toolbox::renderer;
std::unique_ptr<World> Toolbox::world;