
#version 330 core
  
layout (location = 0) in vec3 position;
out vec3 heightColor;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
    
	gl_Position = projection_matrix * view_matrix * model_matrix * vec4(position.x, position.y, position.z, 1.0);
	heightColor = vec3(position.y/255.0f, 0, 1.0f - position.y/255.0f);
}