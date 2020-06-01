#version 410 core

in vec3 aPosition;

uniform mat4 uLightTransform;
uniform mat4 uModel;

void main()
{
    gl_Position = uLightTransform * uModel * vec4(aPosition, 1.0);
}  