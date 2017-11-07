#include "SceneLoader.hpp"

SceneLoader::SceneLoader(std::string filename) {
	load(filename);
}

bool SceneLoader::load(std::string scene_file) {
	std::ifstream textfile(scene_file);
	if (!textfile) {
		std::cout << "Cannot find " << scene_file << std::endl;
		return false;
	}

	std::string line; //temp variable
	while (std::getline(textfile, line)) {
		if (line == "camera") {
			std::getline(textfile, line);
			setCameraPos(line);
			std::getline(textfile, line);
			setCameraFov(line);
			std::getline(textfile, line);
			setCameraF(line);
			std::getline(textfile, line);
			setCameraAspect(line);

			camera.reininitalize(camera_pos,camera_fov,camera_f,camera_a);
		}
		else if (line == "light") {
			std::getline(textfile, line);
			setLightPos(line);
			std::getline(textfile, line);
			setLightCol(line);
			
			Light newlight(light_pos,light_col);
			lights.emplace_back(std::move(newlight));
		}
		else if (line == "sphere") {
			std::getline(textfile, line);
			setSpherePos(line);
			std::getline(textfile, line);
			setSphereRad(line);
			std::getline(textfile, line);
			setSphereAmb(line);
			std::getline(textfile, line);
			setSphereDif(line);
			std::getline(textfile, line);
			setSphereSpe(line);
			std::getline(textfile, line);
			setSphereShi(line);

			Sphere newsphere(sphere_pos, sphere_rad,sphere_amb,sphere_dif,sphere_spe,sphere_shi);
			spheres.emplace_back(std::move(newsphere));
		}
		else if (line == "model") {
			std::getline(textfile, line);
			setModelName(line);
			std::getline(textfile, line);
			setModelAmb(line);
			std::getline(textfile, line);
			setModelDif(line);
			std::getline(textfile, line);
			setModelSpe(line);
			std::getline(textfile, line);
			setModelShi(line);

			Model newmodel(model_file, model_amb, model_dif, model_spe, model_shi);
			models.emplace_back(std::move(newmodel));
		}
		else if (line == "triangle") {//
			std::getline(textfile, line);
			setTriangleV1(line);
			std::getline(textfile, line);
			setTriangleV2(line);
			std::getline(textfile, line);
			setTriangleV3(line);
			std::getline(textfile, line);
			setTriangleAmb(line);
			std::getline(textfile, line);
			setTriangleDif(line);
			std::getline(textfile, line);
			setTriangleSpe(line);
			std::getline(textfile, line);
			setTriangleShi(line);

			Triangle newtriangle(triangle_v1, triangle_v2, triangle_v3, triangle_amb, triangle_dif, triangle_spe, triangle_shi);
			triangles.emplace_back(std::move(newtriangle));
		}
		else {
			std::cout << "The following line was ignored: " << line << std::endl;
		}
	}

	textfile.close();
	return true;
}//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//convert vertices to triangles
void SceneLoader::addOBJ(std::vector<glm::vec3> vertices, Model model) {
	for (int i = 0; i + 2 < vertices.size(); i += 3) {
		Triangle newtriangle(vertices.at(i), vertices.at(i + 1), vertices.at(i + 2), model.ambient(), model.diffuse(), model.specular(), model.shininess());
		triangles.emplace_back(newtriangle);
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//split string
std::vector<std::string> SceneLoader::split(std::string s, std::string delimiter) {
	//using method from https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
	size_t pos = 0;
	std::vector<std::string> result;
	while ((pos = s.find(delimiter)) != std::string::npos) {
		result.emplace_back(s.substr(0, pos));
		s.erase(0, pos + delimiter.length());
	}
	result.emplace_back(s);
	return result;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//camera functions
void  SceneLoader::setCameraPos(std::string input) {
	std::vector<std::string> str_values = split(input, " ");
	float x, y, z;
	x = std::stod(str_values[1]);
	y = std::stod(str_values[2]);
	z = std::stod(str_values[3]);
	camera_pos = glm::vec3(x, y, z);
	return;
}
void  SceneLoader::setCameraFov(std::string input) {
	std::vector<std::string> str_values = split(input, " ");
	camera_fov = std::stod(str_values[1]);
	return;

}
void SceneLoader::setCameraF(std::string input) {
	std::vector<std::string> str_values = split(input, " ");
	camera_f = std::stod(str_values[1]);
	return;
}
void SceneLoader::setCameraAspect(std::string input) {
	std::vector<std::string> str_values = split(input, " ");
	camera_a = std::stod(str_values[1]);
	return;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//Light functions
void SceneLoader::setLightPos(std::string input) {
	std::vector<std::string> str_values = split(input, " ");
	float x, y, z;
	x = std::stod(str_values[1]);
	y = std::stod(str_values[2]);
	z = std::stod(str_values[3]);
	light_pos = glm::vec3(x, y, z);
	return;
}
void SceneLoader::setLightCol(std::string input) {
	std::vector<std::string> str_values = split(input, " ");
	float x, y, z;
	x = std::stod(str_values[1]);
	y = std::stod(str_values[2]);
	z = std::stod(str_values[3]);
	light_col = glm::vec3(x, y, z);
	return;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//Sphere functions
void SceneLoader::setSpherePos(std::string input) {
	std::vector<std::string> str_values = split(input, " ");
	float x, y, z;
	x = std::stod(str_values[1]);
	y = std::stod(str_values[2]);
	z = std::stod(str_values[3]);
	sphere_pos = glm::vec3(x, y, z);
	return;
}
void SceneLoader::setSphereRad(std::string input) {
	std::vector<std::string> str_values = split(input, " ");
	sphere_rad = std::stod(str_values[1]);
	return;
}
void SceneLoader::setSphereAmb(std::string input) {
	std::vector<std::string> str_values = split(input, " ");
	float x, y, z;
	x = std::stod(str_values[1]);
	y = std::stod(str_values[2]);
	z = std::stod(str_values[3]);
	sphere_amb = glm::vec3(x, y, z);
	return;
}
void SceneLoader::setSphereDif(std::string input) {
	std::vector<std::string> str_values = split(input, " ");
	float x, y, z;
	x = std::stod(str_values[1]);
	y = std::stod(str_values[2]);
	z = std::stod(str_values[3]);
	sphere_dif = glm::vec3(x, y, z);
	return;
}
void SceneLoader::setSphereSpe(std::string input) {
	std::vector<std::string> str_values = split(input, " ");
	float x, y, z;
	x = std::stod(str_values[1]);
	y = std::stod(str_values[2]);
	z = std::stod(str_values[3]);
	sphere_spe = glm::vec3(x, y, z);
	return;
}
void SceneLoader::setSphereShi(std::string input) {
	std::vector<std::string> str_values = split(input, " ");
	sphere_shi = std::stod(str_values[1]);
	return;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//model functions
void SceneLoader::setModelName(std::string input) {
	model_file = input;
	return;
}
void SceneLoader::setModelAmb(std::string input) {
	std::vector<std::string> str_values = split(input, " ");
	float x, y, z;
	x = std::stod(str_values[1]);
	y = std::stod(str_values[2]);
	z = std::stod(str_values[3]);
	model_amb = glm::vec3(x, y, z);
	return;
}
void SceneLoader::setModelDif(std::string input) {
	std::vector<std::string> str_values = split(input, " ");
	float x, y, z;
	x = std::stod(str_values[1]);
	y = std::stod(str_values[2]);
	z = std::stod(str_values[3]);
	model_dif = glm::vec3(x, y, z);
	return;
}
void SceneLoader::setModelSpe(std::string input) {
	std::vector<std::string> str_values = split(input, " ");
	float x, y, z;
	x = std::stod(str_values[1]);
	y = std::stod(str_values[2]);
	z = std::stod(str_values[3]);
	model_spe = glm::vec3(x, y, z);
	return;
}
void SceneLoader::setModelShi(std::string input) {
	std::vector<std::string> str_values = split(input, " ");
	model_shi = std::stod(str_values[1]);
	return;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//triangle functions
void SceneLoader::setTriangleV1(std::string input) {
	std::vector<std::string> str_values = split(input, " ");
	float x, y, z;
	x = std::stod(str_values[1]);
	y = std::stod(str_values[2]);
	z = std::stod(str_values[3]);
	triangle_v1 = glm::vec3(x, y, z);
	return;
}
void SceneLoader::setTriangleV2(std::string input) {
	std::vector<std::string> str_values = split(input, " ");
	float x, y, z;
	x = std::stod(str_values[1]);
	y = std::stod(str_values[2]);
	z = std::stod(str_values[3]);
	triangle_v2 = glm::vec3(x, y, z);
	return;
}
void SceneLoader::setTriangleV3(std::string input) {
	std::vector<std::string> str_values = split(input, " ");
	float x, y, z;
	x = std::stod(str_values[1]);
	y = std::stod(str_values[2]);
	z = std::stod(str_values[3]);
	triangle_v3 = glm::vec3(x, y, z);
	return;
}
void SceneLoader::setTriangleAmb(std::string input) {
	std::vector<std::string> str_values = split(input, " ");
	float x, y, z;
	x = std::stod(str_values[1]);
	y = std::stod(str_values[2]);
	z = std::stod(str_values[3]);
	triangle_amb = glm::vec3(x, y, z);
	return;
}
void SceneLoader::setTriangleDif(std::string input) {
	std::vector<std::string> str_values = split(input, " ");
	float x, y, z;
	x = std::stod(str_values[1]);
	y = std::stod(str_values[2]);
	z = std::stod(str_values[3]);
	triangle_dif = glm::vec3(x, y, z);
	return;
}
void SceneLoader::setTriangleSpe(std::string input) {
	std::vector<std::string> str_values = split(input, " ");
	float x, y, z;
	x = std::stod(str_values[1]);
	y = std::stod(str_values[2]);
	z = std::stod(str_values[3]);
	triangle_spe = glm::vec3(x, y, z);
	return;
}
void SceneLoader::setTriangleShi(std::string input) {
	std::vector<std::string> str_values = split(input, " ");
	triangle_shi = std::stod(str_values[1]);
	return;
}