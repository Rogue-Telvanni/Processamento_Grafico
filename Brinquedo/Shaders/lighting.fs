#version 430

//structs

// dados do material
struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

// dados da luz
struct Light{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

//propriedaes do vertex shader
in vec2 TexCoord;
in vec3 ScaledNormal;
in vec3 FragPos;

// cor final
out vec4 frag_color;

//Propriedades da câmera
uniform vec3 cameraPos;
uniform bool useColor;

// materiais e luz
uniform Material material;
uniform Light light;

//Buffer da textura
uniform sampler2D texture_diffuse1;
uniform vec3 Color;

void main()
{
    // ambient
    vec3 ambient = light.ambient * material.ambient;
  	
    // diffuse 
    vec3 normal = normalize(ScaledNormal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    
    // specular
    vec3 cameraDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(cameraDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  
        
    vec4 texColor;
    if(useColor)
        texColor = vec4(Color, 1.0);
    else
        texColor = texture(texture_diffuse1,TexCoord);

    vec3 result = (ambient + diffuse + specular) * vec3(texColor) ;
    frag_color = vec4(result,1.0);
}