#pragma once

#include <iostream>
#include <vector>
#include <fstream>

#include "Camera.hpp"
#include "Light.hpp"
#include "Sphere.hpp"
#include "Model.hpp"

class SceneLoader 
{
	private:	

		//camera variables
		glm::vec3 camera_pos;	//x y z coordinate position
		float camera_fov;		//theta for field of view
		float camera_f;			//focal_length
		float camera_a;			//aspect ratio

		//light variables
		glm::vec3 light_pos;	//x y z coordinate position
		glm::vec3 light_col;	//color of light

		//sphere variables
		glm::vec3 sphere_pos;	//x y z coordinate position
		glm::vec3 sphere_amb;	//ambient color
		glm::vec3 sphere_dif;	//diffuse color
		glm::vec3 sphere_spe;	//specular color
		float sphere_rad;		//radius of sphere
		float sphere_shi;		//shininess factor

		//model variables
		std::string model_file;	//filename.obj contains mesh
		glm::vec3 model_amb;		//ambient color
		glm::vec3 model_dif;		//diffuse color
		glm::vec3 model_spe;		//specular color
		float model_shi;			//shininess

		//FUNCTIONS------------------------------------------------
		//camera functions
		void setCameraPos(std::string input);
		void setCameraFov(std::string input);
		void setCameraF(std::string input);
		void setCameraAspect(std::string input);

		//Light functions
		void setLightPos(std::string input);
		void setLightCol(std::string input);

		//Sphere functions
		void setSpherePos(std::string input);
		void  setSphereRad(std::string input);
		void setSphereAmb(std::string input);
		void setSphereDif(std::string input);
		void setSphereSpe(std::string input);
		void setSphereShi(std::string input);

		//model functions
		void  setModelName(std::string input);
		void setModelAmb(std::string input);
		void setModelDif(std::string input);
		void setModelSpe(std::string input);
		void  setModelShi(std::string input);

	public:
		Camera camera;
		std::vector<Light> lights;
		std::vector<Sphere> spheres;
		std::vector<Model> models;
		
		//functions
		SceneLoader(std::string filename);
		bool load(std::string scene_file);
		std::vector<std::string> split(std::string s, std::string delimiter);
};