#version 450 core

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D Texture0;

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

    if(fragColor.x != 0.0)
    {
        return;
    }

    vec2 fragCoor;
    fragCoor.x = float(texelCoord.x)/(gl_NumWorkGroups.x);
    fragCoor.y = float(texelCoord.y)/(gl_NumWorkGroups.y);

    vec2 coord = fragCoor*3 - vec2(2,1.5);
    vec2 z = vec2(0,0);
    vec2 c = coord;
    
    for (int i=0; i<10; i++) 
    {
        z = complexSquare(z) + c;
        if (length(z) < 4) 
        {
            vec2 newCoord = ((z + vec2(2.,1.5)) / 3.);
            ivec2 newTexelCoord;
            newTexelCoord.x = int(gl_NumWorkGroups.x * newCoord.x);
            newTexelCoord.y = int(gl_NumWorkGroups.y * newCoord.y);

            fragColor = imageLoad(Texture0, newTexelCoord);
            fragColor += vec4(0.1, 0.1, 0.1, 1);
            imageStore(Texture0, newTexelCoord, fragColor);
        }
        else
        {
            break;
        }
    }
}