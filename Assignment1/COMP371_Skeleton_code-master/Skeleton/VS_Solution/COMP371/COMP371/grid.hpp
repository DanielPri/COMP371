#ifndef GRID_H_
#define GRID_H_

#include <vector>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

const int GRID_DIMENSIONS = 21;
const int VERTEX_QTY = GRID_DIMENSIONS * 4;
glm::vec3 gr[VERTEX_QTY];
//glm::vec3 grid_vertices[VERTEX_QTY];
std::vector<glm::vec3> grid_vertices = {
	//vertical lines
	glm::vec3 (-1.0f, 1.0f, 0.0f),
	glm::vec3 (-1.0f, -1.0f, 0.0f),
	glm::vec3 (-0.9f, 1.0f, 0.0f),
	glm::vec3 (-0.9f, -1.0f, 0.0f),
	glm::vec3 (-0.8f, 1.0f, 0.0f),
	glm::vec3 (-0.8f, -1.0f, 0.0f),
	glm::vec3 (-0.7f, 1.0f, 0.0f),
	glm::vec3 (-0.7f, -1.0f, 0.0f),
	glm::vec3 (-0.6f, 1.0f, 0.0f),
	glm::vec3 (-0.6f, -1.0f, 0.0f),
	glm::vec3 (-0.5f, 1.0f, 0.0f),
	glm::vec3 (-0.5f, -1.0f, 0.0f),
	glm::vec3 (-0.4f, 1.0f, 0.0f),
	glm::vec3 (-0.4f, -1.0f, 0.0f),
	glm::vec3 (-0.3f, 1.0f, 0.0f),
	glm::vec3 (-0.3f, -1.0f, 0.0f),
	glm::vec3 (-0.2f, 1.0f, 0.0f),
	glm::vec3 (-0.2f, -1.0f, 0.0f),
	glm::vec3 (-0.1f, 1.0f, 0.0f),
	glm::vec3 (-0.1f, -1.0f, 0.0f),
	glm::vec3 (0.0f, 1.0f, 0.0f),
	glm::vec3 (0.0f, -1.0f, 0.0f),
	glm::vec3 (0.1f, 1.0f, 0.0f),
	glm::vec3 (0.1f, -1.0f, 0.0f),
	glm::vec3 (0.2f, 1.0f, 0.0f),
	glm::vec3 (0.2f, -1.0f, 0.0f),
	glm::vec3 (0.3f, 1.0f, 0.0f),
	glm::vec3 (0.3f, -1.0f, 0.0f),
	glm::vec3 (0.4f, 1.0f, 0.0f),
	glm::vec3 (0.4f, -1.0f, 0.0f),
	glm::vec3 (0.5f, 1.0f, 0.0f),
	glm::vec3 (0.5f, -1.0f, 0.0f),
	glm::vec3 (0.6f, 1.0f, 0.0f),
	glm::vec3 (0.6f, -1.0f, 0.0f),
	glm::vec3 (0.7f, 1.0f, 0.0f),
	glm::vec3 (0.7f, -1.0f, 0.0f),
	glm::vec3 (0.8f, 1.0f, 0.0f),
	glm::vec3 (0.8f, -1.0f, 0.0f),
	glm::vec3 (0.9f, 1.0f, 0.0f),
	glm::vec3 (0.9f, -1.0f, 0.0f),
	glm::vec3 (1.0f, 1.0f, 0.0f),
	glm::vec3 (1.0f, -1.0f, 0.0f),

	//horizontal lines
	glm::vec3 (-1.0f, 1.0f, 0.0f),
	glm::vec3 (1.0f, 1.0f, 0.0f),
	glm::vec3 (-1.0f, 0.9f, 0.0f),
	glm::vec3 (1.0f, 0.9f, 0.0f),
	glm::vec3 (-1.0f, 0.8f, 0.0f),
	glm::vec3 (1.0f, 0.8f, 0.0f),
	glm::vec3 (-1.0f, 0.7f, 0.0f),
	glm::vec3 (1.0f, 0.7f, 0.0f),
	glm::vec3 (-1.0f, 0.6f, 0.0f),
	glm::vec3 (1.0f, 0.6f, 0.0f),
	glm::vec3 (-1.0f, 0.5f, 0.0f),
	glm::vec3 (1.0f, 0.5f, 0.0f),
	glm::vec3 (-1.0f, 0.4f, 0.0f),
	glm::vec3 (1.0f, 0.4f, 0.0f),
	glm::vec3 (-1.0f, 0.3f, 0.0f),
	glm::vec3 (1.0f, 0.3f, 0.0f),
	glm::vec3 (-1.0f, 0.2f, 0.0f),
	glm::vec3 (1.0f, 0.2f, 0.0f),
	glm::vec3 (-1.0f, 0.1f, 0.0f),
	glm::vec3 (1.0f, 0.1f, 0.0f),
	glm::vec3 (-1.0f, 0.0f, 0.0f),
	glm::vec3 (1.0f, 0.0f, 0.0f),
	glm::vec3 (-1.0f, -0.1f, 0.0f),
	glm::vec3 (1.0f, -0.1f, 0.0f),
	glm::vec3 (-1.0f, -0.2f, 0.0f),
	glm::vec3 (1.0f, -0.2f, 0.0f),
	glm::vec3 (-1.0f, -0.3f, 0.0f),
	glm::vec3 (1.0f, -0.3f, 0.0f),
	glm::vec3 (-1.0f, -0.4f, 0.0f),
	glm::vec3 (1.0f, -0.4f, 0.0f),
	glm::vec3 (-1.0f, -0.5f, 0.0f),
	glm::vec3 (1.0f, -0.5f, 0.0f),
	glm::vec3 (-1.0f, -0.6f, 0.0f),
	glm::vec3 (1.0f, -0.6f, 0.0f),
	glm::vec3 (-1.0f, -0.7f, 0.0f),
	glm::vec3 (1.0f, -0.7f, 0.0f),
	glm::vec3 (-1.0f, -0.8f, 0.0f),
	glm::vec3 (1.0f, -0.8f, 0.0f),
	glm::vec3 (-1.0f, -0.9f, 0.0f),
	glm::vec3 (1.0f, -0.9f, 0.0f),
	glm::vec3 (-1.0f, -1.0f, 0.0f),
	glm::vec3 (1.0f, -1.0f, 0.0f),
};

std::vector<glm::vec3> axis_vertices = {
	glm::vec3 (0.0f, 0.0f, 0.0f),
	glm::vec3 (0.5f, 0.0f, 0.0f),
	glm::vec3 (0.0f, 0.0f, 0.0f),
	glm::vec3 (0.0f, 0.5f, 0.0f),
	glm::vec3 (0.0f, 0.0f, 0.0f),
	glm::vec3 (0.0f, 0.0f, 0.5f),
};

std::vector<glm::vec3> axis_colors = {
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(0.0f, 0.0f, 1.0f),
};
#endif // GRID_H_
