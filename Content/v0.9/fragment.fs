#version 330 core

out vec4 FragColor;

in vec2 texCoord;
in vec3 normal;
in vec4 fragPos;

uniform vec3 cameraPos;
uniform sampler2D albedoTexture;
uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform float ambientIntensity;
uniform float specularIntensity;

void main()
{
    vec3 normal1 = normalize(normal);
    vec4 tex = texture(albedoTexture, texCoord);

    float diffuse = dot(-lightDirection, normal1); 
    diffuse = max(diffuse,0); //避免负值

    vec3 viewDir = cameraPos - vec3(fragPos);
    vec3 halfDir = normalize(-lightDirection + viewDir);
    float specular = pow(max(dot(halfDir, normal1), 0), 32) * specularIntensity;

    FragColor = vec4(lightColor * vec3(tex) * (diffuse + ambientIntensity + specular), 1);
}