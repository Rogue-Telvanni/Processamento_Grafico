#include "CurveManager.h"

CurveManager::CurveManager()
{
}

CurveManager::~CurveManager()
{
}

void CurveManager::Initialize()
{
    smile = GenerateSmileControlPoints();
    //curvas de catmull
    //left_eye
    smile.catmull_left_eye.controlPoints.push_back(smile.left_eye[0]);
    for (int i=0; i<smile.left_eye.size(); i++)
    {
        smile.catmull_left_eye.controlPoints.push_back(smile.left_eye[i]);
    }
    smile.catmull_left_eye.controlPoints.push_back(smile.left_eye[smile.left_eye.size()-1]);

    // right_eye
    smile.catmull_rigth_eye.controlPoints.push_back(smile.right_eye[0]);
    for (int i=0; i<smile.right_eye.size(); i++)
    {
        smile.catmull_rigth_eye.controlPoints.push_back(smile.right_eye[i]);
    }
    smile.catmull_rigth_eye.controlPoints.push_back(smile.right_eye[smile.right_eye.size()-1]);

    // smile
    smile.catmull_smile.controlPoints.push_back(smile.smile[0]);
    for (int i=0; i<smile.smile.size(); i++)
    {
        smile.catmull_smile.controlPoints.push_back(smile.smile[i]);
    }
    smile.catmull_smile.controlPoints.push_back(smile.smile[smile.smile.size()-1]);

    // face
    smile.catmull_face.controlPoints.push_back(smile.face[0]);
    for (int i=0; i<smile.face.size(); i++)
    {
        smile.catmull_face.controlPoints.push_back(smile.face[i]);
    }
    smile.catmull_face.controlPoints.push_back(smile.face[smile.face.size()-1]);

    smile.VAO_left_eye = GenerateControlPointsBuffer(smile.left_eye);
    smile.VAO_right_eye = GenerateControlPointsBuffer(smile.right_eye);
    smile.VAO_smile = GenerateControlPointsBuffer(smile.smile);
    smile.VAO_face = GenerateControlPointsBuffer(smile.face);

    GenerateCatmullRomCurvePoints(smile.catmull_left_eye, 30);
    GenerateCatmullRomCurvePoints(smile.catmull_rigth_eye, 30);
    GenerateCatmullRomCurvePoints(smile.catmull_smile, 50);
    GenerateCatmullRomCurvePoints(smile.catmull_face, 100);
    smile.VAO_cat_left_eye = GenerateControlPointsBuffer(smile.catmull_left_eye.curvePoints);
    smile.VAO_cat_right_eye = GenerateControlPointsBuffer(smile.catmull_rigth_eye.curvePoints);
    smile.VAO_cat_smile = GenerateControlPointsBuffer(smile.catmull_smile.curvePoints);
    smile.VAO_cat_face = GenerateControlPointsBuffer(smile.catmull_face.curvePoints);



    // curvaBezier.controlPoints = GenerateHeartControlPoints();
    // curvaCatmullRom.controlPoints.push_back(curvaBezier.controlPoints[0]);
    // for (int i=0; i<curvaBezier.controlPoints.size(); i++)
    // {
    //     curvaCatmullRom.controlPoints.push_back(curvaBezier.controlPoints[i]);
    // }
    // curvaCatmullRom.controlPoints.push_back(curvaBezier.controlPoints[curvaBezier.controlPoints.size()-1]);

    // // Gerar pontos da curva de Bézier
    // int numCurvePoints = 100; // Quantidade de pontos por segmento na curva
    // GenerateGlobalBezierCurvePoints(curvaBezier, numCurvePoints);
    // GenerateCatmullRomCurvePoints(curvaCatmullRom, numCurvePoints);
    
    // //smile
    
    // GenerateCatmullRomCurvePoints(curvaCatmullRom, numCurvePoints);


    //Cria a grid de debug
    grid = GenerateGrid(0.1f);
    axes = CreateAxesVAO();

    // //Cria os buffers de geometria dos pontos da curva
    // VAOControl = GenerateControlPointsBuffer(curvaBezier.controlPoints);
    // VAOBezierCurve = GenerateControlPointsBuffer(curvaBezier.curvePoints);
    // VAOCatmullRomCurve = GenerateControlPointsBuffer(curvaCatmullRom.curvePoints);

    cout << curvaBezier.controlPoints.size() << endl;
    cout << curvaBezier.curvePoints.size() << endl;
    cout << curvaCatmullRom.curvePoints.size() << endl;
}

glm::vec3 CurveManager::GetCurvePosition(CurveType type, int pos)
{
    if (type == BEZIER)
    {
        return curvaBezier.curvePoints[pos];
    }

    return smile.catmull_face.curvePoints[pos];
}

std::size_t CurveManager::GetCurveSize(CurveType type)
{
    if (type == BEZIER)
    {
        return curvaBezier.curvePoints.size();
    }

    return smile.catmull_face.curvePoints.size();
}

void CurveManager::Draw(CurveType type, Shader &shader)
{
    shader.Use();
    //Desenhar a grid
    DrawGrid(grid,shader.ID);
    DrawAxesVAO(axes,shader.ID);


    if(type == BEZIER)
    {
        // Desenhar pontos da curva de Bezier e conectar com linhas
        glBindVertexArray(VAOBezierCurve);
        shader.setVec4("finalColor", 1.0f, 0.0f, 1.0f,1.0f); // Magenta para a curva
        glLineWidth(5.0);
        glDrawArrays(GL_LINE_STRIP, 0, curvaBezier.curvePoints.size()); // Desenha a curva como uma linha contínua
    }
    else
    {
        // Desenhar pontos da curva de Catmull e conectar com linhas
        glBindVertexArray(smile.VAO_cat_left_eye);
        shader.setVec4("finalColor", 0.0f, 1.0f, 0.0f,1.0f); // Verde para a curva
        glDrawArrays(GL_LINE_STRIP, 0, smile.catmull_left_eye.curvePoints.size()); // Desenha a curva como uma linha contínua
        
        glBindVertexArray(smile.VAO_cat_right_eye);
        shader.setVec4("finalColor", 0.0f, 1.0f, 0.0f,1.0f); // Verde para a curva
        glDrawArrays(GL_LINE_STRIP, 0, smile.catmull_rigth_eye.curvePoints.size()); // Desenha a curva como uma linha contínua

        glBindVertexArray(smile.VAO_cat_smile);
        shader.setVec4("finalColor", 0.0f, 1.0f, 0.0f,1.0f); // Verde para a curva
        glDrawArrays(GL_LINE_STRIP, 0, smile.catmull_smile.curvePoints.size()); // Desenha a curva como uma linha contínua

        glBindVertexArray(smile.VAO_cat_face);
        shader.setVec4("finalColor", 0.0f, 1.0f, 0.0f,1.0f); // Verde para a curva
        glDrawArrays(GL_LINE_STRIP, 0, smile.catmull_face.curvePoints.size()); // Desenha a curva como uma linha contínua
    }

    // Desenhar pontos de controle maiores e com cor diferenciada
    glBindVertexArray(smile.VAO_left_eye);
    shader.setVec4("finalColor", 0.0f, 0.0f, 0.0f, 1.0f); // Preto para pontos de controle
    glPointSize(12.0f);
    glDrawArrays(GL_POINTS, 0, smile.left_eye.size());

    glBindVertexArray(smile.VAO_right_eye);
    shader.setVec4("finalColor", 0.0f, 0.0f, 0.0f, 1.0f); // Preto para pontos de controle
    glPointSize(12.0f);
    glDrawArrays(GL_POINTS, 0, smile.right_eye.size());

    glBindVertexArray(smile.VAO_smile);
    shader.setVec4("finalColor", 0.0f, 0.0f, 0.0f, 1.0f); // Preto para pontos de controle
    glPointSize(12.0f);
    glDrawArrays(GL_POINTS, 0, smile.smile.size());

    glBindVertexArray(smile.VAO_face);
    shader.setVec4("finalColor", 0.0f, 0.0f, 0.0f, 1.0f); // Preto para pontos de controle
    glPointSize(12.0f);
    glDrawArrays(GL_POINTS, 0, smile.face.size());
}

void CurveManager::InitializeBernsteinMatrix(glm::mat4 &matrix)
{
    //matrix[0] = glm::vec4(1.0f, -3.0f, 3.0f, -1.0f);
    //matrix[1] = glm::vec4(0.0f, 3.0f, -6.0f, 3.0f);
    //matrix[2] = glm::vec4(0.0f, 0.0f, 3.0f, -3.0f);
    //matrix[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    matrix[0] = glm::vec4(-1.0f, 3.0f, -3.0f, 1.0f); // Primeira coluna
    matrix[1] = glm::vec4(3.0f, -6.0f, 3.0f, 0.0f);  // Segunda coluna
    matrix[2] = glm::vec4(-3.0f, 3.0f, 0.0f, 0.0f);  // Terceira coluna
    matrix[3] = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);    // Quarta coluna

}


void CurveManager::InitializeCatmullRomMatrix(glm::mat4 &matrix)
{
    //matrix[0] = glm::vec4(-1.0f, 3.0f, -3.0f, 1.0f);
    //matrix[1] = glm::vec4(2.0f, -5.0f, 4.0f, -1.0f);
    //matrix[2] = glm::vec4(-1.0f, 0.0f, 1.0f, 0.0f);
    //matrix[3] = glm::vec4(0.0f, 2.0f, 0.0f, 0.0f);

    matrix[0] = glm::vec4(-0.5f, 1.5f, -1.5f, 0.5f);  // Primeira linha
    matrix[1] = glm::vec4(1.0f, -2.5f, 2.0f, -0.5f);   // Segunda linha
    matrix[2] = glm::vec4(-0.5f, 0.0f, 0.5f, 0.0f);   // Terceira linha
    matrix[3] = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);    // Quarta linha
}

void CurveManager::GenerateBezierCurvePoints(Curve &curve, int numPoints) {
    curve.curvePoints.clear(); // Limpa quaisquer pontos antigos da curva

    InitializeBernsteinMatrix(curve.M);
    // Calcular os pontos ao longo da curva com base em Bernstein
     // Loop sobre os pontos de controle em grupos de 4

    float piece = 1.0/ (float) numPoints;
    float t;
    for (int i = 0; i < curve.controlPoints.size() - 3; i+=3) {
        
        // Gera pontos para o segmento atual
        for (int j = 0; j < numPoints;j++) {
            t = j * piece;
            
            // Vetor t para o polinômio de Bernstein
            glm::vec4 T(t * t * t, t * t, t, 1);
            
            glm::vec3 P0 = curve.controlPoints[i];
			glm::vec3 P1 = curve.controlPoints[i + 1];
			glm::vec3 P2 = curve.controlPoints[i + 2];
			glm::vec3 P3 = curve.controlPoints[i + 3];

			glm::mat4x3 G(P0, P1, P2, P3);

            // Calcula o ponto da curva multiplicando tVector, a matriz de Bernstein e os pontos de controle
            glm::vec3 point = G * curve.M * T;
            
            curve.curvePoints.push_back(point);
        }
    }
}

void CurveManager::GenerateCatmullRomCurvePoints(Curve &curve, int numPoints) {
    curve.curvePoints.clear(); // Limpa quaisquer pontos antigos da curva

    InitializeCatmullRomMatrix(curve.M);
    // Calcular os pontos ao longo da curva com base em Bernstein
    // Loop sobre os pontos de controle em grupos de 4

    float piece = 1.0 / (float) numPoints;
    float t;
    for (int i = 0; i < curve.controlPoints.size()-3; i++) {
        
        // Gera pontos para o segmento atual
        for (int j = 0; j < numPoints;j++) {
            t = j * piece;
            
            // Vetor t para o polinômio de Bernstein
            glm::vec4 T(t * t * t, t * t, t, 1);
            
            glm::vec3 P0 = curve.controlPoints[i];
			glm::vec3 P1 = curve.controlPoints[i + 1];
			glm::vec3 P2 = curve.controlPoints[i + 2];
			glm::vec3 P3 = curve.controlPoints[i + 3];

			glm::mat4x3 G(P0, P1, P2, P3);


            // Calcula o ponto da curva multiplicando tVector, a matriz de Bernstein e os pontos de controle
            glm::vec3 point = G * curve.M * T;
            curve.curvePoints.push_back(point);
        }
    }
}


GLuint CurveManager::GenerateControlPointsBuffer(vector <glm::vec3> controlPoints)
{
	GLuint VBO, VAO;

	//Geração do identificador do VBO
	glGenBuffers(1, &VBO);

	//Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, controlPoints.size() * sizeof(GLfloat)* 3, controlPoints.data(), GL_STATIC_DRAW);

	//Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);

	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);

	//Atributo posição (x, y, z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice 
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}

GeometryGrid CurveManager::GenerateGrid(float cellSize) {
    GeometryGrid grid;
    grid.dimensions = glm::vec2(2.0f, 2.0f);  // Dimensões totais da grid de -1 a 1 em X e Y
    grid.initialPos = glm::vec2(-1.0f, -1.0f); // Posição inicial de desenho

    std::vector<glm::vec3> vertices;
    std::vector<GLuint> indices;

    int numCells = static_cast<int>(2.0f / cellSize); // Calcula o número de células de 0.1 entre -1 e 1

    // Gera os vértices da grid
    for (int i = 0; i <= numCells; i++) {
        float pos = grid.initialPos.x + i * cellSize;

        // Linhas verticais
        vertices.push_back(glm::vec3(pos, grid.initialPos.y, 0.0f));          // Parte inferior
        vertices.push_back(glm::vec3(pos, grid.initialPos.y + 2.0f, 0.0f));   // Parte superior

        // Linhas horizontais
        vertices.push_back(glm::vec3(grid.initialPos.x, pos, 0.0f));          // Parte esquerda
        vertices.push_back(glm::vec3(grid.initialPos.x + 2.0f, pos, 0.0f));   // Parte direita
    }

    // Índices de elementos para conectar as linhas
    for (int i = 0; i <= numCells; i++) {
        // Índices das linhas verticais
        indices.push_back(i * 2);
        indices.push_back(i * 2 + 1);

        // Índices das linhas horizontais
        indices.push_back((numCells + 1) * 2 + i * 2);
        indices.push_back((numCells + 1) * 2 + i * 2 + 1);
    }

    // Configuração dos buffers VAO e EBO]
    // wolfram
    // desmos
    // geogebra
    glGenVertexArrays(1, &grid.VAO);
    glGenBuffers(1, &grid.EBO);

    glBindVertexArray(grid.VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grid.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    // Configura o layout dos atributos dos vértices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0); // Desvincula o VAO atual

    // Limpeza
    glDeleteBuffers(1, &VBO);

    return grid;
}

void CurveManager::DrawGrid(const GeometryGrid &grid, GLuint shaderID)
{
    glUseProgram(shaderID);

    // Define a cor cinza médio para a grid
    GLint colorLocation = glGetUniformLocation(shaderID, "finalColor");
    glUniform4f(colorLocation, 0.5f, 0.5f, 0.5f, 1.0f); // RGBA: cinza médio

    // Ativa o VAO da grid
    glBindVertexArray(grid.VAO);

    // Largura da grid
    glLineWidth(1.0f);

    // Desenha a grid como linhas usando GL_LINES para contorno
    glDrawElements(GL_LINES, (grid.dimensions.x / 0.1f + 1) * 4, GL_UNSIGNED_INT, 0);

    // Desvincula o VAO
    glBindVertexArray(0);
}

GeometryAxes CurveManager::CreateAxesVAO()
{
    GeometryAxes axes;
    glm::vec3 axisVertices[] = {
        glm::vec3(-1.0f, 0.0f, 0.0f), // X axis start
        glm::vec3(1.0f, 0.0f, 0.0f),  // X axis end
        glm::vec3(0.0f, -1.0f, 0.0f), // Y axis start
        glm::vec3(0.0f, 1.0f, 0.0f)   // Y axis end
    };

    glGenVertexArrays(1, &axes.VAO);
    glGenBuffers(1, &axes.VBO);

    glBindVertexArray(axes.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, axes.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axisVertices), axisVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0); // Unbind VAO
    return axes;
}

void CurveManager::DrawAxesVAO(const GeometryAxes &axes, GLuint shaderID)
{
    glUseProgram(shaderID);

    // Desenha o eixo X em vermelho
    GLint colorLocation = glGetUniformLocation(shaderID, "finalColor");
    glUniform4f(colorLocation, 1.0f, 0.0f, 0.0f, 1.0f); // Cor vermelha

    // Largura dos eixos
    glLineWidth(3.0f);

    glBindVertexArray(axes.VAO);
    glDrawArrays(GL_LINES, 0, 2); // Desenha o eixo X

    // Desenha o eixo Y em azul
    glUniform4f(colorLocation, 0.0f, 0.0f, 1.0f, 1.0f); // Cor azul
    glDrawArrays(GL_LINES, 2, 2);                       // Desenha o eixo Y

    glBindVertexArray(0); // Unbind VAO
}

std::vector<glm::vec3> CurveManager::GenerateHeartControlPoints(int numPoints)
{
    std::vector<glm::vec3> controlPoints;

    // Define number of points for each feature
    int numFacePoints = 100; // For the outer circle
    int numEyePoints = 30;   // For each eye
    int numSmilePoints = 50; // For the smile

    // Outer circle (face boundary)
    float stepFace = 2 * 3.14159f / numFacePoints;
    for (int i = 0; i < numFacePoints; i++)
    {
        float t = i * stepFace;
        float x = cos(t);
        float y = sin(t);
        controlPoints.push_back(glm::vec3(x, y, 0.0f));
    }

    // Left eye
    float eyeRadius = 0.1f;
    float eyeOffsetX = -0.3f, eyeOffsetY = 0.3f;
    float stepEye = 2 * 3.14159f / numEyePoints;
    for (int i = 0; i < numEyePoints; i++)
    {
        float t = i * stepEye;
        float x = eyeOffsetX + eyeRadius * cos(t);
        float y = eyeOffsetY + eyeRadius * sin(t);
        controlPoints.push_back(glm::vec3(x, y, 0.0f));
    }

    // Right eye
    eyeOffsetX = 0.3f; // Adjust x offset for the right eye
    for (int i = 0; i < numEyePoints; i++)
    {
        float t = i * stepEye;
        float x = eyeOffsetX + eyeRadius * cos(t);
        float y = eyeOffsetY + eyeRadius * sin(t);
        controlPoints.push_back(glm::vec3(x, y, 0.0f));
    }

    // Smile (a parametric curve)
    float smileRadius = 0.6f;
    float smileStart = 3.14159f * 0.1f; // Start slightly off horizontal
    float smileEnd = 3.14159f * 0.9f;   // End slightly off horizontal
    float stepSmile = (smileEnd - smileStart) / numSmilePoints;
    for (int i = 0; i <= numSmilePoints; i++)
    {
        float t = smileStart + i * stepSmile;
        float x = smileRadius * cos(t);
        float y = -0.4f + smileRadius * sin(t); // Adjust y for the smile position
        controlPoints.push_back(glm::vec3(x, y, 0.0f));
    }

    return controlPoints;

    // std::vector<glm::vec3> controlPoints;

    // // Define o intervalo para t: de 0 a 2 * PI, dividido em numPoints
    // float step = 2 * 3.14159 / (numPoints-1);

    // for (int i = 0; i < numPoints-1; i++) {
    //     float t = i * step;

    //     // Calcula x(t) e y(t) usando as fórmulas paramétricas
    //     float x = 16 * pow(sin(t), 3);
    //     float y = 13 * cos(t) - 5 * cos(2 * t) - 2 * cos(3 * t) - cos(4 * t);

    //     // Normaliza os pontos para mantê-los dentro de [-1, 1] no espaço 3D
    //     x /= 16.0f;  // Dividir por 16 para normalizar x entre -1 e 1
    //     y /= 16.0f;  // Dividir por 16 para normalizar y aproximadamente entre -1 e 1
    //     y += 0.15;
    //     // Adiciona o ponto ao vetor de pontos de controle
    //     controlPoints.push_back(glm::vec3(x, y, 0.0f));
    // }
    // controlPoints.push_back(controlPoints[0]);

    // return controlPoints;
}

Smile CurveManager::GenerateSmileControlPoints()
{
    Smile smile;
    std::vector<glm::vec3> controlPoints;

    // Define number of points for each feature
    int numFacePoints = 100; // For the outer circle
    int numEyePoints = 30;   // For each eye
    int numSmilePoints = 50; // For the smile

    // Outer circle (face boundary)
    float stepFace = 2 * 3.14159f / numFacePoints;
    for (int i = 0; i < numFacePoints; i++)
    {
        float t = i * stepFace;
        float x = cos(t);
        float y = sin(t);
        smile.face.push_back(glm::vec3(x, y, 0.0f));
    }

    // Left eye
    float eyeRadius = 0.1f;
    float eyeOffsetX = -0.3f, eyeOffsetY = 0.3f;
    float stepEye = 2 * 3.14159f / numEyePoints;
    for (int i = 0; i < numEyePoints; i++)
    {
        float t = i * stepEye;
        float x = eyeOffsetX + eyeRadius * cos(t);
        float y = eyeOffsetY + eyeRadius * sin(t);
        smile.left_eye.push_back(glm::vec3(x, y, 0.0f));
    }

    // Right eye
    eyeOffsetX = 0.3f; // Adjust x offset for the right eye
    for (int i = 0; i < numEyePoints; i++)
    {
        float t = i * stepEye;
        float x = eyeOffsetX + eyeRadius * cos(t);
        float y = eyeOffsetY + eyeRadius * sin(t);
        smile.right_eye.push_back(glm::vec3(x, y, 0.0f));
    }

    // Smile (a parametric curve)
    float smileRadius = 0.6f;
    float smileStart = 3.14159f * 0.1f; // Start slightly off horizontal
    float smileEnd = 3.14159f * 0.9f;   // End slightly off horizontal
    float stepSmile = (smileEnd - smileStart) / numSmilePoints;
    for (int i = 0; i <= numSmilePoints; i++)
    {
        float t = smileStart + i * stepSmile;
        float x = smileRadius * cos(t);
        float y = -0.4f + smileRadius * sin(t); // Adjust y for the smile position
        smile.smile.push_back(glm::vec3(x, y, 0.0f));
    }

    return smile;
}

void CurveManager::GenerateGlobalBezierCurvePoints(Curve &curve, int numPoints) {
    curve.curvePoints.clear(); // Limpa quaisquer pontos antigos da curva

    int n = curve.controlPoints.size() - 1; // Grau da curva
    float t;
    float piece = 1.0f / (float)numPoints;

    for (int j = 0; j <= numPoints; ++j) {
        t = j * piece;
        glm::vec3 point(0.0f); // Ponto na curva

        // Calcula o ponto da curva usando a fórmula de Bernstein
        for (int i = 0; i <= n; ++i) {
            // Coeficiente binomial
            float binomialCoeff = (float) (tgamma(n + 1) / (tgamma(i + 1) * tgamma(n - i + 1)));
            // Polinômio de Bernstein
            float bernsteinPoly = binomialCoeff * pow(1 - t, n - i) * pow(t, i);
            // Soma ponderada dos pontos de controle
            point += bernsteinPoly * curve.controlPoints[i];
        }

        curve.curvePoints.push_back(point);
    }
}