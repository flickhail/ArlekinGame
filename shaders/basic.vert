#version 330 core

layout(location = 0) in vec3 Position;
//layout(location = 1) in vec3 Color;
layout(location = 1) in vec2 TexPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragColor;
out vec2 FragTexPos;

void main()
{
	gl_Position = projection * view * model * vec4(Position, 1.0);
	//FragColor = Color;
	FragTexPos = TexPos;
}