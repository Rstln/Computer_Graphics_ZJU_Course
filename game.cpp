#include "game.h"
#include "stb_image.h"

bool isNight = false;
Ship* ourship;
Stone* stones[NUM_STONE];
Sphere* luminary;
Wave* ourWave;
Lamp* ourLamp;
ParticleGenerator* ourParticles;
ParticleGenerator* ourParticles2;
ParticleGenerator* ourParticles3;
GLfloat particle_scale = 0.02f;

// -------------wave stuffs-------------
int N = MESH_RESOLUTION;
int M = MESH_RESOLUTION;
float L_x = 2000;
float L_z = 2000;
float A = 1e-7f;
float V = 20;
glm::vec2 omega(1, 1);
//----------------------------------
// ----------light stuffs------------------
enum Light_Movment{
    LIGHT_UP, LIGHT_DOWN, LIGHT_Z_LESS, LIGHT_Z_MORE
};

glm::vec3 lightPos(0.0f, 5.0f, -5.0f);
GLfloat light_coeff = 1.0f;

void moveLight(Light_Movment dir, GLfloat deltaTime) {
    GLfloat velocity = 2.0f;
    if (dir == LIGHT_UP)
        lightPos += glm::vec3(0.0f, 1.0f, 0.0f) * deltaTime * velocity;
    if (dir == LIGHT_DOWN)
        lightPos -= glm::vec3(0.0f, 1.0f, 0.0f) * deltaTime * velocity;
    if (dir == LIGHT_Z_LESS)
        lightPos -= glm::vec3(0.0f, 0.0f, 1.0f) * deltaTime * velocity;
    if (dir == LIGHT_Z_MORE)
        lightPos += glm::vec3(0.0f, 0.0f, 1.0f) * deltaTime * velocity;


}
// -------------------------------------------
// --------------Stone stuffs------------------
glm::vec3 stone_pos[NUM_STONE]{
    glm::vec3(0.0f, 1.0f, -20.0f),
    glm::vec3(10.0f, 1.0f, -20.0f),
    glm::vec3(10.0f, 1.0f, -60.0f),
    glm::vec3(-10.0f, 1.0f, -70.0f),
};
glm::vec3 stone_size[NUM_STONE]{
    glm::vec3(1.0f, 1.0f, 1.0f),
    glm::vec3(5.0f, 5.0f, 5.0f),
    glm::vec3(10.0f, 10.0f, 10.0f),
    glm::vec3(7.0f, 7.0f, 7.0f),
};




Game::Game(GLuint width, GLuint height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{
    ResourceManager::lastX = width / 2.0f;
    ResourceManager::lastY = height / 2.0f;
}

Game::~Game() {}

void Game::Init() {
    // ------------------------1 Loading shaders----------------------------------
    ResourceManager::LoadShader("Shader/skybox.vert", "Shader/skybox.frag", NULL, "skyboxShader");
    ResourceManager::LoadShader("Shader/box.vert", "Shader/box.frag", NULL, "boxShader");
    ResourceManager::LoadShader("Shader/sphere.vert", "Shader/sphere.frag", NULL, "sphereShader");
    //ResourceManager::LoadShader("Shader/shadow_mapping_depth.vert", "Shader/shadow_mapping_depth.frag", NULL, "depthShader");
    //ResourceManager::LoadShader("Shader/shadowShader.vert", "Shader/shadowShader.frag", NULL, "shadowShader");
    ResourceManager::LoadShader("Shader/Phong.vert", "Shader/Phong.frag", NULL, "phongShader");
    ResourceManager::LoadShader("Shader/surface.vert", "Shader/surface.frag", NULL, "waveShader");
    ResourceManager::LoadShader("Shader/lamp.vert", "Shader/lamp.frag", NULL, "lampShader");
    ResourceManager::LoadShader("Shader/particle.vert", "Shader/particle.frag", NULL, "particleShader");




    // ------------------------2 Setting shaders----------------------------------
    ResourceManager::GetShader("skyboxShader").use();
    ResourceManager::GetShader("skyboxShader").setInt("skybox", 0);

    ResourceManager::GetShader("particleShader").use();
    ResourceManager::GetShader("particleShader").setInt("sprite", 0);



    // ------------------------3 Loading models----------------------------------
    ResourceManager::LoadModel("../Models/stone/stone.obj", "stoneModel");
    ResourceManager::LoadModel("../Models/ship/Pirate_ship.obj", "shipModel");




    // ------------------------4 instantiation----------------------------------

    ResourceManager::skybox = new Skybox(ResourceManager::GetShader("skyboxShader"));
    for (int i = 0; i < NUM_STONE; i++) {
        stones[i] = new Stone(ResourceManager::GetShader("phongShader"));
        stones[i]->Position = stone_pos[i];
        stones[i]->Size = stone_size[i];
    }
    ourship = new Ship(ResourceManager::GetShader("phongShader"));
    luminary = new Sphere(ResourceManager::GetShader("sphereShader")); 
    ourWave = new Wave(N, M, L_x, L_z, omega, V, A, 1, ResourceManager::GetShader("waveShader"));
    ourLamp = new Lamp(ResourceManager::GetShader("lampShader"));
    ourParticles = new ParticleGenerator(ResourceManager::GetShader("particleShader"), 500);
    ourParticles2 = new ParticleGenerator(ResourceManager::GetShader("particleShader"), 500);
    ourParticles3 = new ParticleGenerator(ResourceManager::GetShader("particleShader"), 500);
    ExportCube();

}

void Game::Update(GLfloat dt) {
    ourship->Update(dt);
    for (int i = 0; i < NUM_STONE; i++) {
        stones[i]->update(stone_pos[i], glm::vec3(0.0f, 1.0f, 1.0f));
    }

    ourParticles->Update(dt, *ourship, 5, ourship->getDirection() + glm::vec3(0.0f, 0.0f, 5.0f));
    ourParticles->Update(dt, *ourship, 5, ourship->getDirection() + glm::vec3(2.0f, 0.0f, 5.0f));
    ourParticles->Update(dt, *ourship, 5, ourship->getDirection() + glm::vec3(-2.0f, 0.0f, 5.0f));

    if (ResourceManager::followMode)
        ResourceManager::camera.SetPosition(ourship->Position + glm::vec3(0.0f, 4.0f, -3.0f));







    // ----------------------output data------------------------------
    //std::cout << "LightPos: " << lightPos.x << " " <<
    //    lightPos.y << " " <<
    //    lightPos.z << std::endl;

    //std::cout << "CamPos: " << ResourceManager::camera.Position.x << " " <<
    //    ResourceManager::camera.Position.y << " " <<
    //    ResourceManager::camera.Position.z << std::endl;
}


void Game::ProcessInput(GLfloat dt) {
    if (ResourceManager::Keys[GLFW_KEY_UP])
        ResourceManager::camera.ProcessKeyboard(FORWARD, dt);
    if (ResourceManager::Keys[GLFW_KEY_DOWN])
        ResourceManager::camera.ProcessKeyboard(BACKWARD, dt);
    if (ResourceManager::Keys[GLFW_KEY_LEFT])
        ResourceManager::camera.ProcessKeyboard(LEFT, dt);
    if (ResourceManager::Keys[GLFW_KEY_RIGHT])
        ResourceManager::camera.ProcessKeyboard(RIGHT, dt);
    if (ResourceManager::Keys[GLFW_KEY_X])
        isNight ^= 1;
    if (ResourceManager::Keys[GLFW_KEY_F])
        ResourceManager::followMode ^= 1;
    if (ResourceManager::Keys[GLFW_KEY_I])
        moveLight(LIGHT_UP, dt);
    if (ResourceManager::Keys[GLFW_KEY_K])
        moveLight(LIGHT_DOWN, dt);
    if (ResourceManager::Keys[GLFW_KEY_J])
        moveLight(LIGHT_Z_LESS, dt);
    if (ResourceManager::Keys[GLFW_KEY_L])
        moveLight(LIGHT_Z_MORE, dt);
    if (ResourceManager::Keys[GLFW_KEY_U])
        if (light_coeff < 2.0f)
            light_coeff += 0.01f;
    if (ResourceManager::Keys[GLFW_KEY_O])
        if (light_coeff > 0.2f)
            light_coeff -= 0.01f;
    if (ResourceManager::Keys[GLFW_KEY_N])
        if (particle_scale < 0.08f)
            particle_scale += 0.002f;
    if (ResourceManager::Keys[GLFW_KEY_M])
        if (particle_scale > 0.01f)
            particle_scale -= 0.002f;



}


GLboolean CheckCollision(BaseOBJ& box0, BaseOBJ& box1) {
    glm::vec3 Amax = box0.Center + box0.Size;
    glm::vec3 Amin = box0.Center - box0.Size;
    glm::vec3 Bmax = box1.Center + box1.Size;
    glm::vec3 Bmin = box1.Center - box1.Size;

    return (Amin.x < Bmax.x && Amax.x > Bmin.x) && (Amin.y < Bmax.y && Amax.y > Bmin.y) && (Amin.z < Bmax.z && Amax.z > Bmin.z);
}

void Game::DoCollision(BaseOBJ& box0, BaseOBJ& box1) {
    if (CheckCollision(box0, box1)) {
        std::cout << "Collision!!!!!!!!!!!!!!!" << std::endl;

        if (box0.getType() == ObjectionType::ship) {
            glm::vec3 bumpVec = glm::normalize(box0.Position - box1.Position);
            box0.Speed *= -0.7;
            box0.Position += bumpVec * REBOUNCE;
            box0.Center += bumpVec * REBOUNCE;
        }
        //TO DO: Mine explosion

    }
}


void Game::Render() {
    glm::mat4 projection = glm::perspective(glm::radians(ResourceManager::camera.Zoom), (float)Width / (float)Height, NEAR_PLANE, FAR_PLANE);
    glm::mat4 view = ResourceManager::camera.GetViewMatrix();


    // ------------------Set Weather------------------------------
    ResourceManager::skybox->isNight = isNight;
    luminary->isNight = isNight;



    // ------------------Draw Skybox---------------------------
    view = glm::mat4(glm::mat3(ResourceManager::camera.GetViewMatrix()));
    ResourceManager::skybox->Render(view, projection);
    view = ResourceManager::camera.GetViewMatrix();



    //--------------------Render as normal----------------------

    // Phong shading setting light properties
    ResourceManager::GetShader("phongShader").use();
    ResourceManager::GetShader("phongShader").setVec3("light.ambient", POINTLIGHT_AMBIENT * light_coeff);
    ResourceManager::GetShader("phongShader").setVec3("light.diffuse", POINTLIGHT_DIFFUSE * light_coeff);
    ResourceManager::GetShader("phongShader").setVec3("light.specular", POINTLIGHT_SPECULAR * light_coeff);
    ResourceManager::GetShader("phongShader").setFloat("material.shininess", 32.0f);


    ResourceManager::GetShader("waveShader").use();
    ResourceManager::GetShader("waveShader").setVec3("light.ambient", WAVE_AMBIENT * light_coeff);
    ResourceManager::GetShader("waveShader").setVec3("light.diffuse", WAVE_DIFFUSE * light_coeff);
    ResourceManager::GetShader("waveShader").setVec3("light.specular", WAVE_SPECULAR * light_coeff);
    ResourceManager::GetShader("waveShader").setFloat("material.shininess", 32.0f);

    ResourceManager::GetShader("sphereShader").use();
    ResourceManager::GetShader("sphereShader").setVec3("light.ambient_k", POINTLIGHT_AMBIENT * light_coeff);
    ResourceManager::GetShader("sphereShader").setVec3("light.diffuse_k", POINTLIGHT_DIFFUSE * light_coeff);
    ResourceManager::GetShader("sphereShader").setVec3("light.specular_k", POINTLIGHT_SPECULAR * light_coeff);
    ResourceManager::GetShader("sphereShader").setFloat("material.shininess", 32.0f);

    // Phong Shading setting view properties
    ResourceManager::GetShader("phongShader").use();
    ResourceManager::GetShader("phongShader").setVec3("viewPos", ResourceManager::camera.Position);
    ResourceManager::GetShader("phongShader").setVec3("light.position", lightPos);

    ResourceManager::GetShader("waveShader").use();
    ResourceManager::GetShader("waveShader").setVec3("viewPos", ResourceManager::camera.Position);
    ResourceManager::GetShader("waveShader").setVec3("light.position", lightPos);

    ResourceManager::GetShader("sphereShader").use();   
    ResourceManager::GetShader("sphereShader").setVec3("viewPos", ResourceManager::camera.Position);
    ResourceManager::GetShader("sphereShader").setVec3("light.position", lightPos);



    // set PV matrices
    ResourceManager::GetShader("phongShader").use();
    ResourceManager::GetShader("phongShader").setMat4("view", view);
    ResourceManager::GetShader("phongShader").setMat4("projection", projection);

    ResourceManager::GetShader("waveShader").use();
    ResourceManager::GetShader("waveShader").setMat4("view", view);
    ResourceManager::GetShader("waveShader").setMat4("projection", projection);

    ResourceManager::GetShader("sphereShader").use();
    ResourceManager::GetShader("sphereShader").setMat4("projection", projection);
    ResourceManager::GetShader("sphereShader").setMat4("view", view);

    ResourceManager::GetShader("lampShader").use();
    ResourceManager::GetShader("lampShader").setMat4("projection", projection);
    ResourceManager::GetShader("lampShader").setMat4("view", view);

    ResourceManager::GetShader("particleShader").use();
    ResourceManager::GetShader("particleShader").setMat4("projection", projection);
    ResourceManager::GetShader("particleShader").setMat4("view", view);


    ourship->Render();
    ourship->drawBox(view, projection);
    for (int i = 0; i < NUM_STONE; i++) {
        stones[i]->Render(stone_size[i].x);
        stones[i]->drawBox(view, projection);
    }

    ourWave->Render();
    ourLamp->Render(lightPos);
    luminary->Render();

    
    ourParticles->Draw(particle_scale);
    ourParticles2->Draw(particle_scale);
    ourParticles3->Draw(particle_scale);



    // ------------Draw AABB boxes-----------------------
    
    


    // ----------------------Check Collision-----------------------
    for (int i = 0; i < NUM_STONE; i++) {
        if (CheckCollision(*ourship, *stones[i])) {
            DoCollision(*ourship, *stones[i]);
        }
    }
    
}