#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texc;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

//Variáveis que irão para o fragment shader
out vec2 TexCoord;
out vec3 ScaledNormal;
out vec3 FragPos;

void main()
{
	//...pode ter mais linhas de código aqui!
	gl_Position = projection * view * model * vec4(position, 1.0);
    TexCoord = vec2(texc.s, 1 - texc.t);
    FragPos = vec3(model * vec4(position, 1.0));
    ScaledNormal = vec3(model * vec4(normal, 1.0));
}