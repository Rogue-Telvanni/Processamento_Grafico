#include "SceneManager.h"
#include <filesystem>


GLuint WIDTH, HEIGHT;
int FPS = 60;
bool rotateX = false, rotateY = false, rotateZ = false;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool firstMouse = true;
float lastX = 0, lastY = 0;

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;


// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::initialize(GLuint w, GLuint h)
{
	WIDTH = w;
	HEIGHT = h;

	// GLFW - GLEW - OPENGL general setup -- TODO: config file
	initializeGraphics();
}

void SceneManager::initializeGraphics()
{
	// Inicialização da GLFW
	glfwInit();

	// Criação da janela GLFW
	this->window = glfwCreateWindow(WIDTH, HEIGHT, "Camera", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);  
	glfwSetScrollCallback(window, scroll_callback);
	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	stbi_set_flip_vertically_on_load(true);

	// Obtendo as informações de versão
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;
	
	setupScene();
}

void SceneManager::setupScene()
{
	glEnable(GL_DEPTH_TEST);

	// Compilando e buildando o programa de shader
	addShader("Shaders/lighting.vs", "Shaders/lighting.fs");
	addShader("Shaders/cube_light.vs", "Shaders/cube_light.fs");
	// Gerando um buffer simples, com a geometria de um triângulo
	loadObjs();

	// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
	int width, height;
	glfwGetFramebufferSize(this->window, &width, &height);
	glViewport(0, 0, width, height);
}

void SceneManager::render()
{
	// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
	// Limpa o buffer de cor
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // cor de fundo
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLineWidth(10);

	Shader lighting_shader = *shaders[0];
	lighting_shader.Use();
    lighting_shader.setVec4("Color", glm::vec4(1.f, 0.f, 0.f, 1.0));

	for (int i = 0; i < models.size(); i++)
	{
		Model obj = models[i];

		glm::mat4 model = glm::mat4(1);

		filesystem::path p(obj.directory);
		cout << p.stem() << endl;
		if (p.stem() == "Naves")
			model = glm::translate(model, glm::vec3(0.5f, 0.0f, -8.0f));
		else if (p.stem() == "obj")
		{
			model = glm::translate(model, glm::vec3(-2.5f, 0.0f, -3.0f));
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		}
		else if (p.stem() == "Suzannes")
			model = glm::translate(model, glm::vec3(-1.1f, 0.0f, -3.0f));

		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		if (rotateX)
		{
			model = glm::rotate(model, lastFrame, glm::vec3(1.0f, 0.0f, 0.0f));
		}
		else if (rotateY)
		{
			model = glm::rotate(model, lastFrame, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		else if (rotateZ)
		{
			model = glm::rotate(model, lastFrame, glm::vec3(0.0f, 0.0f, 1.0f));
		}

		lighting_shader.setMat4("model", glm::value_ptr(model));

		obj.Draw(lighting_shader);
	}

	Shader cube_shader = *shaders[1];
	cube_shader.Use();
	glm::mat4 model = glm::mat4(1);
	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.3f));
	cube_shader.setMat4("model", glm::value_ptr(model));
	Model obj = models[models.size() - 1];
	obj.Draw(cube_shader);
}

void SceneManager::update(GLFWwindow *window)
{
	processInput(window);

	Shader lighting_shader = *shaders[0];
	lighting_shader.Use();
	// light properties
	glm::vec3 lightColor;
	lightColor.x = static_cast<float>(sin(lastFrame * 2.0));
	lightColor.y = static_cast<float>(sin(lastFrame * 0.7));
	lightColor.z = static_cast<float>(sin(lastFrame * 1.3));
	glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);	 // decrease the influence
	glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
	lighting_shader.setVec3("light.ambient", ambientColor);
	lighting_shader.setVec3("light.diffuse", diffuseColor);
	lighting_shader.setVec3("light.specular", glm::vec3(1.0f));

	// matrix de projeção
	this->projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
	lighting_shader.setMat4("projection", glm::value_ptr(projection));

	// propriedades da camera
	//  Matrix de view
	this->view = camera.GetViewMatrix();
	lighting_shader.setMat4("view", glm::value_ptr(view));

	Shader cube_shader = *shaders[1];
	cube_shader.Use();
    cube_shader.setVec4("Color", glm::vec4(lightColor, 1.0));
	cube_shader.setMat4("projection", glm::value_ptr(projection));
	cube_shader.setMat4("view", glm::value_ptr(view));

	return;
}

void SceneManager::finish()
{
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
}

void SceneManager::run()
{
	// GAME LOOP
	while (!glfwWindowShouldClose(window))
	{

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		float frameTime = (GLfloat)glfwGetTime();
		deltaTime = frameTime - lastFrame;
		lastFrame = frameTime;

		// if (deltaTime < 1 / FPS)
		// 	continue;

		// Update method(s)
		update(window);

		// Render scene
		render();

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
}

void SceneManager::addShader(string vFilename, string fFilename)
{
	Shader *shader = new Shader(vFilename.c_str(), fFilename.c_str());
	shaders.push_back(shader);
}

// Função de callback de teclado - só pode ter uma instância (deve ser estática se
// estiver dentro de uma classe) - É chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void SceneManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	/*
	valid only once on press the call back will only trigger after the user
	lifts the finger and clicks again
	*/ 

	if(action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_X)
		{
			rotateX = !rotateX;
			rotateY = false;
			rotateZ = false;
		}

		if (key == GLFW_KEY_Y)
		{
			rotateX = false;
			rotateY = !rotateY;
			rotateZ = false;
		}

		if (key == GLFW_KEY_Z)
		{
			rotateX = false;
			rotateY = false;
			rotateZ = !rotateZ;
		}
		
	}
}


void SceneManager::processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void SceneManager::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void SceneManager::mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

	float x = (2.0f * xpos) /WIDTH - 1.0f;
	float y = 1.0f - (2.0f * ypos) / HEIGHT;
	float z = 1.0f;
	glm::vec3 ray_nds = glm::vec3(x, y, z);

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void SceneManager::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// special function for object loading 

void SceneManager::loadObjs()
{
	models.push_back(Model(std::filesystem::path("../assets/Naves/LightCruiser05.obj")));
	cout << "load cruiser"<< endl;

	models.push_back(Model(std::filesystem::path("../assets/aratwearingabackpack/obj/model.obj")));
	cout << "load rato"<< endl;

	models.push_back(Model(std::filesystem::path("../assets/Suzannes/SuzanneHigh.obj")));
	cout << "load SuzanneHigh"<< endl;
	
	models.push_back(Model(std::filesystem::path("../assets/cube.obj")));
	cout << "load cube"<< endl;
	// models.push_back(Model(std::filesystem::path("../assets/Carro/1963 Volkswagen Beetle/1963 Volkswagen Beetle.obj")));
	// cout << "load Carro"<< endl;
}