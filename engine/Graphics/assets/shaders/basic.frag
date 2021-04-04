#version 330 core

layout(location = 0) out vec4 colour;

in vec2 v_TexCoord;
in vec3 v_Tangent;
in vec3 v_BiTangent;
  
uniform sampler2D u_Albedo;
uniform sampler2D u_Normal;
uniform sampler2D u_Roughness;
uniform sampler2D u_Metallic;

void main()
{
    colour = texture(u_Albedo, v_TexCoord);

    //vec3 normal = normalize(texture( u_Normal, v_TexCoord ).rgb * 2.0 - 1.0);
    //colour = vec4(normal * 0.5 + 0.5, 1.0);
}   