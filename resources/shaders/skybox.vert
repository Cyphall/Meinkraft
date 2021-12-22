#version 460 core

layout (location = 0) in ivec3 a_position;

out V2F
{
	vec3 fragDir;
} v2f;

uniform mat4 u_mvp;

void main()
{
	v2f.fragDir = vec3(a_position);
	vec4 pos = u_mvp * vec4(a_position, 1.0);
	gl_Position = pos.xyww;
} 