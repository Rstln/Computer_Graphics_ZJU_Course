#ifndef BASEOBJ_H
#define BASEOBJ_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Resource/resource_manager.h"
#include "global_param.h"

enum class ObjectionType {
	stone,
	ship,
	particle
};


class BaseOBJ
{
public:
	ObjectionType Type;/* type of this obj*/
	glm::vec3 Position;/* the coordinate of this obj*/
	glm::vec3 Center;
	glm::vec3 Correct;
	glm::vec3 Front;/* the vector of this obj, which will be normalized in this class*/
	glm::vec3 Size;
	glm::vec3 Right;
	glm::vec3 Up;
	float Speed;/* adjust obj speed individually, other wise it would be tedious*/
	bool isDead;
	GLuint boxVAO, boxVBO;
	Shader boxShader;

	BaseOBJ(ObjectionType type, glm::vec3 pos, glm::vec3 size, glm::vec3 dir, float speed);
	void update(glm::vec3 pos, glm::vec3 dir);
	virtual ~BaseOBJ() {}
	virtual ObjectionType getType() = 0;
	/* operation on postion */
	glm::vec3 getPosition();
	void setPosition(glm::vec3 pos);
	void setPosition(float px, float py, float pz);
	void addPosition(float dx, float dy, float dz);
	void addPosition(glm::vec3 increment);
	/* operation on direction */
	glm::vec3 getDirection() const;
	void setDirection(glm::vec3 dir);
	void setDirection(float px, float py, float pz);
	// add direction by rectangular
	void addDirection(float dx, float dy, float dz);
	void addDirection(glm::vec3 increment);
	// add direction by angel
	void rotateDirection(float angelX, float angelY, float angelZ);
	void setSpeed(float speed);
	// see AABB box
	void drawBox(glm::mat4 view, glm::mat4 projection);
	void setBox();
	void renderBox(glm::mat4 view, glm::mat4 projection);
};

#endif