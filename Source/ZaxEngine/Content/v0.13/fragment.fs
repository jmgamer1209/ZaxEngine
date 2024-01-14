#version 330 core

out vec4 FragColor;

in vec2 texCoord;
in vec3 normal;
in vec4 fragPos;
in vec4 fragPosInLight;

uniform vec3 cameraPos;
uniform sampler2D albedoTexture;
uniform vec3 ambientColor;
uniform float ambientIntensity;
uniform float specularIntensity;
uniform sampler2D shadowMap;

struct DirectionalLight
{
    vec3 direction;
    vec3 color;
    float depthBias;
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

struct SpotLight 
{
    vec3 position;
    vec3 direction;
    vec3 color;
    float range;
    float cosInner;
    float cosOuter;
};
#define NUMBER_SPOT_LIGHT 4
uniform SpotLight[NUMBER_POINT_LIGHT] spotLights;
uniform int spotLightsNumber;

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

    // 聚光灯
    for(int i=0; i<spotLightsNumber; i++)
    {
        vec3 pointDir = spotLights[i].position - vec3(fragPos);
        float attenuation = clamp(1 - length(pointDir) / spotLights[i].range,0,1);  // range线性衰减计算
        attenuation *= (dot(normalize(-pointDir), spotLights[i].direction) - spotLights[i].cosOuter)/(spotLights[i].cosInner - spotLights[i].cosOuter); // 边缘线性衰减计算
        attenuation = max(attenuation,0);
        diffuse += max(dot(normalize(pointDir), normal1) * attenuation,0) * spotLights[i].color;

        halfDir = normalize(normalize(pointDir) + viewDir);
        specular += pow(max(dot(halfDir, normal1), 0), 32) * specularIntensity * spotLights[i].color * attenuation;
    }

    diffuse = diffuse * vec3(tex); // 最后乘上albedo
    vec3 ambient = ambientColor * ambientIntensity * vec3(tex);

    //**********************
    // 计算阴影
    //**********************
    vec3 fragScreenCoordInLight = (fragPosInLight.xyz / fragPosInLight.w).xyz * 0.5 + vec3(0.5);
    float shadowDepth = texture(shadowMap, fragScreenCoordInLight.xy).r;
    float shadowSize = textureSize(shadowMap, 0).x;
    float frustumSize = 40.0;
    float a = frustumSize / shadowSize * 0.5;
    float b = 1.0 - dot(normal1, -directionalLight.direction);
    float bias = directionalLight.depthBias * a * b;

    //PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0); 
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, fragScreenCoordInLight.xy + vec2(x, y) * texelSize).r; 
            shadow += (fragScreenCoordInLight.z - bias > pcfDepth ? 1.0 : 0.0);        
        }    
    }
    shadow /= 9.0;    

    //裁剪
    if(fragScreenCoordInLight.z > 1.0)
        shadow = 0.0;

    // 最后计算
    FragColor = vec4((diffuse + specular) * (1-shadow) + ambient,1);
}