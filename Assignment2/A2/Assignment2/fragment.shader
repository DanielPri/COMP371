#version 330 core

out vec4 color;
in float ypos;

void main()
{
	color = vec4(ypos, ypos, ypos, 1.0f);
	//orange
    //color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
} 