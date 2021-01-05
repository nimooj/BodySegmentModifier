#version 330 core
// TODO: define in/out and uniform variables.

out vec4 FragColor;

uniform vec3 Color;

void main()
{

	FragColor = vec4(Color, 1.0f);
}