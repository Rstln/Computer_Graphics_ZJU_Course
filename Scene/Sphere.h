#ifndef SHPERE_H
#define SHPERE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "../global_param.h"
#include "../Resource/resource_manager.h"



const GLuint LON = 30;
const GLuint LAT = 60;

class Sphere {
public:
	GLuint VAO, VBO;
	Shader sphereShader;
	GLuint Map[2];
	bool isNight = false;

	Sphere(Shader shader);
	~Sphere() {};
	glm::vec3 getPoint(GLfloat u, GLfloat v);
	void setSphere(GLfloat* sphere, GLuint Longitude, GLuint Latitude);
	void Render();
};




#endif