// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Shader
#include "Shader.h"

// STL
#include <vector>
#include <random>
#include <algorithm>

struct Curve {
    std::vector<glm::vec3> controlPoints; // Pontos de controle da curva
    std::vector<glm::vec3> curvePoints;   // Pontos da curva
    glm::mat4 M;          // Matriz dos coeficientes da curva
};

struct GeometryGrid {
    GLuint VAO, EBO;
    glm::vec2 dimensions;   // Dimensões da grade
    glm::vec2 initialPos;   // Posição inicial da grade
};

struct GeometryAxes {
    GLuint VAO;
    GLuint VBO;
};

struct Smile{
    std::vector<glm::vec3> right_eye;
    std::vector<glm::vec3> left_eye;
    std::vector<glm::vec3> smile;
    std::vector<glm::vec3> face;

    Curve catmull_rigth_eye;
    Curve catmull_left_eye;
    Curve catmull_smile;
    Curve catmull_face;

    GLuint VAO_right_eye;
    GLuint VAO_left_eye;
    GLuint VAO_smile;
    GLuint VAO_face;

    GLuint VAO_cat_right_eye;
    GLuint VAO_cat_left_eye;
    GLuint VAO_cat_smile;
    GLuint VAO_cat_face;
};

class CurveManager
{
public:
	CurveManager();
	~CurveManager();

	void Draw(Shader &shader);
    glm::vec3 GetCurvePosition(int pos);
    std::size_t GetCurveSize();
	void Initialize();
private:
	/// Outras funções
    void InitializeBernsteinMatrix(glm::mat4x4 &matrix);
    void GenerateBezierCurvePoints(Curve &curve, int numPoints);
    void InitializeCatmullRomMatrix(glm::mat4x4 &matrix);
    void GenerateCatmullRomCurvePoints(Curve &curve, int numPoints);
    void DisplayCurve(const Curve &curve);
    GLuint GenerateControlPointsBuffer(vector<glm::vec3> controlPoints);

    // Funções para geração da grid
    GeometryGrid GenerateGrid(float cellSize = 0.1f);
    void DrawGrid(const GeometryGrid &grid, GLuint shaderID);
    GeometryAxes CreateAxesVAO();
    void DrawAxesVAO(const GeometryAxes &axes, GLuint shaderID);
    std::vector<glm::vec3> GenerateHeartControlPoints(int numPoints = 20);
    Smile GenerateSmileControlPoints();

    void GenerateGlobalBezierCurvePoints(Curve &curve, int numPoints);

    Curve curvaBezier;
    Curve curvaCatmullRom;

    GeometryGrid grid;
    GeometryAxes axes;

    GLuint VAOControl;
    GLuint VAOBezierCurve;
    GLuint VAOCatmullRomCurve;

    Smile smile;
};