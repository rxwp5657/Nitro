#version 330 core

in vec2 fTexCoord;

uniform sampler2D texture_diffuse1;

out vec4 outColor;

void main()
{
    outColor = texture(texture_diffuse1, fTexCoord);
    //outColor = vec4(1.0, 1.0, 1.0, 1.0);
}