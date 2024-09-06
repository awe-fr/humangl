#version 430 core

layout(location = 0) in vec3 vertexPosition_modelspace;

uniform mat4 MVP;

out vec4 col;

vec4 colors[3] = vec4[3](vec4(1, 0, 0, 1),vec4(0, 1, 0, 1),vec4(0, 0, 1, 1));

void main() {
	gl_Position = MVP * vec4(vertexPosition_modelspace, 1);
	col = colors[gl_VertexID % 3];
}