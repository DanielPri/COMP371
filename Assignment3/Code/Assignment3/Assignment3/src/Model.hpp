#pragma once
#include <glm\vec3.hpp>
#include <string>

class Model
{
	private:
		std::string file;	//filename.obj contains mesh
		glm::vec3 amb;		//ambient color
		glm::vec3 dif;		//diffuse color
		glm::vec3 spe;		//specular color
		float shi;			//shininess

	public:
		Model();
		Model(std::string filename, glm::vec3 axayaz, glm::vec3 dxdydz, glm::vec3 sxsysz, float s);
		
		//getters
		std::string filename();
		glm::vec3 ambient();
		glm::vec3 diffuse();
		glm::vec3 specular();
		float shininess();
};

///default constructor
inline Model::Model(){
	file = "test.obj";
	amb = glm::vec3(0.0f);
	dif = glm::vec3(0.0f);
	spe = glm::vec3(0.0f);
	shi = 0.0f;
}

///custom constructor
inline Model::Model(std::string filename, glm::vec3 axayaz, glm::vec3 dxdydz, glm::vec3 sxsysz, float s) {
	file = filename;
	amb = axayaz;
	dif = dxdydz;
	spe = sxsysz;
	shi = s;
}

//------------------------------------------GETTERS---------------------------------------------

inline std::string Model::filename() {
	std::string temp = file;
	return temp;
}

inline glm::vec3 Model::ambient() {
	glm::vec3 temp = amb;
	return temp;
}

inline glm::vec3 Model::diffuse() {
	glm::vec3 temp = dif;
	return temp;
}

inline glm::vec3 Model::specular() {
	glm::vec3 temp = spe;
	return temp;
}

inline float Model::shininess() {
	float temp = shi;
	return temp;
}