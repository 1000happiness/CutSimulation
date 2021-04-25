#version 330 core
in vec2 TexCoord;
out vec4 FragColor;
in vec4 color;
void main()
{
   FragColor = color;
}