#pragma once

#include "Meinkraft/GL/VertexData.h"
#include "Meinkraft/BlockId.h"
#include "Meinkraft/Block/BlockType.h"

#include <memory>
#include <array>
#include <vector>
#include <functional>

struct VertexGenerationInfo;

struct BlockDefinition
{
	BlockType type;
	void (*vertexGenerator)(std::vector<VertexData>& output, glm::u8vec3 pos, BlockType xp1, BlockType xm1, BlockType yp1, BlockType ym1, BlockType zp1, BlockType zm1, const std::unordered_map<std::string_view, BlockTextureId>& textures);
	std::unordered_map<std::string_view, BlockTextureId> textures;
};