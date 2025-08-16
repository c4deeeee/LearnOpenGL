#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// 메인 함수에서 직접 호출되는 함수(public)

bool initBeforeOfLoop();
void shutDown();



// 다른 함수 내부에서만 호출되는 함수(private)

bool generatedContext();
bool generatedBuffer();
bool compileShader();



// 콜백 및 이벤트 처리 함수

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
