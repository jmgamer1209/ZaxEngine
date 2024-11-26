#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out VS_OUT
{
    vec2 texCoord;
    vec3 normal;
    vec4 fragPos;
    vec4 fragPosInLight;
    mat3 TBN;
} vs_out;



uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 normalMatrix;

uniform mat4 lightView;
uniform mat4 lightProjection;

void main()
{
    vs_out.fragPos = model*vec4(aPos,1.0f);
    gl_Position = projection * view * vs_out.fragPos;
    vs_out.texCoord = inTexCoord;
    vs_out.normal = mat3(normalMatrix) * inNormal;

    vs_out.fragPosInLight = lightProjection * lightView * vs_out.fragPos;

    vec3 T = normalize(vec3(model * vec4(tangent,   0.0)));
    vec3 B = normalize(vec3(model * vec4(bitangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(inNormal,    0.0)));
    vs_out.TBN = mat3(T, B, N);
}