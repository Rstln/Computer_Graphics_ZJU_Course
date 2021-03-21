#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include "texture.h"
#include "shader.h"
#include "model.h"
#include "camera.h"
#include "skybox.h"

class ResourceManager {
public:
    // Hardware
    static GLFWwindow* window;
    static GLfloat lastX, lastY;
    static GLboolean followMode;
    static GLboolean firstMouse;
    static GLboolean Keys[1024];

    // Resource storage
    static std::map<std::string, Shader> Shaders;
    static std::map<std::string, Texture2D> Textures;
    static std::map<std::string, Model> Models;
    static Camera camera;
    static Skybox* skybox;


    static Shader LoadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile, std::string name);
    static Shader GetShader(std::string name);
    static Texture2D LoadTexture(const GLchar* file, GLboolean alpha, std::string name);
    static Texture2D GetTexture(std::string name);
    static Model* LoadModel(const GLchar* file, std::string name);
    static Model* GetModel(std::string name);

    static void window_initialize();
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    static void Clear();

private:
    static Shader loadShaderFromFile(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile = nullptr);
    static Texture2D loadTextureFromFile(const GLchar* filepath, GLuint format);
};

#endif