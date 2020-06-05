#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D image;
uniform bool vertical;


void main()
{             
    float kernel[5] = float[](0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);
    
    //texel size
    vec2 texel_size = 1.0 / textureSize(image, 0);
    vec3 result = texture(image, TexCoords).rgb * kernel[0];
    vec2 offset = vertical ? vec2(0, 1) : vec2(1, 0);
    
    for(int i = 1; i < 5; i++)
    {
        result += texture(image, TexCoords + offset * i * texel_size).rgb * kernel[i];
        result += texture(image, TexCoords - offset * i * texel_size).rgb * kernel[i];
    }

    FragColor = vec4(result, 1.0);
}