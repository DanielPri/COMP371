#version 330 core

out vec4 color;
in vec3 heightColor;

void main()
{
	color = vec4(heightColor, 1.0f);
	//orange
    //color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
} 