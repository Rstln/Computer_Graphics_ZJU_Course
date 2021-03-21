#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Resource/resource_manager.h"
#include "BaseOBJ.h"
#include "Ship/Ship.h"
#include "Stone.h"
#include "Scene/Sphere.h"
#include "global_param.h"
#include "Scene/wave.h"
#include "Scene/lamp.h"
#include "Particle/particle_generator.h"
#include "OBJWriter.h"

// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
    // Game state
    GameState              State;
    GLboolean              Keys[1024];
    GLuint                 Width, Height;

    // Constructor/Destructor
    Game(GLuint width, GLuint height);
    ~Game();
    // Initialize game state (load all shaders/textures/levels)
    void Init();
    // GameLoop
    void ProcessInput(GLfloat dt);
    void Update(GLfloat dt);
    void Render();
    void DoCollision(BaseOBJ& box0, BaseOBJ& box1);
};

#endif