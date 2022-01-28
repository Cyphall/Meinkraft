#version 460 core

// ---------- DEFINITIONS ----------

struct GlobalUniform
{
	mat4 viewProjection;
	vec3 viewPos;
	float padding;
};

struct ChunkUniform
{
	mat4 model;
	mat4 normalMatrix;
};

// ---------- INPUTS ----------

layout(location = 0) in uvec3 i_position;
layout(location = 1) in uvec2 i_uv;
layout(location = 2) in uint i_face;
layout(location = 3) in uint i_textureId;

// ---------- UNIFORMS ----------

layout(std430, binding = 0) buffer _0
{
	GlobalUniform globalUniform;
};

layout(std430, binding = 1) buffer _1
{
	ChunkUniform chunkUniforms[];
};

// ---------- OUTPUTS ----------

layout(location = 0) out vec2 o_texCoords;
layout(location = 1) out mat3 o_tangentToWorld;
layout(location = 4) out mat3 o_worldToTangent;
layout(location = 7) out vec3 o_fragPos;
layout(location = 8) flat out uint o_textureId;

// ---------- LOGIC ----------

const vec3 normalLookup[6] = vec3[](
	vec3( 1,  0,  0),
	vec3(-1,  0,  0),
	vec3( 0,  1,  0),
	vec3( 0, -1,  0),
	vec3( 0,  0,  1),
	vec3( 0,  0, -1)
);

const vec3 tangentLookup[6] = vec3[](
	vec3( 0,  0, -1),
	vec3( 0,  0,  1),
	vec3( 0,  0,  1),
	vec3( 0,  0,  1),
	vec3( 1,  0,  0),
	vec3(-1,  0,  0)
);

void main()
{
	vec3 position = vec3(i_position);
	
	o_texCoords = vec2(i_uv);

	vec4 worldPosition = chunkUniforms[gl_DrawID].model * vec4(position, 1.0);
	o_fragPos = vec3(worldPosition);
	
	o_textureId = i_textureId;

	vec3 N = normalize(mat3(chunkUniforms[gl_DrawID].normalMatrix) * normalLookup[i_face]);
	vec3 T = normalize(mat3(chunkUniforms[gl_DrawID].normalMatrix) * tangentLookup[i_face]);
	vec3 B = cross(N, T);
	
	o_tangentToWorld = mat3(T, B, N);
	o_worldToTangent = transpose(o_tangentToWorld);

	gl_Position = globalUniform.viewProjection * worldPosition;
}