#version 430
	
in vec3 reflection_direction;

out vec4 color;

struct material_s
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess; 
};

uniform material_s material;

struct light_s
{
	int type;
	vec4 position;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float cutoff;
	float exponent;
};

uniform light_s light;

layout (binding = 0) uniform samplerCube texture_sample;

void main()
{
	vec4 texture_color = texture(texture_sample, reflection_direction);
	color = texture_color;
}
