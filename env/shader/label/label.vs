#version 330 core 
layout(location = 0) in vec2 position;
layout(location = 1) in vec2 tex;

uniform vec2 center;
uniform vec2 scale;
uniform float depth;

out vec2 tex_coord;

void main()
{
    gl_Position = vec4(center.x + position.x * scale.x / 2, center.y + position.y * scale.y / 2, depth, 1.0f);
    tex_coord = tex;
}