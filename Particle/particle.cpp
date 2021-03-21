#include "particle.h"

Particle::Particle(glm::vec3 pos, glm::vec3 front, GLfloat life, GLfloat speed, glm::vec4 color = PARTICLE_COLOR)
	: BaseOBJ(ObjectionType::particle,
	pos,
	PARTICLE_SIZE,
	front,
	speed) 
{
	Life = PARTICLE_LIFE;
	Color = color;
	Scale = 1.0f;
}

Particle::Particle() :BaseOBJ(ObjectionType::particle, 
	PARTICLE_POS,
	PARTICLE_SIZE,
	PARTICLE_FRONT,
	PARTICLE_SPEED)
{
	Life = PARTICLE_LIFE;
	Color = PARTICLE_COLOR;
	Scale = 1.0f;
}

Particle::~Particle() {

}

ObjectionType Particle::getType() {
	return ObjectionType::particle;
}