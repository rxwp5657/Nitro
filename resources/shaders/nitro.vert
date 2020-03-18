#version 410 core

const int POINT_LIGHTS = 2;
const int SPOT_LIGHTS  = 2;
const int DIRECTIONAL_LIGHTS = 2;

in vec3 aPosition;
in vec3 aNormal;
in vec2 aTexCoord;
in vec3 aTangent;

out VS_OUT
{
    vec2 TextCoord;
    vec3 FragPos;
    vec3 TangentFragPos;
    vec3 ViewPos;
    vec3 LightPos;
} vs_out;

uniform vec4 viewPos; 
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;
uniform mat4 uNormalMat;

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

void main()
{
    gl_Position = uProj * uView * uModel * vec4(aPosition,1.0);
    
    vec3 N   = normalize(mat3(uNormalMat) * aNormal);
    vec3 T   = normalize(mat3(uNormalMat) * aTangent);
    T        = normalize(T - dot(T , N) * N);
    vec3 B   = cross(N, T);
    mat3 TBN = transpose(mat3(T, B, N));

    vs_out.TextCoord = aTexCoord;
    vs_out.FragPos  = vec3(uModel * vec4(aPosition, 1.0));
    vs_out.ViewPos  = TBN * vec3(viewPos);
    vs_out.LightPos = TBN * vec3(point_lights[0].position);
    vs_out.TangentFragPos = TBN * vs_out.FragPos; 
}