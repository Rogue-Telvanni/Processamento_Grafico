#version 450

in vec4 finalColor;
in vec3 fragPos;
in vec3 scaledNormal;

out vec4 color;

//Propriedades da superfície
uniform float ka;
uniform float kd;
uniform float ks;
uniform float q;
//Propriedades da fonte de luz
uniform vec3 lightPos;
uniform vec3 lightColor;
//Posiçãoo da Câmera
uniform vec3 cameraPos;

void main()
{
    color = finalColor;
}