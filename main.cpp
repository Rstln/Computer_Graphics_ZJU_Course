#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "game.h"


float lastFrame = 0.0f, deltaTime = 0.0f;
Game ourGame(SCR_WIDTH, SCR_HEIGHT);

int main() {
	ResourceManager::window_initialize();

	ourGame.Init();
	
	while (!glfwWindowShouldClose(ResourceManager::window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();
		ourGame.ProcessInput(deltaTime);
		ourGame.Update(deltaTime);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		ourGame.Render();

		glfwSwapBuffers(ResourceManager::window);
	}

	ResourceManager::Clear();
	glfwTerminate();
	return 0;
}