#version 430
	
in vec3 fposition;
in vec3 fnormal;
in vec2 ftexcoord;

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
	vec4 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform light_s light;
uniform float discard_threshold;

layout (binding = 0) uniform sampler2D texture_sample;

void main()
{
	vec4 texture_color = texture(texture_sample, ftexcoord);
	if (texture_color.a < discard_threshold) discard;

	vec3 position_to_light = normalize(vec3(light.position) - fposition);

	// ambient
	vec3 ambient = material.ambient * light.ambient;

	// diffuse
	float lDotN = max(0.0, dot(fnormal, position_to_light));
	vec3 diffuse = (material.diffuse * light.diffuse) * lDotN;

	// specular
	vec3 specular = vec3(0.0);
	if (lDotN > 0.0)
	{
		vec3 position_to_view = normalize(-fposition.xyz);
		vec3 reflect_light = reflect(-position_to_light, fnormal);
		float specular_intensity = max(dot(reflect_light, position_to_view), 0.0);
		specular_intensity = pow(specular_intensity, material.shininess);
		specular = light.specular * material.specular * specular_intensity;
	}

	color = (vec4(ambient + diffuse, 1.0f) * texture(texture_sample, ftexcoord)) + vec4(specular, texture_color.a);
}
