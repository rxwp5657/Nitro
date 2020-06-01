#version 410 core

in vec3 aPosition;
in vec3 aNormal;
in vec2 aTexCoord;
in vec3 aTangent;
in vec3 aBitangent;

out VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TextCoord;
    vec4 FragPosL;
} vs_out;

uniform mat4 uModel;
uniform mat4 uNormalMat;
uniform mat4 uView;
uniform mat4 uProj;

uniform mat4 uLightTransform;

void main()
{
    gl_Position      = uProj * uView * uModel * vec4(aPosition, 1.0);
    vs_out.FragPos   = vec3(uModel * vec4(aPosition, 1.0));
    vs_out.Normal    = mat3(uNormalMat) * aNormal;
    vs_out.TextCoord = aTexCoord;
    vs_out.FragPosL  = uLightTransform * vec4(vs_out.FragPos, 1.0);
}