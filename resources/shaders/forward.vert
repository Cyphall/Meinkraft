#version 460 core

layout(location = 0) in uvec3 in_position;
layout(location = 1) in uvec2 in_uv;
layout(location = 2) in uint in_face;
layout(location = 3) in uint in_block;

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

layout(std430, binding = 0) buffer _0
{
    GlobalUniform globalUniform;
};

layout(std430, binding = 1) buffer _1
{
    ChunkUniform chunkUniforms[];
};

out FRAG {
    vec2 TexCoords;
    mat3 TangentToWorld;
    mat3 WorldToTangent;
    vec3 FragPos;
} frag;

flat out uint Block;
flat out uint Face;

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
    vec3 position = vec3(in_position);

    frag.TexCoords = vec2(in_uv);

    frag.FragPos = vec3(chunkUniforms[gl_DrawID].model * vec4(position, 1.0));

    Block = in_block;
    Face = in_face;

    vec3 N = normalize(mat3(chunkUniforms[gl_DrawID].normalMatrix) * normalLookup[in_face]);
    vec3 T = normalize(mat3(chunkUniforms[gl_DrawID].normalMatrix) * tangentLookup[in_face]);
    vec3 B = cross(N, T);

    frag.TangentToWorld = mat3(T, B, N);
    frag.WorldToTangent = transpose(frag.TangentToWorld);

    gl_Position = globalUniform.viewProjection * chunkUniforms[gl_DrawID].model * vec4(position, 1.0);
}