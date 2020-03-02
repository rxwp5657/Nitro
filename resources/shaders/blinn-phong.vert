#version 410 core

in vec3 aPosition;
in vec3 aNormal;
in vec2 aTexCoord;

out vec2 fTexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;
uniform mat4 uNormalMat;

void main()
{
    gl_Position = uProj * uView * uModel * vec4(aPosition,1.0); 
    fTexCoord = aTexCoord;
    FragPos   = vec3(uModel * vec4(aPosition, 1.0));
    Normal    = mat3(uNormalMat) * aNormal;
}