#version 330 core

//KEYWORD:ShadowCube

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
uniform samplerCube shadowCubeMap;
uniform sampler2D shadowMap;


uniform mat4 lightProjection;

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
    float tanhalf;
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

    //****************** 计算阴影  *****************************//

    float shadow = 0.0;
    float shadowDepth = 0.0;
    vec3 fragNDCInLight = (fragPosInLight.xyz / fragPosInLight.w).xyz;
    vec3 fragScreenCoordInLight = fragNDCInLight * 0.5 + vec3(0.5);
    float fragDepth = fragScreenCoordInLight.z;

    // 片元深度转换为+z的线性深度
    if (light.type == 0 || light.type == 1)
    {
        fragDepth = 0.5 * (fragNDCInLight.z * (light.far - light.near) + (light.far + light.near));
    }
    else if (light.type == 2) 
    {
        fragDepth =  (2.0 * light.near * light.far) / (light.far + light.near - fragNDCInLight.z * (light.far - light.near));    
    }


    #ifndef ShadowCube

       // 阴影深度转换为实际距离的线性深度,即正Z值，由于PCF计算，这里注释掉
        // float shadowDepth = texture(shadowMap, fragScreenCoordInLight.xy).r;
        // float shadowDepthInNDC = shadowDepth * 2.0 - 1.0;

        // if (light.type == 0)
        // {
        //     shadowDepth = 0.5 * (shadowDepthInNDC * (light.far - light.near) + (light.far + light.near));
        // }
        // else if (light.type == 2) 
        // {
        //     shadowDepth = (2.0 * light.near * light.far) / (light.far + light.near - shadowDepthInNDC * (light.far - light.near)); 
        // }

        // 计算 bias
        float shadowSize = textureSize(shadowMap, 0).x;
        float frustumSize = 40.0;
        if (light.type == 2) 
        {
            float tanhalf2 = 2.0 * light.tanhalf;
            float distance = dot(vec3(fragPos) - light.position, light.direction);
            frustumSize = distance * tanhalf2;
        }
        float texelSize = frustumSize / shadowSize;
        float a = texelSize * 0.5 * 1.4143;
        vec3 lightDir = -light.direction;
        if (light.type == 2) lightDir = normalize(light.position - vec3(fragPos)); 
        float b = length(cross(normal1, lightDir)) / dot(normal1, lightDir); //1.0 - dot(normal1, lightDir);
        float bias = light.depthBias * a * b;
        bias = max(bias, 0.1);

        //PCF
        vec2 shadowMapUnitSize = 1.0 / textureSize(shadowMap, 0); 
        for(int x = -1; x <= 1; ++x)
        {
            for(int y = -1; y <= 1; ++y)
            {
                float pcfDepth = textureLod(shadowMap, fragScreenCoordInLight.xy + vec2(x, y) * shadowMapUnitSize, 0).r;  
                if (light.type == 2) 
                {
                    pcfDepth = pcfDepth * 2.0 - 1.0; // ndc坐标
                    pcfDepth = (2.0 * light.near * light.far) / (light.far + light.near - pcfDepth * (light.far - light.near));    
                }
                if (light.type == 0)
                {
                    pcfDepth = pcfDepth * 2.0 - 1.0; // ndc坐标
                    pcfDepth = 0.5 * (pcfDepth * (light.far - light.near) + (light.far + light.near));
                }
                shadow += (fragDepth - (1 + sqrt(abs(x)*abs(x) + abs(y)*abs(y))) * bias > pcfDepth ? 1.0 : 0.0);  // bias cone 修正偏差     
            }    
        }
        shadow /= 9.0;    
        //shadow = (fragDepth - bias > shadowDepth ? 1.0 : 0.0); 

    #endif

    #ifdef ShadowCube

    vec3 lightToFragDir = vec3(fragPos) - light.position; 
    shadowDepth = textureLod(shadowCubeMap,lightToFragDir, 0).r * light.range;
        
    shadow = (fragDepth - 0.05 > shadowDepth ? 1.0 : 0.0); 

    #endif

    //裁剪
    if(fragScreenCoordInLight.z > 1.0)
        shadow = 0.0;

    // 最后计算
    FragColor = vec4((diffuse + specular) * (1-shadow) + ambient,1);
}