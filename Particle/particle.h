#ifndef PARTICLE_H
#define PARTICLE_H

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "../Resource/resource_manager.h"
#include "../BaseOBJ.h"

#define PARTICLE_LIFE 3.8f
#define PARTICLE_SPEED 4.0f
#define PARTICLE_POS glm::vec3(0.0f)
#define PARTICLE_SIZE glm::vec3(0.1f, 0.1f, 0.1f)
#define PARTICLE_COLOR glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)
#define PARTICLE_FRONT glm::vec3(0.0f, 0.0f, -1.0f)


// Represents a single particle and its state
class Particle: public BaseOBJ{
public:

    glm::vec4 Color;
    GLfloat Scale;
    GLfloat Life;

    Particle(glm::vec3 pos, glm::vec3 front, GLfloat life, GLfloat speed, glm::vec4 color);
	Particle();
    ~Particle();
	ObjectionType getType();
	void setScale(float scale) {
		this->Scale = scale;
	}
	float getScale() {
		return this->Scale;
	}
	glm::vec4 getColor() {
		return this->Color;
	}
	void setColor(glm::vec4 Color) {
		this->Color = Color;
		if (this->Color.r < 0.0f) this->Color.r = 0.0f;
		//else if (this->Color.r > 1.0f) this->Color.r = 1.0f;
		if (this->Color.g < 0.0f) this->Color.g = 0.0f;
		//else if (this->Color.g > 1.0f) this->Color.g = 1.0f;
		if (this->Color.b < 0.0f) this->Color.b = 0.0f;
		//else if (this->Color.b > 1.0f) this->Color.b = 1.0f;
		if (this->Color.a < 0.0f) this->Color.a = 0.0f;
		//else if (this->Color.a > 1.0f) this->Color.a = 1.0f;
	}
	void setColorR(float value) {
		if (value >= 0.0f) this->Color.r = value;
	}
	void addColorR(float dv) {
		this->Color.r += dv;
		if (this->Color.r < 0.0f) this->Color.r = 0.0f;
		//else if (this->Color.r > 1.0f) this->Color.r = 1.0f;
	}
	void setColorG(float value) {
		if (value >= 0.0f) this->Color.g = value;
	}
	void addColorG(float dv) {
		this->Color.g += dv;
		if (this->Color.g < 0.0f) this->Color.g = 0.0f;
		//else if (this->Color.g > 1.0f) this->Color.g = 1.0f;
	}
	void setColorB(float value) {
		if (value >= 0.0f) this->Color.b = value;
	}
	void addColorB(float dv) {
		this->Color.b += dv;
		if (this->Color.b < 0.0f) this->Color.b = 0.0f;
		//else if (this->Color.b > 1.0f) this->Color.b = 1.0f;
	}
	void setColorA(float value) {
		if (value >= 0.0f) this->Color.a = value;
	}
	void addColorA(float dv) {
		this->Color.a += dv;
		if (this->Color.a < 0.0f) this->Color.a = 0.0f;
		//else if (this->Color.a > 1.0f) this->Color.a = 1.0f;
	}
};


#endif // !PARTICLE_H
