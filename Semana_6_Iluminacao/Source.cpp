#include "SceneManager.h"

const GLuint WIDTH = 1920, HEIGHT = 1080;
// Função MAIN
int main()
{
	SceneManager* scene = new SceneManager;
	scene->initialize(WIDTH, HEIGHT);
	scene->run();

	scene->finish();

	return 0;
}