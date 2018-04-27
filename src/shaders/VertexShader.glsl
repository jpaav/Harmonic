#version 440 core
layout(location=0) in vec3 vertexPosition_modelspace;
layout(location=1) in vec2 vertexUV;
out vec4 outPosition;


out vec2 UV;

uniform mat4 MVP;

void main(){
    gl_PointSize = 10.0;
	gl_Position = MVP * vec4(vertexPosition_modelspace,1);
	outPosition = gl_Position;
	UV = vec2(vertexUV.x, 1.0-vertexUV.y);
}