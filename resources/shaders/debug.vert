#version 410 core
in vec3 aPosition;
in vec2 aTexCoord;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoord;
    gl_Position = vec4(aPosition, 1.0);
}