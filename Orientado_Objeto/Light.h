#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity);
	void useLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation);
	~Light();


private:
	glm::vec3 color;
	GLfloat ambientIntensity;
};

