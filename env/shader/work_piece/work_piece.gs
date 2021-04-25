#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec2 texcoord;
    vec3 normal;
} gs_in[];

layout (std140) uniform camera_matrices
{
    mat4 projection;
    mat4 view;
    vec3 camera_pos;
};

uniform mat4 model;
uniform float x_length;
uniform bool surface_flag;
uniform float left_radius;
uniform float right_radius;

out GS_OUT {
    float surface_flag;
    vec2 texcoord;
    vec3 frag_pos;
    vec3 camera_pos;
    vec3 normal;
} gs_out;

void main() {
    mat4 trans = projection * view * model;
    
    vec3 position;
    if(!surface_flag) {
        vec3 position0;
        vec3 position1;
        vec3 position2;
        float flag = 0.0f;
        if(gl_in[1].gl_Position.x > gl_in[0].gl_Position.x) {
            position0 = vec3(gl_in[0].gl_Position.x, gl_in[0].gl_Position.y * left_radius, gl_in[0].gl_Position.z * left_radius);
            position1 = vec3(gl_in[1].gl_Position.x, gl_in[1].gl_Position.y * right_radius, gl_in[1].gl_Position.z * right_radius);
            position2 = vec3(gl_in[2].gl_Position.x, gl_in[2].gl_Position.y * left_radius, gl_in[2].gl_Position.z * left_radius);
        }
        else {
            position0 = vec3(gl_in[0].gl_Position.x, gl_in[0].gl_Position.y * right_radius, gl_in[0].gl_Position.z * right_radius);
            position1 = vec3(gl_in[1].gl_Position.x, gl_in[1].gl_Position.y * left_radius, gl_in[1].gl_Position.z * left_radius);
            position2 = vec3(gl_in[2].gl_Position.x, gl_in[2].gl_Position.y * right_radius, gl_in[2].gl_Position.z * right_radius);
        }
        if(left_radius - right_radius > 0.05 || right_radius - left_radius > 0.05)
        {
            flag = 1.0f;
        }
        gl_Position = trans * vec4(position0, 1.0f);
        gs_out.surface_flag = flag;
        gs_out.texcoord = vec2(gl_in[0].gl_Position.x / x_length, gs_in[0].texcoord.y);
        gs_out.frag_pos = vec3(model * vec4(position0, 1.0f));
        gs_out.camera_pos = camera_pos;
        gs_out.normal = mat3(transpose(inverse(model))) * gs_in[0].normal;
        EmitVertex();
        gl_Position = trans * vec4(position1, 1.0f);
        gs_out.surface_flag = flag;
        gs_out.texcoord = vec2(gl_in[1].gl_Position.x / x_length, gs_in[1].texcoord.y);
        gs_out.frag_pos = vec3(model * vec4(position1, 1.0f));
        gs_out.camera_pos = camera_pos;
        gs_out.normal = mat3(transpose(inverse(model))) * gs_in[1].normal;
        EmitVertex();
        gl_Position = trans * vec4(position2, 1.0f);
        gs_out.surface_flag = flag;
        gs_out.texcoord = vec2(gl_in[2].gl_Position.x / x_length, gs_in[2].texcoord.y);
        gs_out.frag_pos = vec3(model * vec4(position2, 1.0f));
        gs_out.camera_pos = camera_pos;
        gs_out.normal = mat3(transpose(inverse(model))) * gs_in[2].normal;
        EmitVertex();
        EndPrimitive();
    }
    else {
        vec3 position0;
        vec3 position1;
        vec3 center;
        vec3 normal;
        if(gl_in[1].gl_Position.x > gl_in[0].gl_Position.x) {
            position0 = vec3(gl_in[0].gl_Position.x, gl_in[0].gl_Position.y * left_radius, gl_in[0].gl_Position.z * left_radius);
            position1 = vec3(gl_in[2].gl_Position.x, gl_in[2].gl_Position.y * left_radius, gl_in[2].gl_Position.z * left_radius);
            center = vec3(gl_in[0].gl_Position.x, 0, 0);
            normal = vec3(-1, 0, 0);
        }
        else {
            position0 = vec3(gl_in[0].gl_Position.x, gl_in[0].gl_Position.y * right_radius, gl_in[0].gl_Position.z * right_radius);
            position1 = vec3(gl_in[2].gl_Position.x, gl_in[2].gl_Position.y * right_radius, gl_in[2].gl_Position.z * right_radius);
            center = vec3(gl_in[0].gl_Position.x, 0, 0);
            normal = vec3(1, 0, 0);
        }
        gl_Position = trans * vec4(position0, 1.0f);
        gs_out.texcoord = vec2(gl_in[0].gl_Position.y / 2 + 0.5, gl_in[0].gl_Position.z / 2 + 0.5);
        gs_out.frag_pos = vec3(model * vec4(position0, 1.0f));
        gs_out.camera_pos = camera_pos;
        gs_out.normal = mat3(transpose(inverse(model))) * normal;
        EmitVertex();
        gl_Position = trans * vec4(position1, 1.0f);
        gs_out.texcoord = vec2(gl_in[2].gl_Position.y / 2 + 0.5, gl_in[0].gl_Position.z / 2 + 0.5);
        gs_out.frag_pos = vec3(model * vec4(position1, 1.0f));
        gs_out.camera_pos = camera_pos;
        gs_out.normal = mat3(transpose(inverse(model))) * normal;
        EmitVertex();
        gl_Position = trans * vec4(center, 1.0f);
        gs_out.texcoord = vec2(0.5, 0.5);
        gs_out.frag_pos = vec3(model * vec4(center, 1.0f));
        gs_out.camera_pos = camera_pos;
        gs_out.normal = mat3(transpose(inverse(model))) * normal;
        EmitVertex();
        EndPrimitive();
    }
}