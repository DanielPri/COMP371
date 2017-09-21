#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 colour;
out vec3 col;

uniform mat4 m_matrix;
uniform mat4 view_matrix;
uniform mat4 p_matrix;

void main()
{
	gl_Position = p_matrix * view_matrix * m_matrix * vec4(position, 1.0);
	col = colour;
}