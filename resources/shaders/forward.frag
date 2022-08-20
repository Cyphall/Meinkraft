#version 460 core
#extension GL_ARB_bindless_texture : enable

// ---------- DEFINITIONS ----------

struct GlobalUniform
{
	mat4 viewProjection;
	vec3 viewPos;
	float padding;
};

vec3 toSRGB(vec3 linear);
vec3 ACESFilm(vec3 x);

// ---------- INPUTS ----------

layout(location = 0) in vec2 i_texCoords;
layout(location = 1) in mat3 i_tangentToWorld;
layout(location = 4) in mat3 i_worldToTangent;
layout(location = 7) in vec3 i_fragPos;
layout(location = 8) flat in uint i_textureId;

// ---------- UNIFORMS ----------

layout(std430, binding = 0) buffer _2
{
	layout(bindless_sampler) sampler2D textures[];
};

layout(std430, binding = 1) buffer _0
{
	GlobalUniform globalUniform;
};

// ---------- OUTPUTS ----------

out vec4 o_color;

// ---------- LOGIC ----------

void main()
{
	vec3 viewDir = normalize(globalUniform.viewPos - i_fragPos);
	vec3 normal = i_tangentToWorld * vec3(0, 0, 1);
	vec3 lightColor = vec3(1);
	vec3 lightDir = normalize(vec3(1, 1, 1));
	vec3 albedo = texture(textures[i_textureId], i_texCoords).rgb;

	vec3 finalColor = vec3(0);

	//ambient
	finalColor += albedo * 0.1;

	//diffuse
	finalColor += albedo * max(dot(lightDir, normal), 0);

	//specular
	finalColor += vec3(1) * pow(max(dot(reflect(-lightDir, normal), viewDir), 0), 128) * 0.3;

	o_color = vec4(toSRGB(ACESFilm(finalColor)), 1);
}

vec3 toSRGB(vec3 linear)
{
	bvec3 cutoff = lessThan(linear, vec3(0.0031308));
	vec3 higher = vec3(1.055) * pow(linear, vec3(1.0/2.4)) - vec3(0.055);
	vec3 lower = linear * vec3(12.92);

	return mix(higher, lower, cutoff);
}

vec3 ACESFilm(vec3 x)
{
	float a = 2.51f;
	float b = 0.03f;
	float c = 2.43f;
	float d = 0.59f;
	float e = 0.14f;
	return clamp((x*(a*x+b))/(x*(c*x+d)+e), 0.0f, 1.0f);
}