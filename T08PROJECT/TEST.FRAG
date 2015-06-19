/* Filename: TEST.FRAG: */

// верси€ €зыка шейдера
#version 330

// выходные параметры - цвет рисовани€
out vec4 OutColor;

// входные параметры от вершинного шейдера
in vec4 DrawColor;
in vec3 DrawPosition;
in vec3 DrawNormal;
in vec2 DrawTexCoord;

/* “екстура */
uniform float IsTextureUse;
uniform sampler2D DrawTexture;



void main( void )
{
  vec4 tc = texture2D(DrawTexture, DrawTexCoord);
  OutColor = DrawColor * tc;
}


