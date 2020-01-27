#version 330 core

in vec2 fTexCoord;

uniform sampler2D texture_diffuse1;
uniform sampler2D normal;

out vec4 outColor;

void main()
{
    outColor = texture(texture_diffuse1, fTexCoord);
}