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
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

//unsigned int indices[] = 
//{
//    0, 1, 3,
//    1, 2, 3
//};

glm::vec3 cubePositions[] = {
      glm::vec3(0.0f,  0.0f,  0.0f),
      glm::vec3(2.0f,  5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),
      glm::vec3(-1.7f,  3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),
      glm::vec3(1.5f,  2.0f, -2.5f),
      glm::vec3(1.5f,  0.2f, -1.5f),
      glm::vec3(-1.3f,  1.0f, -1.5f)
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

    // 메인 루프
    while (!glfwWindowShouldClose(window))
    {
        // 입력 처리
        processInput(window);

        /**
         * 버퍼 클리어
         * OpenGL은 두 개의 버퍼로 화면 출력과 다음 프레임 생성을 반복함(glfwSwapBuffers() 함수 설명에서 후술)
         * 지금 프레임의 픽셀들이 모니터 화면에 이미 출력되어 있는 상태로 버퍼가 스왑되면 이전에 출력되었던 픽셀들이 여전히 모니터에 출력되고 있어
         * 새로 그려지는 프레임의 픽셀을 그릴 때 이전 픽셀 위를 덮어 씌우므로 부자연 스러운 느낌을 줌, 다음 프레임 출력 직전 미리 설정한 배경색으로
         * 버퍼를 클리어 하여 그런 현상을 없애줄 수 있음
         * 
         * 깊이 버퍼의 경우 깊이 테스트에서 사용되며 프레임 마다 깊이 버퍼가 달라질 수 있으므로 초기화 필요
         */

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 텍스쳐 바인딩
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        
        /**
         * 뷰(view), 투영(projection) 행렬 생성
         * 3D에 대한 표현은 2D보다 좀 더 많은 공간 변형을 거쳐감, 이 거쳐가는 공간들은 로컬, 월드, 뷰, 클립, NDC, 스크린 총 5개로 구성됨
         * 
         * 로컬 공간 : 각 오브젝트가 중심이 되는 공간으로 공간의 원점(0, 0, 0) 이 물체 한 가운데 존재함
         * 
         * 월드 공간 : 로컬 공간에 모델 행렬을 곱하여 구할 수 있는 공간으로 모델 행렬은 월드의 다른 오브젝트에 비해 상대적으로 얼마나 크고 작은지(크기, Scale),
         *            어떤 축을 기준으로 몇 도 기울어져 있는지(회전, Rotation), 월드의 원점으로 부터 얼마나 떨어져 있는지(이동)로 구성되어 있음
         *            Model = Translate * Rotate * Scale(OpenGL에서 행렬간 곱셈은 오른쪽에서 왼쪽으로 수행, 행렬곱은 교환법칙이 성립하지 않으므로 주의)
         * 
         * 뷰 공간 : 월드 공간에서 뷰 행렬을 곱하여 구할 수 있는 공간으로 뷰 행렬은 관찰자 즉 카메라의 시점을 반영함(카메라 시스템 추가 후 디테일한 설명 추가)
         * 
         * 클립 공간 : 뷰 공간에서 투영 행렬을 곱하여 구할 수 있는 공간으로 투영 행렬은 직교 투영(Orthographic)과 원근 투영(Perspective)으로 구분됨
         *            직교 투영은 카메라로 부터 버텍스가 멀리 있어도 원근을 표현하지 않아 현실적인 3D 표현에서는 사용되지 않고 일부 3D 툴이나 탑뷰 게임에서 사용
         *            원근 투영은 원근을 표현하는 행렬로 카메라로 부터 먼 물체일수록 작게 표현되고 원근 왜곡이 발생
         *            투영 행렬을 구성하기 위해서는 카메라의 중심으로 부터 좌, 우, 위, 아래 이등변 삼각형에서 꼭지각의 크기인 시야각(Field of View, FOV)
         *            화면의 너비를 높이로 나눈 종횡비, 카메라와 가까운 근평면, 먼 원평면의 깊이가 필요함
         *            그중에서 근평면, 원평면과 좌, 우, 위, 아래 4개의 평면을 합쳐 하나의 입체 도형으로 만들면 절두체(Frustum)라고 하며, 이 절두체 바깥에 존재하는
         *            삼각형은 OpenGL에서 렌더링 되지 않음(Frustum Culling), 만약 삼각형이 절두체 면에 걸쳐 있다면 절두체 안에 있는 면에서 삼각형을 재구성
         * 
         * NDC : Normalized Device Coordinates(정규화된 장치 좌표)로 범위가 x, y, z -1.0 ~ 1.0 사이로 이루어진 좌표계
         *       클립 공간(x, y, z, w)의 x, y, z 속성을 각각 네 번째 속성인 w로 나누면(원근 분할) 버텍스의 좌표가 NDC 좌표 내에 속하도록 변형됨
         *       
         * 스크린 공간 : NDC에서 뷰포트의 크기(glViewPort로 설정)를 이용해 해당 버텍스가 뷰포트 내의 어떤 픽셀에 출력될지 결정하는 뷰포트 변환 수행하여
         *              프래그먼트로 변환
         * 
         * 이 모든 공간 변형은 렌더링 파이프라인에서 버텍스 쉐이더와 래스터라이저 사이에 수행되며 클립 공간까지의 생성은 버텍스 쉐이더에서 수행하는데
         * 로컬 -> 월드 변환에서와 같이 Clip = Projection * View * Model * Local 순서대로 곱하여야 함, 버텍스 쉐이더에서 클립 공간을 생성하면
         * 이후 단계에서 원근 분할을 통해 NDC 좌표 생성, 뷰포트 변환을 통해 스크린 공간을 생성
         */

        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        
        shader->use();
        shader->setMat4("projection", projection);
        shader->setMat4("view", view);

        glBindVertexArray(VAO);

        /**
         * 모델 행렬 생성
         * 뷰, 투영 행렬의 경우 하나의 행렬을 계속 사용하여도 무방하지만(뷰 행렬의 경우 다수의 카메라 시점을 표현하고 싶을 경우 행렬을 여러개 구성)
         * 모델 행렬의 경우 저마다 위치와 크기, 바라보는 방향이 다르기 때문에 모든 오브젝트에 개별적으로 존재해야 함
         */

        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f)); // 곱하고자 하는 행렬, 회전각, 회전축
            shader->setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36); // 드로우 콜
        }

        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 인덱스 버퍼용

        /**
         * 버퍼 스왑
         * 더블 버퍼(double buffer)는 프론트, 백 두 개의 버퍼를 사용해 화면 출력과 다음 프레임 생성을 반복하고
         * 프론트 버퍼에서 프레임을 모두 모니터에 출력했으면 다음 프레임 생성을 위해 백 버퍼로 역할을 바꾸고
         * 이전 프레임을 출력하는 동안 백 버퍼에서 다음 프레임에 대한 생성을 마치면 프론트 버퍼로 역할을 바꿈
         * glfwSwapBuffers()는 버퍼 스왑을 지시하는 함수
         */

        glfwSwapBuffers(window);
        glfwPollEvents(); // 입력 감지
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
     * 실행환경이 MacOS일 경우 다음 구문 추가 glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE)
     */

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

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

    /**
     * 깊이 테스트
     * 정육면체를 렌더링 하고자 할 때 투영 행렬이 적용될 경우 보고있는 시점을 기준으로 가까운 평면과 먼 평면이 존재함
     * 만약 깊이 테스트를 활성화 하지 않고 렌더링 하면 OpenGL 에서는 각 평면의 렌더링 순서를 보장하지 않기 때문에 가장 앞 평면을 먼저 렌더링 하고
     * 그 다음 가장 뒤에있는 평면을 렌더링 할 경우 앞면을 뒷면이 덮어 씌워버림, 정확한 렌더링을 위해서는 각 점들의 깊이값을 통해 뒤에 있는 점의 경우
     * 렌더링 하지 않도록 깊이 테스트를 활성화 해야함(기본값은 비활성화)
     */
    glEnable(GL_DEPTH_TEST);

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

    /*glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

    /**
     * 버텍스 어레이 속성 등록
     * 버텍스 버퍼의 GLSL 레이아웃 로케이션은 몇번인지, 몇 개의 데이터가 하나의 버텍스인지, 데이터의 자료형은 무엇인지 힌트 제공
     * 버텍스 속성은 기본값이 비활성화이기 때문에 glEnableVertexAttribArray()를 통해 반드시 활성화 해줘야 함
     * glVertexAttribPointer() 함수의 첫번째 인자는 버텍스 쉐이더에서 'layout (location = n) vec3 var'로 사용
     */

    // 버텍스 좌표 속성
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), static_cast<void*>(0));
    glEnableVertexAttribArray(0);

    // 버텍스 UV맵 속성
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // VBO 정보가 이미 VAO에 저장되어 있기 때문에 버텍스 버퍼를 언바인드 해도 무관함
    glBindVertexArray(0); // 버텍스 어레이는 glDeleteVertexArrays()로 제거하기 전까지 유지되므로 바인딩 해체후 나중에 메인 루프에서 변수 VAO로 리바인딩

    // 버퍼가 정상적으로 생성되었을 경우 버퍼 ID가 할당되며, 실패할 경우 버퍼 ID가 0
    if (VAO && VBO/* && EBO*/)
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
