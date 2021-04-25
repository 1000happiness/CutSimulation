#version 330 core 
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;

out VS_OUT {
    vec2 texcoord;
    vec3 normal;
} vs_out;

uniform float x_index;

void main()
{
    gl_Position = vec4(position.x + x_index, position.y, position.z, 1.0f);
    vs_out.texcoord = texcoord;
    vs_out.normal = normal;
}