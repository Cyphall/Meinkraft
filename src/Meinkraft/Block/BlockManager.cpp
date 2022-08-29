#include "BlockManager.h"

#include "Meinkraft/Block/BlockDefinition.h"

#include "Meinkraft/Block/VertexGenerators.h"

std::unordered_map<BlockId, BlockDefinition> BlockManager::_blocks;

void BlockManager::init()
{
	registerBlock(BlockId::AIR, {
		.type = BlockType::TRANSPARENT,
		.vertexGenerator = emptyVertexGenerator
	});
	
	registerBlock(BlockId::STONE, {
		.type = BlockType::OPAQUE,
		.vertexGenerator = cubicVertexGenerator,
		.textures = {
			{"xp1", BlockTextureId::STONE},
			{"xm1", BlockTextureId::STONE},
			{"yp1", BlockTextureId::STONE},
			{"ym1", BlockTextureId::STONE},
			{"zp1", BlockTextureId::STONE},
			{"zm1", BlockTextureId::STONE}
		}
	});
	
	registerBlock(BlockId::GRASS, {
		.type = BlockType::OPAQUE,
		.vertexGenerator = cubicVertexGenerator,
		.textures = {
			{"xp1", BlockTextureId::GRASS_SIDE},
			{"xm1", BlockTextureId::GRASS_SIDE},
			{"yp1", BlockTextureId::GRASS_TOP},
			{"ym1", BlockTextureId::DIRT},
			{"zp1", BlockTextureId::GRASS_SIDE},
			{"zm1", BlockTextureId::GRASS_SIDE}
		}
	});
	
	registerBlock(BlockId::DIRT, {
		.type = BlockType::OPAQUE,
		.vertexGenerator = cubicVertexGenerator,
		.textures = {
			{"xp1", BlockTextureId::DIRT},
			{"xm1", BlockTextureId::DIRT},
			{"yp1", BlockTextureId::DIRT},
			{"ym1", BlockTextureId::DIRT},
			{"zp1", BlockTextureId::DIRT},
			{"zm1", BlockTextureId::DIRT}
		}
	});
	
	registerBlock(BlockId::WOOD, {
		.type = BlockType::OPAQUE,
		.vertexGenerator = cubicVertexGenerator,
		.textures = {
			{"xp1", BlockTextureId::WOOD},
			{"xm1", BlockTextureId::WOOD},
			{"yp1", BlockTextureId::WOOD},
			{"ym1", BlockTextureId::WOOD},
			{"zp1", BlockTextureId::WOOD},
			{"zm1", BlockTextureId::WOOD}
		}
	});
	
	registerBlock(BlockId::IRON, {
		.type = BlockType::OPAQUE,
		.vertexGenerator = cubicVertexGenerator,
		.textures = {
			{"xp1", BlockTextureId::IRON},
			{"xm1", BlockTextureId::IRON},
			{"yp1", BlockTextureId::IRON},
			{"ym1", BlockTextureId::IRON},
			{"zp1", BlockTextureId::IRON},
			{"zm1", BlockTextureId::IRON}
		}
	});
}

BlockDefinition& BlockManager::getBlockDefinition(BlockId blockId)
{
	return _blocks.at(blockId);
}

void BlockManager::registerBlock(BlockId blockId, const BlockDefinition& definition)
{
	if (!_blocks.try_emplace(blockId, definition).second)
	{
		throw;
	}
}