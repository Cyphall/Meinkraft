#include "Toolbox.h"
#include "World.h"
#include "Camera.h"
#include "Renderer.h"

GLFWwindow* Toolbox::window = nullptr;
std::unique_ptr<Camera> Toolbox::camera;
std::unique_ptr<World> Toolbox::world;
std::unique_ptr<Renderer> Toolbox::renderer;