#pragma once
#include <glm\vec3.hpp>

class Light
{
	private:
		glm::vec3 pos;	//x y z coordinate position
		glm::vec3 col;	//color of light


	public:
		Light();
		Light(glm::vec3 xyz, glm::vec3 cxcycz);

		//getters
		glm::vec3 position();
		glm::vec3 color();
};

///default constructor
inline Light::Light(){
	pos = glm::vec3(0.0f);
	col = glm::vec3(0.0f);
}

///custom constructor
inline Light::Light(glm::vec3 xyz, glm::vec3 cxcycz) {
	pos = xyz;
	col = cxcycz;
}

//------------------------------------------GETTERS---------------------------------------------

///x,y,z position of light source
inline glm::vec3 Light::position() {
	glm::vec3 temp = pos;
	return temp;
}

///color of light
inline glm::vec3 Light::color() {
	glm::vec3 temp = col;
	return temp;
}