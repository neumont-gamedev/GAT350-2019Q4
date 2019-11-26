#version 430
	
layout (location = 0) in vec3 vposition;
layout (location = 1) in vec3 vnormal;
layout (location = 3) in vec2 vtexcoord;
layout (location = 4) in vec3 vtangent;

out vec3 fposition;
out vec3 fnormal;
out vec2 ftexcoord;
out vec3 flight_position;
out vec3 flight_direction;
out vec3 fview_direction;

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

uniform mat4 mvp_matrix;
uniform mat4 model_view_matrix;
	
void main()
{
	fposition = vec3(model_view_matrix * vec4(vposition, 1.0));
	ftexcoord = vtexcoord;

	// create tbn matrix
	mat3 normal_matrix = transpose(inverse(mat3(model_view_matrix)));

	fnormal = normalize(normal_matrix * vnormal);
	vec3 tangent = normalize(normal_matrix * vtangent);
	vec3 bitangent = normalize(cross(fnormal, tangent));

	// transform from model view space to tangent (local) space
    mat3 tbn_matrix = transpose(mat3(tangent, bitangent, fnormal));

	flight_position = tbn_matrix * light.position.xyz;
	flight_direction = tbn_matrix * light.direction;
	fview_direction = tbn_matrix * normalize(-fposition);

	gl_Position = mvp_matrix * vec4(vposition, 1.0);
}
