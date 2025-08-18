#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

bool initBeforeOfLoop();
void shutDown();

bool generatedContext();
bool generatedBuffer();

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
