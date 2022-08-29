#pragma once

#include "Meinkraft/GL/VertexData.h"
#include "Meinkraft/Block/BlockType.h"

#include <vector>
#include <unordered_map>

void emptyVertexGenerator(std::vector<VertexData>& output, glm::u8vec3 pos, BlockType xp1, BlockType xm1, BlockType yp1, BlockType ym1, BlockType zp1, BlockType zm1, const std::unordered_map<std::string_view, BlockTextureId>& textures);
void cubicVertexGenerator(std::vector<VertexData>& output, glm::u8vec3 pos, BlockType xp1, BlockType xm1, BlockType yp1, BlockType ym1, BlockType zp1, BlockType zm1, const std::unordered_map<std::string_view, BlockTextureId>& textures);