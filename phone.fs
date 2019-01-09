
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
void main(){
	//objectColor
	vec3 objectColor = texture(texture1,fs_in.TexCoords).rgb;
	
	//ambient
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * objectColor;

	//diffuse
	vec3 normal = normalize(fs_in.Normal);
	vec3 lightDir = normalize(lightPos -fs_in.FragPos);
	float diff = max(dot(normal,lightDir),0.0);
	vec3 diffuse = diff * objectColor;

	//specular
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	vec3 reflectDir = reflect(-lightDir,normal);
	float spec = pow(max(dot(viewDir,reflectDir),0.0),32);
	vec3 specular = spec * lightDir * specularStrength;

	vec3 result = (ambient + diffuse + specular) ;
	FragColor = vec4(result,1.0);
}