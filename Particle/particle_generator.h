#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "../Resource/resource_manager.h"
#include "../BaseOBJ.h"
#include "particle.h"



class ParticleGenerator {
public:
    GLuint amount;
    GLuint Map;
    Shader shader;
    GLuint VAO;
    std::vector<Particle> particles;

    ParticleGenerator(Shader shader, GLuint amount);
    void Update(GLfloat dt, BaseOBJ& object, GLuint newParticles, glm::vec3 offset = glm::vec3(0.0f));
    void Draw(GLfloat scale_coeff = 0.02f);
    void init();
    GLuint firstUnusedParticle();
    void respawnParticle(Particle& particle,BaseOBJ& object, glm::vec3 offset = glm::vec3(0.0f));
};

#endif

