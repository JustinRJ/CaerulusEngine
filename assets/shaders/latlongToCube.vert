#version 460 core

layout (location = 0) in vec3 inPosition;

out vec3 WorldPos;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    WorldPos = inPosition;  

    gl_Position =  projection * view * vec4(WorldPos, 1.0f);
}
