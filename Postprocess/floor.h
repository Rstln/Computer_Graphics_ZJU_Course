#ifndef FLOOR_H
#define FLOOR_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "../global_param.h"
#include "../Resource/resource_manager.h"

class Floor {
public:
	GLuint VAO, VBO;
	Shader floorShader;
	GLuint floorMap;


	Floor(Shader shader);
	~Floor();
	void Render(glm::mat4 view, glm::mat4 projection);
	void Render(glm::mat4 view, glm::mat4 projection, glm::mat4 lightSpaceMatrix, GLuint depthMap);
	void Render_depth(Shader depthShader);
};

#endif // !FLOOR_H