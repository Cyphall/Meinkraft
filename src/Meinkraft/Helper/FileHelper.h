#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <format>

class FileHelper
{
public:
	static std::string readAllText(const std::string &path)
	{
		std::ifstream in(path, std::ios::in | std::ios::binary);
		if (in)
		{
			std::string contents;
			in.seekg(0, std::ios::end);
			contents.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&contents[0], contents.size());
			in.close();
			return contents;
		}
		throw std::ios_base::failure(std::format("Could not find file \"{}\"", path));
	}
};