#version 410 core

in  vec3 TexCoords;
out vec4 FragColor;

uniform samplerCube texture_diffuse1;

void main()
{    
    FragColor = texture(texture_diffuse1, TexCoords);
}