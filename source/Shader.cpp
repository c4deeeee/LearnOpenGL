#include "Shader.h"

/**
* 쉐이더 컴파일 및 링크
* 렌더링 파이프라인이 실행되면 GPU 내부에서 버퍼의 버텍스 데이터를 사용해 미리 정의된 프로그램(쉐이더)을 실행
* 버텍스 쉐이더 -> 지오메트리 쉐이더 -> 도형 어셈블리 -> 프래그먼트 쉐이더 -> 테스트 및 블렌딩 -> 화면 출력
* 버텍스 쉐이더와 프래그먼트 쉐이더는 프로그래머가 직접 프로그래밍 할 수 있음(선택사항: 지오메트리 쉐이더)
*/
Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertex, fragment;

    /**
     * 버텍스 쉐이더 : 버텍스 데이터를 3D 공간상의 정점 정보로 변환(버텍스 데이터와 MVP 행렬의 행렬 연산을 통해 클립 공간으로 변환)
     *                버텍스 데이터중 색상, 법선 정보를 프래그먼트 쉐이더로 전달하기도 함
     *                * MVP : Model(이동, 회전, 크기), View(카메라), Perspective(원근 투영)
     */

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    /**
     * 프래그먼트 쉐이더 : 파이프라인을 거쳐 래스터화 과정에서 스크린 좌표상의 삼각형이 모니터의 어느 픽셀을 덮는지 계산하여 프래그먼트 생성
     *                   프래그먼트 쉐이더에서는 해당 프래그먼트에 버텍스의 색상 데이터와 광원을 계산해 최종 색상 결정(프래그먼트는 모니터 픽셀에 대응)
     */

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    /**
     * 각각의 쉐이더를 컴파일 하면 쉐이더 객체가 생성되며 그것들을 링크해야 함
     * 링크 과정에서는 각각의 쉐이더의 입출력(in/out) 인터페이스를 체크함(VS에서 out 한 적 없는 데이터를 FS에서 in으로 사용하면 Error)
     */

    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);
    checkCompileErrors(id, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader()
{
    glDeleteProgram(id);
}

void Shader::use()
{
    glUseProgram(id);
}
    
/**
 * CPU -> GPU 데이터 전달 방식은 버텍스 버퍼 외에 유니폼을 통해 해당 쉐이더 프로그램(id)에 const 전역 변수 설정
 * 클립 공간을 만들때 사용하는 MVP 행렬을 CPU에서 계산하지 않고 GPU로 데이터를 넘겨 버텍스 쉐이더에서 계산하는 등의 방식으로 사용 가능
 */

void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setVec2(const std::string& name, const glm::vec2& value) const
{
    glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void Shader::setVec2(const std::string& name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
    glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const std::string& name, const glm::mat2& mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << std::endl;
        }
    }
}
