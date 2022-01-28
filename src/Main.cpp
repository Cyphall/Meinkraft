#include <iostream>
#include "Camera.h"
#include "World.h"
#include "Renderer.h"
#include "Toolbox.h"
#include <fmt/format.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>

#include <GLFW/glfw3.h>

#define VERSION(major, minor, revision) (major * 100 + minor * 10 + revision)

void messageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
	switch (severity)
	{
		case GL_DEBUG_SEVERITY_HIGH:
			std::cout << fmt::format("ERROR {}: {}", id, message) << std::endl;
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

bool checkDriverSupport()
{
	glfwWindowHint(GLFW_VISIBLE, false);
	
	GLFWwindow* window = glfwCreateWindow(1, 1, "CompatibilityQuery", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	
	gladLoadGL(glfwGetProcAddress);
	
	int major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
	int minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
	int revision = glfwGetWindowAttrib(window, GLFW_CONTEXT_REVISION);
	int maxSupportedOpenGLVersion = VERSION(major, minor, revision);
	int requestedOpenGLVersion = VERSION(4, 6, 0);
	
	std::stringstream errorMessage;
	bool error = false;
	if (maxSupportedOpenGLVersion < requestedOpenGLVersion)
	{
		errorMessage << "Required OpenGL " << requestedOpenGLVersion << " is not supported by this driver.\n";
		errorMessage << "Please make sure your GPU is compatible and your driver is up to date.\n\n";
		errorMessage << "Driver: " << glGetString(GL_VERSION) << "\n";
		errorMessage << "GPU: " << glGetString(GL_RENDERER) << "\n";
		error = true;
	}
	
	std::string requiredExtensions[] = {
			"GL_ARB_bindless_texture",
			"GL_NV_bindless_multi_draw_indirect",
			"GL_NV_shader_buffer_load",
			"GL_NV_vertex_buffer_unified_memory"
	};
	
	for (const std::string& extension : requiredExtensions)
	{
		if (error)
			break;
		
		if (!glfwExtensionSupported(extension.c_str()))
		{
			errorMessage << "OpenGL extension " << extension << " is not supported by this driver.\n";
			errorMessage << "Please make sure your GPU is compatible and your driver is up to date.\n\n";
			errorMessage << "Driver: " << glGetString(GL_VERSION) << "\n";
			errorMessage << "GPU: " << glGetString(GL_RENDERER) << "\n";
			error = true;
		}
	}
	
	glfwMakeContextCurrent(nullptr);
	glfwDestroyWindow(window);
	
	glfwWindowHint(GLFW_VISIBLE, true);
	
	if (error)
	{
		std::cout << errorMessage.str() << std::endl;
	}
	
	return !error;
}

int main(int argc, char** argv)
{
	stbi_set_flip_vertically_on_load(true);
	
	glfwInit();
	
	glfwSetErrorCallback([](int code, const char* message) {
		std::cout << "GLFW Error" << code << ": " << message << std::endl;
	});
	
	if (!checkDriverSupport())
	{
		system("pause");
		return 1;
	}
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
#if _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

#if _DEBUG
	if (true)
#else
	if (argc == 2 && strcmp(argv[1], "--windowed") == 0)
#endif

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
	
	glEnable(GL_DEBUG_OUTPUT);
#if _DEBUG
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
//	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
#endif
	glDebugMessageCallback(messageCallback, nullptr);
	
	BlockDefinition::init();
	
	Toolbox::camera = std::make_unique<Camera>(glm::dvec3(8, 170, 8));
	try
	{
		Toolbox::renderer = std::make_unique<Renderer>();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		throw e;
	}
	Toolbox::world = std::make_unique<World>();
	
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
	
	Toolbox::world.reset();
	Toolbox::renderer.reset();
	Toolbox::camera.reset();
	
	glfwTerminate();
}