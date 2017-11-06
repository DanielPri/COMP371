#include <iostream>

#include "CImg.h"
#include "SceneLoader.hpp"
#include "glm\trigonometric.hpp"	//tangent
#include "glm\geometric.hpp"		//normalize
#include "glm\mat4x4.hpp"			//4x4 matrix

using namespace cimg_library;
#define PI 3.1415926535

void output(SceneLoader sceneloader);

int main() {

	SceneLoader sceneloader("E:\\Dropbox\\Dropbox\\Concordia\\SOEN\\05_Fall2017\\COMP371\\Assignments\\COMP371\\Assignment3\\scene_files\\scene1.txt");
	output(sceneloader);
	std::cout <<std::endl;

	//finding width and height
	//height = 2 * (1000 * tan(30));
	int height = 2 * (sceneloader.camera.focal_length()*glm::tan(glm::radians(sceneloader.camera.field_of_view())/2));
	int width = sceneloader.camera.aspect_ratio() * height;
	std::cout << "height: "<< height << std::endl;
	std::cout << "width: " << width << std::endl;
	
	//setting up CImg
	CImg<float> image(width, height, 1, 3, 0);

	std::vector<glm::vec3> directions;

	//using following link as methodology for raytracing
	//https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-generating-camera-rays/generating-camera-rays
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			//Pixel Camera
			float PCx = (2 * ((i + 0.5) / width) - 1) * tan(sceneloader.camera.field_of_view() / 2 * PI / 180) * sceneloader.camera.aspect_ratio();
			float PCy = (1 - 2 * ((j + 0.5) / height)) * tan(sceneloader.camera.field_of_view() / 2 * PI / 180);

			//Pcamera Space
			glm::vec3 camera = sceneloader.camera.position();
			glm::vec3 ray_direction = glm::vec3(PCx, PCy, -1) - camera;
			ray_direction = glm::normalize(ray_direction);
			
			//vector of all our directions. can be deleted.
			directions.emplace_back(ray_direction);

			glm::vec3 pixelColor(1.0f);
			float color[3]{pixelColor.x*ray_direction.x ,pixelColor.y *ray_direction.y, pixelColor.z *ray_direction.z};

			image.draw_point(i, j, color);
		}
	}
	directions;
	
	CImgDisplay main_disp(image, "Resulting Image");
	image.normalize(0, 255);
	image.save("raycastrender.bmp");




	std::cout << "\nThe end! :D\n(press enter to close) " << std::endl;
	std::cin.ignore();
}

void output(SceneLoader sceneloader) {

	std::cout << "\nCAMERA" << std::endl;
	std::cout << "camera position: " << sceneloader.camera.position().x << ", " << sceneloader.camera.position().y << ", " << sceneloader.camera.position().z << std::endl;
	std::cout << "camera fov: " << sceneloader.camera.field_of_view() << std::endl;
	std::cout << "camera focal length: " << sceneloader.camera.focal_length() << std::endl;
	std::cout << "camera aspect ratio: " << sceneloader.camera.aspect_ratio() << std::endl;

	std::cout << "\nSPHERE(S)" << std::endl;
	std::cout << "there are " << sceneloader.spheres.size() << " sphere(s)" << std::endl;
	for (auto sphere : sceneloader.spheres) {
		std::cout << "\nsphere position: " << sphere.position().x << ", " << sphere.position().y << ", " << sphere.position().z << std::endl;
		std::cout << "sphere radius: " << sphere.radius() << std::endl;
		std::cout << "sphere ambience: " << sphere.ambient().x << ", " << sphere.ambient().y << ", " << sphere.ambient().z << std::endl;
		std::cout << "sphere diffuse: " << sphere.diffuse().x << ", " << sphere.diffuse().y << ", " << sphere.diffuse().z << std::endl;
		std::cout << "sphere specular: " << sphere.specular().x << ", " << sphere.specular().y << ", " << sphere.specular().z << std::endl;
		std::cout << "sphere shininess: " << sphere.shininess() << std::endl;
	}

	std::cout << "\nMODEL(S)" << std::endl;
	std::cout << "there are " << sceneloader.models.size() << " model(s)" << std::endl;
	for (auto model : sceneloader.models) {
		std::cout << "\nmodel name: " << model.filename() << std::endl;
		std::cout << "model ambience: " << model.ambient().x << ", " << model.ambient().y << ", " << model.ambient().z << std::endl;
		std::cout << "model diffuse: " << model.diffuse().x << ", " << model.diffuse().y << ", " << model.diffuse().z << std::endl;
		std::cout << "model specular: " << model.specular().x << ", " << model.specular().y << ", " << model.specular().z << std::endl;
		std::cout << "model shininess: " << model.shininess() << std::endl;
	}

	std::cout << "\nLIGHT(S)" << std::endl;
	std::cout << "there are " << sceneloader.lights.size() << " light(s)" << std::endl;
	for (auto light : sceneloader.lights) {
		std::cout << "\nlight position: " << light.position().x << ", " << light.position().y << ", " << light.position().z << std::endl;
		std::cout << "light color: " << light.color().x << ", " << light.color().y << ", " << light.color().z << std::endl;
	}

}