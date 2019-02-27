#version 330

uniform int gDrawIndex;
uniform int views;//1前 2后 3左 4右 5上 6下

out vec3 FragColor;

void main()
{
    FragColor = vec3(float(views), gDrawIndex,float(gl_PrimitiveID+1));
} 