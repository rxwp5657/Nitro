#version 410 core

in VS_OUT
{
    vec2 TextCoord;
    vec3 FragPos;
    vec3 TangentFragPos;
    vec3 ViewPos;
    vec3 LightPos;
} fs_in;

out vec4 outColor;

struct PointLight
{
    vec4  position;
    vec4  color;
    float range;
};

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;

layout(std140) uniform Lights {
   PointLight lights[1];
};

float attenuation(float r, float range)
{
    return pow(max(1 - pow(r / range, 4), 0.0), 2);
}

vec2 ParalaxMapping(vec2 text_coords, vec3 view_dir)
{
    float height =  texture(texture_height1, text_coords).r;    
    vec2 p = view_dir.xy / view_dir.z * (height * 0.1);
    return text_coords - p;    
}   

void main()
{
    PointLight light = lights[0];
    
    vec3  d = vec3(light.position) - fs_in.FragPos;
    float r = sqrt(dot(d,d));

    vec3 V = normalize(fs_in.ViewPos  - fs_in.TangentFragPos);
    vec2 text_coords = ParalaxMapping(fs_in.TextCoord, V);

    vec3 light_color    = vec3(light.color) * attenuation(r, light.range);
    vec3 diffuse_color  = texture(texture_diffuse1,  text_coords).rgb;
    vec3 specular_color = texture(texture_specular1, text_coords).rgb;
    
    vec3 N = normalize(texture(texture_normal1, text_coords).rgb * 2.0 - 1.0);;
    vec3 L = normalize(fs_in.LightPos - fs_in.TangentFragPos);
    vec3 H = normalize(L + V);

    vec3 diffuse  = light_color * diffuse_color  * max(dot(L,N), 0.0);
    vec3 specular = light_color * specular_color * pow(max(dot(V, H), 0.0), 50.0);
    vec3 ambient  = 0.1 * diffuse_color;
    
    outColor = vec4(ambient + diffuse + specular, 1.0);
}