#version 460 core

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gAlbedo;
layout (location = 2) out vec4 gNormal;
layout (location = 3) out vec3 gEffects;

in vec3 ViewPos;
in vec2 TexCoords;
in vec3 Normal;
in vec4 FragPosition;
in vec4 FragPrevPosition;

uniform float nearPlane;
uniform float farPlane;
uniform sampler2D texAlbedo;
uniform sampler2D texNormal;
uniform sampler2D texRoughness;
uniform sampler2D texMetalness;
uniform sampler2D texAO;

float LinearizeDepth(float depth);
vec3 ComputeTexureNormal(vec3 viewNormal, vec3 texNormal);

void main()
{
    vec3 texNormal = normalize(texture(texNormal, TexCoords).rgb * 2.0f - 1.0f);
    // In case the normal map was made with DX3D coordinates system in mind
    texNormal.g = -texNormal.g;   

    vec2 fragPosA = (FragPosition.xy / FragPosition.w) * 0.5f + 0.5f;
    vec2 fragPosB = (FragPrevPosition.xy / FragPrevPosition.w) * 0.5f + 0.5f;

    gPosition = vec4(ViewPos, LinearizeDepth(gl_FragCoord.z));

    gAlbedo.rgb = vec3(texture(texAlbedo, TexCoords));
    gAlbedo.a =  vec3(texture(texRoughness, TexCoords)).r;

    gNormal.rgb = ComputeTexureNormal(Normal, texNormal);
    gNormal.a = vec3(texture(texMetalness, TexCoords)).r;

    gEffects.r = vec3(texture(texAO, TexCoords)).r;
    gEffects.gb = fragPosA - fragPosB;
}

float LinearizeDepth(float depth)
{
    float z = depth * 2.0f - 1.0f;
    return (2.0f * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));
}

vec3 ComputeTexureNormal(vec3 viewNormal, vec3 texNormal)
{
    vec3 dPosX  = dFdx(ViewPos);
    vec3 dPosY  = dFdy(ViewPos);
    vec2 dTexX = dFdx(TexCoords);
    vec2 dTexY = dFdy(TexCoords);

    vec3 normal = normalize(viewNormal);
    vec3 tangent = normalize(dPosX * dTexY.t - dPosY * dTexX.t);
    vec3 binormal = -normalize(cross(normal, tangent));
    mat3 TBN = mat3(tangent, binormal, normal);

    return normalize(TBN * texNormal);
}
