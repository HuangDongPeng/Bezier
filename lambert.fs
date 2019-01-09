
#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;


uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

uniform sampler2D texture1;

uniform bool isHalfLambert;

void main(){
	vec3 lightDir =normalize(lightPos - fs_in.FragPos);
	vec3 normal = normalize(fs_in.Normal);
	float diff = max(dot(lightDir,normal),0.0);
	if(isHalfLambert){
		diff = diff*0.5 + 0.5;
	}

	vec3 objectColor = texture(texture1,fs_in.TexCoords).rgb;
	float ambientStrength = 0.2f;
	vec3 ambient = ambientStrength * objectColor;

	vec3 res = ambient*diff;

	FragColor = vec4(res,1.0);
}