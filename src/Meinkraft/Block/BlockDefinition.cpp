#include "BlockDefinition.h"

#include "Meinkraft/Block/BlockDefinition/Air.h"
#include "Meinkraft/Block/BlockDefinition/Stone.h"
#include "Meinkraft/Block/BlockDefinition/Dirt.h"
#include "Meinkraft/Block/BlockDefinition/Grass.h"

void BlockDefinition::init()
{
	_blocks[static_cast<uint8_t>(BlockId::AIR)] = std::make_unique<Air>();
	_blocks[static_cast<uint8_t>(BlockId::STONE)] = std::make_unique<Stone>();
	_blocks[static_cast<uint8_t>(BlockId::DIRT)] = std::make_unique<Dirt>();
	_blocks[static_cast<uint8_t>(BlockId::GRASS)] = std::make_unique<Grass>();
}

BlockType BlockDefinition::getType() const
{
	return _type;
}

BlockDefinition::BlockDefinition(BlockType type):
_type(type)
{}

BlockDefinition* BlockDefinition::getBlockDefinition(BlockId blockId)
{
	return _blocks[static_cast<uint8_t>(blockId)].get();
}