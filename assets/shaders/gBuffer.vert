#version 460 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoords;

out vec3 ViewPos;
out vec2 TexCoords;
out vec3 Normal;
out vec4 FragPosition;
out vec4 FragPrevPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 projViewModel;
uniform mat4 projViewModelPrev;

void main()
{
    TexCoords = inTexCoords;

    // View Space
    vec4 viewFragPos = view * model * vec4(inPosition, 1.0f);
    ViewPos = viewFragPos.xyz;

    mat3 normalMatrix = transpose(inverse(mat3(view * model)));
    Normal = normalMatrix * inNormal;

    FragPosition = projViewModel * vec4(inPosition, 1.0f);
    FragPrevPosition = projViewModelPrev * vec4(inPosition, 1.0f);

    gl_Position = projection * viewFragPos;
}
