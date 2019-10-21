#version 430
	
layout (location = 0) in vec3 vposition;
layout (location = 1) in vec3 vnormal;

out vec3 fcolor;
	
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

uniform mat4 mvp_matrix;
uniform mat4 model_view_matrix;
	
void main()
{
	vec3 normal = normalize(mat3(model_view_matrix) * vnormal);
	vec4 position = model_view_matrix * vec4(vposition, 1.0);
	vec3 position_to_light = normalize(vec3(light.position - position));

	vec3 ambient = material.ambient * light.ambient;

	float lDotN = max(0.0, dot(normal, position_to_light));
	vec3 diffuse = (material.diffuse * light.diffuse) * lDotN;

	vec3 specular = vec3(0.0);
	if (lDotN > 0.0)
	{
		vec3 position_to_view = normalize(-position.xyz);
		vec3 reflect_light = reflect(-position_to_light, normal);
		float specular_intensity = max(dot(reflect_light, position_to_view), 0.0);
		specular_intensity = pow(specular_intensity, material.shininess);
		specular = light.specular * material.specular * specular_intensity;
	}

	fcolor = min(ambient + diffuse + specular, vec3(1.0f));
	gl_Position = mvp_matrix * vec4(vposition, 1.0);
}
