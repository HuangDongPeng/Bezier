#version 330

uniform int gDrawIndex;
uniform int views;//1ǰ 2�� 3�� 4�� 5�� 6��

out vec3 FragColor;

void main()
{
    FragColor = vec3(float(views), gDrawIndex,float(gl_PrimitiveID+1));
} 