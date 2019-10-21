#version 430
	
layout (location = 0) in vec3 vposition;
layout (location = 1) in vec3 vnormal;

out vec3 fcolor;

uniform vec3 ambient;
uniform vec4 light_position;
uniform mat4 model_view_matrix;
uniform mat4 mvp_matrix;

void main()
{
	vec3 normal_view = normalize(mat3(model_view_matrix) * vnormal);
	vec4 position_view = model_view_matrix * vec4(vposition, 1.0);
	vec3 position_to_light = normalize(vec3(light_position - position_view));

	float intensity = max(dot(position_to_light, normal_view), 0.0);
	fcolor = ambient + vec3(intensity);

	gl_Position = mvp_matrix * vec4(vposition, 1.0);
}
