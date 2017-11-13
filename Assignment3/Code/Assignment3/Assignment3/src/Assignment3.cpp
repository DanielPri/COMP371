#include <iostream>
#include <math.h>

#include "objloader.hpp"
#include "CImg.h"
#include "SceneLoader.hpp"
#include "glm\trigonometric.hpp"	//tangent and radians
#include "glm\geometric.hpp"		//normalize

using namespace cimg_library;
#define PI 3.141592653589

void output(SceneLoader sceneloader);
void progressReport(long count, long size);
bool sphere_intersect(glm::vec3 spherepos, glm::vec3 camerapos, glm::vec3 ray, float rad, glm::vec3& ouIntersectpoint, float &distance);
bool RayIntersectsTriangle(glm::vec3 rayOrigin, glm::vec3 rayVector, Triangle inTriangle, glm::vec3& outIntersectionPoint, float& distance);

int main() {
	std::string location = "..\\..\\..\\scene_files\\";
	SceneLoader sceneloader(location + "scene.txt");
	output(sceneloader);
	
	//load all objects
	std::vector<glm::vec3> all_vertices;
	std::vector<glm::vec3> all_normals;
	std::vector<glm::vec2> all_uvs;
	for (auto model : sceneloader.models) {
		std::string filename = location + model.filename();
		const char* objectname= filename.c_str();
		if (loadOBJ(objectname, all_vertices, all_normals, all_uvs)) {
			sceneloader.addOBJ(all_vertices, model);
		}
		else
			std::cout << "failed to load object " << model.filename() << std::endl;
	}
	//TODO
	std::cout << "\nthere are now " << sceneloader.triangles.size() << " triangles" << std::endl;

	std::cout <<std::endl;

	//finding width and height
	//height = 2 * (1000 * tan(30));
	int height = 2 * (sceneloader.camera.focal_length()*glm::tan(glm::radians(sceneloader.camera.field_of_view())/2));
	int width = sceneloader.camera.aspect_ratio() * height;
	std::cout << "height: "<< height << std::endl;
	std::cout << "width: " << width << std::endl;
	
	//setting up CImg
	CImg<float> image(width, height, 1, 3, 0);

	std::cout << "Now Loading! please wait! There are " << height*width << " pixels to compute" << std::endl;
	//using following link as methodology for raytracing
	//https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-generating-camera-rays/generating-camera-rays
	
	long counter = 0;
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			//Pixel Camera
			float PCx = (2 * ((i + 0.5) / width) - 1) * tan(sceneloader.camera.field_of_view() / 2 * PI / 180) * sceneloader.camera.aspect_ratio();
			float PCy = (1 - 2 * ((j + 0.5) / height)) * tan(sceneloader.camera.field_of_view() / 2 * PI / 180);

			//Pcamera Space
			glm::vec3 camera = sceneloader.camera.position();
			glm::vec3 ray_direction = glm::vec3(PCx, PCy, -1) - camera;
			ray_direction = glm::normalize(ray_direction);

			glm::vec3 pixelColor(0.0f);

			//sphere intersect - finds nearest sphere and its distance
			glm::vec3 intersectpoint;
			float nearest_object; //distance of nearest object
			int object_index;
			int intersects = 0;		//intersects variable indicates what is interesected	
									//0 = no intersection
									//1 = sphere

			for (int k = 0; k < sceneloader.spheres.size(); k++) {
				float temp_distance;
				glm::vec3 temp_intersectpoint;
				if (sphere_intersect(sceneloader.spheres[k].position(), camera, ray_direction, sceneloader.spheres[k].radius(), temp_intersectpoint, temp_distance)) {
					if (intersects == 0 || temp_distance < nearest_object) {
						nearest_object = temp_distance;
						intersectpoint = temp_intersectpoint;
						object_index = k;
						intersects = 1;
					}
				}
			}
			glm::vec3 triangle_intersect;
			for (int k = 0; k < sceneloader.triangles.size(); k++) {
				float temp_distance;
				glm::vec3 temp_intersectpoint;
				if (RayIntersectsTriangle(camera, ray_direction, sceneloader.triangles[k], temp_intersectpoint, temp_distance)) {
					if (intersects == 0 || temp_distance < nearest_object) {
						nearest_object = temp_distance;
						intersectpoint = temp_intersectpoint;
						object_index = k;
						intersects = 2;
					}
				}
			}
			//place all other intersects here
			
			//check if shadowed and color
			bool isShadowed = false;
			float bias = 1e-3;

			//for each light, check values
			for (int li = 0; li < sceneloader.lights.size(); li++) {
				if (intersects != 0) {
					glm::vec3 light_ray = glm::normalize(sceneloader.lights[li].position() - intersectpoint);

					//check if spheres in the way
					for (int k = 0; k < sceneloader.spheres.size(); k++) {
						float temp_distance;
						glm::vec3 temp_intersectpoint;
						if (sphere_intersect(sceneloader.spheres[k].position(), intersectpoint + (bias*light_ray), light_ray, sceneloader.spheres[k].radius(), temp_intersectpoint, temp_distance)) {
							isShadowed = true;
						}
					}

					//check for triangles in the way
					for (int k = 0; k < sceneloader.triangles.size(); k++) {
						float temp_distance;
						glm::vec3 temp_intersectpoint;
						if (RayIntersectsTriangle(intersectpoint + (bias*light_ray), light_ray, sceneloader.triangles[k], temp_intersectpoint, temp_distance)) {
							isShadowed = true;
						}
					}
				}



				//color the pixels
				if (intersects == 1) {
					if (!isShadowed) {
						glm::vec3 normal = glm::normalize(sceneloader.spheres[object_index].position() - intersectpoint);
						glm::vec3 v = -ray_direction;
					
							glm::vec3 light_direction = glm::normalize(intersectpoint - sceneloader.lights[li].position());
							glm::vec3 reflection = glm::reflect(light_direction, normal);
							float ln = glm::dot(normal, light_direction);
							float rv = glm::dot(reflection, v);
							//std::max(ln, 0.0f);
							//std::max(rv, 0.0f);
							if (ln < 0) { ln = 0; }
							if (rv < 0) { rv = 0; }
							rv = pow(rv, sceneloader.spheres[object_index].shininess());

							pixelColor += sceneloader.spheres[object_index].ambient();
							glm::vec3 lightAddition = sceneloader.lights[li].color()*(sceneloader.spheres[object_index].diffuse()*ln + sceneloader.spheres[object_index].specular()*rv);
							pixelColor += lightAddition;
						
					}
					//shadow
					else {
						pixelColor += sceneloader.spheres[object_index].ambient();
					}
				}
				else if (intersects == 2) {
					if (!isShadowed) {
						glm::vec3 line1 = sceneloader.triangles[object_index].coordinate2() - sceneloader.triangles[object_index].coordinate1();
						glm::vec3 line2 = sceneloader.triangles[object_index].coordinate3() - sceneloader.triangles[object_index].coordinate1();
						glm::vec3 normal = glm::normalize(glm::cross(line1, line2));
						normal = -normal;

						glm::vec3 v = -ray_direction;
							glm::vec3 light_direction = glm::normalize(intersectpoint - sceneloader.lights[li].position());
							glm::vec3 reflection = glm::reflect(light_direction, normal);
							float ln = glm::dot(normal, light_direction);
							float rv = glm::dot(reflection, v);
							if (ln < 0) { ln = 0; }
							if (rv < 0) { rv = 0; }
							rv = pow(rv, sceneloader.triangles[object_index].shininess());

							pixelColor += sceneloader.triangles[object_index].ambient();
							glm::vec3 lightAddition = sceneloader.lights[li].color()*(sceneloader.triangles[object_index].diffuse()*ln + sceneloader.triangles[object_index].specular()*rv);
							pixelColor += lightAddition;
						
					}
					else {
						pixelColor += sceneloader.triangles[object_index].ambient();
					}

				}
			}
			//shadow
			float color[3]{pixelColor.x, pixelColor.y, pixelColor.z};
			image.draw_point(i, j, color);
			counter++;
			progressReport(counter,height*width);
		}
	}
	
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

	std::cout << "\nTRIANGLE(S)" << std::endl;
	std::cout << "there are " << sceneloader.triangles.size() << " triangle(s)" << std::endl;
	for (auto triangle : sceneloader.triangles) {
		std::cout << "triangle coordinate 1: " << triangle.coordinate1().x << ", " << triangle.coordinate1().y << ", " << triangle.coordinate1().z << std::endl;
		std::cout << "triangle coordinate 2: " << triangle.coordinate2().x << ", " << triangle.coordinate2().y << ", " << triangle.coordinate2().z << std::endl;
		std::cout << "triangle coordinate 3: " << triangle.coordinate3().x << ", " << triangle.coordinate3().y << ", " << triangle.coordinate3().z << std::endl;
		std::cout << "triangle ambience: " << triangle.ambient().x << ", " << triangle.ambient().y << ", " << triangle.ambient().z << std::endl;
		std::cout << "triangle diffuse: " << triangle.diffuse().x << ", " << triangle.diffuse().y << ", " << triangle.diffuse().z << std::endl;
		std::cout << "triangle specular: " << triangle.specular().x << ", " << triangle.specular().y << ", " << triangle.specular().z << std::endl;
		std::cout << "triangle shininess: " << triangle.shininess() << std::endl;
	}

	std::cout << "\nLIGHT(S)" << std::endl;
	std::cout << "there are " << sceneloader.lights.size() << " light(s)" << std::endl;
	for (auto light : sceneloader.lights) {
		std::cout << "\nlight position: " << light.position().x << ", " << light.position().y << ", " << light.position().z << std::endl;
		std::cout << "light color: " << light.color().x << ", " << light.color().y << ", " << light.color().z << std::endl;
	}

}

//sphere intersect using https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
bool sphere_intersect(glm::vec3 spherepos, glm::vec3 camerapos, glm::vec3 ray, float rad, glm::vec3& ouIntersectpoint, float &distance) {
	float t0, t1;
	glm::vec3 L = spherepos - camerapos;
	float tca = glm::dot(L, ray);
	// if (tca < 0) return false;
	float d2 = glm::dot(L, L) - tca * tca;
	if (d2 > (rad * rad)) return false;
	float thc = sqrt((rad * rad) - d2);
	t0 = tca - thc;
	t1 = tca + thc;
	if (t0 > t1) std::swap(t0, t1);

	if (t0 < 0) {
		t0 = t1; // if t0 is negative, let's use t1 instead 
		if (t0 < 0) return false; // both t0 and t1 are negative 
	}
	distance = t0;
	ouIntersectpoint = camerapos + ray * distance;
	return true;
}

//from https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
bool RayIntersectsTriangle(glm::vec3 rayOrigin, glm::vec3 rayVector, Triangle inTriangle, glm::vec3& outIntersectionPoint, float &distance)
{
	const float E = 0.0000001;
	glm::vec3 vertex0 = inTriangle.coordinate1();
	glm::vec3 vertex1 = inTriangle.coordinate2();
	glm::vec3 vertex2 = inTriangle.coordinate3();
	glm::vec3 edge1, edge2, h, s, q;
	float a, f, u, v;
	edge1 = vertex1 - vertex0;
	edge2 = vertex2 - vertex0;
	h = glm::cross(rayVector, edge2);
	a = glm::dot(edge1, h);
	if (a > -E && a < E)
		return false;
	f = 1 / a;
	s = rayOrigin - vertex0;
	u = f * (glm::dot(s, h));
	if (u < 0.0 || u > 1.0)
		return false;
	q = glm::cross(s, edge1);
	v = f * glm::dot(rayVector, q);
	if (v < 0.0 || u + v > 1.0)
		return false;
	// At this stage we can compute t to find out where the intersection point is on the line.
	float t = f * glm::dot(edge2, q);
	if (t > E) // ray intersection
	{
		outIntersectionPoint = rayOrigin + rayVector * t;
		distance = t;
		return true;
	}
	else // This means that there is a line intersection but not a ray intersection.
		return false;
}

void progressReport(long count, long size) {
	long tenth = size / 10;
	long hundredth = tenth / 10;

	if (count == tenth) {
		std::cout << "\r" << std::flush << "10% loaded |---------";
	}
	else if (count == tenth * 2) {
		std::cout << "\r" << std::flush << "20% loaded ||--------";
	}
	else if (count == tenth * 3) {
		std::cout << "\r" << std::flush << "30% loaded |||-------";
	}
	else if (count == tenth * 4) {
		std::cout << "\r" << std::flush << "40% loaded ||||------";
	}
	else if (count == tenth * 5) {
		std::cout << "\r" << std::flush << "50% loaded |||||----";
	}
	else if (count == tenth * 6) {
		std::cout << "\r" << std::flush << "60% loaded ||||||----";
	}
	else if (count == tenth * 7) {
		std::cout << "\r" << std::flush << "70% loaded |||||||---";
	}
	else if (count == tenth * 8) {
		std::cout << "\r" << std::flush << "80% loaded ||||||||--";
	}
	else if (count == tenth * 9) {
		std::cout << "\r" << std::flush << "90% loaded |||||||||-";
	}
	else if (count == size) {
		std::cout << "\r" << std::flush << "100% loaded ||||||||||";
	}
}