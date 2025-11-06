#version 450 core

layout(location = 0) in vec3 vertex_pos;
layout(location = 1) in vec3 color_data;

out vec3 colors;

uniform mat4 model;
uniform mat4 perspective;
uniform mat4 view;

void main() {
    colors = color_data;
    gl_Position = perspective * view * model * vec4(vertex_pos, 1.0f);
}
