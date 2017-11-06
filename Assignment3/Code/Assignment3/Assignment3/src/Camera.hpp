#pragma once
#include <glm\vec3.hpp>

class Camera
{
	private:
		glm::vec3 pos;	//x y z coordinate position
		float fov;		//theta for field of view
		float f;		//focal_length
		float a;		//aspect ratio

	public:
		Camera();
		Camera(glm::vec3 xyz, float theta, float focal_length, float aspect_ratio);
		
		void reininitalize(glm::vec3 xyz, float theta, float focal_length, float aspect_ratio);

		//getters
		glm::vec3 position();
		float field_of_view();
		float focal_length();
		float aspect_ratio();
};

///default constructor
inline Camera::Camera(){
	pos = glm::vec3(0.0f);
	fov = 60.0f;
	f = 1000.0f;
	a = 1.33f;
}

///custom constructor
inline Camera::Camera(glm::vec3 xyz, float theta, float focal_length, float aspect_ratio) {
	pos = xyz;
	fov = theta;
	f = focal_length;
	a = aspect_ratio;
}
inline void Camera::reininitalize(glm::vec3 xyz, float theta, float focal_length, float aspect_ratio) {
	pos = xyz;
	fov = theta;
	f = focal_length;
	a = aspect_ratio;
}

//------------------------------------------GETTERS---------------------------------------------

inline glm::vec3 Camera::position() {
	glm::vec3 temp = pos;
	return temp;
}

inline float Camera::field_of_view() {
	float temp = fov;
	return temp;
}

inline float Camera::focal_length() {
	float temp = f;
	return temp;
}

inline float Camera::aspect_ratio() {
	float temp = a;
	return temp;
}