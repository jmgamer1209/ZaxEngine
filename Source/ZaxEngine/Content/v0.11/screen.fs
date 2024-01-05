#version 330 core

out vec4 FragColor;

in vec2 texCoord;
uniform sampler2D screenTex;


void main()
{
    vec3 color = texture(screenTex,texCoord).rgb;
    FragColor = vec4(1-color, 1.0);
}