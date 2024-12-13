#version 330 core

in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragPos;

out vec4 outColor;

uniform int uLightOn;	//	26 only

uniform vec3 uLightPos;
uniform vec3 uLightColor;
uniform vec3 uCameraPos;

uniform vec3 uEmissiveColor;	//	πﬂªÍ±§

uniform float uAmbientLight;
uniform float uSpecularShininess;
uniform float uSpecularStrength; 

uniform sampler2D uTexture;

void main()
{
	vec3 ambient = uAmbientLight * uLightColor;

	vec3 normatVec = normalize(fragNormal);
	vec3 lightDir = normalize(uLightPos - fragPos);		// ∫˚¿« πÊ«‚∫§≈Õ
	float diffuseLight = max(dot(normatVec, lightDir), 0.0);	// 0 ~ 1ªÁ¿Ã¿« ∞™
	vec3 diffuse = diffuseLight * uLightColor;
	//vec3 diffuse = (uLightOn == 1) ? (diffuseLight * uLightColor) : vec3(0.0);
	
	int shininess = 64;
	vec3 cameraDir = normalize(uCameraPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normatVec);
	float specularLight = pow(max(dot(cameraDir, reflectDir), 0.0), uSpecularShininess);
	vec3 specular = specularLight * uLightColor;
	//vec3 specular = uSpecularStrength * specularLight * uLightColor;
	
	vec3 result = uEmissiveColor + ambient + diffuse + specular;
	
	outColor = texture(uTexture, fragTexCoord) * vec4(result, 1.f);
}