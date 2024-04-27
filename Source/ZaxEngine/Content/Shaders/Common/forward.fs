#version 330 core

//KEYWORD:SHADOWCUBE
//KEYWORD:NORMALMAP

out vec4 FragColor;

in VS_OUT
{
    vec2 texCoord;
    vec3 normal;
    vec4 fragPos;
    vec4 fragPosInLight;
    mat3 TBN;
} vs_out;

uniform vec3 cameraPos;
uniform sampler2D AlbedoTexture;
uniform sampler2D NormalMap;
uniform vec3 ambientColor;
uniform float ambientIntensity;
uniform float Specular;
uniform int SurfaceType;
uniform float Alpha;


#ifdef SHADOWCUBE

    uniform samplerCube shadowCubeMap;
    vec3 sampleOffsetDirections[20] = vec3[]
    (
    vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
    vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
    vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
    vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
    vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
    );

#else

    uniform sampler2D shadowMap;

#endif

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
#ifndef NORMALMAP
    vec3 normal = normalize(vs_out.normal);
#else
    // vec3 normal = normalize(vs_out.normal);
    vec3 normal = texture(NormalMap, vs_out.texCoord).rgb;
    normal = normalize(normal * 2.0 - 1.0);   // 从[0,1]变为[-1,1]
    normal = normalize(vs_out.TBN * normal);
#endif
    vec4 tex = texture(AlbedoTexture, vs_out.texCoord);
    vec3 diffuse = vec3(0,0,0);
    vec3 specular = vec3(0,0,0);
    vec3 ambient = vec3(0,0,0);
    vec3 viewDir = normalize(cameraPos - vec3(vs_out.fragPos)); 

    // 定向光
    if (light.type == 0)
    {
        diffuse += max(dot(-light.direction, normal),0) * light.color; 
        vec3 halfDir = normalize(-light.direction + viewDir);
        specular += pow(max(dot(halfDir, normal), 0), 32) * Specular * light.color;
        ambient = ambientColor * ambientIntensity * vec3(tex);
    }
    else if (light.type == 1) // 点光
    {
        vec3 pointDir = light.position - vec3(vs_out.fragPos);
        float attenuation = clamp(1 - length(pointDir) / light.range,0,1);  // 衰减计算，这个是线性衰减，并且到达 range 时，scale 会为 0
        diffuse += max(dot(normalize(pointDir), normal) * attenuation,0) * light.color;
        vec3 halfDir = normalize(normalize(pointDir) + viewDir);
        specular += pow(max(dot(halfDir, normal), 0), 32) * Specular * light.color * attenuation;
    }
    else if (light.type == 2) // 聚光
    {
        vec3 pointDir = light.position - vec3(vs_out.fragPos);
        float attenuation = clamp(1 - length(pointDir) / light.range,0,1);  // range线性衰减计算
        attenuation *= (dot(normalize(-pointDir), light.direction) - light.cosOuter)/(light.cosInner - light.cosOuter); // 边缘线性衰减计算
        attenuation = max(attenuation,0);
        diffuse += max(dot(normalize(pointDir), normal) * attenuation,0) * light.color;
        vec3 halfDir = normalize(normalize(pointDir) + viewDir);
        specular += pow(max(dot(halfDir, normal), 0), 32) * Specular * light.color * attenuation;
    }

    diffuse = diffuse * vec3(tex); // 最后乘上albedo

    //****************** 计算阴影  *****************************//

    float shadow = 0.0;
    float shadowDepth = 0.0;
    float fragDepth = 0.0;

    // 片元深度转换为+z的线性深度

    #ifndef SHADOWCUBE

        vec3 fragNDCInLight = (vs_out.fragPosInLight.xyz / vs_out.fragPosInLight.w).xyz;
        vec3 fragScreenCoordInLight = fragNDCInLight * 0.5 + vec3(0.5);
        fragDepth = fragScreenCoordInLight.z;

        if (light.type == 0)
        {
            fragDepth = 0.5 * (fragNDCInLight.z * (light.far - light.near) + (light.far + light.near));
        }
        else if (light.type == 2) 
        {
            fragDepth =  (2.0 * light.near * light.far) / (light.far + light.near - fragNDCInLight.z * (light.far - light.near));    
        }

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
            float distance = dot(vec3(vs_out.fragPos) - light.position, light.direction);
            frustumSize = distance * tanhalf2;
        }
        float texelSize = frustumSize / shadowSize;
        float a = texelSize * 0.5 * 1.4143;
        vec3 lightDir = -light.direction;
        //if (light.type == 2) lightDir = normalize(light.position - vec3(fragPos)); 
        float b = length(cross(normal, lightDir)) / dot(normal, lightDir); //1.0 - dot(normal1, lightDir);
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

        //裁剪
        if(fragScreenCoordInLight.z > 1.0)
            shadow = 0.0;
    #else
        
        fragDepth = distance(vec3(vs_out.fragPos),light.position);
        vec3 lightToFragDir = vec3(vs_out.fragPos) - light.position; 
        //shadowDepth = texture(shadowCubeMap,lightToFragDir).r * light.range;

        float bias = 0.15;
        int samples = 20;
        float viewDistance = length(cameraPos - vec3(vs_out.fragPos));
        float diskRadius = (1.0 + (viewDistance / light.range)) / 25.0;
        for(int i = 0; i < samples; ++i)
        {
            float closestDepth = texture(shadowCubeMap, lightToFragDir + sampleOffsetDirections[i] * diskRadius).r;
            closestDepth *= light.range;   // Undo mapping [0;1]
            if(fragDepth - bias > closestDepth)
            shadow += 1.0;
        }
        shadow /= float(samples);
       // shadow = (fragDepth - 0.05 > shadowDepth ? 1.0 : 0.0); 

        if (fragDepth > light.range) shadow = 0; //裁剪
    
    #endif

    // 最后计算
    FragColor = vec4((diffuse + specular) * (1-shadow) + ambient, SurfaceType==1?Alpha:1);
}