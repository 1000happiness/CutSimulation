#version 330 core
uniform vec4 light_color;
out vec4 fragColor;
void main()
{
    // fragColor = vec4(1.0f, color.y, 1.0f, 1.0f);
    fragColor = light_color;
}