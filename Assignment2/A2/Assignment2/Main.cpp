//example skeleton code
//modified from http://learnopengl.com/

#include "stdafx.h"
#include "CImg.h"

#include "..\glew\glew.h"	// include GL Extension Wrangler
#include "..\glfw\glfw3.h"	// include GLFW helper library
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "objloader.hpp"  //include the object loader
#include "camera.h"

using namespace std;
using namespace cimg_library;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
vector<vector<glm::vec3>> calcStepVertices(int stepSize, vector<vector<glm::vec3>> all_vertices3d, int image_height, int image_width, vector<vector<int>> &index3d);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

int objRenderMode = GL_POINTS;
 
Camera camera(glm::vec3(35.0f, 50.0f, 20.0f));
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

//old camera
/*
glm::vec3 camera_position;
glm::vec3 triangle_scale;
glm::mat4 projection_matrix;


// Constant vectors
const glm::vec3 center(0.0f, 0.0f, 0.0f);
const glm::vec3 up(0.0f, 1.0f, 0.0f);
const glm::vec3 eye(100.0f, 150.0f, 300.0f);
*/

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	std::cout << key << std::endl;	
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

	// Create a GLFWwindow object that we can use for GLFW's  functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Heightmap", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

	//old camera
	//projection_matrix = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.0f, 100.0f);

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
	
	//old camera
	//triangle_scale = glm::vec3(0.2f);

	GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection_matrix");
	GLuint viewMatrixLoc = glGetUniformLocation(shaderProgram, "view_matrix");
	GLuint transformLoc = glGetUniformLocation(shaderProgram, "model_matrix");

	//---------------------CImg stuff-----------------------------------------------------------------
	//load image
	cout << "loading image" << endl;
	CImg<float> image("depth.bmp");
	//CImgDisplay main_disp(image, "2D image");
	
	vector<vector<glm::vec3>> all_vertices3d;
	int x = 0, z= 0;
	const int image_width = image.width();
	const int image_height = image.height();
	//cycle through every pixel
	cout << "placing in vector" << endl;
	for (int j = 0; j < image_height; j++ ) 
	{
		vector<glm::vec3> temp_row;
		for (int i = 0; i < image_width; i++) 
		{
			temp_row.emplace_back(glm::vec3(i, *image.data(i,j), j));
		}
		all_vertices3d.push_back(std::move(temp_row));
	}
	//flatten 3d matrix
	vector<glm::vec3> all_vertices;
	for (int i = 0; i < all_vertices3d.size(); i++)
	{
		for (int j = 0; j < all_vertices3d.front().size(); j++)
		{
			all_vertices.emplace_back(all_vertices3d.at(i).at(j));
		}
	}

	asdasdasd;
	//create a vector of vectors of vertices for skipped vertices
	//as well as a vector of indices
	int stepSize = 1;
	int counter = 0;
	vector<vector<glm::vec3>> skipped_vertices3d;
	vector<vector<int>> index3d;
	for (int j = 0; j < image_height; j++)
	{
		if (j % stepSize == 0) {
			vector<glm::vec3> temp_skip;
			vector<int> temp_index;
			for (int i = 0; i < image_width; i++)
			{
				if (i % stepSize == 0) 
				{
					temp_skip.emplace_back(all_vertices3d.at(j).at(i));
					temp_index.emplace_back(counter);
					counter++;
				}
			}
			skipped_vertices3d.push_back(std::move(temp_skip));
			index3d.emplace_back(std::move(temp_index));
		}
	}
	//flatten second 3d matrix for skipped matrix
	vector<glm::vec3> skipped_vertices;
	for (int i = 0; i < skipped_vertices3d.size(); i++)
	{
		for (int j = 0; j < skipped_vertices3d.front().size(); j++)
		{
			skipped_vertices.emplace_back(skipped_vertices3d.at(i).at(j));
		}
	}
	cout << "image processing complete" << endl;
	//-----------------------------------------------------------------------------------------------
	//EBO vertex creator
	vector<int> EBO_indices;
	for (int i = 0; i < index3d.size() - 1; i++) 
	{
		for (int j = 0; j < index3d.front().size() - 1; j++)
		{
			//triangle 1
			EBO_indices.emplace_back(index3d.at(i).at(j + 1));
			EBO_indices.emplace_back(index3d.at(i).at(j));
			EBO_indices.emplace_back(index3d.at(i + 1).at(j));

			//triangle 2
			EBO_indices.emplace_back(index3d.at(i).at(j + 1));
			EBO_indices.emplace_back(index3d.at(i + 1).at(j));
			EBO_indices.emplace_back(index3d.at(i + 1).at(j + 1));
		}
	}
	
	//-----------------------------------------------------------------------------------------------
	GLuint VAO_all_pixels, VBO_all_pixels, EBO;

	glGenVertexArrays(1, &VAO_all_pixels);
	glGenBuffers(1, &VBO_all_pixels);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO_all_pixels);
		
		glBindBuffer(GL_ARRAY_BUFFER, VBO_all_pixels);
		glBufferData(GL_ARRAY_BUFFER, skipped_vertices.size() * sizeof(glm::vec3), &skipped_vertices.front(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, EBO_indices.size() * sizeof(int), &EBO_indices.front(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		processInput(window);
		
		// Render
		// Clear the colorbuffer
		glClearColor(0.0f, 0.1f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		//glm::mat4 view_matrix;
		//view_matrix = glm::lookAt(eye, center, up);
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model_matrix;

		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));
		glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO_all_pixels);
		model_matrix = glm::mat4(1.0f);
		model_matrix = glm::scale(model_matrix, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model_matrix));
		glDrawElements(objRenderMode, EBO_indices.size(), GL_UNSIGNED_INT, 0);
		//glDrawElements(GL_LINES, EBO_indices.size(), GL_UNSIGNED_INT, 0);
		//glDrawElements(GL_TRIANGLES, EBO_indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		objRenderMode = GL_POINTS;
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		objRenderMode = GL_LINES;
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		objRenderMode = GL_TRIANGLES;

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

//create a vector of vectors of vertices for skipped vertices
//as well as a vector of indices

vector<vector<glm::vec3>> calcStepVertices(int stepSize, vector<vector<glm::vec3>> all_vertices3d ,int image_height, int image_width, vector<vector<int>> &index3d) 
{
	int counter = 0;
	vector<vector<glm::vec3>> skipped_vertices3d;
	for (int j = 0; j < image_height; j++)
	{
		if (j % stepSize == 0) {
			vector<glm::vec3> temp_skip;
			vector<int> temp_index;
			for (int i = 0; i < image_width; i++)
			{
				if (i % stepSize == 0)
				{
					temp_skip.emplace_back(all_vertices3d.at(j).at(i));
					temp_index.emplace_back(counter);
					counter++;
				}
			}
			skipped_vertices3d.push_back(std::move(temp_skip));
			index3d.emplace_back(std::move(temp_index));
		}
	}
	return skipped_vertices3d;
}