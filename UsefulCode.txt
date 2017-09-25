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

glm::mat4 p_m, v_m, m_m;
glm::vec3 v_p, v_d;

float ROTATOR = 0.0f;
float V_STEP = 0.01f;

const GLuint WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;
void update_p_m(GLFWwindow* window) {
	// Update the Projection matrix after a window resize event
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	float ration = WINDOW_WIDTH / WINDOW_HEIGHT;

	glViewport(0, 0, width, height);
	p_m = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);
}

void fb_cb(GLFWwindow* window, int width, int height)
{
	update_p_m(window);
}
std::ostream& operator<<(std::ostream& stream, const glm::mat4& matrix) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			stream.width(10);
			stream << matrix[i][j];
		}
		stream << '\n';
	}
	return stream;
}
std::ostream& operator<<(std::ostream& stream, const glm::vec3& matrix) {
	for (int i = 0; i < 3; i++) {
		stream.width(10);
		stream << matrix[i];
		stream << '\n';
	}
	return stream;
}
std::ostream& operator<<(std::ostream& stream, const glm::vec4& matrix) {
	for (int i = 0; i < 4; i++) {
		stream.width(10);
		stream << matrix[i];
		stream << '\n';
	}
	return stream;
}

void cursor_cb(GLFWwindow* window, double xpos, double ypos) {
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	double xdiff = xpos - (width / 2);
	double ydiff = ypos - (height / 2);

	glm::vec3 mod_v = glm::vec3(xdiff*0.001, ydiff*-0.001, 0);
	glm::vec4 mod_v4 = glm::vec4(mod_v, 1);
	v_d = glm::normalize(v_d + glm::vec3(mod_v4 * v_m));

	glfwSetCursorPos(window, width / 2, height / 2);
}

void key_cb(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
		V_STEP *= 10.0f;
	if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
		V_STEP /= 10.0f;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_W)
		v_p += v_d*V_STEP;

	if (key == GLFW_KEY_S)
		v_p -= v_d*V_STEP;

	if (key == GLFW_KEY_A)
		v_p -= glm::cross(v_d, glm::vec3(0, 1, 0))*V_STEP;

	if (key == GLFW_KEY_D)
		v_p += glm::cross(v_d, glm::vec3(0, 1, 0))*V_STEP;

	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
		std::cout << v_p << std::endl;


	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
		ROTATOR += 1.0f;
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
		ROTATOR -= 1.0f;
	//v_p.y = 0;
}
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


	glfwSetKeyCallback(window, key_cb);
	glfwSetFramebufferSizeCallback(window, fb_cb);
	glfwSetCursorPosCallback(window, cursor_cb);


	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glCullFace(GL_FRONT);
	GLuint shaderProgram = loadShaders("vertex.shader", "fragment.shader");
	glUseProgram(shaderProgram);

	float colors[] = {
		0.0, 1.0, 0.0,
		0.0, 1.0, 0.0,
		1.0, 0.0, 0.0,
		1.0, 0.0, 0.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0
	};

	float vertices[] = {
		0,0,0,   1,0,0,
		0,0,0,   0,1,0,
		0,0,0,   0,0,1
	};

	float triangle[] = {
		-1, -1, 0,
		1, -1, 0,	
		0,1,0
		
	};

	GLuint VBO, VBO_color, VAO, VAO_triangle, VBO_triangle;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &VBO_color);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glGenVertexArrays(1, &VAO_triangle);
	glBindVertexArray(VAO_triangle);
	glGenBuffers(1, &VBO_triangle);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_triangle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,0, 0);
	glEnableVertexAttribArray(0);



	glm::mat4 axis_model_matrix = glm::mat4(1.0f);
	glm::mat4 triangle_m_m = glm::mat4(1.0f);


	v_p = glm::vec3(0, 0, 2);
	v_d = glm::normalize(glm::vec3(0, 0, -1));
	glm::vec3 camera_up = glm::vec3(0, 1, 0);
	glm::mat4 view_matrix = glm::lookAt(
		v_p, v_d, camera_up);

	glm::mat4 perspective = glm::perspective(
		45.0f, 8.0f / 6.0f, 0.1f, 100.0f);

	GLuint model_addr = glGetUniformLocation(
		shaderProgram, "m_matrix");
	GLuint camera_addr = glGetUniformLocation(
		shaderProgram, "view_matrix");
	GLuint pers_addr = glGetUniformLocation(
		shaderProgram, "p_matrix");
	GLuint color_addr = glGetUniformLocation(
		shaderProgram, "col_tr");
	std::cout << perspective << std::endl;
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	while (!glfwWindowShouldClose(window))
	{
		//GENERAL STUFF
		glUniform3fv(color_addr, 1, glm::value_ptr(glm::vec3(0, 0, 0)));

		view_matrix = glm::lookAt(v_p, v_p + v_d, glm::vec3(0, 1, 0));
		glUniformMatrix4fv(
			camera_addr, 1, GL_FALSE, glm::value_ptr(view_matrix));
		glUniformMatrix4fv(
			pers_addr, 1, GL_FALSE, glm::value_ptr(perspective));
		glfwPollEvents();
		glfwSwapBuffers(window);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


		//AXIS
		glm::mat4 r_m = glm::rotate(glm::mat4(1.0f), ROTATOR*0.1f, glm::vec3(0, 1, 0));
		ROTATOR = 0.0f;
		axis_model_matrix *= r_m;
		glUniformMatrix4fv(
			model_addr, 1, GL_FALSE, glm::value_ptr(axis_model_matrix));

		glBindVertexArray(VAO);
		glDrawArrays(GL_LINES, 0, 6);


		//TRIANGLE
		glBindVertexArray(VAO_triangle);
		triangle_m_m = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		for (int i = 0; i < 10; i++){
			glm::vec3 col = glm::vec3(i / 10.0f, 1.0f - (i / 10.0f), i / 10.0f);
			glUniform3fv(color_addr, 1, glm::value_ptr(col));
			glm::mat4 triangle_m_m_unique = glm::translate(
				triangle_m_m, glm::vec3(i/2.0, i / 2.0, 0));
			glUniformMatrix4fv(
				model_addr, 1, GL_FALSE, glm::value_ptr(triangle_m_m_unique));
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}

	}

}

#FRAGMENT
#version 330 core

out vec4 color;
in vec3 col;
uniform vec3 col_tr;
void main()
{
    color = vec4(col,1.0f);
	if(col_tr != vec3(0,0,0)){
		color = vec4(col_tr,1.0f);
	}
} 

#SHADER

#version 330 core
  
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 co;

uniform mat4 view_matrix;
uniform mat4 p_matrix;
uniform mat4 m_matrix;

out vec3 col;
void main()
{
    gl_Position = p_matrix* view_matrix * m_matrix * vec4(position, 1.0);
	col = co;
}