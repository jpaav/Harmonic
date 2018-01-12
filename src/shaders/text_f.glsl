#version 330 core

out vec4 color;
in vec2 UV;
uniform sampler2D textureMap;

void main()
{
	color = vec4(1,1,1,texture2D(textureMap, UV).r);
	//color = vec4(1-texture2D(textureMap, UV).rgb,1);
	//color = vec4(1,1,1,1);
}