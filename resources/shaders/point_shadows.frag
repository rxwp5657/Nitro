#version 410 core

in vec4 FragPos;

uniform vec4 light_pos;

void main()
{
    vec4 L = FragPos - light_pos;
    gl_FragDepth = dot(L, L);
}  