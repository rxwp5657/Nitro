#version 410 core

const int POINT_LIGHTS = 2;
const int SPOT_LIGHTS  = 2;
const int DIRECTIONAL_LIGHTS = 2;

struct PointLight
{
    vec4  position;
    vec4  color;
    float range;
};

struct SpotLight
{
    vec4  position;
    vec4  direction;
    vec4  color;
    float range;
    float umbra;
    float penumbra;
};

struct DirectionalLight
{
    vec4 direction;
    vec4 color;
};

in VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TextCoord;
} fs_in;

out vec4 FragColor;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform vec4 viewPos;

layout(std140) uniform Num_Lights 
{
   int num_point; 
   int num_spot;
   int num_dir;
};

layout(std140) uniform Lights 
{
   PointLight       point_lights[POINT_LIGHTS];
   SpotLight        spot_lights[SPOT_LIGHTS];
   DirectionalLight dir_lights[DIRECTIONAL_LIGHTS];
};

float attenuation(float r, float range)
{
    return pow(max(1 - pow(r / range, 4), 0.0), 2);
}

void main()
{
    PointLight light = point_lights[0];
    
    vec3  d = vec3(light.position) - fs_in.FragPos;
    float r = sqrt(dot(d,d)); 

    vec3 light_color   = vec3(light.color) * attenuation(r, light.range);
    vec3 surface_color = texture(texture_diffuse1,  fs_in.TextCoord).rgb;
    vec3 surface_spec  = texture(texture_specular1, fs_in.TextCoord).rgb;

    vec3 N = normalize(fs_in.Normal);
    vec3 V = normalize(viewPos.xyz - fs_in.FragPos);
    vec3 L = normalize(light.position.xyz - fs_in.FragPos);
    vec3 R = reflect(-L, N);

    vec3 ambient  = 0.1 * surface_color;
    vec3 diffuse  = surface_color * light_color * max(0.0, dot(N,L));
    vec3 specular = surface_spec  * light_color * pow(max(0.0, dot(R, V)), 50) * 0.9;

    FragColor = vec4(ambient + diffuse + specular,1.0);
}