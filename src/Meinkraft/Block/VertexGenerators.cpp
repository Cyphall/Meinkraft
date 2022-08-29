#include "VertexGenerators.h"

static VertexData makeVertexData(glm::u8vec3 position, glm::u8vec2 uv, uint8_t face, BlockTextureId texture)
{
	return VertexData{
		.position = position,
		.uv = uv,
		.face = face,
		.texture = texture
	};
}

void emptyVertexGenerator(std::vector<VertexData>& output, glm::u8vec3 pos, BlockType xp1, BlockType xm1, BlockType yp1, BlockType ym1, BlockType zp1, BlockType zm1, const std::unordered_map<std::string_view, BlockTextureId>& textures)
{
	
}

void cubicVertexGenerator(std::vector<VertexData>& output, glm::u8vec3 pos, BlockType xp1, BlockType xm1, BlockType yp1, BlockType ym1, BlockType zp1, BlockType zm1, const std::unordered_map<std::string_view, BlockTextureId>& textures)
{
	if (xp1 == BlockType::TRANSPARENT)
	{
		BlockTextureId texture = textures.at("xp1");
		
		VertexData vertexBL = makeVertexData(pos + glm::u8vec3(1, 0, 1), glm::u8vec2(0, 0), 0, texture);
		VertexData vertexTL = makeVertexData(pos + glm::u8vec3(1, 1, 1), glm::u8vec2(0, 1), 0, texture);
		VertexData vertexBR = makeVertexData(pos + glm::u8vec3(1, 0, 0), glm::u8vec2(1, 0), 0, texture);
		VertexData vertexTR = makeVertexData(pos + glm::u8vec3(1, 1, 0), glm::u8vec2(1, 1), 0, texture);
		
		output.push_back(vertexBL);
		output.push_back(vertexTL);
		output.push_back(vertexBR);
		output.push_back(vertexBR);
		output.push_back(vertexTL);
		output.push_back(vertexTR);
	}
	
	if (xm1 == BlockType::TRANSPARENT)
	{
		BlockTextureId texture = textures.at("xm1");
		
		VertexData vertexBL = makeVertexData(pos + glm::u8vec3(0, 0, 0), glm::u8vec2(0, 0), 1, texture);
		VertexData vertexTL = makeVertexData(pos + glm::u8vec3(0, 1, 0), glm::u8vec2(0, 1), 1, texture);
		VertexData vertexBR = makeVertexData(pos + glm::u8vec3(0, 0, 1), glm::u8vec2(1, 0), 1, texture);
		VertexData vertexTR = makeVertexData(pos + glm::u8vec3(0, 1, 1), glm::u8vec2(1, 1), 1, texture);
		
		output.push_back(vertexBL);
		output.push_back(vertexTL);
		output.push_back(vertexBR);
		output.push_back(vertexBR);
		output.push_back(vertexTL);
		output.push_back(vertexTR);
	}
	
	if (yp1 == BlockType::TRANSPARENT)
	{
		BlockTextureId texture = textures.at("yp1");
		
		VertexData vertexBL = makeVertexData(pos + glm::u8vec3(0, 1, 0), glm::u8vec2(0, 0), 2, texture);
		VertexData vertexTL = makeVertexData(pos + glm::u8vec3(1, 1, 0), glm::u8vec2(0, 1), 2, texture);
		VertexData vertexBR = makeVertexData(pos + glm::u8vec3(0, 1, 1), glm::u8vec2(1, 0), 2, texture);
		VertexData vertexTR = makeVertexData(pos + glm::u8vec3(1, 1, 1), glm::u8vec2(1, 1), 2, texture);
		
		output.push_back(vertexBL);
		output.push_back(vertexTL);
		output.push_back(vertexBR);
		output.push_back(vertexBR);
		output.push_back(vertexTL);
		output.push_back(vertexTR);
	}
	
	if (ym1 == BlockType::TRANSPARENT)
	{
		BlockTextureId texture = textures.at("ym1");
		
		VertexData vertexBL = makeVertexData(pos + glm::u8vec3(1, 0, 0), glm::u8vec2(0, 0), 3, texture);
		VertexData vertexTL = makeVertexData(pos + glm::u8vec3(0, 0, 0), glm::u8vec2(0, 1), 3, texture);
		VertexData vertexBR = makeVertexData(pos + glm::u8vec3(1, 0, 1), glm::u8vec2(1, 0), 3, texture);
		VertexData vertexTR = makeVertexData(pos + glm::u8vec3(0, 0, 1), glm::u8vec2(1, 1), 3, texture);
		
		output.push_back(vertexBL);
		output.push_back(vertexTL);
		output.push_back(vertexBR);
		output.push_back(vertexBR);
		output.push_back(vertexTL);
		output.push_back(vertexTR);
	}
	
	if (zp1 == BlockType::TRANSPARENT)
	{
		BlockTextureId texture = textures.at("zp1");
		
		VertexData vertexBL = makeVertexData(pos + glm::u8vec3(0, 0, 1), glm::u8vec2(0, 0), 4, texture);
		VertexData vertexTL = makeVertexData(pos + glm::u8vec3(0, 1, 1), glm::u8vec2(0, 1), 4, texture);
		VertexData vertexBR = makeVertexData(pos + glm::u8vec3(1, 0, 1), glm::u8vec2(1, 0), 4, texture);
		VertexData vertexTR = makeVertexData(pos + glm::u8vec3(1, 1, 1), glm::u8vec2(1, 1), 4, texture);
		
		output.push_back(vertexBL);
		output.push_back(vertexTL);
		output.push_back(vertexBR);
		output.push_back(vertexBR);
		output.push_back(vertexTL);
		output.push_back(vertexTR);
	}
	
	if (zm1 == BlockType::TRANSPARENT)
	{
		BlockTextureId texture = textures.at("zm1");
		
		VertexData vertexBL = makeVertexData(pos + glm::u8vec3(1, 0, 0), glm::u8vec2(0, 0), 5, texture);
		VertexData vertexTL = makeVertexData(pos + glm::u8vec3(1, 1, 0), glm::u8vec2(0, 1), 5, texture);
		VertexData vertexBR = makeVertexData(pos + glm::u8vec3(0, 0, 0), glm::u8vec2(1, 0), 5, texture);
		VertexData vertexTR = makeVertexData(pos + glm::u8vec3(0, 1, 0), glm::u8vec2(1, 1), 5, texture);
		
		output.push_back(vertexBL);
		output.push_back(vertexTL);
		output.push_back(vertexBR);
		output.push_back(vertexBR);
		output.push_back(vertexTL);
		output.push_back(vertexTR);
	}
}