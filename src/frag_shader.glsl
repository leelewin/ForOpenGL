#version 330 core
out vec4 FragColor;

in vec2 texCood;
in vec3 normal;
in vec3 fragPos;

//如果没有被使用则默认被删除
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;
uniform sampler2D texture_specular3;
uniform sampler2D texture_specular4;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_diffuse4;

// uniform vec3 objectColor;
// uniform vec3 lightColor;

// uniform vec3 lightPos;
uniform vec3 viewPos;

// struct Material {
// 	vec3 ambient;
// 	vec3 diffuse;
// 	vec3 specular;
// 	float shininess;
// };

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

// uniform Material material;
uniform Light light;

void main()
{
	// FragColor = mix(texture(texture_diffuse1, texCood), texture(texture_specular1, texCood), 0.0);
	//ambient
	vec3 ambient = vec3(texture(texture_diffuse1, texCood)) * light.ambient;
	//diffuse
	//TODO衰减
	vec3 lightDir = normalize(light.position - fragPos);
	vec3 norm = normalize(normal);
	float diff = max(dot(lightDir, norm), 0.0);
	vec3 diffuse = vec3(texture(texture_diffuse1, texCood)) * diff * light.diffuse;
	//specular
	//TODO衰减
	vec3 viewDirect = normalize(viewPos - fragPos);
	vec3 halfVect = normalize(lightDir + viewDirect);
	float spec = max(dot(halfVect, norm), 0.0);
	spec = pow(spec, 32.0);
	vec3 specular = vec3(texture(texture_specular1, texCood)) * spec * light.specular;


	vec3 result = (ambient + diffuse + specular);
	FragColor = vec4(result, 1.0);
}
