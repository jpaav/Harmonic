#version 330 core

layout(location=0) in vec4 position; //object space vertex
out vec2 UV;   // texture coordinates for texture lookup in the fragment shader
 
void main()
{
	gl_Position = vec4(position.xy,0,1);
	UV = position.zw; // texture coordinate
}
