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
    float cutoff;
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

uniform vec3  mat_diffuse;
uniform vec3  mat_specular;
uniform vec3  mat_ambient;
uniform float mat_shininess;
uniform float mat_reflectiveness;
uniform float mat_refractiveness;

uniform sampler2D   texture_diffuse1;
uniform sampler2D   texture_specular1;
uniform samplerCube skybox1;

uniform int  has_textures;
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

// Distance attenuation/falloff
float fdist(float r, float range)
{
    return pow(max(1 - pow(r / range, 4), 0.0), 2);
}

// Direactional attenuation/falloff
float fdir(SpotLight spot_light, vec3 L)
{
    float s = dot(normalize(spot_light.direction.xyz), -L);
    float u = spot_light.umbra;
    float p = spot_light.penumbra;
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

    return vec4(ambient + diffuse + specular,1.0);
}

void main()
{
    vec4 result_color = vec4(0.0);

    for(int i = 0; i < num_point; i++)
    {
        PointLight light = point_lights[i];
        vec3  d = vec3(light.position) - fs_in.FragPos;
        float r = sqrt(dot(d,d)); 

        vec3 light_color   = vec3(light.color) * fdist(r, light.range);

        result_color += blinn(fs_in.FragPos, fs_in.Normal, viewPos.xyz, d / r, light_color);
    }

    for(int i = 0; i < num_spot; i++)
    {
        SpotLight light = spot_lights[i];
        vec3  d = vec3(light.position) - fs_in.FragPos;
        float r = sqrt(dot(d,d)); 

        vec3 light_color   = vec3(light.color) * fdist(r, light.range) * fdir(light, d / r);
        result_color += blinn(fs_in.FragPos, fs_in.Normal, viewPos.xyz, d / r, light_color);
    }
    
    for(int i = 0; i < num_dir; i++)
    {
        DirectionalLight light = dir_lights[i];
        result_color += blinn(fs_in.FragPos, fs_in.Normal, viewPos.xyz, normalize(-light.direction.xyz), vec3(light.color));
    }

    FragColor = result_color;
}