#version 330 core

out vec4 FragColor;

in vec2 texCoord;
in vec3 normal;

uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform float ambientIntensity;
uniform sampler2D texture1;

void main()
{
    vec4 tex = texture(texture1, texCoord);
    float diffuse = dot(-lightDirection, normal); 
    diffuse = max(diffuse,0); //避免负值
    FragColor = vec4(lightColor * vec3(tex) * (diffuse + ambientIntensity), 1);
}