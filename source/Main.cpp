#include <Main.h>
#include <Shader.h>

#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

GLFWwindow* window = nullptr;
Shader* shader = nullptr;
unsigned int shaderProgram;
unsigned int VBO, VAO, EBO;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float vertices[] = {
    // positions          // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
     0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
};

unsigned int indices[] = 
{
    0, 1, 3,  // 우상단, 우하단, 좌상단 버텍스
    1, 2, 3   // 우하단, 좌하단, 좌상단 버텍스
};

int main() 
{
    // 메인 루프 전 초기화
    if (!initBeforeOfLoop())
        return -1;

    // 쉐이더 컴파일 및 링크
    shader = new Shader("GLSL/vertex.glsl", "GLSL/fragment.glsl");

    /**
     * 텍스쳐 생성
     * glGenTextures() : 텍스쳐의 갯수와 각 텍스쳐의 ID를 저장할 비부호형 정수 배열을 인자로 받음(VAO등 객체 생성 방식과 동일)
     * glActiveTexture() : 텍스쳐 유닛 활성화, 텍스쳐 유닛은 OpenGL에서 여러개의 텍스쳐를 사용할 수 있도록 함
     *                     텍스쳐 생성 전 참조할 텍스쳐 유닛 인덱스(GL_TEXTURE0, GL_TEXTURE1 ...)를 설정
     *                     기본 텍스쳐 유닛은 GL_TEXTURE0이므로 하나의 텍스쳐만 사용할 때는 생략해도 무방함
     * glBindTexture() : 두번째 인자로 전달받은 텍스쳐 ID의 텍스쳐 타입을 설정
     */

    unsigned int texture1;
    glActiveTexture(GL_TEXTURE0); // 텍스쳐 유닛 인덱스 0
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    /**
     * 텍스쳐 래핑
     * 텍스쳐의 좌표는 좌하단 (0, 0) ~ 우상단 (1, 1) 까지이나 좌표를 초과한 경우 어떤 방식으로 표현할 지 설정, GL_REPEAT는 좌표를 초과해도 텍스쳐가 반복되도록 설정함
     * OpenGL에서는 텍스쳐 좌표를 (S, T)라고 표현하는데 S는 좌표평면 상에서 x좌표(가로), T는 y좌표(세로)를 의미, 3D 텍스쳐에서는 (S, T, R)까지 확장
     * 3D 모델링 툴에서 보통 UV맵이라고 표현함
     * 텍스쳐 좌표를 이용해 텍스쳐 색상을 검색하는 것을 샘플링(sampling)이라고 함
     */

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    /**
     * 텍스쳐 필터링
     * 텍스쳐를 입히려는 도형이 텍스쳐 이미지 해상도 보다 클 경우 프래그먼트의 색상을 어떤식으로 결정할 지 설정
     * GL_LINEAR : 색상 베이스는 해당 프래그먼트의 중심과 가장 가까운 이미지 픽셀의 색상을 선택하나, 인접한 다른 픽셀 색상을 보간하여 표현
     * GL_NEAREST : 해당 프래그먼트의 중심에 해당하는 이미지 픽셀 색상만 표현
     */

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /**
     * glTexImage2D() : stbi_load 함수를 통해 로드한 이미지 파일을 텍스쳐에 설정
     *                  첫번째 인자는 이미지 파일이 어느 타입의 텍스쳐에 설정 될것인지 결정함(1D, 3D 텍스쳐에는 영향 없음)
     *                  두번째 인자는 밉맵 레벨을 수동으로 지정할 때 전달하며 기본값은 0
     * 밉맵 : 카메라로 부터 멀리 떨어진 오브젝트의 경우 카메라 바로 앞에 있는 오브젝트의 텍스쳐와 비슷한 해상도를 가질 필요가 없음(메모리 낭비)
     *        밉맵은 이전 레벨 텍스쳐 해상도의 절반으로 다운 스케일링한 여러개의 텍스쳐를 미리 만들어 놓은것으로 거리에 맞게 해당하는 레벨의 텍스쳐 사용 가능
     *        glGenerateMipmap() 함수로 자동 밉맵 생성이 가능
     */

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("texture/container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data); // 이미지 바이너리 데이터가 텍스쳐에 복사 되었으므로 메모리 해제

    unsigned int texture2;
    glActiveTexture(GL_TEXTURE1); // 텍스쳐 유닛 인덱스 1
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    data = stbi_load("texture/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    shader->use(); // 쉐이더에 유니폼을 전달하기 위해 쉐이더 활성화
    glUniform1i(glGetUniformLocation(shader->id, "texture1"), 0); // 유니폼 설정
    shader->setInt("texture2", 1); // 쉐이더 클래스에서 미리 유니폼 설정 함수를 정의했으므로 그것 이용

    /**
     * 메인 루프
     * 별도의 종료 시그널이 발생하기 전까지 계속 렌더링이 되어야 함
     * glfwSwapBuffers() : OpenGL은 두개의 버퍼를 사용해 화면에 렌더링 하는데, 첫번째 버퍼는 완성된 화면을 출력하는 버퍼
     *                     두번째 버퍼는 다음 화면에 렌더링할 픽셀을 만드는 버퍼이며 두번째 버퍼가 완성되면 첫번째 버퍼와 스왑하여 렌더링
     *                     Double Buffer 방식이라고 하며 이 함수는 버퍼를 스왑하는 함수
     * glfwPollEvents() : 키보드, 마우스 입력을 감지하면 입력에 따른 콜백함수를 호출하는 함수
     * glClearColor() : 버퍼 클리어 색상(R, G, B, A) 설정 함수
     * glClear() : 버퍼를 클리어 해주지 않으면 이전 버퍼에서 생성된 프레임이 계속 출력됨, 설정한 RGBA 값으로 버퍼를 클리어 해주는 함수
     */

    while (!glfwWindowShouldClose(window))
    {
        // 입력 처리
        processInput(window);

        // 렌더링
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 텍스쳐 바인딩
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // 변환 행렬 생성
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

        // 유니폼으로 변환 행렬 전달
        shader->use();
        unsigned int transformLoc = glGetUniformLocation(shader->id, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        // 드로우 콜
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 인덱스 버퍼용

        // 입력 감지 및 버퍼 스왑
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    shutDown();
}

bool initBeforeOfLoop()
{
    if (generatedContext() && generatedBuffer())
        return true;
    return false;
}

/**
 * OpenGL 초기화
 */
bool generatedContext()
{
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

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        return false;
    }
    glfwMakeContextCurrent(window);

    /**
     * GLAD는 OpenGL 함수 로더 라이브러리
     * OpenGL은 크로노스 그룹에서 표준 사양을 제시하면 GPU 제조사 또는 운영체제에서 레퍼런스에 따라 구현하여 드라이버에 포함시킴
     * GLAD는 구현된 OpenGL 함수 포인터를 로드하여 쉽게 사용할 수 있도록 관리함
     */

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // 콜백 framebuffer_size_callback()을 컨텍스트에 등록

    return true;
}

bool generatedBuffer()
{
    /**
     * 버텍스 어레이 등록
     * 버텍스 버퍼와 인덱스 버퍼, 그리고 각 버퍼의 속성 정보를 한번에 관리하는 객체
     * 메인 루프에서 VAO만 바인드 하고 드로우 콜 하면 VBO와 EBO 정보를 이용해 렌더링 파이프라인 수행
     */

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

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

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /**
     * 인덱스 버퍼 등록
     * 인덱스는 각각의 버텍스가 어느 순서로 삼각형을 구성해야 하는지 저장(다수의 삼각형으로 구성된 물체의 경우 필요)
     * 예를들어 하나의 사각형은 4개의 버텍스 데이터로 그릴 수 있음 그 중 어떤 버텍스로 삼각형 두 개를 그릴지에 대한 힌트 제공
     * 등록 과정은 버퍼 종류가 GL_ELEMENT_ARRAY_BUFFER인 것을 제외하면 버텍스 버퍼와 동일함
     */

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    /**
     * 버텍스 어레이 속성 등록
     * 버텍스 버퍼의 GLSL 레이아웃 로케이션은 몇번인지, 몇 개의 데이터가 하나의 버텍스인지, 데이터의 자료형은 무엇인지 힌트 제공
     * 버텍스 속성은 기본값이 비활성화이기 때문에 glEnableVertexAttribArray()를 통해 반드시 활성화 해줘야 함
     * glVertexAttribPointer() 함수의 첫번째 인자는 버텍스 쉐이더에서 'layout (location = n) vec3 var'로 사용
     */

    // 버텍스 좌표 속성
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), static_cast<void*>(0));
    glEnableVertexAttribArray(0);

    // 버텍스 색상 속성
    /*glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);*/

    // 버텍스 UV맵 속성
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // VBO 정보가 이미 VAO에 저장되어 있기 때문에 버텍스 버퍼를 언바인드 해도 무관함
    glBindVertexArray(0); // 버텍스 어레이는 glDeleteVertexArrays()로 제거하기 전까지 유지되므로 바인딩 해체후 나중에 메인 루프에서 변수 VAO로 리바인딩

    /**
     * 버퍼가 정상적으로 생성되었을 경우 버퍼 ID가 할당되며, 실패할 경우 버퍼 ID가 0
     */

    if (VAO && VBO && EBO)
        return true;
    return false;
}

/**
 * 프로그램 종료 처리
 */
void shutDown()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    delete shader;
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
