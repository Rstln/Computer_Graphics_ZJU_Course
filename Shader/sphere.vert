#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    float pi = 3.1415926f;
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  

    float r = sqrt(aPos.x*aPos.x + aPos.y*aPos.y + aPos.z*aPos.z);
    float theta = acos(aPos.y / r);
    float phi = atan(aPos.x / aPos.z);

    TexCoords = vec2(phi/2/pi, theta/pi);
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}

