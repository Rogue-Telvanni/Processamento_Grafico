//C++
#include <iostream>
#include <string>
#include <assert.h>
#include <vector>
#include <fstream>
#include <sstream>


// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Shader
#include "Shader.h"
#include "Camera.h"

//OBJECT
#include "object.cpp"
#include <Model.h>

class SceneManager
{
public:
	SceneManager();
	~SceneManager();
	void initialize(GLuint width, GLuint height);
	void initializeGraphics();

	void addShader(string vFilename, string fFilename);
    // Protótipos das funções
    int loadSimpleOBJ(string filePath, int &nVertices);
    void loadObjs();
    //GLFW callbacks
	static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
	static void mouse_callback(GLFWwindow *window, double xposIn, double yposIn);
	static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
	static void framebuffer_size_callback(GLFWwindow *window, int width, int height);

	void update();
	void render();
	void run();
	void finish();


	// scene
	void setupScene();
	void setupCamera2D();
private:

	//GFLW window
	GLFWwindow *window;

	//Array de programas de shader
	vector <Shader*> shaders;

	//Transformations - Model Matrix
	glm::mat4 model;
	glm::mat4 projection;
	glm::mat4 view;

    //array de objetos para renderizar
    vector <Object> objects;
	vector <Model> models;
};