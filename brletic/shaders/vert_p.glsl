#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in  vec3 color;
layout(location = 2) in  vec2 in_texcoord;
layout(location = 3) in  vec3  in_normal;

uniform mat4 model;
uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * transform * vec4(position, 1.0);
}
