#version 460 core

layout(location = 0) out vec3 o_color;

in V2F
{
	vec3 fragDir;
} v2f;

vec3 background(vec3 dir);
vec3 srgbToLinear(vec3 srgb);
vec3 linearToSRGB(vec3 linear);

void main()
{
	o_color = background(normalize(v2f.fragDir));
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
