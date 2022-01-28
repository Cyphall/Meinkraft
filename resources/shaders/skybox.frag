#version 460 core

// ---------- DEFINITIONS ----------

#define PI 3.14159265359f

vec3 background(vec3 dir);
vec3 srgbToLinear(vec3 srgb);
vec3 linearToSRGB(vec3 linear);
float degToRad(float deg);
float radToDeg(float rad);

// ---------- INPUTS ----------
layout(location = 0) in vec3 i_fragDir;
// ---------- UNIFORMS ----------

// ---------- OUTPUTS ----------

layout(location = 0) out vec3 o_color;

// ---------- LOGIC ----------

void main()
{
	vec3 fragDir = normalize(i_fragDir);
	
	vec3 sunDir = normalize(vec3(1, 1, 1));
	float fragDirDotSun = max(dot(fragDir, sunDir), 0);
	float sunIntensity = pow(fragDirDotSun, 32);
	o_color = mix(background(fragDir), vec3(1), sunIntensity);
}

vec3 background(vec3 dir)
{
	const vec3 sky = vec3(0.21763764, 0.45626346, 1.0);
	const vec3 ground = vec3(1.0, 1.0, 1.0);
	
	float t = dir.y * 0.5 + 0.5;
	return mix(ground, sky, t);
}

vec3 srgbToLinear(vec3 srgb)
{
	bvec3 cutoff = lessThan(srgb, vec3(0.04045));
	vec3 higher = pow((srgb + vec3(0.055)) / vec3(1.055), vec3(2.4));
	vec3 lower = srgb / vec3(12.92);
	
	return mix(higher, lower, cutoff);
}

vec3 linearToSRGB(vec3 linear)
{
	bvec3 cutoff = lessThan(linear, vec3(0.0031308));
	vec3 higher = vec3(1.055) * pow(linear, vec3(1.0 / 2.4)) - vec3(0.055);
	vec3 lower = linear * vec3(12.92);
	
	return mix(higher, lower, cutoff);
}

float degToRad(float deg)
{
	return deg * PI / 180.0f;
}

float radToDeg(float rad)
{
	return rad * 180.0f / PI;
}
