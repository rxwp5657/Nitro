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

uniform vec4  uLightPos;
uniform vec4  uLightColor;
uniform float uMaxDistance;
uniform bool  uCastsShadow;
uniform float uFarPlane;
uniform float uNearPlane;

uniform mat4 uDPView;

uniform sampler2D shadow_map_front;
uniform sampler2D shadow_map_rear;

float line_step(float min, float max, float value)
{
    return clamp((value - min) / (max - min), 0.0, 1.0);
}

float reduce_bleeding(float p_max, float amount) 
{   
    // Remove the [0, Amount] tail and linearly rescale (Amount, 1].    
    return line_step(amount, 1, p_max); 
} 

vec4 paraboloid(vec4 frag_pos, float dir)
{
    vec4 result = frag_pos;

    result /= result.w;

    float len = length(result.xyz);
    result   /= len;
    
    result.z = (result.z >= 0.0) ? (1.0 + result.z) : (1.0 - result.z);

    result.x /= result.z;
    result.y /= result.z;

    result.z = (len - uNearPlane) / (uMaxDistance - uNearPlane);

    result.w = 1.0;

    return result;
}

float VSM(vec2 moments, float depht)
{
    float m1       = moments.x;
    float m2       = moments.y;
    float variance = m2 - m1 * m1; 
    float diff     = depht - m1;
    float p_max    = variance / (variance + diff * diff);

    return depht < moments.x ? 1.0 : reduce_bleeding(p_max, 0.4);
}   

float shadows(vec4 frag_pos)
{
    frag_pos     = uDPView * frag_pos;

    float alpha  = 0.5 + frag_pos.z / uMaxDistance;

    vec4 front_pos   = paraboloid(frag_pos, 1.0);
    vec4 rear_pos    = paraboloid(frag_pos,-1.0);

    vec2 front_coods = 0.5 + 0.5 * front_pos.xy;
    vec2 rear_coods  = 0.5 + 0.5 * rear_pos.xy;

    vec2  moments = (alpha >= 0.5) ? texture(shadow_map_front, front_coods).rg : texture(shadow_map_rear, rear_coods).rg;
    float depht   = (alpha >= 0.5) ? front_pos.z : rear_pos.z;
    
    return VSM(moments, depht);
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

    float shadow = uCastsShadow ? shadows(vec4(fs_in.FragPos, 1.0)) : 0.0; 

    return vec4(ambient + (shadow) * (diffuse + specular),1.0);
}

void main()
{
    vec4 result_color = vec4(0.0);

    vec3  d = vec3(uLightPos) - fs_in.FragPos;
    float r = sqrt(dot(d,d)); 

    vec3 light_color   = vec3(uLightColor) * fdist(r, uMaxDistance);

    result_color += blinn(fs_in.FragPos, fs_in.Normal, viewPos.xyz, d / r, light_color);

    FragColor = result_color;
}