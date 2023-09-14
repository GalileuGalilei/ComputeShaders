#version 450 core


out vec4 FragColor;
//uniform float time; //temporariamente inutil
in vec2 TexCood;
uniform sampler2D Texture0;

void main()
{
	FragColor = texture(Texture0, TexCood);
	//FragColor = vec4(1.0, 1.0, 0.0, 1.0);
}