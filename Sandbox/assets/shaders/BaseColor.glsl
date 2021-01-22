#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

uniform mat4 u_ViewProjection;

out vec4 v_Color;

void main()
{
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
	v_Color = vec4(0.2, 0.8, 0.3, 1);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_Color;

void main()
{
	color = v_Color;
}