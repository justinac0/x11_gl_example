#version 450 core

layout(location = 0) in vec3 vertex_pos;
layout(location = 1) in vec3 color_data;

out vec3 colors;

void main() {
    colors = color_data;
    gl_Position.xyz = vertex_pos;
    gl_Position.w = 1.0;
}
