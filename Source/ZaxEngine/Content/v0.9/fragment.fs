#version 330 core

out vec4 FragColor;

in vec2 texCoord;
in vec3 normal;
in vec4 fragPos;

uniform vec3 cameraPos;
uniform sampler2D albedoTexture;
uniform float ambientIntensity;
uniform float specularIntensity;

struct DirectionalLight
{
    vec3 direction;
    vec3 color;
};
uniform DirectionalLight directionalLight;

struct PointLight 
{
    vec3 position;
    vec3 color;
    float range;
};
#define NUMBER_POINT_LIGHT 4
uniform PointLight[NUMBER_POINT_LIGHT] pointLights;
uniform int pointLightsNumber;

void main()
{
    vec3 normal1 = normalize(normal);
    vec4 tex = texture(albedoTexture, texCoord);
    vec3 diffuse = vec3(0,0,0);
    vec3 specular = vec3(0,0,0);

    // 定向光
    diffuse += max(dot(-directionalLight.direction, normal1),0) * directionalLight.color; 
    vec3 viewDir = normalize(cameraPos - vec3(fragPos)); 
    vec3 halfDir = normalize(-directionalLight.direction + viewDir);
    specular += pow(max(dot(halfDir, normal1), 0), 32) * specularIntensity * directionalLight.color;

    // 点光源
    for(int i=0; i<pointLightsNumber; i++)
    {
        vec3 pointDir = pointLights[i].position - vec3(fragPos);
        float attenuation = clamp(1 - length(pointDir) / pointLights[i].range,0,1);  // 衰减计算，这个是线性衰减，并且到达 range 时，scale 会为 0
        diffuse += max(dot(normalize(pointDir), normal1) * attenuation,0) * pointLights[i].color;

        halfDir = normalize(normalize(pointDir) + viewDir);
        specular += pow(max(dot(halfDir, normal1), 0), 32) * specularIntensity * pointLights[i].color * attenuation;
    }

    diffuse = diffuse * vec3(tex); // 最后乘上albedo
    FragColor = vec4(diffuse + ambientIntensity + specular,1);
}