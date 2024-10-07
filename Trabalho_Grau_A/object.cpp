#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// GLAD
#include <glad/glad.h>

struct Object
{
	GLuint VAO; //Índice do buffer de geometria
	int nVertices; //nro de vértices
	glm::mat4 model; //matriz de transformações do objeto
};