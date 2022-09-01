#version 330 core

in vec2 FragTexPos;

uniform sampler2D texSample1;
uniform sampler2D texSample2;

out vec4 OutColor;

void main()
{
	OutColor = mix(texture(texSample1, FragTexPos), texture(texSample2, FragTexPos), 0.2);
}