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

enum CurveType
{
    CAT_MULL = 0,
    BEZIER = 1
};

class CurveManager
{
public:
	CurveManager();
	~CurveManager();

	void Draw(CurveType type, Shader &shader);
    glm::vec3 GetCurvePosition(CurveType type, int pos);
    std::size_t GetCurveSize(CurveType type);
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

    void GenerateGlobalBezierCurvePoints(Curve &curve, int numPoints);

    Curve curvaBezier;
    Curve curvaCatmullRom;

    GeometryGrid grid;
    GeometryAxes axes;

    GLuint VAOControl;
    GLuint VAOBezierCurve;
    GLuint VAOCatmullRomCurve;
};