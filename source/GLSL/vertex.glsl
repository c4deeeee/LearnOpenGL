#version 330 core
// VAO의 glVertexAttribPointer()로 설정한 버텍스 속성 슬롯에서 설정한 데이터를 주입
// glVertexAttribPointer() 함수의 첫번째 인자가 location = n에 해당
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

// in/out 키워드는 쉐이더와 쉐이더 끼리 데이터를 주고 받을 때(다음 단계 쉐이더로 일방향)
// 출력 변수 ourColor는 이후 프래그먼트 쉐이더에서 in으로 전달받아 색상을 결정하는데 사용
// out vec3 ourColor;

// 텍스쳐 좌표는 래스터라이저 단계에서 프래그먼트 좌표와 보간하여 프래그먼트 쉐이더로 전달
out vec2 TexCoord;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(aPos, 1.0f);
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}