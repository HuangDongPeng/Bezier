#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform int selectedPrimID;

void main()
{   
	vec4 color = vec4(1,0,0,1);
	if(selectedPrimID!=0){
		//selectedPrimID-=1;
		int tmp = selectedPrimID - 1;
		if(tmp%2==0)
		{
			if(gl_PrimitiveID==tmp||gl_PrimitiveID==tmp+1)
				color=vec4(1,1,0,1);
		}
		else
		{
			if(gl_PrimitiveID==tmp||gl_PrimitiveID==tmp-1)
				color=vec4(1,1,0,1);
		}
		
	}
	FragColor = color;

}

