#pragma once

#include <glm/glm.hpp>

class Light {
public:


	void draw(light_shader);

	glm::vec3 pos_;
	glm::vec3 color_;
	glm::vec3 intensity_;


};














