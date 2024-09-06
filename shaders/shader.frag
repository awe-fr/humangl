#version 430 core

in vec4 col;

out vec4 color;

uniform vec4 cvec;

void main(){
  color = col;
}