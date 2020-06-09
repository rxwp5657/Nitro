#version 410 core

in vec3 aPosition;

out VS_OUT
{
    float Depht;
    float Alpha;
} vs_out;

uniform mat4  uModel;
uniform mat4  uView;
uniform float uFar;
uniform float uNear;
uniform float uDir;

void main()
{
    vec4 result = uView * uModel * vec4(aPosition, 1.0);

    result.z *= uDir;
    
    result /= result.w;

    float len = length(result.xyz);
    result   /= len;
    
    vs_out.Alpha = 0.5 + (result.z / uFar);

    result.z += 1.0;

    result.x /= result.z;
    result.y /= result.z;

    result.z = (len - uNear) / (uFar - uNear);

    result.w = 1.0;

    vs_out.Depht = result.z;

    gl_Position = result;  
}  