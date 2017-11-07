#pragma once
#include <glm\vec3.hpp>

class Triangle
{
	private:
		glm::vec3 v1;
		glm::vec3 v2;
		glm::vec3 v3;
		glm::vec3 amb;		//ambient color
		glm::vec3 dif;		//diffuse color
		glm::vec3 spe;		//specular color
		float shi;			//shininess

	public:
		Triangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 axayaz, glm::vec3 dxdydz, glm::vec3 sxsysz, float s);

		//getters
		glm::vec3 coordinate1();
		glm::vec3 coordinate2();
		glm::vec3 coordinate3();
		glm::vec3 ambient();
		glm::vec3 diffuse();
		glm::vec3 specular();
		float shininess();
};

///custom constructor
inline Triangle::Triangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 axayaz, glm::vec3 dxdydz, glm::vec3 sxsysz, float s) {
	v1 = p1;
	v2 = p2;
	v3 = p3;
	amb = axayaz;
	dif = dxdydz;
	spe = sxsysz;
	shi = s;
}

//------------------------------------------GETTERS---------------------------------------------


inline glm::vec3 Triangle::coordinate1() {
	glm::vec3 temp = v1;
	return temp;
}

inline glm::vec3 Triangle::coordinate2() {
	glm::vec3 temp = v2;
	return temp;
}

inline glm::vec3 Triangle::coordinate3() {
	glm::vec3 temp = v3;
	return temp;
}

inline glm::vec3 Triangle::ambient() {
	glm::vec3 temp = amb;
	return temp;
}

inline glm::vec3 Triangle::diffuse() {
	glm::vec3 temp = dif;
	return temp;
}

inline glm::vec3 Triangle::specular() {
	glm::vec3 temp = spe;
	return temp;
}

inline float Triangle::shininess() {
	float temp = shi;
	return temp;
}