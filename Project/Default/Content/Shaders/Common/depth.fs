#version 330 core

out vec4 FragColor;

void main()
{
   FragColor = vec4(vec3(1-pow(gl_FragCoord.z, 200)), 1.0);
}