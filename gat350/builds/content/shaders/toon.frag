#version 430 core

in vec3 fposition;
in vec3 fnormal;

struct material_s
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct light_s
{
	vec4 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform light_s		light;
uniform material_s	material;
//uniform int			steps;

out vec4 color;

int steps = 4;
float scale = 1.0 / steps;

vec3 toon(vec3 position, vec3 normal)
{
	vec3 position_to_light = normalize(vec3(light.position) - position);

	float lDotN = max(dot(position_to_light, normal), 0.0);
	vec3 diffuse = material.diffuse * light.diffuse * floor(lDotN * steps) * scale;

	return diffuse;
}

void main()
{
	vec3 ambient =  light.ambient * material.ambient;
	vec3 diffuse = toon(fposition, fnormal);

	color = vec4(ambient + diffuse, 1.0);
}