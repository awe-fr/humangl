#version 430 core

layout(location = 0) in vec3 vertexPosition_modelspace;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;

void main() {
	mat4 MVP = Projection * View * Model;
	gl_Position = MVP * vec4(vertexPosition_modelspace, 1);
}