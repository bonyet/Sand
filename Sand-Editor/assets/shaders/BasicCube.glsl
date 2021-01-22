#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in int a_ObjectID;

uniform mat4 u_ViewProjection;

out vec4 v_Color;
out flat int v_ObjectID;

void main()
{
	v_Color = a_Color;
	v_ObjectID = a_ObjectID;

	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 color;
layout(location = 1) out int o_IDBuffer;

in vec4 v_Color;
in flat int v_ObjectID;

void main()
{
	color = v_Color;
	o_IDBuffer = v_ObjectID;
}