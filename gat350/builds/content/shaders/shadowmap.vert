#version 430

layout (location = 0) in vec3 position;

uniform mat4 mvp_matrix;
uniform mat4 model_view_matrix;

void main()
{
    gl_Position = mvp_matrix * vec4(position, 1.0);
}
