#version 410 core

in VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TextCoord;
} fs_in;

out vec4 FragColor;

uniform vec3  mat_diffuse;
uniform vec3  mat_specular;
uniform vec3  mat_ambient;
uniform float mat_shininess;
uniform float mat_reflectiveness;
uniform float mat_refractiveness;

uniform samplerCube skybox1;
uniform sampler2D   texture_diffuse1;
uniform sampler2D   texture_specular1;

uniform int  has_textures;
uniform vec4 viewPos;

uniform vec4  light_pos;
uniform vec4  light_color;
uniform float max_distance;
uniform bool  cast_shadow;
uniform sampler3D shadow_map0;

float shadows(float squared_dist, vec3 L)
{
    float bias  = 0.005;
    float shadow_sample = texture(shadow_map0, -L).r;
    float depht  = (squared_dist * bias) - shadow_sample;
    float shadow = (depht - shadow_sample < 0.0f) ? 1.0f : 0.0f;

    return shadow;
}

// Distance attenuation/falloff
float fdist(float r, float range)
{
    return pow(max(1 - pow(r / range, 4), 0.0), 2);
}

vec4 blinn(vec3 FragPos, vec3 Normal, vec3 View, vec3 L, vec3 light_color)
{

    vec3 surface_color = has_textures != 0 ? texture(texture_diffuse1,  fs_in.TextCoord).rgb : mat_diffuse;
    vec3 surface_spec  = has_textures != 0 ? texture(texture_specular1, fs_in.TextCoord).rgb : mat_specular;

    vec3 N = normalize(Normal);
    vec3 V = normalize(View - FragPos);
    vec3 R = reflect(-L, N);

    vec3 ambient  = 0.1 * surface_color;
    vec3 diffuse  = surface_color * light_color * max(0.0, dot(N,L));
    vec3 specular = surface_spec  * light_color * pow(max(0.0, dot(R, V)), 50) * 0.9;

    float shadow = cast_shadow ? shadows(dot(L,L), L) : 0.0; 

    return vec4((ambient + (1.0 - shadow) * (diffuse + specular)) * surface_color, 1.0);
}

void main()
{
    vec4 result_color = vec4(0.0);

    vec3  d = vec3(light_pos) - fs_in.FragPos;
    float r = sqrt(dot(d,d)); 

    vec3 light_color   = vec3(light_color) * fdist(r, max_distance);

    result_color += blinn(fs_in.FragPos, fs_in.Normal, viewPos.xyz, d / r, light_color);

    FragColor = result_color;
}