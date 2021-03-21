#include "Ship.h"



Ship::Ship(Shader shader): BaseOBJ(ObjectionType::ship, 
	SHIP_DEFAULT_POSITION,
	SHIP_SIZE,
	SHIP_DEFAULT_FRONT,
	0.0f){
	Correct = glm::vec3(-0.5f, 0.0f, -2.6f);
	// AABB correction
	Center += Correct;
	////////////

	Acc = SHIP_DEFAULT_ACC;
	Resist = SHIP_DEFAULT_RESIST;
	
	Right = SHIP_DEFAULT_RIGHT;
	Up = SHIP_DEFAULT_UP;

	rotSpeed = SHIP_ROT_SPEED;
	rotAngle = -PI/2;
	rotAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	waggle = 0;


	shipShader = shader;

	GetModel();
}

ObjectionType Ship::getType() {
	return Type;
}


void Ship::GetModel() {
	modelptr = ResourceManager::GetModel("shipModel");
}


void Ship::Render() {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, Position);
	model = glm::rotate(model, rotAngle, rotAxis);
	model = glm::rotate(model, PI * glm::cos(waggle) / 18.0f, Front);
	model = glm::scale(model, glm::vec3(0.01f));

	shipShader.use();

	shipShader.setMat4("model", model);

	modelptr->Draw(shipShader);
}


void Ship::Update(GLfloat deltaTime) {
	UpdateSpeed(deltaTime);
	UpdatePosition(deltaTime);
}

void Ship::UpdatePosition(GLfloat deltaTime) {
	if (rotAngle > PI)
		rotAngle - PI;
	if (rotAngle < -PI)
		rotAngle + PI;
	
	Position += Front * Speed * deltaTime;
	Center += Front * Speed * deltaTime;

	// For AABB box
	GLfloat tmpAngle = glm::abs(rotAngle + PI / 2);
	while (tmpAngle > PI / 2)
		tmpAngle -= PI / 2;
	Size.x = SHIP_SIZE.x + SHIP_SIZE.z * glm::sin(tmpAngle) * 0.5f;
	Size.z = SHIP_SIZE.z - SHIP_SIZE.z * glm::sin(tmpAngle) * 0.5f;
}

void Ship::UpdateSpeed(GLfloat deltaTime) {
	GLfloat delta_v = 0.0f;
	GLfloat delta_angle = 0.0f;
	bool pressKey = false;

	if (ResourceManager::Keys[GLFW_KEY_W]) {
		delta_v += Acc * deltaTime;
		pressKey = true;
	}
	if (ResourceManager::Keys[GLFW_KEY_S]) {
		delta_v -=  Acc * deltaTime;
		pressKey = true;
	}
	if (ResourceManager::Keys[GLFW_KEY_A]) {
		delta_angle += rotSpeed * deltaTime;
		pressKey = true;
	}
	if (ResourceManager::Keys[GLFW_KEY_D]) {
		delta_angle -= rotSpeed * deltaTime;
		pressKey = true;
	}
	rotAngle += delta_angle;
	waggle += 0.02f;
	if (waggle > 2*PI)
		waggle = 0.0f;

		
	// Resistance
	GLfloat moduleSpeed = glm::abs(Speed);

	GLfloat resistance = 0.0f;
	if (moduleSpeed > eps) {
		if (moduleSpeed < SHIP_SPEED_THRESHOLD) {
			resistance = Resist * deltaTime;
		}
		else {
			resistance = Resist * Resist * deltaTime;
		}

		if (resistance > moduleSpeed) {
			Speed = 0;
		}
		else {
			if (Speed > eps)
				Speed -= resistance;
			else
				Speed += resistance;
		}
	}

	Speed += delta_v;

	//std::cout << "delta_v: " << delta_v << std::endl;
	//std::cout << "Ship_Speed: " << Speed << std::endl;


	UpdateDirection(delta_angle);
}


void Ship::UpdateDirection(GLfloat delta_angle) {	
	Front = glm::normalize(Front);
	Up = glm::normalize(Up);
	Front = Front * glm::cos(delta_angle) - Right * glm::sin(delta_angle);
	Front = glm::normalize(Front);
	Right = glm::normalize(glm::cross(Front, Up));

	//std::cout << "Front: " << Front.x << " " << Front.y << " " << Front.z << std::endl;
}