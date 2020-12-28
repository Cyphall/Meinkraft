#include "BlockTextures.h"
#include <fmt/format.h>

BlockTextures::BlockTextures(const std::string& name):
_forward(fmt::format("{}/forward.png", name)),
_back(fmt::format("{}/back.png", name)),
_top(fmt::format("{}/top.png", name)),
_bottom(fmt::format("{}/bottom.png", name)),
_right(fmt::format("{}/right.png", name)),
_left(fmt::format("{}/left.png", name))
{}

BlockTextures::NativeData BlockTextures::getData() const
{
	return
	{
		.forwardTexture = _forward.getBindlessHandle(),
		.backTexture = _back.getBindlessHandle(),
		.topTexture = _top.getBindlessHandle(),
		.bottomTexture = _bottom.getBindlessHandle(),
		.rightTexture = _right.getBindlessHandle(),
		.leftTexture = _left.getBindlessHandle()
	};
}
