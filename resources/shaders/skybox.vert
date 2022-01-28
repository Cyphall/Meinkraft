#version 460 core

// ---------- DEFINITIONS ----------

// ---------- INPUTS ----------

layout (location = 0) in ivec3 a_position;

// ---------- UNIFORMS ----------

uniform mat4 u_mvp;

// ---------- OUTPUTS ----------

layout(location = 0) out vec3 o_fragDir;

// ---------- LOGIC ----------

void main()
{
	o_fragDir = vec3(a_position);
	vec4 pos = u_mvp * vec4(a_position, 1.0);
	gl_Position = pos.xyww;
} 