#version 330 core

layout(location = 0) in  vec3 position;
layout(location = 1) in  vec3 color;
layout(location = 2) in  vec2 in_texcoord;
layout(location = 3) in  vec3  in_normal;


uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

out vec2 ex_texcoord;
out vec3 ex_normal;
out vec3 ex_fragPos;

void main()
{    
    ex_fragPos = vec3(transform * vec4(position, 1.0));
    gl_Position = projection * view * transform * vec4(position, 1.0);

    //ex_color = in_color;
    ex_texcoord = in_texcoord;
    ex_normal = mat3(transpose(inverse(transform))) * in_normal;
}