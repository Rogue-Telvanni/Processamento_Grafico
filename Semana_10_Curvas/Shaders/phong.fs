#version 430

in vec2 texCoord;
in vec3 scaledNormal;
in vec3 fragPos;

out vec4 fragColor;

//Propriedades da superficie
uniform float ka, kd, ks, q;

//Propriedades da fonte de luz
uniform vec3 lightPos, lightColor;

//Propriedades da câmera
uniform vec3 cameraPos;
uniform bool useColor;

//Buffer da textura
uniform sampler2D texture_diffuse1;
uniform vec3 color;

// dados do material
uniform struct{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
} material;

void main()
{
    //Coeficiente luz ambiente
    vec3 ambient = material.ambient * lightColor;

    //Coeficiente reflexão difusa
    vec3 diffuse;
    vec3 N = normalize(scaledNormal);
    vec3 L = normalize(lightPos - fragPos);
    float diff = max(dot(N,L),0.0);
    diffuse = diff * lightColor;

    //Coeficiente reflexão especular
    vec3 specular;
    vec3 R = normalize(reflect(-L,N));
    vec3 V = normalize(cameraPos - fragPos);
    float spec = max(dot(R,V),0.0);
    spec = pow(spec, material.shininess);
    specular = material.specular * spec * lightColor;

    vec4 texColor;
    if(useColor)
        texColor = vec4(color, 1.0);
    else
        texColor = texture(texture_diffuse1,texCoord);

    vec3 result = (ambient + diffuse) * vec3(texColor) + specular;

    fragColor = vec4(result,1.0);
    //fragColor = texture(texture_diffuse1, texColor);
}