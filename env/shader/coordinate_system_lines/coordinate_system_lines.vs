#version 330 core 
layout(location = 0) in vec3 position;

layout (std140) uniform camera_matrices
{
    mat4 projection;
    mat4 view;
    vec3 aCameraPos;
};

uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
}