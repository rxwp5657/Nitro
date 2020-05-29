#version 410 core

in VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TextCoord;
    vec4 FragPosL;
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

uniform vec4 light_pos;   
uniform vec4 light_dir;     
uniform vec4 light_color;
uniform float cutoff;
uniform float max_distance;
uniform float umbra;
uniform float penumbra;
uniform bool cast_shadow;

uniform sampler2D shadow_map;

float shadow(vec4 frag_pos, float bias)
{
    vec3 coord = frag_pos.xyz / frag_pos.w;
    coord = coord * 0.5 + 0.5;

    float light_depth   = texture(shadow_map, coord.xy).r;
    float current_depht = coord.z;

    return current_depht - bias > light_depth ? 1.0 : 0.0;
}   

// Distance attenuation/falloff
float fdist(float r, float range)
{
    return pow(max(1 - pow(r / range, 4), 0.0), 2);
}

// Direactional attenuation/falloff
float fdir(vec3 L)
{
    float s = dot(normalize(light_dir.xyz), -L);
    float u = umbra;
    float p = penumbra;
    float t = clamp((s - u) / (p - u) , 0.0, 1.0);
    return pow(t,2);
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
    
    float shadow_val = (cast_shadow) ? shadow(fs_in.FragPosL, 0.005) : 0.0;

    return vec4(ambient + (1.0 - shadow_val) * (diffuse + specular), 1.0);
}

void main()
{
    vec4 result_color = vec4(0.0);

    vec3  d = vec3(light_pos) - fs_in.FragPos;
    float r = sqrt(dot(d,d)); 

    vec3 l_color   = vec3(light_color) * fdist(r, max_distance) * fdir(d / r);
    result_color += blinn(fs_in.FragPos, fs_in.Normal, viewPos.xyz, d / r, l_color);

    FragColor = result_color;
}