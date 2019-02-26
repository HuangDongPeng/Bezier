

#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform bool isSelected;

void main()
{    
	if(isSelected)
		FragColor = vec4(1,1,1,1);
	else
		FragColor = texture(texture_diffuse1, TexCoords);
}

