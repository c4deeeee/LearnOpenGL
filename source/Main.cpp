#include <Main.h>
#include <iostream>

GLFWwindow* window = nullptr;

float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
};

unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,  // first Triangle
    1, 2, 3   // second Triangle
};

const char* vertexShaderSource = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char* fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

int main() {
    if (initialize())
        return -1;

    /**
     * 버텍스 버퍼 등록
     * 화면에 그려낼 도형의 버텍스 데이터를 GPU 메모리에 등록 하면 GPU는 그 데이터를 이용해 렌더링 파이프라인을 수행
     * glGenBuffers() : 첫번째 인자는 생성할 버퍼의 갯수, 버퍼의 고유 ID *다수의 버퍼를 생성하려면 VBO를 배열로 선언하여 전달
     * glBindBuffer() : 버퍼의 종류를 설정, 종류가 다르다면 여러개의 버퍼를 한번에 설정할 수 있음
     *                  버퍼를 바인딩 하면 OpenGL 상태머신에 해당 버퍼를 등록 해두고, 그 버퍼를 이용해 렌더링 파이프라인을 수행
     *                  GL_ARRAY_BUFFER는 정점 데이터를 위한 버퍼이며 정점 데이터 버퍼가 이미 설정되어 있으므로 다른 정점 데이터 버퍼 설정 불가
     *                  glGenBuffers의 설명에 더해 여러개의 정점 데이터 버퍼를 설정하려면 버퍼 ID를 담을 변수(VBO)를 배열로 선언하여 다수 등록
     * glBufferData() : 버텍스 데이터를 버퍼로 복사하는 함수, 첫번째 인자는 전송할 버퍼 종류, 두번째 인자와 세번째 인자는 데이터의 크기와 데이터
     *                  네번째 인자는 GPU의 버퍼 관리 방식 설정, GL_STATIC_DRAW는 한 번의 버퍼 설정(정적)으로 무한히 사용(설정값 저장)
     *                  GL_STREAM_DRAW : 정적 설정, 설정값 미저장 / GL_DYNAMIC_DRAW : 동적 설정, 설정값 저장
     */
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(0));
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    /**
     * 쉐이더 컴파일 및 링크
     */
    int  success;
    char infoLog[512];

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);



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

        // 렌더링
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // 입력 감지 및 버퍼 스왑
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
}

/**
 * OpenGL 초기화
 */
bool initialize() {
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
    window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        return false;
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
        return false;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // 콜백 framebuffer_size_callback()을 컨텍스트에 등록
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
