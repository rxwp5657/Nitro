#version 410 core

in vec2 fTexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 outColor;

struct PointLight
{
    vec4  position;
    vec4  color;
    float range;
};

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform vec4 view_pos; 

layout(std140) uniform Lights {
   PointLight lights[1];
};

float attenuation(float r, float range)
{
    return pow(max(1 - pow(r / range, 4), 0.0), 2);
}

void main()
{
    PointLight light = lights[0];
    
    vec3  d = vec3(light.position) - FragPos;
    float r = sqrt(dot(d,d));

    vec3 light_color    = vec3(light.color) * attenuation(r, light.range);
    vec3 diffuse_color  = texture(texture_diffuse1,  fTexCoord).rgb;
    vec3 specular_color = texture(texture_specular1, fTexCoord).rgb;
    
    vec3 N = normalize(Normal);
    vec3 V = normalize(vec3(view_pos)  - FragPos);
    vec3 L = d / r;
    vec3 H = normalize(L + V);

    vec3 diffuse  = light_color * diffuse_color  * max(dot(L,N), 0.0);
    vec3 specular = light_color * specular_color * pow(max(dot(V, H), 0.0), 50.0);
    vec3 ambient  = 0.1 * diffuse_color;
    
    outColor = vec4(ambient + diffuse + specular, 1.0);
}