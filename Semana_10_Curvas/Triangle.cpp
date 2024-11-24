//Triangle
#include "Triangle.h"

Triangle::Triangle(){}

Triangle::~Triangle(){}

void Triangle::Initialize()
{
    // Aqui setamos as coordenadas x, y e z do triângulo e as armazenamos de forma
    // sequencial, já visando mandar para o VBO (Vertex Buffer Objects)
    // Cada atributo do vértice (coordenada, cores, coordenadas de textura, normal, etc)
    // Pode ser arazenado em um VBO único ou em VBOs separados
    GLfloat vertices[] = {
        // x    y    z
        // T0
        -0.5, -0.5, 0.0, // v0
        0.5, -0.5, 0.0,  // v1
        0.0, 0.5, 0.0,   // v2
    };

    // Geração do identificador do VBO
    glGenBuffers(1, &VBO);
    // Faz a conexão (vincula) do buffer como um buffer de array
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Envia os dados do array de floats para o buffer da OpenGl
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Geração do identificador do VAO (Vertex Array Object)
    glGenVertexArrays(1, &VAO);
    // Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
    // e os ponteiros para os atributos
    glBindVertexArray(VAO);
    // Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando:
    //  Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
    //  Numero de valores que o atributo tem (por ex, 3 coordenadas xyz)
    //  Tipo do dado
    //  Se está normalizado (entre zero e um)
    //  Tamanho em bytes
    //  Deslocamento a partir do byte zero
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    // Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice
    // atualmente vinculado - para que depois possamos desvincular com segurança
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
    glBindVertexArray(0);
}

void Triangle::Draw(Shader &shader)
{
    shader.Use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

void Triangle::Crawl(Shader &shader, glm::vec3 position, glm::vec3 dimensions, float angle, glm::vec3 axis, float color_delta)
{
    // Matriz de modelo: transformações na geometria (objeto)
    glm::mat4 model = glm::mat4(1); // matriz identidade
    // Translação
    model = glm::translate(model, position);
    // Rotação
    model = glm::rotate(model, angle, axis);
    // Escala
    model = glm::scale(model, dimensions);
    shader.setMat4("model", glm::value_ptr(model));
    shader.setVec4("finalColor", glm::cos(color_delta), glm::sin(color_delta), glm::sin(color_delta), 1.0f);
}