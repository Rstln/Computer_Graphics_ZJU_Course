#include "resource_manager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"

// Instantiate static variables
std::map<std::string, Texture2D> ResourceManager::Textures;
std::map<std::string, Shader> ResourceManager::Shaders;
std::map<std::string, Model> ResourceManager::Models;
Camera ResourceManager::camera;
Skybox* ResourceManager::skybox;
GLfloat ResourceManager::lastX, ResourceManager::lastY;
GLboolean ResourceManager::followMode = true;
GLboolean ResourceManager::firstMouse = true;
GLboolean ResourceManager::Keys[1024];
GLFWwindow* ResourceManager::window;


Shader ResourceManager::LoadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile, std::string name)
{
    Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return Shaders[name];
}

Shader ResourceManager::GetShader(std::string name)
{
    return Shaders[name];
}

Texture2D ResourceManager::LoadTexture(const GLchar* file, GLboolean alpha, std::string name)
{
    Textures[name] = loadTextureFromFile(file, alpha);
    return Textures[name];
}

Texture2D ResourceManager::GetTexture(std::string name)
{
    return Textures[name];
}

Model* ResourceManager::LoadModel(const GLchar* file, std::string name)
{
    Models[name] = Model(file);
    return &Models[name];
}

Model* ResourceManager::GetModel(std::string name)
{
    return &Models[name];
}

void ResourceManager::Clear()
{
    // (Properly) delete all shaders	
    for (auto iter : Shaders)
        glDeleteProgram(iter.second.ID);
    // (Properly) delete all textures
    for (auto iter : Textures)
        glDeleteTextures(1, &iter.second.ID);
}

Shader ResourceManager::loadShaderFromFile(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile) {
    Shader shader(vShaderFile, fShaderFile, gShaderFile);
    return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const GLchar* filepath, GLuint format) {
    Texture2D texture;
    texture.Internal_Format = texture.Image_Format = format;
    int width, height, nrComponents;
    unsigned char* image = stbi_load(filepath, &width, &height, &nrComponents, 0);
    if (image != nullptr) {
        texture.Generate(static_cast<GLuint>(width), static_cast<GLuint>(height), image);
    }
    else {
        std::cout << "Texture failed to load at path: " << filepath << std::endl;
    }
    stbi_image_free(image);
    return texture;
}

void ResourceManager::window_initialize() {
    srand(time(0));
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, WINDOW_NAME, NULL, NULL);
    if (window == NULL) {
        std::cout << "Fail to create GLFW window!" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Fail to initialize glad." << std::endl;
        glfwTerminate();
        return;
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    //    glEnable(GL_BLEND);
    //    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ResourceManager::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = static_cast<GLfloat>(xpos);
        lastY = static_cast<GLfloat>(ypos);
        firstMouse = false;
    }
    auto xoffset = static_cast<GLfloat>(xpos - lastX);
    auto yoffset = static_cast<GLfloat>(lastY - ypos);
    lastX = static_cast<GLfloat>(xpos);
    lastY = static_cast<GLfloat>(ypos);
    camera.ProcessMouseMovement(xoffset, yoffset);
}

void ResourceManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS)
            Keys[key] = GL_TRUE;
        else if (action == GLFW_RELEASE)
            Keys[key] = GL_FALSE;
    }
}

void ResourceManager::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}

void ResourceManager::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}