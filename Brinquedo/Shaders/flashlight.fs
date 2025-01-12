

#version 450 core
out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse_1;
    sampler2D texture_specular_1;    
    float shininess;
}; 

struct Light {
    vec3 position;  
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
};

in vec3 Frag_Pos;  
in vec3 Scaled_Normal;  
in vec2 Text_Coord;
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    // check if lighting is inside the spotlight cone
  
    vec3 lightDir = normalize(light.position - Frag_Pos);

    // ambient
    vec3 ambient = light.ambient * texture(material.texture_diffuse_1, Text_Coord).rgb;
    
    // diffuse 
    vec3 norm = normalize(Scaled_Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse_1, Text_Coord).rgb;  
    
    // specular
    vec3 viewDir = normalize(viewPos - Frag_Pos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.texture_specular_1, Text_Coord).rgb;  
    
    // spotlight (soft edges)
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;

    // attenuation
    float distance    = length(light.position - Frag_Pos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    ambient  *= attenuation; 
    diffuse   *= attenuation;
    specular *= attenuation;   
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
   
} 

