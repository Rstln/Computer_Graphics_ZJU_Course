#ifndef GLOBAL_PARAM_H
#define GLOBAL_PARAM_H


//Screen
#define WINDOW_NAME "GAME"
#define SCR_WIDTH 1200
#define SCR_HEIGHT 900

#define NEAR_PLANE 0.1f
#define FAR_PLANE 200.0f

#define SHADOW_WIDTH 1200
#define SHADOW_HEIGHT 900

//Scene


enum Meshtype {
    LAND,
    WATER
};
enum Direction {
    ORIGIN_POS,
    MOVE_XPOS,
    MOVE_XNEG,
    MOVE_ZPOS,
    MOVE_ZNEG
};



//Water
#define MESH_RESOLUTION 64
#define N_WAVES 60
#define WAVE_POS glm::vec3(0.0f, 0.0f, 0.0f)
#define WAVE_AMBIENT glm::vec3(.5f, .5f, .5f)
#define WAVE_DIFFUSE glm::vec3(1.0f, 1.0f, 1.0f)
#define WAVE_SPECULAR glm::vec3(1.0f, .9f, .7f)

// AABB box
#define BOX_SHADER ResourceManager::GetShader("boxShader")


//Light
#define POINTLIGHT_POS glm::vec3(0.0f, 5.0f, -5.0f)
#define POINTLIGHT_AMBIENT glm::vec3(0.5f)
#define POINTLIGHT_DIFFUSE glm::vec3(0.6f)
#define POINTLIGHT_SPECULAR glm::vec3(1.0f)
#define POINTLIGHT_CONSTANT 1.0f
#define POINTLIGHT_LINEAR 0.045f
#define POINTLIGHT_QUADRATIC 0.0075f

// Math
#define PI 3.1415926535f
#define eps 0.0000001

// For Luminary
#define SPHERE_POS glm::vec3(4.0f, 5.0f, 0.0f)


// For Camera
#define YAW -90.0f
#define PITCH 0.0f
#define SPEED 5.0f
#define YAW_SPEED 2.0f
#define SENSITIVITY 0.1f
#define ZOOM 45.0f
#define MIN_PITCH -89.5f
#define MAX_PITCH 89.5f
#define MIN_ZOOM 1.0f
#define MAX_ZOOM 45.0f

#define CAMERA_DEFAULT_POSITION glm::vec3(0.0f, 3.0f, 5.0f)
#define CAMERA_DEFAULT_UP glm::vec3(0.0f, 1.0f, 0.0f)
#define CAMERA_DEFAULT_FRONT glm::vec3(0.0f, 0.0f, -1.0f)


// For Stone
#define NUM_STONE 4



//Material
#define MODEL_DIFFUSE_NAME "texture_diffuse"
#define MODEL_SPECULAR_NAME "texture_specular"

//Particle
#define MINSPEED 0.15f
#define MAXSPEED 0.35f;
#define MINLIFENUMBER 0.7f;
#define MAXLIFENUMBER 1.3f;
#define DEFAULT_ACCELERATION glm::vec3(0.0f, 1.0f, 0.0f)

#endif
