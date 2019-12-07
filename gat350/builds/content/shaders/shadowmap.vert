#version 430

layout (location = 0) in vec3 position;

uniform mat4 model_matrix;
uniform mat4 lvp_matrix;

void main()
{
    gl_Position = lvp_matrix * model_matrix * vec4(position, 1.0);
}
