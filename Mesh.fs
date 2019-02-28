#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform int selectedPrimID;

void main()
{   
	vec4 color = vec4(1,0,0,1);
	FragColor = color;
}

