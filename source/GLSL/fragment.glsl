#version 330 core
// in/out 키워드는 쉐이더와 쉐이더 끼리 데이터를 주고 받을 때(다음 단계 쉐이더로 일방향)
// 프래그먼트 색상을 결정하면 다음 단계인 테스트 및 출력 단계로 FragColor 전달
out vec4 FragColor;

// 버텍스 쉐이더에서 out vec3 ourColor;로 출력한 데이터를 전달받아 사용
// in vec3 ourColor;

// 래스터라이저 단계에서 프래그먼트 좌표와 보간되어 해당 프래그먼트가 이미지상에서 보간된 좌표로 받음
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2); // 텍스쳐 좌표를 이용해 텍스쳐링
}