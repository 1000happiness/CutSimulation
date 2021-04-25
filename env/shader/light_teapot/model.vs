#version 330 core 
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

layout (std140) uniform camera_matrices
{
    mat4 projection;
    mat4 view;
    vec3 aCameraPos;
};

uniform mat4 model;

out vec3 normal;
out vec3 fragPos;
out vec3 cameraPos;
void main()
{
    fragPos = vec3(model * vec4(aPos, 1.0f));
    gl_Position = projection * view * vec4(fragPos, 1.0f);
    normal = mat3(transpose(inverse(model))) * aNormal;
    cameraPos = aCameraPos;
}