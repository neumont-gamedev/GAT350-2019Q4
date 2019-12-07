#version 430
	
layout (location = 0) in vec3 vposition;
layout (location = 1) in vec3 vnormal;
layout (location = 3) in vec2 vtexcoord;

out vec3 fposition;
out vec3 fnormal;
out vec2 ftexcoord;
out vec4 fshadowcoord;

uniform mat4 mvp_matrix;
uniform mat4 model_matrix;
uniform mat4 model_view_matrix;
uniform mat4 lvp_matrix;

void main()
{
	fnormal = normalize(mat3(model_view_matrix) * vnormal);
	fposition = vec3(model_view_matrix * vec4(vposition, 1.0));
	ftexcoord = vtexcoord;

	mat4 mx = lvp_matrix * model_matrix; 
	fshadowcoord = lvp_matrix * vec4(vposition, 1.0);

	gl_Position = mvp_matrix * vec4(vposition, 1.0);
}
