#version 330 core
in vec4 position;
out vec4 frag_color;
uniform vec4 color;
uniform int use_position_flag;
void main()
{
   if(use_position_flag == 1)
   {
      frag_color = position;
   }
   else
   {
      frag_color = color;
   }
}