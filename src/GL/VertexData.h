#pragma once

#include <glm/glm.hpp>

/* faces:
0: positive x
1: negative x
2: positive y
3: negative y
4: positive z
5: negative z
 */
struct VertexData
{
	glm::u8vec3 position; // xxxxxxxx, yyyyyyyy, zzzzzzzz
	glm::u8vec2 uv;       // uuuuuuuu, vvvvvvvv
	uint8_t face;         // ffffffff
	uint8_t block;        // bbbbbbbb
};
