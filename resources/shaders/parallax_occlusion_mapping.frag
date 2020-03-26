#version 410 core

const int POINT_LIGHTS = 2;
const int SPOT_LIGHTS  = 2;
const int DIRECTIONAL_LIGHTS = 2;

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

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;

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

/*
    Parallax mapping is a technique that boost surface detail by making 
    bumps appear to have height as the viewer moves. To archive this, 
    Parallax mapping actually tries to find the point of intersection
    of the view ray and the displaced surface described by a height or
    depth map. (This point will be named ß). 

    Now, plain parallax mapping approximates this itersection ß
    by sampling the height value of a shifted texture coordinate
    (u', v'). For instance, in order to calculate this shifted 
    texture coordinate the original coordinate (u,v) must
    be shifted by some offset ƒ. Thus, this displaced texture
    coordinate is calculated as follows:

    h = texture(height / depth map, (u, v))

    ƒ = view_dir.xy * h / view_dir.z  
    
    (u', v') = f + (u, v)

    However, this method fall apart at shallow view angles. To ameliorate
    this problem, Welsh introduced the idea of offset limiting by eliminating
    the division by the view_dir.z component. Hence, giving the new formula:

    h = texture(height / depth map, (u, v))

    ƒ = view_dir.xy * h
    
    (u', v') = f + (u, v)
    
*/          

vec2 ParalaxMapping(vec2 text_coords, vec3 view_dir)
{
    float height = 1.0 - texture(texture_height1, text_coords).r; // convert height map to depthmap    
    vec2 p = view_dir.xy / view_dir.z * (height * 0.1); // offset ƒ
    return text_coords - p; // shifted texture coordinate (u',v')
}   

/*
    Since plain Parallax mapping produces undesired artifacts because
    it barely approximates the intersection point ß, some other techniques
    have been invented, mainly:

    - Steep Parallax Mapping
    - Relief Parallax Mapping
    - Parallax Occlusion Mapping

    First, Steep Parallax Mapping defines two bounds: the upper bound 
    (1.0 if working with height maps and 0.0 if working with dephtmaps)
    and the lower bound (0.0 if working with height maps and 1.0 if 
    working with dephtmaps). Then, it uses a ray marching algorithm
    for moving the ray casted by the view direction from the upper bound 
    to the lower bound (it actually moves the text coords in direction of 
    the view vector) util the ray enters the volume generated by 
    the height and the surface (bump area). Finally, it returns the new found
    text coord.

    Second, Relief Parallax Mapping actually follows the sample principle
    of Steep Parallax Mapping but, it differs on that it uses ray marching
    until it finds the first point inside the volume. Then, it also takes the 
    previously tested point that was outside the volume and uses the binary 
    root finding algoritm to better approximate the intersecting point ß.

    Finally, because the binary root finding algorithm is quite "expensive"
    to execute a good and cheap approximation can be made using Parallax Occlusion 
    Mapping. This technique follows the same principle of Relief Parallax Mapping
    but, instead of doing the binary root finding algorithm once the inner point
    of the volume and the previous outer point is found, it linearly interpolates 
    both points (actually texture cordinates) to find a pretty good approximation 
    of point ß.

    Also, it is important to notice two things:

    - This algorithms depend on the step size, this is, when doing ray marching
    the march/step must be small enough to get as close as the point ß (so it 
    doesn't get too far from ß thus, having aliasing problems) and big enough 
    to reach to the desired location with a small number of iterations.

    - To actually know if a point is inside the volume the comparisson goes as
    follows:
        + When using height maps, we expect the current_height_value
          to be less than the height value sampled at the current_text_cood.
          
        + When using depht maps, we expect the current_depht_value
          to be greater than the height value sampled at the current_text_cood.
         

*/

vec2 ParallaxOcclusionMapping(vec2 text_coords, vec3 view_dir)
{
    /*
        Define possible step sizes for the ray marching algorithm 
        based on the view direction. The more perpendicular the 
        view direction is, the lower the size of the step will be
        since at more perpendicual angles the effect of Parallax is not
        really visible.
    */
    const float min_steps = 8;
    const float max_steps = 32;
    float num_steps = mix(max_steps, min_steps, abs(dot(vec3(0.0, 0.0, 1.0), view_dir)));  

    float step_size = 1.0 / num_steps;

    /*
        Define the starting point for ray marching and the variable
        that will define the current "height" of the view ray.

        Use this variable if using depht map:

        float current_depht = 0.0; 
    */
    
    float current_height = 1.0;

    /*
        Actually we want to descend from the view dir to the fragment position, 
        the 0.1 value is an extra scale parameter used to control the parallax 
        effect. (This is used when using height maps)
    */

    // vec2  P = view_dir.xy * 0.1; //uncomment if using dephtmaps

    vec2  P = (-view_dir.xy) * 0.1; 
    
    // The actual step value that will be used to shift texture coords. 

    vec2  delta_text_coords = P / num_steps;

    vec2  current_text_coords = text_coords;
    float current_height_map = texture(texture_height1, current_text_coords).r;

    /*
        Check if the current texture coordinate is inside the volume of the bump. (Height maps) 
        
        Use this comparisson if ussing depht map:
        
        current_depht < current_depht_map
    */
    while(current_height > current_height_map)
    {
        // If not inside the volume, shift the current texture in direction
        // of the view_dir vector by delta_text_coords amounts.

        current_text_coords += delta_text_coords;
        // current_text_coords -= delta_text_coords; (uncomment  if using dephtmaps)

        // Sample new height.
        current_height_map = texture(texture_height1, current_text_coords).r;

        // move the ray position lower.
        current_height -= step_size;
        // current_depht += step_size; (uncomment  if using dephtmaps)
    }

    // calculate the previous position before getting into the volume.
    vec2 previous_text_coords = current_text_coords - delta_text_coords;
    
    // get the distance between the sampled height and current height
    float after_height  = current_height - current_height_map;
    // float after_depht  = current_depht_map - current_depht; (uncomment  if using dephtmaps)

    // get the distance between the previous point sampled height and previous height
    float before_height = current_height - texture(texture_height1, previous_text_coords).r + step_size;
    // float before_depht = texture(texture_height1, previous_text_coords).r - current_depht + step_size;
    // (uncomment  if using dephtmaps)

    // Linearly interpolate actual and previous texture coord values
    float weight = after_height / (after_height - before_height);
    vec2  result  = previous_text_coords * weight + current_text_coords * (1.0 - weight);
    return result;
}

void main()
{
    PointLight light = point_lights[0];
    
    vec3  d = vec3(light.position) - fs_in.FragPos;
    float r = sqrt(dot(d,d));

    vec3 V = normalize(fs_in.ViewPos  - fs_in.TangentFragPos);
    vec2 text_coords = ParallaxOcclusionMapping(fs_in.TextCoord, V);
    
    if(text_coords.y > 1.0 || text_coords.y < 0.0)
        discard;

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