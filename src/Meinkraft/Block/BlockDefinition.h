#pragma once

#include <memory>
#include <array>
#include <vector>
#include "BlockType.h"
#include "../GL/VertexData.h"
#include "../BlockId.h"

class BlockDefinition
{
public:
	static void init();
	static BlockDefinition* getBlockDefinition(BlockId blockId);
	
	BlockDefinition(BlockType type);
	
	BlockType getType() const;
	
	
	virtual void genVertices(std::vector<VertexData>& vertices, glm::u8vec3 pos, BlockType xp1, BlockType xm1, BlockType yp1, BlockType ym1, BlockType zp1, BlockType zm1) = 0;
	
protected:
	BlockType _type;
	
private:
	static inline std::array<std::unique_ptr<BlockDefinition>, 256> _blocks{};
	
	
};