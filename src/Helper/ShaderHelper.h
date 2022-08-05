#pragma once

#include <string>
#include <format>

class ShaderHelper
{
public:
	static std::string shaderTypeToExtension(GLenum type)
	{
		switch (type)
		{
			case GL_VERTEX_SHADER:
				return "vert";
			case GL_FRAGMENT_SHADER:
				return "frag";
			case GL_GEOMETRY_SHADER:
				return "geom";
			default:
				throw std::runtime_error(std::format("Shader type {} is not currently supported", type));
		}
	}
};