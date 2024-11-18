#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texc;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

//Variáveis que irão para o fragment shader
out vec2 texCoord;
out vec3 scaledNormal;
out vec3 fragPos;

void main()
{
	//...pode ter mais linhas de código aqui!
	gl_Position = projection * view * model * vec4(position, 1.0);
    texCoord = vec2(texc.s, 1 - texc.t);
    fragPos = vec3(model * vec4(position, 1.0));
    scaledNormal = vec3(model * vec4(normal, 1.0));
}