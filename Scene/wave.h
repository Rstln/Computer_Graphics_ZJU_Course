#ifndef WAVE_H
#define WAVE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include "../Resource/resource_manager.h"
#include "../global_param.h"
#include <string>

#include <cmath>
#include <iostream>
#include <random>
#include <complex>
#include "fftw/fftw3.h"
#include <time.h>


class Wave {
public:
	// N, M:      Resolution
	// L_x, L_z:  Actual lengths of the grid (as in meters)
	// omega_hat: Direction of wind
	// V:         Speed of wind	
	Wave(int N, int M, float L_x, float L_z, glm::vec2 omega, float V, float A, float lambda, Shader shader);
	~Wave();
	void buildField(float time);
	void buildTessendorfWaveMesh();
	void Render();

	GLfloat modelScale;
	GLuint VAO, VBO, EBO;
	int indexSize;

	bool record;

	GLfloat wave_time;

	Shader waveShader;
	glm::vec3* heightField;
	glm::vec3* normalField;
	GLfloat heightMax, heightMin;


	std::complex<float>* value_h_twiddle_0 = NULL;
	std::complex<float>* value_h_twiddle_0_conj = NULL;
	std::complex<float>* value_h_twiddle = NULL;

	std::default_random_engine generator;
	std::normal_distribution<float> normal_dist;

	const float pi = float(PI);
	const float g = 9.8f; // Gravitational constant
	const float l = 0.1;
	float A;
	float V;
	int N, M;
	int kNum;
	glm::vec2 omega_hat;
	float lambda;
	float L_x, L_z;

	inline float func_omega(float k) const;
	inline float func_P_h(glm::vec2 vec_k) const;
	inline std::complex<float> func_h_twiddle_0(glm::vec2 vec_k);
	inline std::complex<float> func_h_twiddle(int kn, int km, float t) const;
};


#endif // !WAVE_H
