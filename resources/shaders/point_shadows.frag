#version 410 core

in vec4 FragPos;

uniform vec4  light_pos;
uniform float far_plane;
void main()
{
    float light_dist = length(FragPos - light_pos);
    light_dist       = light_dist / far_plane;
    gl_FragDepth     = light_dist;
}

/*
uniform vec4 light_pos;

void main()
{
    vec4 L = FragPos - light_pos;
    gl_FragDepth = dot(L, L);
}  
*/