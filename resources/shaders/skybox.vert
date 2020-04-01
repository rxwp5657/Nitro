#version 410 core

in  vec3 aPosition;
out vec3 TexCoords = aPosition;

uniform mat4 uView;
uniform mat4 uProj;

void main()
{
    TexCoords = aPosition;
    vec4 pos = uProj * mat4(mat3(uView)) * vec4(aPosition, 1.0);
    gl_Position = pos.xyww;
}  