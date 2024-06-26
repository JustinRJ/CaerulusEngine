#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 biTangent;

out vec2 v_TexCoord;
out vec3 v_Tangent;
out vec3 v_BiTangent;
  
uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * vec4(position, 1.);
    v_TexCoord = texCoord;
    v_Tangent = tangent;
    v_BiTangent = biTangent;
}