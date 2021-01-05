#version 330 core
// TODO: define in/out and uniform variables.
out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;
in vec3 Color;

uniform vec3 CameraPos;
uniform vec3 LightPos;
uniform vec3 LightColor;


uniform int isTexture = 0;
// uniform vec3 Color;


uniform sampler2D texture;

void main()
{

	if (isTexture == 0) {
		// ambient
		float ambientStrength = 1.0f;
		vec3 ambient = ambientStrength * LightColor;

		// diffuse
		vec3 norm = normalize(Normal);
		vec3 lightDirection = normalize(LightPos - FragPos);
		float diff = max(dot(norm, lightDirection), 0.0);
		vec3 diffuse = diff * LightColor;

		// specular
		float specularStrength = 0.5;
		vec3 viewDirection = normalize(CameraPos - FragPos);
		vec3 reflectDirection = reflect(-lightDirection, norm);
		float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
		vec3 specular = specularStrength * spec * LightColor;

		vec3 lightingColor = (ambient + diffuse + specular) * Color;

		FragColor = vec4(lightingColor, 1);
	}
	else
		FragColor = texture(texture, TexCoord);
}