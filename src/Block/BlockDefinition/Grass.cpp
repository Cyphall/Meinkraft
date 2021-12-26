#include "Grass.h"

Grass::Grass():
	BlockDefinition(BlockType::OPAQUE)
{
	
}

void Grass::genVertices(std::vector<VertexData>& vertices, glm::u8vec3 pos, BlockType xp1, BlockType xm1, BlockType yp1, BlockType ym1, BlockType zp1, BlockType zm1)
{
	if (xp1 == BlockType::TRANSPARENT)
	{
		VertexData vertexBL;
		VertexData vertexTL;
		VertexData vertexBR;
		VertexData vertexTR;
		
		vertexBL.position = pos + glm::u8vec3(1, 0, 1);
		vertexTL.position = pos + glm::u8vec3(1, 1, 1);
		vertexBR.position = pos + glm::u8vec3(1, 0, 0);
		vertexTR.position = pos + glm::u8vec3(1, 1, 0);
		
		vertexBL.uv = glm::u8vec2(0, 0);
		vertexTL.uv = glm::u8vec2(0, 1);
		vertexBR.uv = glm::u8vec2(1, 0);
		vertexTR.uv = glm::u8vec2(1, 1);
		
		vertexBL.texture = BlockTextureId::GRASS_SIDE;
		vertexTL.texture = BlockTextureId::GRASS_SIDE;
		vertexBR.texture = BlockTextureId::GRASS_SIDE;
		vertexTR.texture = BlockTextureId::GRASS_SIDE;
		
		vertexBL.face = 0;
		vertexTL.face = 0;
		vertexBR.face = 0;
		vertexTR.face = 0;
		
		vertices.push_back(vertexBL);
		vertices.push_back(vertexTL);
		vertices.push_back(vertexBR);
		vertices.push_back(vertexBR);
		vertices.push_back(vertexTL);
		vertices.push_back(vertexTR);
	}
	
	if (xm1 == BlockType::TRANSPARENT)
	{
		VertexData vertexBL;
		VertexData vertexTL;
		VertexData vertexBR;
		VertexData vertexTR;
		
		vertexBL.position = pos + glm::u8vec3(0, 0, 0);
		vertexTL.position = pos + glm::u8vec3(0, 1, 0);
		vertexBR.position = pos + glm::u8vec3(0, 0, 1);
		vertexTR.position = pos + glm::u8vec3(0, 1, 1);
		
		vertexBL.uv = glm::u8vec2(0, 0);
		vertexTL.uv = glm::u8vec2(0, 1);
		vertexBR.uv = glm::u8vec2(1, 0);
		vertexTR.uv = glm::u8vec2(1, 1);
		
		vertexBL.texture = BlockTextureId::GRASS_SIDE;
		vertexTL.texture = BlockTextureId::GRASS_SIDE;
		vertexBR.texture = BlockTextureId::GRASS_SIDE;
		vertexTR.texture = BlockTextureId::GRASS_SIDE;
		
		vertexBL.face = 1;
		vertexTL.face = 1;
		vertexBR.face = 1;
		vertexTR.face = 1;
		
		vertices.push_back(vertexBL);
		vertices.push_back(vertexTL);
		vertices.push_back(vertexBR);
		vertices.push_back(vertexBR);
		vertices.push_back(vertexTL);
		vertices.push_back(vertexTR);
	}
	
	if (yp1 == BlockType::TRANSPARENT)
	{
		VertexData vertexBL;
		VertexData vertexTL;
		VertexData vertexBR;
		VertexData vertexTR;
		
		vertexBL.position = pos + glm::u8vec3(0, 1, 0);
		vertexTL.position = pos + glm::u8vec3(1, 1, 0);
		vertexBR.position = pos + glm::u8vec3(0, 1, 1);
		vertexTR.position = pos + glm::u8vec3(1, 1, 1);
		
		vertexBL.uv = glm::u8vec2(0, 0);
		vertexTL.uv = glm::u8vec2(0, 1);
		vertexBR.uv = glm::u8vec2(1, 0);
		vertexTR.uv = glm::u8vec2(1, 1);
		
		vertexBL.texture = BlockTextureId::GRASS_TOP;
		vertexTL.texture = BlockTextureId::GRASS_TOP;
		vertexBR.texture = BlockTextureId::GRASS_TOP;
		vertexTR.texture = BlockTextureId::GRASS_TOP;
		
		vertexBL.face = 2;
		vertexTL.face = 2;
		vertexBR.face = 2;
		vertexTR.face = 2;
		
		vertices.push_back(vertexBL);
		vertices.push_back(vertexTL);
		vertices.push_back(vertexBR);
		vertices.push_back(vertexBR);
		vertices.push_back(vertexTL);
		vertices.push_back(vertexTR);
	}
	
	if (ym1 == BlockType::TRANSPARENT)
	{
		VertexData vertexBL;
		VertexData vertexTL;
		VertexData vertexBR;
		VertexData vertexTR;
		
		vertexBL.position = pos + glm::u8vec3(1, 0, 0);
		vertexTL.position = pos + glm::u8vec3(0, 0, 0);
		vertexBR.position = pos + glm::u8vec3(1, 0, 1);
		vertexTR.position = pos + glm::u8vec3(0, 0, 1);
		
		vertexBL.uv = glm::u8vec2(0, 0);
		vertexTL.uv = glm::u8vec2(0, 1);
		vertexBR.uv = glm::u8vec2(1, 0);
		vertexTR.uv = glm::u8vec2(1, 1);
		
		vertexBL.texture = BlockTextureId::DIRT;
		vertexTL.texture = BlockTextureId::DIRT;
		vertexBR.texture = BlockTextureId::DIRT;
		vertexTR.texture = BlockTextureId::DIRT;
		
		vertexBL.face = 3;
		vertexTL.face = 3;
		vertexBR.face = 3;
		vertexTR.face = 3;
		
		vertices.push_back(vertexBL);
		vertices.push_back(vertexTL);
		vertices.push_back(vertexBR);
		vertices.push_back(vertexBR);
		vertices.push_back(vertexTL);
		vertices.push_back(vertexTR);
	}
	
	if (zp1 == BlockType::TRANSPARENT)
	{
		VertexData vertexBL;
		VertexData vertexTL;
		VertexData vertexBR;
		VertexData vertexTR;
		
		vertexBL.position = pos + glm::u8vec3(0, 0, 1);
		vertexTL.position = pos + glm::u8vec3(0, 1, 1);
		vertexBR.position = pos + glm::u8vec3(1, 0, 1);
		vertexTR.position = pos + glm::u8vec3(1, 1, 1);
		
		vertexBL.uv = glm::u8vec2(0, 0);
		vertexTL.uv = glm::u8vec2(0, 1);
		vertexBR.uv = glm::u8vec2(1, 0);
		vertexTR.uv = glm::u8vec2(1, 1);
		
		vertexBL.texture = BlockTextureId::GRASS_SIDE;
		vertexTL.texture = BlockTextureId::GRASS_SIDE;
		vertexBR.texture = BlockTextureId::GRASS_SIDE;
		vertexTR.texture = BlockTextureId::GRASS_SIDE;
		
		vertexBL.face = 4;
		vertexTL.face = 4;
		vertexBR.face = 4;
		vertexTR.face = 4;
		
		vertices.push_back(vertexBL);
		vertices.push_back(vertexTL);
		vertices.push_back(vertexBR);
		vertices.push_back(vertexBR);
		vertices.push_back(vertexTL);
		vertices.push_back(vertexTR);
	}
	
	if (zm1 == BlockType::TRANSPARENT)
	{
		VertexData vertexBL;
		VertexData vertexTL;
		VertexData vertexBR;
		VertexData vertexTR;
		
		vertexBL.position = pos + glm::u8vec3(1, 0, 0);
		vertexTL.position = pos + glm::u8vec3(1, 1, 0);
		vertexBR.position = pos + glm::u8vec3(0, 0, 0);
		vertexTR.position = pos + glm::u8vec3(0, 1, 0);
		
		vertexBL.uv = glm::u8vec2(0, 0);
		vertexTL.uv = glm::u8vec2(0, 1);
		vertexBR.uv = glm::u8vec2(1, 0);
		vertexTR.uv = glm::u8vec2(1, 1);
		
		vertexBL.texture = BlockTextureId::GRASS_SIDE;
		vertexTL.texture = BlockTextureId::GRASS_SIDE;
		vertexBR.texture = BlockTextureId::GRASS_SIDE;
		vertexTR.texture = BlockTextureId::GRASS_SIDE;
		
		vertexBL.face = 5;
		vertexTL.face = 5;
		vertexBR.face = 5;
		vertexTR.face = 5;
		
		vertices.push_back(vertexBL);
		vertices.push_back(vertexTL);
		vertices.push_back(vertexBR);
		vertices.push_back(vertexBR);
		vertices.push_back(vertexTL);
		vertices.push_back(vertexTR);
	}
}
