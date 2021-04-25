#version 330 core 
layout(location = 0) in vec3 vs_pos;
layout(location = 1) in vec3 vs_color;
layout(location = 2) in vec3 vs_speed;

layout (std140) uniform camera_matrices
{
    mat4 projection;
    mat4 view;
    vec3 aCameraPos;
};

out VS_OUT {
    vec3 color;
    vec3 speed;
} vs_out;

void main()
{
    gl_Position = projection * view * vec4(vs_pos, 1.0f);
    vs_out.color = vs_color;
    vs_out.speed = vs_speed;
}