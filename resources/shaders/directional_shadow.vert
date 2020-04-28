#version 410 core

in vec3 aPosition;

uniform mat4 light_transform;
uniform mat4 uModel;

void main()
{
    gl_Position = light_transform * uModel * vec4(aPosition, 1.0);
}  