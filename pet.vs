#version 330 core

layout (location = 0 ) in vec3 aPos;

out vec4 vertexColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform bool isDrawBezierSurface;

void main(){
	
	vec3 pos=vec3(aPos.x,aPos.y,aPos.z/1000);
	float z = aPos.z;
	if(isDrawBezierSurface)
	{
		gl_Position=projection*view*model vec4(pos,1.0);	
	}
	else
	{
		gl_Position=projection*view*model vec4(pos.x,pos.y,0.0,1.0);
	}

	float r=0,g=0,b=0;
	if(z > 65025)
	{
		float tmp = z/65025;
		r = tmp;
		z = z - (r * 65025);
	}
	if(z > 255)
	{
		float tmp = z/255;
		g = tmp;
		z = z - (g * 255);
	}

	b = z;

	vertexColor = vec4(r/255,g/255,b/255,1.0);

	//vertexColor = vec4(pos.z,pos.z,pos.z,1.0);
}