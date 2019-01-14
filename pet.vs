#version 330 core

layout (location = 0 ) in vec3 aPos;

out vec4 vertexColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform bool isDrawBezierSurface;

void main(){
	
	float z = aPos.z;

	//颜色值计算
	vec3 color;
	if(z > 65025)
	{
		color = vec3(1,0,0);
	}
	else if(z > 255)
	{
		color = vec3(0,1,0);
	}
	else{
		color = vec3(0,0,1);
	}

	//高度计算
	float height = 0;
	if(z > 65025)
	{
		float tmp = z/65025;
		z = z - tmp*65025;

		height += (tmp/255);
	}
    if(z > 255)
	{
		float tmp = z/255;
		z = z - tmp*255;

		height += (tmp/255)*0.5;
	}

	height += (z/255)*0.01f;

	if(aPos.z>20000)
		vertexColor = vec4(color,1.0);
	else{
		vertexColor = vec4(0,0,0,1.0);
	}

	if(isDrawBezierSurface)
	{
		vec3 pos=vec3(aPos.x,aPos.y,height);
		gl_Position=projection*view*model*vec4(pos,1.0);	
	}
	else
	{
		gl_Position=projection*view*model*vec4(aPos.x,aPos.y,0.0,1.0);
	}

	

}