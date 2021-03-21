#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>
#include "../global_param.h"


class Texture2D {
public:
    GLuint ID;
    GLuint Width, Height;
    GLuint Internal_Format; // Format of texture object
    GLuint Image_Format; // Format of loaded image

    GLuint Wrap_S; // Wrapping mode on S axis
    GLuint Wrap_T; // Wrapping mode on T axis
    GLuint Filter_Min; // Filtering mode if texture pixels < screen pixels
    GLuint Filter_Max; // Filtering mode if texture pixels > screen pixels

    Texture2D();
    Texture2D(GLuint id);
    // Generates texture from image data
    void Generate(GLuint width, GLuint height, unsigned char* data);
    void GenerateShadowMap(GLuint width, GLuint height);
    // Binds the texture as the current active GL_TEXTURE_2D texture object
    void Bind() const;
};

#endif