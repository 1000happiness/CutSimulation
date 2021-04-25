#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 2) out;

// in vec3 vs_color;
// in vec3 vs_speed;

in VS_OUT {
    vec3 color;
    vec3 speed;
} gs_in[];

out vec3 fs_color;
void main() {
    vec3 normalize_speed = normalize(gs_in[0].speed);
    fs_color = gs_in[0].color;

    gl_Position = gl_in[0].gl_Position + 0.01 * vec4(normalize_speed, 0.0); 
    EmitVertex();

    gl_Position = gl_in[0].gl_Position - 0.01 * vec4(normalize_speed, 0.0);
    EmitVertex();

    EndPrimitive();
}