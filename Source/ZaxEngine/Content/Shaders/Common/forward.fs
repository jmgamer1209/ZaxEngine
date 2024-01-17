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

struct Light
{
    int type;
    vec3 position;
    vec3 direction;
    vec3 color;
    float range;
    float depthBias;
    float cosInner;
    float cosOuter;
    float near;
    float far;
};
uniform Light light;

void main()
{
    vec3 normal1 = normalize(normal);
    vec4 tex = texture(albedoTexture, texCoord);
    vec3 diffuse = vec3(0,0,0);
    vec3 specular = vec3(0,0,0);
    vec3 ambient = vec3(0,0,0);
    vec3 viewDir = normalize(cameraPos - vec3(fragPos)); 

    // 定向光
    if (light.type == 0)
    {
        diffuse += max(dot(-light.direction, normal1),0) * light.color; 
        vec3 halfDir = normalize(-light.direction + viewDir);
        specular += pow(max(dot(halfDir, normal1), 0), 32) * specularIntensity * light.color;
        ambient = ambientColor * ambientIntensity * vec3(tex);
    }
    else if (light.type == 1) // 点光
    {
        vec3 pointDir = light.position - vec3(fragPos);
        float attenuation = clamp(1 - length(pointDir) / light.range,0,1);  // 衰减计算，这个是线性衰减，并且到达 range 时，scale 会为 0
        diffuse += max(dot(normalize(pointDir), normal1) * attenuation,0) * light.color;
        vec3 halfDir = normalize(normalize(pointDir) + viewDir);
        specular += pow(max(dot(halfDir, normal1), 0), 32) * specularIntensity * light.color * attenuation;
    }
    else if (light.type == 2) // 聚光
    {
        vec3 pointDir = light.position - vec3(fragPos);
        float attenuation = clamp(1 - length(pointDir) / light.range,0,1);  // range线性衰减计算
        attenuation *= (dot(normalize(-pointDir), light.direction) - light.cosOuter)/(light.cosInner - light.cosOuter); // 边缘线性衰减计算
        attenuation = max(attenuation,0);
        diffuse += max(dot(normalize(pointDir), normal1) * attenuation,0) * light.color;
        vec3 halfDir = normalize(normalize(pointDir) + viewDir);
        specular += pow(max(dot(halfDir, normal1), 0), 32) * specularIntensity * light.color * attenuation;
    }

    diffuse = diffuse * vec3(tex); // 最后乘上albedo

    float shadow = 0.0;
    if (light.type == 0 || light.type == 2) 
    {
    //**********************
    // 计算阴影
    //**********************
    vec3 fragScreenCoordInLight = (fragPosInLight.xyz / fragPosInLight.w).xyz * 0.5 + vec3(0.5);
    float fragDepth = fragScreenCoordInLight.z;
    if (light.type == 2) fragDepth = (light.far - ((light.far * light.near) / fragDepth)) / (light.far - light.near);
    //float shadowDepth = texture(shadowMap, fragScreenCoordInLight.xy).r;
    float shadowSize = textureSize(shadowMap, 0).x;
    float frustumSize = 40.0;
    float a = frustumSize / shadowSize * 0.5;
    float b = 1.0 - dot(normal1, -light.direction);
    float bias = max(light.depthBias * a * b, 0.005);
    
    //PCF
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0); 
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, fragScreenCoordInLight.xy + vec2(x, y) * texelSize).r; 
            if (light.type == 2) pcfDepth = (light.far - ((light.far * light.near) / pcfDepth)) / (light.far - light.near);
            shadow += (fragDepth - bias > pcfDepth ? 1.0 : 0.0);        
        }    
    }
    shadow /= 9.0;    

    //裁剪
    if(fragScreenCoordInLight.z > 1.0)
        shadow = 0.0;
    }

    // 最后计算
    FragColor = vec4((diffuse + specular) * (1-shadow) + ambient,1);
}