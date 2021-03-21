#include "particle_generator.h"

ParticleGenerator::ParticleGenerator(Shader shader, GLuint amount)
    : shader(shader), amount(amount)
{
    this->init();
}

void ParticleGenerator::Update(GLfloat dt, BaseOBJ& object, GLuint newParticles, glm::vec3 offset)
{
    //random effect
    float offx = 0.06f * ((float)(rand() % 100 - 50) / 100.0f);
    float offz = 0.06f * ((float)(rand() % 100 - 50) / 100.0f);
    float offy = 0.06f * ((float)(rand() % 100 - 50) / 100.0f);
    offset = offset + glm::vec3(offx, offy, offz);


    // Add new particles 
    for (GLuint i = 0; i < newParticles; ++i)
    {
        int unusedParticle = this->firstUnusedParticle();
        this->respawnParticle(this->particles[unusedParticle], object, offset);
    }
    // Update all particles
    for (GLuint i = 0; i < this->amount; ++i)
    {
        Particle& p = this->particles[i];
        p.Life -= dt; // reduce life
        if (p.Life > 0.0f)
        {	// particle is alive, thus update
            p.addPosition(p.getDirection() * p.Speed * dt);
            p.addColorR(-dt * 50.0f);
            p.addColorG(-dt * 20.0f);
            p.addColorA(-dt * 11.0f);
        }
    }
}

// Render all particles
void ParticleGenerator::Draw(GLfloat scale_coeff)
{
    // Use additive blending to give it a 'glow' effect
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    this->shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(scale_coeff));
    //model = glm::rotate(model, PI, glm::vec3(0.0f, 0.0f, 1.0f));
    shader.setMat4("model", model);
    for (Particle particle : this->particles)
    {
        if (particle.Life > 0.0f)
        {
            this->shader.setFloat("scale", particle.Scale);
            this->shader.setVec3("offset", particle.Position);
            this->shader.setVec4("color", particle.Color);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, Map);         
            glBindVertexArray(this->VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
            
        }
    }
    // Don't forget to reset to default blending mode
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init()
{
    // Set up mesh and attribute properties
    GLuint VBO;
    GLfloat particle_quad[] = {
            -1.0f, -1.0f, -1.0f,  0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  1.0f, 0.0f, // bottom-right         
             1.0f,  1.0f, -1.0f,  1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f, 1.0f, // top-right         
             1.0f, -1.0f, -1.0f,  0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  0.0f, 0.0f, // bottom-left     
            // bottom face
            -1.0f, -1.0f, -1.0f,  0.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  1.0f, 1.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  1.0f, 0.0f, // bottom-left
            -1.0f, -1.0f,  1.0f,  0.0f, 0.0f, // bottom-right
            -1.0f, -1.0f, -1.0f,  0.0f, 1.0f, // top-right
            // top face
            -1.0f,  1.0f, -1.0f,  0.0f, 1.0f, // top-left
             1.0f,  1.0f , 1.0f,  1.0f, 0.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f, 1.0f, // top-right     
             1.0f,  1.0f,  1.0f,  1.0f, 0.0f, // bottom-right
            -1.0f,  1.0f, -1.0f,  0.0f, 1.0f, // top-left
            -1.0f,  1.0f,  1.0f,  0.0f, 0.0f  // bottom-left   
    };
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(this->VAO);
    // Fill mesh buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
    // Set mesh attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)3);
    glBindVertexArray(0);

    Map = loadTexture("../Images/particle/gold.jpg");

    // Create this->amount default particle instances
    for (GLuint i = 0; i < this->amount; ++i)
        this->particles.push_back(Particle());
}

// Stores the index of the last particle used (for quick access to next dead particle)
GLuint lastUsedParticle = 0;
GLuint ParticleGenerator::firstUnusedParticle()
{
    // First search from last used particle, this will usually return almost instantly
    for (GLuint i = lastUsedParticle; i < this->amount; ++i) {
        if (this->particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    // Otherwise, do a linear search
    for (GLuint i = 0; i < lastUsedParticle; ++i) {
        if (this->particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    // All particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
    lastUsedParticle = 0;
    return 0;
}

void ParticleGenerator::respawnParticle(Particle& particle, BaseOBJ& object, glm::vec3 offset)
{
    particle.setPosition(object.getPosition() + offset);
    float diff = 5.0f * (float)(rand() % 100 - 50) / 100.0f;
    particle.setColor(glm::vec4(15.0f + diff, 15.0f + diff, 15.0f + diff, 15.0f));
    particle.setScale(1.0f);
    particle.Life = PARTICLE_LIFE;
    particle.setDirection(-object.getDirection()+glm::vec3(0.0f, 1.0f, 0.0f));
}

