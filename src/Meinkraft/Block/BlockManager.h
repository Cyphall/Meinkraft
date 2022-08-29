#pragma once

#include "Meinkraft/GL/VertexData.h"
#include "Meinkraft/BlockId.h"
#include "Meinkraft/Block/BlockType.h"

#include <unordered_map>

struct BlockDefinition;

class BlockManager
{
public:
	static void init();
	static BlockDefinition& getBlockDefinition(BlockId blockId);
	
private:
	static std::unordered_map<BlockId, BlockDefinition> _blocks;
	
	static void registerBlock(BlockId blockId, const BlockDefinition& definition);
};