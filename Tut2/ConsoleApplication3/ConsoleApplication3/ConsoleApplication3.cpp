// ConsoleApplication3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "..\glew\glew.h"
#include "..\glfw\glfw3.h"
#include "..\glm\glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

const GLuint WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;

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
	printf("Compiling shader : %s", vertex_shader_path.c_str());
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

int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Lab 1", nullptr, nullptr);
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

	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
	GLuint shaderProgram = loadShaders("vertex.shader", "fragment.shader");
	glUseProgram(shaderProgram);

	float colors[] = {
		0.0f, 1.0f, 0.0f,	0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 1.0f,	0.0f, -0.5f, 1.0f
	};

	float vertices[] = {
		0, 0, 0,	1,0,0,
		0, 0, 0,	0,1,0,
		0, 0, 0,	0,0,1
	};

	GLuint VBO, VBO_color, VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
		
	//first VBO (vertices)
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//second VBO (colors)
	glGenBuffers(1, &VBO_color);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(VAO);

	//4x4 Identity matrix 
	glm::mat4 model_matrix = glm::mat4(1.0f);

	//camera positioning matrices
	glm::vec3 camera_position = glm::vec3(2, 2, 10);
	glm::vec3 camera_target = glm::vec3(0, 0, 0);
	glm::vec3 camera_up = glm::vec3(0, 1, 0);
	glm::mat4 view_matrix = glm::lookAt(
		camera_position, camera_target, camera_up);
	glm::mat4 perspective = glm::perspective(45.0f, 8.0f/6.0f, 0.1f, 100.0f);

	//matrix addresses
	GLuint model_addr = glGetUniformLocation(
		shaderProgram, "m_matrix");
	GLuint camera_addr = glGetUniformLocation(
		shaderProgram, "view_matrix");
	GLuint pers_addr = glGetUniformLocation(
		shaderProgram, "p_matrix");


	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	while (!glfwWindowShouldClose(window))
	{
		
		glClear(GL_COLOR_BUFFER_BIT);

		//axis 1
		model_matrix = glm::mat4(1.0f);
		model_matrix = glm::translate(model_matrix, glm::vec3(2, 0, 0));
		glUniformMatrix4fv(model_addr, 1, GL_FALSE, glm::value_ptr(model_matrix));
		glDrawArrays(GL_LINES, 0, 6);

		//axis 2
		for (int i = 0; i < 10; i++) {
			model_matrix = glm::mat4(1.0f);
			model_matrix = glm::translate(model_matrix, glm::vec3(i-5, i-5, 0));
			model_matrix = glm::rotate(model_matrix, glm::radians((float)glfwGetTime() * 40), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(model_addr, 1, GL_FALSE, glm::value_ptr(model_matrix));
			glDrawArrays(GL_LINES, 0, 6);
		}
		//Camera
		glUniformMatrix4fv(camera_addr, 1, GL_FALSE, glm::value_ptr(view_matrix));
		glUniformMatrix4fv(pers_addr, 1, GL_FALSE, glm::value_ptr(perspective));
		
		glfwPollEvents();
		glfwSwapBuffers(window);

	}

}

