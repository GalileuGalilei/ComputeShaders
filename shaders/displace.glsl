#version 450 core

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D Texture0;
uniform float iteration;

vec2 complexSquare(vec2 z) {
    vec2 z2 = vec2(0);
    z2.x = z.x*z.x - z.y*z.y;
    z2.y = 2.*(z.x*z.y);
    return z2;
}

void main() 
{  
    ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);
    vec4 fragColor = imageLoad(Texture0, texelCoord);

    vec2 fragCoor;
    fragCoor.x = float(texelCoord.x)/(gl_NumWorkGroups.x);
    fragCoor.y = float(texelCoord.y)/(gl_NumWorkGroups.y);

    float zoom = 4.0;
    vec2 coord = fragCoor*zoom - vec2(zoom/2.,zoom/2.);

    vec2 z = vec2(0,0);
    vec2 c = coord;
    float step = 0.0001;

    if(iteration < step)
    {
        bool isMandelbroth = true;
        int i;
        for (i=0; i < 200; i++) 
        {
            z = complexSquare(z) + c;
            if (length(z) > 100) 
            {
                isMandelbroth = false;
                break;
            }
        }

        if(!isMandelbroth)
        {
            imageStore(Texture0, texelCoord, vec4(float(i) / 200., 0, c.x, c.y));
        }
    }
}