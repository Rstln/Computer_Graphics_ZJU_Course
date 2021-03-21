#ifndef SKYBOX_H
#define SKYBOX_H

#include <glad/glad.h>
#include <vector>
#include <string>
#include "texture.h"
#include "shader.h"
#include "../global_param.h"

class Skybox {
public:
    GLuint VAO, VBO;
    Shader shader;
    GLuint cubemapTexture;
    GLuint day_id, night_id;
    bool isNight = false;
    std::vector<std::string> faces{
    "../Images/skybox/right.jpg", "../Images/skybox/left.jpg", "../Images/skybox/top.jpg",
    "../Images/skybox/bottom.jpg", "../Images/skybox/front.jpg", "../Images/skybox/back.jpg"
    };
    std::vector<std::string> night_faces{
    "../Images/night_skybox/right.png", "../Images/night_skybox/left.png", "../Images/night_skybox/top.png",
    "../Images/night_skybox/bottom.png", "../Images/night_skybox/front.png", "../Images/night_skybox/back.png"
    };

    Skybox(Shader shader);
    ~Skybox() {};
    void Initialize();
    GLuint loadCubemap(std::vector<std::string> faces);
    void Render(glm::mat4 view, glm::mat4 projection);
};


#endif
