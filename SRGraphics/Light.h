#pragma once
#include <glm/glm.hpp>

class Light {
public:
	float		intensity			= 1.0;
	glm::vec3	position			= glm::vec3();
	glm::vec3	color				= glm::vec3();
};

class DirectionLight : public Light {

};

class PointLight : public Light {

};

class SpotLight : public Light {

};
