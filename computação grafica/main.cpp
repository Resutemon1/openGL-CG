#include <stdio.h>
#include <gl\glew.h>
#include <GLFW/glfw3.h>
const GLint WIDTH = 800, HEIGHT = 600;     //LARGURA E ALTURA GLOBAL
int main() {
	//iniciar o GLFW
	if (!glfwInit()) {
		printf("GLFW Nao foi iniciado");
		glfwTerminate();
		return 1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//GLFW -> GLFW
	//GL -> GLEW
	GLFWwindow *window = glfwCreateWindow(WIDTH,HEIGHT,"OpenGL",NULL,NULL);
	if (!window) {
		printf("erro ao carregar a janela");
		glfwTerminate();
		return 1;
	}
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(window,&bufferWidth,&bufferHeight);
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		printf("erro ao iniciar o glew");
		glfwDestroyWindow(window);
		glfwTerminate();
		return 1;
	}

	glViewport(0, 0, bufferWidth, bufferHeight);
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0f, 1.0f,1.0f,1.0f);
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
	}
	//aleatorio
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}