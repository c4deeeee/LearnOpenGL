#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//GLFWwindow* window;
//float vertices[];
//const char* vertexShaderSource;
//const char* fragmentShaderSource;

bool initialize();

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
