#version 330 core
layout(location=0) in vec3 vertexPosition_modelspace;

out vec4 col;

uniform mat4 MVP;

uniform vec3 color;

void main(){
    gl_PointSize = 10.0;
	gl_Position = MVP * vec4(vertexPosition_modelspace,1.0);
	col = vec4(color, 1.0);
}