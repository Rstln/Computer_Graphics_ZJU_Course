#include "wave.h"


// Get k vector from mesh grid (n,m)
#define K_VEC(n,m) glm::vec2(2 * PI * (n - N / 2) / L_x, 2 * PI * (m  - M / 2) / L_z)




Wave::Wave(int N, int M, float L_x, float L_z, glm::vec2 omega, float V, float A, float lambda, Shader shader) :
	N(N), M(M),
	omega_hat(normalize(omega)),
	V(V), L_x(L_x), L_z(L_z),
	A(A),
	lambda(lambda)
{
	generator.seed(time(NULL));
	kNum = N * M;

	heightField = new glm::vec3[kNum];
	normalField = new glm::vec3[kNum];

	value_h_twiddle_0 = new std::complex<float>[kNum];
	value_h_twiddle_0_conj = new std::complex<float>[kNum];
	value_h_twiddle = new std::complex<float>[kNum];

	// Initialize value_h_twiddle_0 and value_h_twiddle_0_conj in Eq26
	for (int n = 0; n < N; ++n)
		for (int m = 0; m < M; ++m)
		{
			int index = m * N + n;
			glm::vec2 k = K_VEC(n, m);
			value_h_twiddle_0[index] = func_h_twiddle_0(k);
			value_h_twiddle_0_conj[index] = conj(func_h_twiddle_0(k));
		}




	//------------------init buffer objects-----------------
	indexSize = (N - 1) * (M - 1) * 6;
	GLuint* indices = new GLuint[indexSize];

	int p = 0;

	for (int j = 0; j < N - 1; j++)
		for (int i = 0; i < M - 1; i++)
		{
			indices[p++] = i + j * N;
			indices[p++] = (i + 1) + j * N;
			indices[p++] = i + (j + 1) * N;

			indices[p++] = (i + 1) + j * N;
			indices[p++] = (i + 1) + (j + 1) * N;
			indices[p++] = i + (j + 1) * N;
		}

	// Element buffer object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize * sizeof(GLuint), indices, GL_STATIC_DRAW);

	delete[] indices;

	waveShader = shader;
	modelScale = 0.1f;
	wave_time = 0;
	heightMax = 0;
	heightMin = 0;
}

Wave::~Wave()
{
	delete[] heightField;
	delete[] normalField;

	delete[] value_h_twiddle_0;
	delete[] value_h_twiddle;
	delete[] value_h_twiddle_0_conj;
}




void Wave::buildTessendorfWaveMesh() {
	int nVertex = N * M;

	buildField(wave_time);

	int p = 0;

	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
		{
			int index = j * N + i;

			if (heightField[index].y > heightMax) heightMax = heightField[index].y;
			else if (heightField[index].y < heightMin) heightMin = heightField[index].y;
		}


	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	int fieldArraySize = sizeof(glm::vec3) * nVertex;
	glBufferData(GL_ARRAY_BUFFER, fieldArraySize * 2, NULL, GL_STATIC_DRAW);

	// Copy height to buffer
	glBufferSubData(GL_ARRAY_BUFFER, 0, fieldArraySize, heightField);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Copy normal to buffer
	glBufferSubData(GL_ARRAY_BUFFER, fieldArraySize, fieldArraySize, normalField);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)fieldArraySize);
	glEnableVertexAttribArray(1);
}


void Wave::Render() {
	wave_time += 0.2f;
	buildTessendorfWaveMesh();

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(modelScale));	// Scale the surface
	model = glm::rotate(model, PI, glm::vec3(0.0f, 0.0f, 1.0f));

	waveShader.use();
	waveShader.setMat4("model", model);
	waveShader.setFloat("heightMin", heightMin*modelScale);
	waveShader.setFloat("heightMax", heightMax * modelScale);


	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}



// Eq14
inline float Wave::func_omega(float k) const
{
	return sqrt(g * k);
}

// Eq23 Phillips spectrum 
inline float Wave::func_P_h(glm::vec2 vec_k) const
{
	if (vec_k == glm::vec2(0.0f, 0.0f))
		return 0.0f;

	float L = V * V / g; // Largest possible waves arising from a continuous wind of speed V

	float k = length(vec_k);
	glm::vec2 k_hat = normalize(vec_k);

	float dot_k_hat_omega_hat = glm::dot(k_hat, omega_hat);
	float result = A * exp(-1 / (k * L * k * L)) / pow(k, 4) * pow(dot_k_hat_omega_hat, 2);

	result *= exp(-k * k * l * l);  // Eq24

	return result;
}

// Eq25
inline std::complex<float> Wave::func_h_twiddle_0(glm::vec2 vec_k)
{
	float xi_r = normal_dist(generator);
	float xi_i = normal_dist(generator);
	return sqrt(0.5f) * std::complex<float>(xi_r, xi_i) * sqrt(func_P_h(vec_k));
}

// Eq26
inline std::complex<float> Wave::func_h_twiddle(int kn, int km, float t) const
{
	int index = km * N + kn;
	float k = length(K_VEC(kn, km));
	std::complex<float> term1 = value_h_twiddle_0[index] * exp(std::complex<float>(0.0f, func_omega(k) * t));
	std::complex<float> term2 = value_h_twiddle_0_conj[index] * exp(std::complex<float>(0.0f, -func_omega(k) * t));
	return term1 + term2;
}

//Eq19
void Wave::buildField(float time)
{
	fftwf_complex* in_height, * in_slope_x, * in_slope_z, * in_D_x, * in_D_z;
	fftwf_complex* out_height, * out_slope_x, * out_slope_z, * out_D_x, * out_D_z;

	fftwf_plan p_height, p_slope_x, p_slope_z, p_D_x, p_D_z;

	// Eq20 ikh_twiddle
	std::complex<float>* slope_x_term = new std::complex<float>[kNum];
	std::complex<float>* slope_z_term = new std::complex<float>[kNum];

	// Eq29 
	std::complex<float>* D_x_term = new std::complex<float>[kNum];
	std::complex<float>* D_z_term = new std::complex<float>[kNum];

	for (int n = 0; n < N; n++)
		for (int m = 0; m < M; m++)
		{
			int index = m * N + n;

			value_h_twiddle[index] = func_h_twiddle(n, m, time);

			glm::vec2 kVec = K_VEC(n, m);
			float kLength = glm::length(kVec);
			glm::vec2 kVecNormalized = kLength == 0 ? kVec : normalize(kVec);

			slope_x_term[index] = std::complex<float>(0, kVec.x) * value_h_twiddle[index];
			slope_z_term[index] = std::complex<float>(0, kVec.y) * value_h_twiddle[index];
			D_x_term[index] = std::complex<float>(0, -kVecNormalized.x) * value_h_twiddle[index];
			D_z_term[index] = std::complex<float>(0, -kVecNormalized.y) * value_h_twiddle[index];
		}

	// Prepare fft input and output
	in_height = (fftwf_complex*)value_h_twiddle;
	in_slope_x = (fftwf_complex*)slope_x_term;
	in_slope_z = (fftwf_complex*)slope_z_term;
	in_D_x = (fftwf_complex*)D_x_term;
	in_D_z = (fftwf_complex*)D_z_term;

	out_height = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * kNum);
	out_slope_x = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * kNum);
	out_slope_z = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * kNum);
	out_D_x = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * kNum);
	out_D_z = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * kNum);

	p_height = fftwf_plan_dft_2d(N, M, in_height, out_height, FFTW_BACKWARD, FFTW_ESTIMATE);
	p_slope_x = fftwf_plan_dft_2d(N, M, in_slope_x, out_slope_x, FFTW_BACKWARD, FFTW_ESTIMATE);
	p_slope_z = fftwf_plan_dft_2d(N, M, in_slope_z, out_slope_z, FFTW_BACKWARD, FFTW_ESTIMATE);
	p_D_x = fftwf_plan_dft_2d(N, M, in_D_x, out_D_x, FFTW_BACKWARD, FFTW_ESTIMATE);
	p_D_z = fftwf_plan_dft_2d(N, M, in_D_z, out_D_z, FFTW_BACKWARD, FFTW_ESTIMATE);

	fftwf_execute(p_height);
	fftwf_execute(p_slope_x);
	fftwf_execute(p_slope_z);
	fftwf_execute(p_D_x);
	fftwf_execute(p_D_z);

	for (int n = 0; n < N; n++)
		for (int m = 0; m < M; m++)
		{
			int index = m * N + n;
			float sign = 1;

			// Flip the sign
			if ((m + n) % 2) sign = -1;

			normalField[index] = glm::normalize(glm::vec3(
				sign * out_slope_x[index][0],
				-1,
				sign * out_slope_z[index][0]));

			heightField[index] = glm::vec3(
				(n - N / 2) * L_x / N - sign * lambda * out_D_x[index][0],
				sign * out_height[index][0],
				(m - M / 2) * L_z / M - sign * lambda * out_D_z[index][0]);
		}

	fftwf_destroy_plan(p_height);
	fftwf_destroy_plan(p_slope_x);
	fftwf_destroy_plan(p_slope_z);
	fftwf_destroy_plan(p_D_x);
	fftwf_destroy_plan(p_D_z);


	// Free
	delete[] slope_x_term;
	delete[] slope_z_term;
	delete[] D_x_term;
	delete[] D_z_term;
	fftwf_free(out_height);
	fftwf_free(out_slope_x);
	fftwf_free(out_slope_z);
	fftwf_free(out_D_x);
	fftwf_free(out_D_z);
}
