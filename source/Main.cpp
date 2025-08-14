#include <Main.h>
#include <iostream>

int main() {
    /**
     * GLFW을 초기화하여 OpenGL 버전, 프로필을 설정함
     * 아래는 OpenGL 3.3 Core Profile을 사용함을 설정한 것
     * glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE) Mac OS 에서 설정해줘야 하는 설정
     */
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    /**
     * GLFW는 윈도우 창 생성을 도와주는 라이브러리
     * glfwCreateWindow() : 첫번째와 두번째 인자는 창의 가로, 세로 크기 세번째 인자는 창의 이름을 설정
     * glfwMakeContextCurrent() : 창이 생성되면 OpenGL 컨텍스트도 함께 생성되며 OpenGL 상태머신 상에서 활성화할 컨텍스트를 등록
     */
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL); 
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    /**
     * GLAD는 OpenGL 함수 로더 라이브러리
     * OpenGL은 크로노스 그룹에서 표준 사양을 제시하면 GPU 제조사 또는 운영체제에 레퍼런스에 따라 구현하여 드라이버에 포함시킴
     * GLAD는 구현된 OpenGL 함수 포인터를 로드하여 쉽게 사용할 수 있도록 관리함
     */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // 콜백함수를 창에 등록

    /**
     * 메인 루프
     * 별도의 종료 시그널이 발생하기 전까지 계속 렌더링이 되어야 함
     * glfwSwapBuffers() : OpenGL은 두개의 버퍼를 사용해 화면에 렌더링 하는데, 첫번째 버퍼는 완성된 화면을 출력하는 버퍼
     *                     두번째 버퍼는 다음 화면에 렌더링할 픽셀을 만드는 버퍼이며 두번째 버퍼가 완성되면 첫번째 버퍼와 스왑하여 렌더링
     *                     Double Buffer 방식이라고 하며 이 함수는 버퍼를 스왑하는 함수
     * glfwPollEvents() : 키보드, 마우스 입력을 감지하면 입력에 따른 콜백함수를 호출하는 함수
     * glClearColor() : 버퍼 클리어 색상(R, G, B, A) 설정 함수
     * glClear() : 버퍼를 클리어 해주지 않으면, 이전 버퍼에서 생성된 프레임이 계속 출력됨, 설정한 RGBA 값으로 버퍼를 클리어 해주는 함수
     */
    while (!glfwWindowShouldClose(window))
    {
        // 입력 처리
        processInput(window);

        // 드로우 콜
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 입력 감지 및 버퍼 스왑
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
}

/**
 * 입력처리
 */
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // ESC
        glfwSetWindowShouldClose(window, true);
}

/**
 * 윈도우 창 크기가 변경되면 자동으로 호출하는 콜백함수
 * glViewport() : 첫번째와 두번째 인자는 뷰포트 왼쪽 하단 모서리의 좌표, 세번째와 네번째 인자는 뷰포트의 해상도
 *                뷰포트는 OpenGL이 실제로 렌더링을 수행하는 창으로 창 크기와는 별개임(흔히 게임 엔진이나 모델링툴에서 모델이 보여지는 뷰포트)
 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
