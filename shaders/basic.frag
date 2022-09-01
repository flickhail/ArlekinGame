#version 330 core

in vec3 FragColor;
in vec2 FragTexPos;

uniform sampler2D texSample1;
uniform sampler2D texSample2;

out vec4 OutColor;

void main()
{
	OutColor = mix(texture(texSample1, FragTexPos), texture(texSample2, FragTexPos), 0.2);
	//OutColor = vec4(vec3(gl_FragCoord.z), 1.0);
}