 #version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;

out vec2 texCoord;
out vec3 normal;
out vec4 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    fragPos = model*vec4(aPos,1.0f);
    gl_Position = projection*view*fragPos;
    texCoord = inTexCoord;
    normal = mat3(model) * inNormal;
}