#version 330 core

in vec2 fTexCoord;

uniform sampler2D albedo;
uniform sampler2D normal;

out vec4 outColor;

void main()
{
    outColor = mix(texture(albedo, fTexCoord), texture(normal, fTexCoord), 0.2);
}