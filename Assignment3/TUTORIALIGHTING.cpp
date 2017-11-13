// template.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>


#include "objloader.h"

const GLuint WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;
GLFWwindow* window;

GLuint loadShaders(std::string vertex_shader_path, std::string fragment_shader_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_shader_path, std::ios::in);

	if (VertexShaderStream.is_open()) {
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ?\n", vertex_shader_path.c_str());
		getchar();
		exit(-1);
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_shader_path, std::ios::in);

	if (FragmentShaderStream.is_open()) {
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ?\n", fragment_shader_path.c_str());
		getchar();
		exit(-1);
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_shader_path.c_str());
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, nullptr);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, nullptr, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_shader_path.c_str());
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, nullptr);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, nullptr, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);

	glBindAttribLocation(ProgramID, 0, "in_Position");

	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, nullptr, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDeleteShader(VertexShaderID); //free up memory
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

int init() {
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Lab", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// Define the viewport dimensions
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

int main()
{
	if (init() < 0) {
		return -1;
	}
	
	GLuint shaderProgram = loadShaders("vertex.shader", "fragment.shader");
	glUseProgram(shaderProgram);

	std::vector<glm::vec3> vertices;
	std::vector<GLuint> indices;
	std::vector<glm::vec3> normals;

	

	loadOBJ("cube.obj", vertices, normals);

	//vertices = {
	//	glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0),
	//	glm::vec3(1, 1, 0)
	//};

	//indices = {
	//	2,3,1,2,1,0
	//};

	//GLuint instances[] = {
	//	1, 1, 1, 2,2,2, 0,2,0 };

	GLuint VBO, VAO, EBO, VBO_normals, VBO_inst;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &VBO_normals);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*normals.size(), &normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	//glGenBuffers(1, &EBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), &indices[0], GL_STATIC_DRAW);

	//glGenBuffers(1, &VBO_inst);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO_inst);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(GLuint) * 9, &instances, GL_STATIC_DRAW);
	//glVertexAttribPointer(1, 3, GL_UNSIGNED_INT, GL_FALSE, 0, 0);
	//glVertexAttribDivisor(1, 1);
	//glEnableVertexAttribArray(1);

	glm::mat4 model_matrix = glm::mat4(1.0f);
	glm::mat4 scale = glm::mat4(1.0f);
	glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 rotat = glm::mat4(1.0f);


	glm::vec3 camera_pos = glm::vec3(0, 0, 10);
	glm::vec3 camera_tar = glm::vec3(0, 0, 0);
	glm::vec3 camera_up = glm::vec3(0, 1, 0);
	glm::mat4 view_matrix = glm::lookAt(
		camera_pos, camera_tar, camera_up);

	glm::mat4 perspective = glm::perspective(
		45.0f, 8.0f / 6.0f, 0.1f, 100.0f);

	GLuint model_addr = glGetUniformLocation(
		shaderProgram, "m_matrix");
	GLuint camera_addr = glGetUniformLocation(
		shaderProgram, "view_matrix");
	GLuint pers_addr = glGetUniformLocation(
		shaderProgram, "p_matrix");
	GLuint camera_pos_addr = glGetUniformLocation(
		shaderProgram, "view_pos");

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	while (!glfwWindowShouldClose(window))
	{
		scale = glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1));
		rotat = glm::rotate(rotat, 0.01f, glm::vec3(1, 0, 0));
		//rotat = glm::rotate(rotat, 0.01f, glm::vec3(1, 0, 0));
		trans = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,0));

		model_matrix = trans * scale * rotat;

		glUniformMatrix4fv(
			model_addr, 1, GL_FALSE, glm::value_ptr(model_matrix));
		glUniformMatrix4fv(
			camera_addr,1, GL_FALSE, glm::value_ptr(view_matrix));
		glUniformMatrix4fv(
			pers_addr, 1, GL_FALSE, glm::value_ptr(perspective));
		glUniform3fv(camera_pos_addr, 1, glm::value_ptr(camera_pos));


		glfwPollEvents();
		glfwSwapBuffers(window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);
		//glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL,3);

	}

}

//VERTEX

#version 330 core
  
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normals;
//layout (location = 1) in vec3 off;


uniform mat4 view_matrix;
uniform mat4 p_matrix;
uniform mat4 m_matrix;
uniform vec3 view_pos;

out vec3 norm;
out vec3 col;
out vec3 fragPosition;
out vec3 viewPos;
void main()
{
	viewPos = view_pos;//vec3(view_matrix[3][0],view_matrix[3][1],view_matrix[3][2]);
	col = vec3(position.y, position.y,position.y);
	norm = vec3(transpose(inverse(m_matrix)) * vec4(normals,1.0));
	//norm = vec3(m_matrix * vec4(normals,1.0));
	fragPosition = vec3(m_matrix * vec4(position,1.0));
    gl_Position = p_matrix * view_matrix * m_matrix * vec4(position, 1.0);
}

//FRAGMENT
#version 330 core

out vec4 color;
in vec3 col;
in vec3 norm;
in vec3 fragPosition;
in vec3 viewPos;
void main()
{
    vec3 lightColour = vec3(1.0f, 1.0f, 1.0f);
	vec3 cubeColour = vec3(1.0f, 0.0f, 0.0f);
	float specularStrength = 0.5;

	//light position
	vec3 light_position = vec3(0.0f, 3.0f, 0.0f); //world coords

	//ambient lighting
	float ambientStrength = 0.15f;
    vec3 ambient_contribution = ambientStrength * lightColour;

	vec3 norm = normalize(norm);
	vec3 light_direction = normalize(light_position - fragPosition);
	float incident_degree = max(dot(norm, light_direction), 0.0f);
	vec3 diffuse_contribution = incident_degree * lightColour;

	vec3 viewDir = normalize(viewPos - fragPosition);
	vec3 reflectDir = reflect(-light_direction, norm);  

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
	vec3 specular = specularStrength * spec * lightColour; 

	vec3 resultantColour = (ambient_contribution + diffuse_contribution + specular) * cubeColour;
    color = vec4(resultantColour, 1.0f);


}