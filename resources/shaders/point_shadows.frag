#version 410 core

out vec4 FragColor;

in VS_OUT
{
    float Depht;
    float Alpha;
} fs_in;

void main()
{
    if(fs_in.Alpha < 0.5) discard;

    FragColor = vec4(fs_in.Depht, fs_in.Depht * fs_in.Depht, 0.0, 0.0);
}