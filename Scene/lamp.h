#ifndef LAMP_H
#define LAMP_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "../global_param.h"
#include "../Resource/resource_manager.h"


class Lamp {
public:
	GLuint VAO, VBO;
	Shader lampShader;

	Lamp(Shader shader);
	~Lamp();
	void Render(glm::vec3 lightPos);
	void ExportOBJ();
};


#endif // !LAMP_H
