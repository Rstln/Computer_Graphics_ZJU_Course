#include "BaseOBJ.h"

BaseOBJ::BaseOBJ(ObjectionType type, glm::vec3 pos, glm::vec3 size, glm::vec3 dir, float speed) {
	Type = type;
	Position = pos;
	Size = size;
	Center = pos + size;
	Front = glm::normalize(dir);
	Right = glm::normalize(glm::cross(Front, glm::vec3(0.0f, 1.0f, 0.0f)));
	Up = glm::normalize(glm::cross(Right, Front));
	Speed = speed;
	isDead = false;
	boxShader = BOX_SHADER;
}


void BaseOBJ::update(glm::vec3 pos, glm::vec3 dir) {
	glm::vec3 worldUp(0.0f, 1.0f, 0.0f);
	Right = glm::normalize(glm::cross(getDirection(), worldUp));
	Up = glm::normalize(glm::cross(Right, getDirection()));
	setDirection(dir);
	setPosition(pos);
}


glm::vec3 BaseOBJ::getPosition() {
	return Position;
}

void BaseOBJ::setPosition(glm::vec3 pos) {
	Position = pos;
	Center = Position + Correct;
}

void BaseOBJ::setPosition(float px, float py, float pz) {
	Position.x = px;
	Position.y = py;
	Position.z = pz;
}

void BaseOBJ::addPosition(float dx, float dy, float dz) {
	Position.x += dx;
	Position.y += dy;
	Position.z += dz;
}

void BaseOBJ::addPosition(glm::vec3 increment) {
	Position = Position + increment;
}

glm::vec3 BaseOBJ::getDirection() const {
	return Front;
}

void BaseOBJ::setDirection(glm::vec3 dir) {
	Front = glm::normalize(dir);
}

void BaseOBJ::setDirection(float px, float py, float pz) {
	Front.x = px;
	Front.y = py;
	Front.z = pz;
	Front = glm::normalize(Front);
}

void BaseOBJ::addDirection(float dx, float dy, float dz) {
	Front.x += dx;
	Front.y += dy;
	Front.z += dz;
	Front = glm::normalize(Front);
}

void BaseOBJ::addDirection(glm::vec3 increment) {
	Front = Front + increment;
	Front = glm::normalize(Front);
}

void BaseOBJ::rotateDirection(float angelX, float angelY, float angelZ) {
	glm::mat4 rotate = glm::mat4(1.0f);
	if (angelX != 0.0f) {
		rotate = glm::rotate(rotate, glm::radians(angelX), glm::vec3(1.0f, 0.0f, 0.0f));
		Front = (glm::mat3)rotate * Front;
	}
	if (angelY != 0.0f) {
		rotate = glm::mat4(1.0f);
		rotate = glm::rotate(rotate, glm::radians(angelY), glm::vec3(0.0f, 1.0f, 0.0f));
		Front = (glm::mat3)rotate * Front;
	}
	if (angelZ != 0.0f) {
		rotate = glm::mat4(1.0f);
		rotate = glm::rotate(rotate, glm::radians(angelZ), glm::vec3(0.0f, 0.0f, 1.0f));
		Front = (glm::mat3)rotate * Front;
	}
	Front = glm::normalize(Front);
}

void BaseOBJ::setSpeed(float speed) {
	Speed = speed;
}

void BaseOBJ::setBox() {
	GLfloat box_v[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};
	glGenVertexArrays(1, &boxVAO);
	glGenBuffers(1, &boxVBO);
	glBindVertexArray(boxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, boxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(box_v), &box_v, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glBindVertexArray(0);

}

void BaseOBJ::renderBox(glm::mat4 view, glm::mat4 projection) {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, Center);
	model = glm::scale(model, Size);

	boxShader.use();
	boxShader.setMat4("view", view);
	boxShader.setMat4("projection", projection);
	boxShader.setMat4("model", model);

	glBindVertexArray(boxVAO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(0);
}

void BaseOBJ::drawBox(glm::mat4 view, glm::mat4 projection) {
	setBox();
	renderBox(view, projection);
}