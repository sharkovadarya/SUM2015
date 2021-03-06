/* Filename: TEST.VERT */

// ������ ����� �������
#version 330

// ��� ���� ��������
layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec2 InTexCoord;
layout(location = 2) in vec3 InNormal;
layout(location = 3) in vec4 InColor;

// ���������� ����������
uniform mat4 MatrWorld;
uniform mat4 MatrView;
uniform mat4 MatrProj;
uniform mat4 MatrWVP;
uniform mat4 MatrWV;
uniform mat4 MatrWVInverse;
uniform float Time;

// �������� ���������
out vec4 DrawColor;
out vec3 DrawNormal;
out vec3 DrawPosition;
out vec2 DrawTexCoord;

void main( void )
{
  
  gl_Position = vec4(InPosition, 1);
  
  DrawNormal = mat3(MatrWVInverse) * InNormal;
  
  DrawColor = InColor;
  DrawPosition = InPosition;
  DrawTexCoord = InTexCoord;
}
