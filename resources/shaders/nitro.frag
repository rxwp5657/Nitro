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

void main()
{
    PointLight light = lights[0];
    vec3 diffuse_color  = texture(texture_diffuse1,  fTexCoord).rgb;
    vec3 specular_color = texture(texture_specular1, fTexCoord).rgb;
    
    vec3 N = normalize(Normal);
    vec3 V = normalize(vec3(view_pos)  - FragPos);
    vec3 L = normalize(vec3(light.position) - FragPos);
    vec3 H = reflect(-L, N);

    vec3 diffuse  = max(dot(L,N), 0.0) * diffuse_color;
    vec3 specular = vec3(light.color) * pow(max(dot(V, H), 0.0), 2.0) * specular_color;
    vec3 ambient  = 0.05 * diffuse_color;
    
    outColor = vec4(ambient + diffuse + specular, 1.0);
}