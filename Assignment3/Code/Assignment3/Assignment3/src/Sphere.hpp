#pragma once

#include <glm\vec3.hpp>

class Sphere
{
	private:
		glm::vec3 pos;	//x y z coordinate position
		glm::vec3 amb;	//ambient color
		glm::vec3 dif;	//diffuse color
		glm::vec3 spe;	//specular color
		float rad;		//radius of sphere
		float shi;		//shininess factor

	public:
		Sphere();
		Sphere(glm::vec3 xyz, float r, glm::vec3 axayaz, glm::vec3 dxdydz, glm::vec3 sxsysz, float s);

		//getters
		glm::vec3 position();
		glm::vec3 ambient();
		glm::vec3 diffuse();
		glm::vec3 specular();
		float radius();
		float shininess();
};

///default constructor
inline Sphere::Sphere(){
	pos = glm::vec3(0.0f);
	rad = 1;		
	amb = glm::vec3(0.0f);
	dif = glm::vec3(0.0f);
	spe = glm::vec3(0.0f);
	shi = 0;
}

///custom constructor
inline Sphere::Sphere(glm::vec3 xyz, float r, glm::vec3 axayaz, glm::vec3 dxdydz, glm::vec3 sxsysz, float s) {
	pos = xyz;
	rad = r;
	amb = axayaz;
	dif = dxdydz;
	spe = sxsysz;
	shi = s;
}

//------------------------------------------GETTERS---------------------------------------------

///returns the position of the center of the sphere
inline glm::vec3 Sphere::position() {
	glm::vec3 temp = pos;
	return temp;
}

///ambient color
inline glm::vec3 Sphere::ambient() {
	glm::vec3 temp = amb;
	return temp;
}

///diffuse color
inline glm::vec3 Sphere::diffuse() {
	glm::vec3 temp = dif;
	return temp;
}

///specular color
inline glm::vec3 Sphere::specular() {
	glm::vec3 temp = spe;
	return temp;
}

///returns distance from the center to the edge of sphere
inline float Sphere::radius() {
	float temp = rad;
	return rad;
}

///how shiny the sphere is
inline float Sphere::shininess() {
	float temp = shi;
	return temp;
}