// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Shader
#include "Shader.h"


class Triangle
{

public:
    Triangle();
	~Triangle();

    void Initialize();
    void Draw(Shader &shader);
    void Crawl(Shader &shader, glm::vec3 position, glm::vec3 dimensions, float angle, glm::vec3 axis, float color_delta);

private:
    
    unsigned int VAO, VBO;
};