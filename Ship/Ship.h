#ifndef SHIP_H
#define SHIP_H

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include "../Resource/resource_manager.h"
#include "../Resource/model.h"
#include "../Resource/shader.h"
#include "../global_param.h"
#include "../BaseOBJ.h"


#define SHIP_DEFAULT_POSITION glm::vec3(0.0f, 1.0f, 0.0f)
#define SHIP_SPEED_MAX 50.0f
#define SHIP_DEFAULT_ACC 1.0f
#define SHIP_DEFAULT_RESIST 0.1f
#define SHIP_DEFAULT_FRONT glm::vec3(0.0f, 0.0f, -1.0f)
#define SHIP_DEFAULT_RIGHT glm::vec3(0.0f, 0.0f, 1.0f)
#define SHIP_DEFAULT_UP glm::vec3(0.0f, 1.0f, 0.0f)
#define SHIP_SPEED_THRESHOLD 200.0f
#define SHIP_ROT_SPEED 1.0f
#define SHIP_SIZE glm::vec3(1.0f, 3.0f, 3.8f)
#define REBOUNCE 0.005f


class Ship: public BaseOBJ{
public:
	GLfloat Acc, Resist;
	GLfloat rotSpeed;
	GLfloat rotAngle;
	GLfloat waggle;
	glm::vec3 rotAxis;

	Model* modelptr;
	Shader shipShader;


	Ship(Shader shader);
	~Ship() {};
	ObjectionType getType();
	void GetModel();
	//glm::vec3 SetCamera();
	void Render();
	void Update(GLfloat deltaTime);
	void UpdatePosition(GLfloat deltaTime);
	void UpdateSpeed(GLfloat deltaTime);
	void UpdateDirection(GLfloat delta_angle);
};

#endif 