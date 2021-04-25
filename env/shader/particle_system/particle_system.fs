#version 330 core

in vec3 fs_color;
out vec4 frag_color;

void main()
{
    frag_color = vec4(fs_color, 1.0f);
}