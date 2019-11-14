#version 430
	
layout (location = 0) in vec3 vposition;
layout (location = 1) in vec3 vnormal;
layout (location = 3) in vec2 vtexcoord;

out vec3 fposition;
out vec3 fnormal;
out vec2 ftexcoord;

uniform vec3 scale;

uniform vec2 uv_offset;
uniform vec2 uv_scale;

uniform float amplitude;
uniform float frequency;
uniform float time;
uniform float rate;

uniform vec3 bloat_position;
uniform float bloat_scale;
uniform float bloat_range;

uniform mat4 mvp_matrix;
uniform mat4 model_view_matrix;
	
void main()
{
	// scale
	vec3 position = vposition;// * scale;
	//ftexcoord = position.xy;

	// bloat
	vec3 n = (position - bloat_position);
	float d = length(n);
	float t = abs(d - bloat_range);
	t = min(bloat_range, t) / bloat_range;
	position = position + (normalize(n) * (1.0 - t) * bloat_scale);

	// wave
	position.y = position.y + sin((time * rate) + (vposition.x * frequency)) * amplitude;

	fposition = vec3(model_view_matrix * vec4(position, 1.0));
	fnormal = normalize(transpose(inverse(mat3(model_view_matrix))) * vnormal);

	// scale/offset uv
	ftexcoord = (vtexcoord + uv_offset) * uv_scale;
	//ftexcoord = position.xy;

	gl_Position = mvp_matrix * vec4(position, 1.0);
}
