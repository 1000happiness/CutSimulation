#version 330 core 
layout(location = 0) in vec2 position;

uniform bool background_flag;
uniform bool constraint_flag;
uniform float x_offset;
uniform float ratio;
uniform float standard_ratio;
uniform vec2 center;
uniform vec2 scale;
uniform float depth;

void main()
{
    if(background_flag && !constraint_flag) {
        gl_Position = vec4(center.x + position.x * scale.x / 2, center.y + position.y * scale.y / 2, depth, 1.0f);
    }
    else if(!background_flag && !constraint_flag) {
        vec2 new_scale = vec2(scale.x - x_offset * 2 * scale.x, (scale.x - x_offset * 2 * scale.x) / standard_ratio * ratio);
        vec2 new_center = vec2(center.x, new_scale.y / 2 + center.y - scale.y / 2);
        gl_Position = vec4(new_center.x + position.x * new_scale.x / 2, new_center.y + position.y * new_scale.y / 2, depth - 0.01f, 1.0f);
    }
    else if(!background_flag && constraint_flag) {
        vec2 new_scale = vec2(scale.x - x_offset * 2 * scale.x, (scale.x - x_offset * 2 * scale.x) / standard_ratio * ratio);
        vec2 new_begin = vec2(center.x - scale.x / 2 + scale.x * x_offset, center.y - scale.y / 2);
        vec2 new_position = new_scale * position + new_begin;
        gl_Position = vec4(new_position.x, new_position.y, depth - 0.02f, 1.0f);
    }
}