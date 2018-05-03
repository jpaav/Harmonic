#version 440 core
layout(location=0) in vec3 vertexPosition_modelspace;
layout(location=1) in vec2 vertexUV;
layout(location=2) in vec3 vertexNormal;
out vec4 outPosition;
out vec3 FragPos;
out vec3 normal;


out vec2 UV;

uniform mat4 model;
uniform mat4 viewProj;

void main(){
    gl_PointSize = 10.0;
	gl_Position = viewProj * model * vec4(vertexPosition_modelspace,1);
	outPosition = gl_Position;
	UV = vec2(vertexUV.x, 1.0-vertexUV.y);
	FragPos = vec3(model * vec4(vertexPosition_modelspace,1));
	//Sets fragment normal after correcting for nonuniform scale
	normal = mat3(transpose(inverse(model))) * vertexNormal;
}