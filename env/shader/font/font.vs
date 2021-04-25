#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 tex_coord;

uniform mat4 ortho_projection;

void main()
{
    gl_Position = ortho_projection * vec4(vertex.xy, 0.0, 1.0);
    tex_coord = vertex.zw;
}  