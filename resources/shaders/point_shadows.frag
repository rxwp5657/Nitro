#version 410 core

in vec4 FragPos;

uniform vec4  uLightPos;
uniform float uFarPlane;

void main()
{
    float light_dist = length(FragPos - uLightPos);
    light_dist       = light_dist / uFarPlane;
    gl_FragDepth     = light_dist;
}

/*
uniform vec4 uLightPos;

void main()
{
    vec4 L = FragPos - uLightPos;
    gl_FragDepth = dot(L, L);
}  
*/