//example skeleton code
//modified from http://learnopengl.com/

#include "stdafx.h"

#include "..\glew\glew.h"	// include GL Extension Wrangler
#include "..\glfw\glfw3.h"	// include GLFW helper library
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <string>
#include <fstream>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "grid.hpp"
#include "objloader.hpp"  //include the object loader

using namespace std;

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

glm::vec3 camera_position;
glm::vec3 triangle_scale;
glm::mat4 projection_matrix;

const float PI = 3.141592653;
const int SPHERE_QTY = 15;
bool generateSpheres = true;

// Constant vectors
const glm::vec3 center(0.0f, 0.0f, -1.0f);
const glm::vec3 up(0.0f, 1.0f, 0.0f);
glm::vec3 eye(0.0f, 0.0f, 1.9f);

//global variables for important functions
//for scaling
float object_size = 1;
//for pacman's position
float position_x = 0;
float position_y = 0;
//pacman's orientation
float orientation = 0;
//sphere generation
float sphere_x[SPHERE_QTY];
float sphere_y[SPHERE_QTY];
//wether or not to draw a sphere if it has been eaten
bool draw_sphere[SPHERE_QTY];
//for camera 
glm::vec3 v_d = center;
glm::vec3 v_p = eye;
float rotateX = 0;
float rotateY = 0;
//for window resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//using learnopengl.com/#!Getting-started/Camera method for calculating mouse x and y offsets
void cursor_cb(GLFWwindow* window, double xpos, double ypos) {

	int pan = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
	int zoom = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	int tilt = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);

	if (zoom == GLFW_PRESS) {
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		double xdiff = xpos - (width / 2);
		double ydiff = ypos - (height / 2);
		v_p += v_d*(float)ydiff*-0.005f*0.04f;
		glfwSetCursorPos(window, width / 2, height / 2);
	}

	if (pan == GLFW_PRESS) {
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		double xdiff = xpos - (width / 2);
		double ydiff = ypos - (height / 2);

		glm::vec3 mod_v = glm::vec3(xdiff*0.005, 0, 0);
		glm::vec4 mod_v4 = glm::vec4(mod_v, 1);
		v_d = glm::normalize(v_d + glm::vec3(mod_v4 * 0.05f));
		glfwSetCursorPos(window, width / 2, height / 2);
	}

	if (tilt == GLFW_PRESS) {
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		double xdiff = xpos - (width / 2);
		double ydiff = ypos - (height / 2);

		glm::vec3 mod_v = glm::vec3(0, ydiff*-0.005, 0);
		glm::vec4 mod_v4 = glm::vec4(mod_v, 1);
		v_d = glm::normalize(v_d + glm::vec3(mod_v4 * 0.05f));
		glfwSetCursorPos(window, width / 2, height / 2);
	}
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	std::cout << key << std::endl;
	
	//rotate camera
	if (key == GLFW_KEY_RIGHT && action != GLFW_RELEASE) {
		
		rotateX -= 0.5;
	}
	if (key == GLFW_KEY_LEFT && action != GLFW_RELEASE) {

		rotateX += 0.5;
	}
	if (key == GLFW_KEY_UP && action != GLFW_RELEASE) {

		rotateY -= 0.5;
	}
	if (key == GLFW_KEY_DOWN && action != GLFW_RELEASE) {

		rotateY += 0.5;
	}
	//Show line view
	if (key == GLFW_KEY_L && action != GLFW_RELEASE) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	//show vertex view
	if (key == GLFW_KEY_P && action != GLFW_RELEASE) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	}
	//show triangle view
	if (key == GLFW_KEY_T && action != GLFW_RELEASE) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	//Scale up
	if (key == GLFW_KEY_U && action != GLFW_RELEASE) {
		object_size += 0.1;
	}
	//scale down
	if (key == GLFW_KEY_J && action != GLFW_RELEASE) {
		object_size -= 0.1;
	}
	//resets everything (play again)
	if (key == GLFW_KEY_R && action != GLFW_RELEASE) {
		object_size = 1;
		generateSpheres = true;
		position_x = 0;
		position_y = 0;
		for (int i = 0; i < SPHERE_QTY; i++) {
			draw_sphere[i] = true;
		}
	}
	//move up (also face up, also cannot go past walls)
	if (key == GLFW_KEY_W && action != GLFW_RELEASE) {
		position_y += 0.1;
		if (position_y >= 1.0) position_y = 1.0;
		orientation = 90;
		std::cout << position_x << ", " << position_y << std::endl;
	}
	//move down (also face down, also cannot go past walls)
	if (key == GLFW_KEY_S && action != GLFW_RELEASE) {
		position_y -= 0.1;
		if (position_y <= -1.0) position_y = -1.0;
		orientation = 270;
		std::cout << position_x << ", " << position_y << std::endl;
	}
	//go left (also face left, also cannot go past walls)
	if (key == GLFW_KEY_A && action != GLFW_RELEASE) {
		position_x -= 0.1;
		if (position_x <= -1.0) position_x = -1.0;
		orientation = 180;
		std::cout << position_x << ", " << position_y << std::endl;
	}
	//go right (also face right, also cannot go past walls)
	if (key == GLFW_KEY_D && action != GLFW_RELEASE) {
		position_x += 0.1;
		if (position_x >= 1.0) position_x = 1.0;
		orientation = 0;
		std::cout << position_x << ", " << position_y << std::endl;
	}
	//reset camera
	if (key == GLFW_KEY_HOME && action == GLFW_PRESS) {
		v_d = center;
		v_p = eye;

	}
}



// The MAIN function, from here we start the application and run the game loop
int main()
{
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Hey look it's the Pacman", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// Set the required callback functions
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, cursor_cb);
	glfwSetKeyCallback(window, key_callback);

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
	
	//Capture mouse with glfw
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glViewport(0, 0, width, height);

	projection_matrix = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.0f, 100.0f);

	// Build and compile our shader program
	// Vertex shader

	// Read the Vertex Shader code from the file
	string vertex_shader_path = "vertex.shader";
	string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_shader_path, ios::in);

	if (VertexShaderStream.is_open()) {
		string Line = "";
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
	string fragment_shader_path = "fragment.shader";
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_shader_path, std::ios::in);

	if (FragmentShaderStream.is_open()) {
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory?\n", fragment_shader_path.c_str());
		getchar();
		exit(-1);
	}

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(vertexShader, 1, &VertexSourcePointer, NULL);
	glCompileShader(vertexShader);
	// Check for compile time errors
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(fragmentShader, 1, &FragmentSourcePointer, NULL);
	glCompileShader(fragmentShader);
	// Check for compile time errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Link shaders
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader); //free up memory
	glDeleteShader(fragmentShader);

	glUseProgram(shaderProgram);



	std::vector<glm::vec3> cube_vertices;
	std::vector<glm::vec3> cube_normals;
	std::vector<glm::vec2> cube_UVs;
	loadOBJ("cube.obj", cube_vertices, cube_normals, cube_UVs); //read the vertices from the cube.obj file

	std::vector<glm::vec3> pacman_vertices;
	std::vector<glm::vec3> pacman_normals;
	std::vector<glm::vec2> pacman_UVs;
	loadOBJ("pacman.obj", pacman_vertices, pacman_normals, pacman_UVs); //read the vertices from the pacman.obj file
	
	std::vector<glm::vec3> sphere_vertices;
	std::vector<glm::vec3> sphere_normals;
	std::vector<glm::vec2> sphere_UVs;
	loadOBJ("sphere.obj", sphere_vertices, sphere_normals, sphere_UVs); //read the vertices from the sphere.obj file
	srand(time(NULL));
	for (int i = 0; i < SPHERE_QTY; i++) {
		draw_sphere[i] = true;
	}
	
	//cube VAO
	GLuint cube_VAO, cube_VBO;
	glGenVertexArrays(1, &cube_VAO);
	glGenBuffers(1, &cube_VBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	GLuint cube_vertices_VBO, cube_normals_VBO;

	glGenVertexArrays(1, &cube_VAO);
	//glGenBuffers(1, &cube_vertices_VBO);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(cube_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, cube_VBO);
	glBufferData(GL_ARRAY_BUFFER, cube_vertices.size() * sizeof(glm::vec3), &cube_vertices.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &cube_normals_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, cube_normals_VBO);
	glBufferData(GL_ARRAY_BUFFER, cube_normals.size() * sizeof(glm::vec3), &cube_normals.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO

	//pacman VAO
	GLuint pacman_VAO, pacman_VBO;
	glGenVertexArrays(1, &pacman_VAO);
	glGenBuffers(1, &pacman_VBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).

	glGenVertexArrays(1, &pacman_VAO);
	glGenBuffers(1, &pacman_VBO);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(pacman_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, pacman_VBO);
	glBufferData(GL_ARRAY_BUFFER, pacman_vertices.size() * sizeof(glm::vec3), &pacman_vertices.front(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO

	//grid VAO
	GLuint grid_VAO, grid_VBO;
	glGenVertexArrays(1, &grid_VAO);
	glGenBuffers(1, &grid_VBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).

	glGenVertexArrays(1, &grid_VAO);
	glGenBuffers(1, &grid_VBO);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(grid_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, grid_VBO);
	glBufferData(GL_ARRAY_BUFFER, grid_vertices.size() * sizeof(glm::vec3), &grid_vertices.front(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	//sphere VAO
	GLuint sphere_VAO, sphere_VBO;
	glGenVertexArrays(1, &sphere_VAO);
	glGenBuffers(1, &sphere_VBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).

	glGenVertexArrays(1, &sphere_VAO);
	glGenBuffers(1, &sphere_VBO);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(sphere_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, sphere_VBO);
	glBufferData(GL_ARRAY_BUFFER, sphere_vertices.size() * sizeof(glm::vec3), &sphere_vertices.front(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO

	//pretty much useless but came with skeleton code
	triangle_scale = glm::vec3(1.0f);

	//uniform declarations
	GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection_matrix");
	GLuint viewMatrixLoc = glGetUniformLocation(shaderProgram, "view_matrix");
	GLuint transformLoc = glGetUniformLocation(shaderProgram, "model_matrix");
	//used to fill color
	GLuint fillLoc = glGetUniformLocation(shaderProgram, "fill");
	
	//using deltaTime from learnopengl.com
	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame

	//initialize view
	glm::mat4 view_matrix;
	view_matrix = glm::lookAt(eye, center, up);
	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//change the camera
		view_matrix = glm::lookAt(v_p, v_p + v_d , up);
		//rotate camera
		view_matrix = glm::rotate(view_matrix, glm::radians(rotateX), glm::vec3(0.0f,0.0f,-1.0f));
		view_matrix = glm::rotate(view_matrix, glm::radians(rotateY), glm::vec3(-1.0f, 0.0f, 0.0f));
		
		//set up first empty model_matrix
		glm::mat4 model_matrix;
		model_matrix = glm::scale(model_matrix, triangle_scale);

		//draw empty world
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));
		glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(view_matrix));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

		//draw grid
		glUniform1i(fillLoc, 1);
		glBindVertexArray(grid_VAO);
		model_matrix = glm::mat4(1.0f);
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));
		glDrawArrays(GL_LINES, 0, grid_vertices.size());
		glBindVertexArray(0);

		//draw spheres
		//generate spheres coordinates
		if (generateSpheres) {
			for (int index = 0; index < SPHERE_QTY; index++) {
				sphere_x[index] = (float)(rand() % 21 - 10) / 10;
				sphere_y[index] = (float)(rand() % 21 - 10) / 10;
			}
			generateSpheres = false;
		}
		for (int index = 0; index < SPHERE_QTY; index++) {
			if ((position_x >= sphere_x[index] - 0.03 && position_x <= sphere_x[index] + 0.03) && (position_y >= sphere_y[index] - 0.03 && position_y <= sphere_y[index] + 0.03))
			{	
				draw_sphere[index] = false;
			}
			else if (draw_sphere[index]) {
				glUniform1i(fillLoc, 2);
				glBindVertexArray(sphere_VAO);
				model_matrix = glm::mat4(1.0f);
				model_matrix = glm::mat4(1.0f);
				model_matrix = glm::translate(model_matrix, glm::vec3(sphere_x[index], sphere_y[index], 0.0f));
				model_matrix = glm::scale(model_matrix, glm::vec3(object_size, object_size, object_size));
				model_matrix = glm::scale(model_matrix, glm::vec3(0.05f, 0.05f, 0.05f));
				glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));
				glDrawArrays(GL_TRIANGLES, 0, sphere_vertices.size());
				glBindVertexArray(0);
			}
		}

		//draw pacman
		glUniform1i(fillLoc, 0);
		glBindVertexArray(pacman_VAO);
		model_matrix = glm::mat4(1.0f);
		model_matrix = glm::translate(model_matrix, glm::vec3(position_x, position_y, 0.0f));
		model_matrix = glm::rotate(model_matrix, glm::radians(orientation), glm::vec3(0.0f, 0.0f, 1.0f));
		model_matrix = glm::rotate(model_matrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//uncomment v for teacup, comment out ^
		//model_matrix = glm::scale(model_matrix, glm::vec3(15.0f, 15.0f, 15.0f));
		model_matrix = glm::scale(model_matrix, glm::vec3(object_size, object_size, object_size));
		model_matrix = glm::scale(model_matrix, glm::vec3(0.005f, 0.005f, 0.005f));
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));
		glDrawArrays(GL_TRIANGLES, 0, pacman_vertices.size());
		glBindVertexArray(0);


		// Swap the screen buffers
		glfwSwapBuffers(window);

		//using deltaTime from learnopengl.com
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	//update projection matrix to new width and height
	projection_matrix = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.0f, 100.0f); 
}
