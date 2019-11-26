#version 430

#define POINT		0
#define DIRECTION	1
#define SPOTLIGHT	2
	
in vec3 fposition;
in vec3 fnormal;
in vec2 ftexcoord;

in vec3 flight_position;
in vec3 flight_direction;
in vec3 fview_direction;


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

layout (binding = 0) uniform sampler2D texture_sample;
layout (binding = 1) uniform sampler2D normal_sample;

void phong(light_s in_light, vec3 position, vec3 normal, out vec3 ambient, out vec3 diffuse, out vec3 specular)
{
	vec3 position_to_light;
	if (in_light.type == POINT || in_light.type == SPOTLIGHT) position_to_light = normalize(vec3(flight_position) - position); //
	else if (in_light.type == DIRECTION) position_to_light = normalize(-flight_direction); //

	// ambient
	ambient = material.ambient * in_light.ambient;

	diffuse = vec3(0.0);
	specular = vec3(0.0);

	float spot_intensity = 1.0;

	if (in_light.type == SPOTLIGHT)
	{
		float cos_angle = dot(flight_direction, -position_to_light);
		float angle = acos(cos_angle);

		if (angle > in_light.cutoff) return;
		spot_intensity = pow(cos_angle, in_light.exponent);
	}

	// diffuse
	float lDotN = max(0.0, dot(normal, position_to_light));
	diffuse = ((material.diffuse * in_light.diffuse) * lDotN) * spot_intensity;

	// specular
	specular = vec3(0.0);
	if (lDotN > 0.0)
	{
		vec3 position_to_view = fview_direction;//normalize(-position.xyz);
		vec3 reflect_light = reflect(-position_to_light, normal);
		float specular_intensity = max(dot(reflect_light, position_to_view), 0.0);
		specular_intensity = pow(specular_intensity, material.shininess);
		specular = (in_light.specular * material.specular * specular_intensity) * spot_intensity;
	}
} 

void main()
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 normal = texture(normal_sample, ftexcoord).rgb;
	normal = (normal * 2.0) - vec3(1.0);
	//normal = normalize(tbn_matrix * normal); 
	phong(light, fposition, normal, ambient, diffuse, specular);

	color = (vec4(ambient + diffuse, 1.0f) * texture(texture_sample, ftexcoord)) + vec4(specular, 1.0f);
}
