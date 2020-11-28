#version 330 core

layout(location = 0) out vec4 colour;

in vec2 v_TexCoord;
  
uniform sampler2D u_Albedo;
uniform sampler2D u_Normal;
uniform sampler2D u_Roughness;
uniform sampler2D u_Metallic;

void main()
{
    colour = texture(u_Albedo, v_TexCoord);
}   