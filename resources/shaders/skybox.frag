#version 410 core

in  vec3 TexCoords;
out vec4 FragColor;

uniform samplerCube skybox1;

void main()
{    
    FragColor = texture(skybox1, TexCoords);
}