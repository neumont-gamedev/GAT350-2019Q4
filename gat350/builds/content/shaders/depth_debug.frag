#version 430

in vec2 ftexcoord;

layout (binding = 1) uniform sampler2D texture_sample;

out vec4 color;

void main()
{
	float depth = texture(texture_sample, ftexcoord).x;
    color = vec4(vec3(depth), 1.0);
}
