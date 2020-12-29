#include <iostream>
#include <windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "World.h"
#include "Renderer.h"
#include "Toolbox.h"
#include <fmt/format.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void messageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
	switch (severity)
	{
		case GL_DEBUG_SEVERITY_HIGH:
			std::cout << fmt::format("ERROR {}: {}", id, message) << std::endl;
			if (IsDebuggerPresent())
				__debugbreak();
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			std::cout << fmt::format("WARNING {}: {}", id, message) << std::endl;
			break;
		case GL_DEBUG_SEVERITY_LOW:
			std::cout << fmt::format("INFO {}: {}", id, message) << std::endl;
			break;
//		case GL_DEBUG_SEVERITY_NOTIFICATION:
//			std::cout << fmt::format("NOTIF {}: {}", id, message) << std::endl;
//			break;
	}
}

int main(int argc, char** argv)
{
	stbi_set_flip_vertically_on_load(true);
	
	glfwInit();
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
#if _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
	
	glfwSetErrorCallback([](int code, const char* message) {
		std::cout << "GLFW Error" << code << ": " << message << std::endl;
	});

	if (argc == 2 && strcmp(argv[1], "--windowed") == 0 || IsDebuggerPresent())
	{
		Toolbox::window = glfwCreateWindow(800, 600, "Meinkraft", nullptr, nullptr);
	}
	else
	{
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		Toolbox::window = glfwCreateWindow(mode->width, mode->height, "Meinkraft", monitor, nullptr);
	}
	
	glfwMakeContextCurrent(Toolbox::window);
	glfwSetInputMode(Toolbox::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(Toolbox::window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	
	gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
	
	glEnable(GL_DEBUG_OUTPUT);
#if _DEBUG
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
//	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
#endif
	glDebugMessageCallback(messageCallback, nullptr);
	
	Toolbox::camera = std::make_unique<Camera>(glm::dvec3(8, 180, 8));
	Toolbox::world = std::make_unique<World>();
	Toolbox::renderer = std::make_unique<Renderer>();
	
	double previousTime = 0;
	double previousPrint = 0;
	
	while (!glfwWindowShouldClose(Toolbox::window))
	{
		glfwPollEvents();
		
		if (glfwGetKey(Toolbox::window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(Toolbox::window, GLFW_TRUE);
		
		double currentTime = glfwGetTime();
		double deltaTime = currentTime - previousTime;
		Toolbox::camera->update(deltaTime);
		previousTime = currentTime;
		
		previousPrint += deltaTime;
		if (previousPrint >= 1)
		{
			std::cout << "FPS: " << 1.0 / deltaTime << std::endl;
			previousPrint -= 1;
		}
		
		Toolbox::world->update();
		
		if (glfwGetWindowAttrib(Toolbox::window, GLFW_FOCUSED))
		{
			Toolbox::renderer->render();
			
			glfwSwapBuffers(Toolbox::window);
		}
	}
	
	Toolbox::renderer.reset();
	Toolbox::world.reset();
	Toolbox::camera.reset();
	
	glfwTerminate();
}