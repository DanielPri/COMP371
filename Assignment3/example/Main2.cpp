#include "stdafx.h"

#include "..\cimg\CImg.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "objloader.hpp"  //include the object loader
#include "Camera.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Light.h"

#include <string>
#include <sstream>
#include <vector>
#include <iterator>

#define PI 3.14159265

using namespace cimg_library;
using namespace std;

//The color that should be displayed if a ray hits no object
glm::vec3 blankColor(0, 0, 0);

char *scene_file_name = "scene2.txt";
char *image_file_name = "image1.bmp";

Camera *camera;
std::vector<Sphere*> spheres;
std::vector<Triangle*> triangles;
std::vector<Light*> lights;

//Split a string into its individual values
template<typename Out>
void split(const string &s, char delim, Out result) {
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim)) {
		*(result++) = item;
	}
}

//Split a string into its individual values
std::vector<string> split(const std::string &s, char delim) {
	std::vector<string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}

//Read in the values from the scene file
void Read_File()
{
	ifstream scene_file;
	scene_file.open(scene_file_name, ios::in);

	string line;
	getline(scene_file, line);

	int num_objects = stoi(line);
	while (num_objects > 0)
	{
		getline(scene_file, line);
		if (line == "camera")
		{
			getline(scene_file, line);
			string pos_string = line.substr(5, line.size());
			std::vector<std::string> pos_vector = split(pos_string, ' ');
			glm::vec3 pos(stoi(pos_vector[0]), stoi(pos_vector[1]), stoi(pos_vector[2]));

			getline(scene_file, line);
			string fov_string = line.substr(5, line.size());
			int fov = stoi(fov_string);

			getline(scene_file, line);
			string focal_length_string = line.substr(3, line.size());
			int focal_length = stoi(focal_length_string);

			getline(scene_file, line);
			string aspect_ratio_string = line.substr(3, line.size());
			double aspect_ratio = stod(aspect_ratio_string);

			camera = new Camera(pos, fov, focal_length, aspect_ratio);
		}
		else if (line == "sphere")
		{
			getline(scene_file, line);
			string pos_string = line.substr(5, line.size());
			std::vector<std::string> pos_vector = split(pos_string, ' ');
			glm::vec3 pos(stoi(pos_vector[0]), stoi(pos_vector[1]), stoi(pos_vector[2]));

			getline(scene_file, line);
			string rad_string = line.substr(5, line.size());
			int rad = stoi(rad_string);

			getline(scene_file, line);
			string amb_string = line.substr(5, line.size());
			std::vector<std::string> amb_vector = split(amb_string, ' ');
			glm::vec3 amb(stod(amb_vector[0]), stod(amb_vector[1]), stod(amb_vector[2]));

			getline(scene_file, line);
			string dif_string = line.substr(5, line.size());
			std::vector<std::string> dif_vector = split(dif_string, ' ');
			glm::vec3 dif(stod(dif_vector[0]), stod(dif_vector[1]), stod(dif_vector[2]));

			getline(scene_file, line);
			string spe_string = line.substr(5, line.size());
			std::vector<std::string> spe_vector = split(spe_string, ' ');
			glm::vec3 spe(stod(spe_vector[0]), stod(spe_vector[1]), stod(spe_vector[2]));

			getline(scene_file, line);
			string shi_string = line.substr(5, line.size());
			double shi = stod(shi_string);

			spheres.push_back(new Sphere(pos, rad, amb, dif, spe, shi));
		}
		else if (line == "model")
		{
			getline(scene_file, line);
			string object_file_name = line;
			std::vector<glm::vec3> vertices;
			std::vector<glm::vec3> normals;
			std::vector<glm::vec2> uvs;
			loadOBJ(object_file_name.c_str(), vertices, normals, uvs);

			getline(scene_file, line);
			string amb_string = line.substr(5, line.size());
			std::vector<std::string> amb_vector = split(amb_string, ' ');
			glm::vec3 amb(stod(amb_vector[0]), stod(amb_vector[1]), stod(amb_vector[2]));

			getline(scene_file, line);
			string dif_string = line.substr(5, line.size());
			std::vector<std::string> dif_vector = split(dif_string, ' ');
			glm::vec3 dif(stod(dif_vector[0]), stod(dif_vector[1]), stod(dif_vector[2]));

			getline(scene_file, line);
			string spe_string = line.substr(5, line.size());
			std::vector<std::string> spe_vector = split(spe_string, ' ');
			glm::vec3 spe(stod(spe_vector[0]), stod(spe_vector[1]), stod(spe_vector[2]));

			getline(scene_file, line);
			string shi_string = line.substr(5, line.size());
			double shi = stod(shi_string);

			for (int i = 0; i + 2 < vertices.size(); i += 3)
			{
				triangles.push_back(new Triangle(vertices.at(i), vertices.at(i + 1), vertices.at(i + 2), amb, dif, spe, shi));
			}
		}
		else if (line == "triangle")
		{
			getline(scene_file, line);
			string v1_string = line.substr(4, line.size());
			std::vector<std::string> v1_vector = split(v1_string, ' ');
			glm::vec3 v1(stod(v1_vector[0]), stod(v1_vector[1]), stod(v1_vector[2]));

			getline(scene_file, line);
			string v2_string = line.substr(4, line.size());
			std::vector<std::string> v2_vector = split(v2_string, ' ');
			glm::vec3 v2(stod(v2_vector[0]), stod(v2_vector[1]), stod(v2_vector[2]));

			getline(scene_file, line);
			string v3_string = line.substr(4, line.size());
			std::vector<std::string> v3_vector = split(v3_string, ' ');
			glm::vec3 v3(stod(v3_vector[0]), stod(v3_vector[1]), stod(v3_vector[2]));

			getline(scene_file, line);
			string amb_string = line.substr(5, line.size());
			std::vector<std::string> amb_vector = split(amb_string, ' ');
			glm::vec3 amb(stod(amb_vector[0]), stod(amb_vector[1]), stod(amb_vector[2]));

			getline(scene_file, line);
			string dif_string = line.substr(5, line.size());
			std::vector<std::string> dif_vector = split(dif_string, ' ');
			glm::vec3 dif(stod(dif_vector[0]), stod(dif_vector[1]), stod(dif_vector[2]));

			getline(scene_file, line);
			string spe_string = line.substr(5, line.size());
			std::vector<std::string> spe_vector = split(spe_string, ' ');
			glm::vec3 spe(stod(spe_vector[0]), stod(spe_vector[1]), stod(spe_vector[2]));

			getline(scene_file, line);
			string shi_string = line.substr(5, line.size());
			double shi = stod(shi_string);
			
			triangles.push_back(new Triangle(v1, v2, v3, amb, dif, spe, shi));
		}
		else if (line == "light")
		{
			getline(scene_file, line);
			string pos_string = line.substr(5, line.size());
			std::vector<std::string> pos_vector = split(pos_string, ' ');
			glm::vec3 pos(stoi(pos_vector[0]), stoi(pos_vector[1]), stoi(pos_vector[2]));

			getline(scene_file, line);
			string col_string = line.substr(5, line.size());
			std::vector<std::string> col_vector = split(col_string, ' ');
			glm::vec3 col(stod(col_vector[0]), stod(col_vector[1]), stod(col_vector[2]));

			lights.push_back(new Light(pos, col));
		}

		num_objects--;
	}


	scene_file.close();
}

//If a vector has values above 1, set them to 1. This is used to even out the color values
glm::vec3 snap_to_one(glm::vec3 vector)
{
	if (vector.x > 1)
		vector.x = 1;
	if (vector.y > 1)
		vector.y = 1;
	if (vector.z > 1)
		vector.z = 1;
	return vector;
}

//Given a distance t along a ray and the formula for the ray, it gives the point along the ray that matches these values
glm::vec3 get_intersection_point(double t, glm::vec3 rayOrigin, glm::vec3 rayDirection)
{
	double x = rayOrigin.x + t * rayDirection.x;
	double y = rayOrigin.y + t * rayDirection.y;
	double z = rayOrigin.z + t * rayDirection.z;
	return glm::vec3(x, y, z);
}

//Verifies that the two given points are the same within the first 2 decimal points (because we're working with doubles, sometimes two points should be the same, but are not beyond the first couple decimal points, so we only check the first 2)
bool same_points(glm::vec3 point1, glm::vec3 point2)
{
	//Check if they are different within the first 2 decimal points
	if ((int)(point1.x * 10) != (int)(point2.x * 10) || (int)(point1.y * 10) != (int)(point2.y * 10) || (int)(point1.z * 10) != (int)(point2.z * 10))
		return true;
	return false;
}

//Returns true if the given point is blocked from the light described by the given light ray
bool is_in_shadow(glm::vec3 intersectionPoint, glm::vec3 lightDirection, glm::vec3 lightOrigin)
{
	double lowestT = -1;

	//Find the first sphere the ray intersects with
	int spheresIndex = -1;
	double lowestTSpheres = -1;
	for (int i = 0; i < spheres.size(); i++)
	{
		double t = spheres.at(i)->intersects_at(lightOrigin, lightDirection);
		if (t > 0 && (lowestTSpheres == -1 || t < lowestTSpheres))
		{
			lowestTSpheres = t;
			spheresIndex = i;
		}
	}

	//Find the first triangle the ray intersects with
	int trianglesIndex = -1;
	double lowestTTriangles = -1;
	for (int i = 0; i < triangles.size(); i++)
	{
		double t = triangles.at(i)->intersects_at(lightOrigin, lightDirection);
		if (t > 0 && (lowestTTriangles == -1 || t < lowestTTriangles))
		{
			lowestTTriangles = t;
			trianglesIndex = i;
		}
	}

	//If it intersects with both a triangle and a sphere, see which intersection happens first
	if (trianglesIndex != -1 && spheresIndex != -1)
	{
		if (lowestTSpheres < lowestTTriangles)
		{
			lowestT = lowestTSpheres;
		}
		else
		{
			lowestT = lowestTTriangles;
		}
	}
	//If it only intersects with a sphere
	else if (spheresIndex != -1)
	{
		lowestT = lowestTSpheres;
	}
	else if (trianglesIndex != -1)
	{
		lowestT = lowestTTriangles;
	}

	//The lowestT describes the point along the ray that the light intersects with first
	//So now we compare this point with the given point that the camera is looking at. If they are the same point, that means the light hits that point. Otherwise, it means that point is in shadow
	if (lowestT != -1)
	{
		glm::vec3 light_intersection_point = get_intersection_point(lowestT, lightOrigin, lightDirection);
		return same_points(intersectionPoint, light_intersection_point);
	}
	//It should not arrive here because the light should at least hit the provided intersection point
	return false;
}

//Method to calculate the color for a point based on the phong model
glm::vec3 calculateColor(glm::vec3 objectSpec, glm::vec3 objectDif, glm::vec3 objectAmb, double objectShine, glm::vec3 normal, glm::vec3 intersectionPoint, glm::vec3 viewerDirection)
{
	glm::vec3 color = objectAmb;
	for (int i = 0; i < lights.size(); i++)
	{
		glm::vec3 lightDirection = lights.at(i)->get_pos() - intersectionPoint;
		lightDirection = glm::normalize(lightDirection);
		
		//We add nothing for this light if it does not actually hit the point on the object
		if (is_in_shadow(intersectionPoint, -lightDirection, lights.at(i)->get_pos()))
			continue;

		float ln = glm::dot(lightDirection, normal);
		if(ln < 0)
			ln = 0;

		glm::vec3 reflectionDirection = (2 * ln * normal) - lightDirection;

		float rv = glm::dot(reflectionDirection, viewerDirection);
		if(rv < 0)
			rv = 0;
		rv = pow(rv, objectShine);

		glm::vec3 lightAddition = lights.at(i)->get_col()*(objectDif*ln + objectSpec*rv);
		color += lightAddition;
	}
	color = snap_to_one(color);
	return color;
}

//Calculate the color for a point on a sphere
glm::vec3 calculateColorSphere(int spheresIndex, double lowestTSpheres, glm::vec3 rayOrigin, glm::vec3 rayDirection)
{
	glm::vec3 intersection_point = get_intersection_point(lowestTSpheres, rayOrigin, rayDirection);
	glm::vec3 normal = spheres.at(spheresIndex)->get_normal_at(intersection_point);
	normal = glm::normalize(normal);
	return calculateColor(spheres.at(spheresIndex)->get_spe(), spheres.at(spheresIndex)->get_dif(), spheres.at(spheresIndex)->get_amb(), spheres.at(spheresIndex)->get_shi(), normal, intersection_point, -rayDirection);
}

//Calculate the color for a point on a triangle
glm::vec3 calculateColorTriangle(int trianglesIndex, double lowestTTriangles, glm::vec3 rayOrigin, glm::vec3 rayDirection)
{
	glm::vec3 intersection_point = get_intersection_point(lowestTTriangles, rayOrigin, rayDirection);
	glm::vec3 normal = triangles.at(trianglesIndex)->get_normal_at(intersection_point);
	normal = glm::normalize(normal);
	return calculateColor(triangles.at(trianglesIndex)->get_spe(), triangles.at(trianglesIndex)->get_dif(), triangles.at(trianglesIndex)->get_amb(), triangles.at(trianglesIndex)->get_shi(), normal, intersection_point, -rayDirection);
}

//Cast a ray into the world and determine which object it hits first and then determine the color for the pixel based on that
glm::vec3 castRay(glm::vec3 rayOrigin, glm::vec3 rayDirection)
{
	//Find the first sphere the ray intersects with
	int spheresIndex = -1;
	double lowestTSpheres = -1;
	for (int i = 0; i < spheres.size(); i++)
	{
		double t = spheres.at(i)->intersects_at(rayOrigin, rayDirection);
		if (t > 0 && (lowestTSpheres == -1 || t < lowestTSpheres))
		{
			lowestTSpheres = t;
			spheresIndex = i;
		}
	}

	//Find the first triangle the ray intersects with
	int trianglesIndex = -1;
	double lowestTTriangles = -1;
	for (int i = 0; i < triangles.size(); i++)
	{
		double t = triangles.at(i)->intersects_at(rayOrigin, rayDirection);
		if (t > 0 && (lowestTTriangles == -1 || t < lowestTTriangles))
		{
			lowestTTriangles = t;
			trianglesIndex = i;
		}
	}

	//If it intersects with both a triangle and a sphere, see which intersection happens first and get the color for that
	if (trianglesIndex != -1 && spheresIndex != -1)
	{
		if (lowestTSpheres < lowestTTriangles)
		{
			return calculateColorSphere(spheresIndex, lowestTSpheres, rayOrigin, rayDirection);
		}
		else
		{
			return calculateColorTriangle(trianglesIndex, lowestTTriangles, rayOrigin, rayDirection);
		}
	}
	//If it only intersects with a sphere
	else if (spheresIndex != -1)
	{
		return calculateColorSphere(spheresIndex, lowestTSpheres, rayOrigin, rayDirection);
	}
	//If it only intersects with a triangle
	else if (trianglesIndex != -1)
	{
		return calculateColorTriangle(trianglesIndex, lowestTTriangles, rayOrigin, rayDirection);
	}

	return blankColor;
}

// The MAIN function, from here we start the application and run the ray tracing loop, and finally output a picture with cimg
int main()
{
	Read_File();
	double fov_rads = camera->get_fov() * PI / 180;
	int imageHeight = 2 * (camera->get_focal_length() * tan(fov_rads / 2));
	int imageWidth = camera->get_aspect_ratio() * imageHeight;

	CImg<float> image(imageWidth, imageHeight, 1, 3, 0);
	for (int x = 0; x <= imageWidth; x++)
	{
		double normalizedX = (x + 0.5) / imageWidth;
		double screenX = (2 * normalizedX - 1) * camera->get_aspect_ratio() * tan(fov_rads/2);
		for (int y = 0; y <= imageHeight; y++)
		{
			double normalizedY = (y + 0.5) / imageHeight;
			double screenY = (1 - 2 * normalizedY) * tan(fov_rads / 2);
			glm::vec3 pixel(screenX, screenY, -1);
			glm::vec3 rayDirection = pixel - camera->get_pos();
			glm::vec3 normalizedDirection = glm::normalize(rayDirection);
			glm::vec3 color = castRay(camera->get_pos(), normalizedDirection);
			const float color_array[] = { color.x, color.y, color.z };
			image.draw_point(x, y, color_array);
		}
	}
	image.normalize(0, 255);
	image.save(image_file_name);

	cimg_library::CImgDisplay main_disp(image, "Render");
	while (!main_disp.is_closed())
		main_disp.wait();
	return 0;
}
