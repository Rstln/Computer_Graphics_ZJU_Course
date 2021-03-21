#include "Sphere.h"

GLfloat sphere_v[6 * 3 * LAT * LON];

Sphere::Sphere(Shader shader) {
    sphereShader = shader;
    setSphere(sphere_v, LON, LAT);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_v), sphere_v, GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    Map[0] = loadTexture("../Images/luminary/sun.jpg");
    Map[1] = loadTexture("../Images/luminary/moon.jpg");

    sphereShader.use();
    sphereShader.setInt("material.diffuse", 0);



}


glm::vec3  Sphere::getPoint(GLfloat u, GLfloat v) {
    GLfloat R = 1.0f;
    GLfloat z = R * std::cos(PI * u);
    GLfloat x = R * std::sin(PI * u) * std::cos(2 * PI * v);
    GLfloat y = R * std::sin(PI * u) * std::sin(2 * PI * v);

    return glm::vec3(x, y, z);
}


void Sphere::setSphere(GLfloat* sphere, GLuint Longitude, GLuint Latitude) {
    GLfloat lon_step = 1.0f / Longitude;
    GLfloat lat_step = 1.0f / Latitude;
    GLuint offset = 0;
    for (int lat = 0; lat < Latitude; lat++) {
        for (int lon = 0; lon < Longitude; lon++) {
            // 4 points once, for 2 triangles
            glm::vec3 point1 = getPoint(lat * lat_step, lon * lon_step);
            glm::vec3 point2 = getPoint((lat + 1) * lat_step, lon * lon_step);
            glm::vec3 point3 = getPoint((lat + 1) * lat_step, (lon + 1) * lon_step);
            glm::vec3 point4 = getPoint(lat * lat_step, (lon + 1) * lon_step);
            memcpy(sphere + offset, glm::value_ptr(point1), 3 * sizeof(GLfloat));
            offset += 3;
            memcpy(sphere + offset, glm::value_ptr(point4), 3 * sizeof(GLfloat));
            offset += 3;
            memcpy(sphere + offset, glm::value_ptr(point3), 3 * sizeof(GLfloat));
            offset += 3;

            memcpy(sphere + offset, glm::value_ptr(point1), 3 * sizeof(GLfloat));
            offset += 3;
            memcpy(sphere + offset, glm::value_ptr(point3), 3 * sizeof(GLfloat));
            offset += 3;
            memcpy(sphere + offset, glm::value_ptr(point2), 3 * sizeof(GLfloat));
            offset += 3;
        }
    }
}

void Sphere::Render() {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, SPHERE_POS);

    sphereShader.use();


    sphereShader.setMat4("model", model);

    glActiveTexture(GL_TEXTURE0);
    if (isNight)
        glBindTexture(GL_TEXTURE_2D, Map[1]);
    else
        glBindTexture(GL_TEXTURE_2D, Map[0]);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6 * LAT * LON);
    glBindVertexArray(0);
}