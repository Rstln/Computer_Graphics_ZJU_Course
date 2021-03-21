#include "Stone.h"

Stone::Stone(Shader shader)
	:BaseOBJ(ObjectionType::stone,
		glm::vec3(0.0f, 0.0f, 10.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.0f, 1.0f, 1.0f),
		0.0f) {
	Correct = glm::vec3(-0.5f, 0.0f, -0.3f);
	Center += Correct;
	stoneShader = shader;

	GetModel();
}
Stone::~Stone() {}

ObjectionType Stone::getType() {
	return Type;
}

void Stone::GetModel() {
	modelptr = ResourceManager::GetModel("stoneModel");
}

void Stone::Render(GLfloat Scale) {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, Position);
	model = glm::scale(model, glm::vec3(Scale));

	stoneShader.use();

	// MVP matrices
	stoneShader.setMat4("model", model);

	modelptr->Draw(stoneShader);
}

