#version 330 core

in vec3 aPosition;
in vec3 aNormal;
in vec2 aTexCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out vec2 fTexCoord;

void main()
{
    gl_Position = uProj * uView * uModel * vec4(aPosition,1.0);
    fTexCoord = aTexCoord;
}