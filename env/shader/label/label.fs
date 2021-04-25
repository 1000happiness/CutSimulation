#version 330 core
in vec2 tex_coord; 

uniform sampler2D texture0;
uniform vec3 color;

out vec4 frag_color;
void main()
{
    frag_color = texture(texture0, tex_coord) * vec4(color, 1.0);
    // frag_color = vec4(1.0f,1.0f,1.0f,1.0f);
}