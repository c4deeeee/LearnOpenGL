#version 330 core
// in/out 키워드는 쉐이더와 쉐이더 끼리 데이터를 주고 받을 때(다음 단계 쉐이더로 일방향)
// 프래그먼트 색상을 결정하면 다음 단계인 테스트 및 출력 단계로 FragColor 전달
out vec4 FragColor;

// 버텍스 쉐이더에서 out vec3 ourColor;로 출력한 데이터를 전달받아 사용
in vec3 ourColor;

void main()
{
    FragColor = vec4(ourColor, 1.0f);
}