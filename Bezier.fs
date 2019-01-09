#version 330
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
void main(){
	vec4 texColor = texture(texture1,TexCoord);
	//FragColor=vec4(1.0,1.0,1.0,1.0);
	FragColor=texColor;
}