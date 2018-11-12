#version 330 core

in vec2 UV;
in vec3 FragPos;
in vec3 normal;
out vec4 color;
uniform vec3 tint;
uniform sampler2D myTextureSampler;
uniform int useTextures;
uniform vec3 viewPos;
uniform float shininess;

#define kPi 3.1415926

vec3 find_fresnel(in vec3 f0, in float product)
{
    return mix(f0, vec3(1.0), pow(1.01 - product, 5.0));
}

void main(){
    //Setup
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    vec3 lightPos = vec3(5.0f, 5.0f, 10.0f);
    vec3 ambientLightColor = vec3(0.1f, 0.1f, 0.1f);

    //Calculations
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    vec3 specularColor = pow(max(dot(normal, halfwayDir), 0.0), shininess) * lightColor;
    vec4 objectColor;
    if(useTextures==1)
    {
        objectColor = texture2D(myTextureSampler, UV).rgba * vec4(tint, 1.0f);
    }
    else
    {
        objectColor = vec4(tint, 1.0f);
    }
	color = objectColor * vec4(specularColor, 1.0f);
}