#pragma once

#include <memory>

struct GLFWwindow;
class World;
class Camera;
class Renderer;

struct Toolbox
{
	static GLFWwindow* window;
	static std::unique_ptr<Camera> camera;
	static std::unique_ptr<World> world;
	static std::unique_ptr<Renderer> renderer;
};
