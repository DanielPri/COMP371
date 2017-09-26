#version 330 core

out vec4 color;
uniform int fill;

void main()
{
	//0 is YELLOW
	if (fill == 0) {
		color = vec4(1.0f, 1.0f, 0.0f, 1.0f);
	}
	//1 is WHITE
	if (fill == 1) {
		color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	//2 is RED
	if (fill == 2) {
		color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	//3 is GREEN
	if (fill == 3) {
		color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	}
} 