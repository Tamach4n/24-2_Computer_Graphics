#version 330 core

in vec3 outColor;
in vec3 outFragPos;
in vec3 outNormal;

out vec4 FragColor;

uniform vec3 uLightPos;
uniform vec3 uLightColor;
uniform vec3 uCameraPos;

uniform vec3 uEmissiveColor;	//	¹ß»ê±¤

uniform float uAmbientLight;
uniform float uSpecularShininess;
uniform float uSpecularStrength; 

void main()
{
	vec3 ambient = uAmbientLight * uLightColor;

	vec3 normatVec = normalize(outNormal);
	vec3 lightDir = normalize(uLightPos - outFragPos);		// ºûÀÇ ¹æÇâº¤ÅÍ
	float diffuseLight = max(dot(normatVec, lightDir), 0.0);	// 0 ~ 1»çÀÌÀÇ °ª
	vec3 diffuse = diffuseLight * uLightColor;
	
	int shininess = 64;
	vec3 cameraDir = normalize(uCameraPos - outFragPos);
	vec3 reflectDir = reflect(-lightDir, normatVec);
	float specularLight = pow(max(dot(cameraDir, reflectDir), 0.0), uSpecularShininess);
	vec3 specular = specularLight * uLightColor;
	//vec3 specular = uSpecularStrength * specularLight * uLightColor;

	//vec3 result = (ambient + diffuse + specular) * outColor;		// vec3 * vec3 = ¿ø¼Ò³¢¸®ÀÇ °ö
	vec3 result = (uEmissiveColor + ambient + diffuse + specular) * outColor;
	
	FragColor = vec4(result, 1.0);
}
