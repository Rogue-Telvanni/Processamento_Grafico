#include "SceneManager.h"
#include <filesystem>


GLuint WIDTH, HEIGHT;

bool rotateX = false, rotateY = false, rotateZ = false;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool firstMouse = true;
float lastX = 0, lastY = 0;

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

//object to render
static int selected_obj = 0;


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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);  
	glfwSetScrollCallback(window, scroll_callback);
	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	// Obtendo as informações de versão
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Compilando e buildando o programa de shader
	addShader("Shaders/phong.vs", "Shaders/phong.fs");
	
	setupScene();
}

void SceneManager::setupScene()
{
	glEnable(GL_DEPTH_TEST);

	// Gerando um buffer simples, com a geometria de um triângulo
	Shader shader = *shaders[0];
	//loadObjs();
	models.push_back(Model(std::filesystem::path("assets/cube.obj")));
	//obj.VAO = loadSimpleOBJ("Suzanne.obj",obj.nVertices);

	shader.Use();

	//Propriedades da superfície
	shader.setFloat("ka",0.2);
	shader.setFloat("ks", 0.5);
	shader.setFloat("kd", 0.5);
	shader.setFloat("q", 10.0);

	//Propriedades da fonte de luz
	shader.setVec3("lightPos",-2.0, 10.0, 3.0); 
	shader.setVec3("lightColor",1.0, 1.0, 1.0);
	this->model = glm::mat4(1); //matriz identidade;

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
	Shader shader = *shaders[0];
	
	Model obj = models[0];
	// matrix de projeção
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
	shader.setMat4("projection", glm::value_ptr(projection));

	// propriedades da camera
	//  Matrix de view
	glm::mat4 view = camera.GetViewMatrix();
	shader.setMat4("view", glm::value_ptr(view));

	//Object obj = objects[selected_obj];
	glm::mat4 model = glm::mat4(1);
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

	shader.setMat4("model", glm::value_ptr(model));
	obj.Draw(shader);

	// // Chamada de desenho - drawcall
	// glBindVertexArray(obj.VAO);
	// glDrawArrays(GL_TRIANGLES, 0, obj.nVertices);
}

void SceneManager::update()
{
	return;
}

void SceneManager::finish()
{
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &objects[selected_obj].VAO);
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
		float angle = (GLfloat)glfwGetTime();
		deltaTime = angle - lastFrame;
		lastFrame = angle;

		// Update method(s)
		update();

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
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if(action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_X)
		{
			rotateX = true;
			rotateY = false;
			rotateZ = false;
		}

		if (key == GLFW_KEY_Y)
		{
			rotateX = false;
			rotateY = true;
			rotateZ = false;
		}

		if (key == GLFW_KEY_Z)
		{
			rotateX = false;
			rotateY = false;
			rotateZ = true;
		}

		// object selection, only 3 obj for now
		if (key == GLFW_KEY_1)
			selected_obj = 0;
		if (key == GLFW_KEY_2)
			selected_obj = 1;
		if (key == GLFW_KEY_3)
			selected_obj = 2;
		if (key == GLFW_KEY_4)
			selected_obj = 3;

		if (key == GLFW_KEY_W)
			camera.ProcessKeyboard(FORWARD, deltaTime);
		if (key == GLFW_KEY_S)
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		if (key == GLFW_KEY_A)
			camera.ProcessKeyboard(LEFT, deltaTime);
		if (key == GLFW_KEY_D)
			camera.ProcessKeyboard(RIGHT, deltaTime);
	}
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
	//cube
	Object c_obj;
	c_obj.VAO = loadSimpleOBJ("cube.obj",c_obj.nVertices);
	objects.push_back(c_obj);
	cout << "load cube"<< endl;

	Object su_obj;
	su_obj.VAO = loadSimpleOBJ("Suzanne.obj",su_obj.nVertices);
	objects.push_back(su_obj);
	cout << "load suzanne"<< endl;

	Object na_obj;
	na_obj.VAO = loadSimpleOBJ("nave.obj",na_obj.nVertices);
	objects.push_back(na_obj);
	cout << "load nave"<< endl;

	Object car_obj;
	na_obj.VAO = loadSimpleOBJ("car.obj",na_obj.nVertices);
	objects.push_back(na_obj);
	cout << "load car"<< endl;
}

int SceneManager::loadSimpleOBJ(string filePath, int &nVertices)
{
	vector <glm::vec3> vertices;
	vector <glm::vec2> texCoords;
	vector <glm::vec3> normals;
	vector <GLfloat> vBuffer;

	glm::vec3 color = glm::vec3(1.0, 0.0, 0.0);

	ifstream arqEntrada;

	arqEntrada.open(filePath.c_str());
	if (arqEntrada.is_open())
	{
		//Fazer o parsing
		string line;
		while (!arqEntrada.eof())
		{
			getline(arqEntrada,line);
			istringstream ssline(line);
			string word;
			ssline >> word;
			if (word == "v")
			{
				glm::vec3 vertice;
				ssline >> vertice.x >> vertice.y >> vertice.z;
				//cout << vertice.x << " " << vertice.y << " " << vertice.z << endl;
				vertices.push_back(vertice);

			}
			if (word == "vt")
			{
				glm::vec2 vt;
				ssline >> vt.s >> vt.t;
				//cout << vertice.x << " " << vertice.y << " " << vertice.z << endl;
				texCoords.push_back(vt);

			}
			if (word == "vn")
			{
				glm::vec3 normal;
				ssline >> normal.x >> normal.y >> normal.z;
				//cout << vertice.x << " " << vertice.y << " " << vertice.z << endl;
				normals.push_back(normal);

			}
			else if (word == "f")
			{
				while (ssline >> word) 
				{
					int vi, ti, ni;
					istringstream ss(word);
    				std::string index;

    				// Pega o índice do vértice
    				std::getline(ss, index, '/');
    				vi = std::stoi(index) - 1;  // Ajusta para índice 0

    				// Pega o índice da coordenada de textura
    				std::getline(ss, index, '/');
    				ti = std::stoi(index) - 1;

    				// Pega o índice da normal
    				std::getline(ss, index);
    				ni = std::stoi(index) - 1;

					//Recuperando os vértices do indice lido
					vBuffer.push_back(vertices[vi].x);
					vBuffer.push_back(vertices[vi].y);
					vBuffer.push_back(vertices[vi].z);
					
					//Atributo cor
					vBuffer.push_back(color.r);
					vBuffer.push_back(color.g);
					vBuffer.push_back(color.b);

					//Atributo coordenada de textura
					vBuffer.push_back(texCoords[ti].s);
					vBuffer.push_back(texCoords[ti].t);

					//Atributo vetor normal
					vBuffer.push_back(normals[ni].x);
					vBuffer.push_back(normals[ni].y);
					vBuffer.push_back(normals[ni].z);
					
        			
        			// Exibindo os índices para verificação
       				// std::cout << "v: " << vi << ", vt: " << ti << ", vn: " << ni << std::endl;
    			}
				
			}
		}

		arqEntrada.close();

		cout << "Gerando o buffer de geometria..." << endl;
		GLuint VBO, VAO;

	//Geração do identificador do VBO
	glGenBuffers(1, &VBO);

	//Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, vBuffer.size() * sizeof(GLfloat), vBuffer.data(), GL_STATIC_DRAW);

	//Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);

	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);
	
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se está normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 
	
	//Atributo posição (x, y, z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo cor (r, g, b)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//Atributo coordenada de textura - s, t
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(6*sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	//Atributo vetor normal - x, y, z
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(8*sizeof(GLfloat)));
	glEnableVertexAttribArray(3);

	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice 
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	nVertices = vBuffer.size() / 2;
	return VAO;

	}
	else
	{
		cout << "Erro ao tentar ler o arquivo " << filePath << endl;
		return -1;
	}
}
