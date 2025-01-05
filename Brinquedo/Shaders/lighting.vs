#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texc;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

//Variáveis que irão para o fragment shader
out vec2 Text_Coord;
out vec3 Scaled_Normal;
out vec3 Frag_Pos;

void main()
{
	//...pode ter mais linhas de código aqui!
    Frag_Pos = vec3(model * vec4(position, 1.0));
    Scaled_Normal = mat3(transpose(inverse(model))) * normal;  
    Text_Coord = texc;
    gl_Position = projection * view * vec4(Frag_Pos, 1.0);
}