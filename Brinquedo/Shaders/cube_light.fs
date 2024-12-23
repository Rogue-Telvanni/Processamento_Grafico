#version 430 core
out vec4 FragColor;
uniform vec4 Color;

void main()
{
    FragColor = Color; // set all 4 vector values to 1.0
}