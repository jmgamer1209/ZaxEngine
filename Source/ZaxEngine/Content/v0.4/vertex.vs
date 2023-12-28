 #version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 inTexCoord;

uniform vec3 offset;
out vec2 texCoord;

void main()
{
    gl_Position = vec4(aPos.x+offset.x, aPos.y+offset.y, aPos.z+offset.z, 1.0); 
    texCoord = inTexCoord;
}