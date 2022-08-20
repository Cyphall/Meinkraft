#pragma once

#include <memory>

struct GLFWwindow;
class Camera;
class ChunkBufferManager;
class BlockTextureManager;
class Renderer;
class World;

struct Toolbox
{
	static GLFWwindow* window;
	static std::unique_ptr<Camera> camera;
	static std::unique_ptr<ChunkBufferManager> chunkBufferManager;
	static std::unique_ptr<BlockTextureManager> blockTextureManager;
	static std::unique_ptr<Renderer> renderer;
	static std::unique_ptr<World> world;
};