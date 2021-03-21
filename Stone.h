#ifndef STONE_H
#define STONE_H

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include "global_param.h"
#include "Resource/resource_manager.h"
#include "BaseOBJ.h"

class Stone : public BaseOBJ {
public:
	Shader stoneShader;
	Model* modelptr;

	Stone(Shader shader);
	~Stone();
	ObjectionType getType();
	void GetModel();
	
	void Render(GLfloat Scale);
};


#endif // !STONE_H
