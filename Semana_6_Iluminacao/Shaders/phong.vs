#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 textc;
layout (location = 3) in vec3 normal;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec4 finalColor;
out vec3 fragPos;
out vec3 scaledNormal;

void main()
{
    //...pode ter mais linhas de código aqui!
    gl_Position = projection * view * model * vec4(position, 1.0);
    finalColor = vec4(color, 1.0);
    fragPos = vec3(model * vec4(position, 1.0));
    scaledNormal = vec3(model * vec4(normal, 1.0));
}