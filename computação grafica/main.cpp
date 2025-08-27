#include <stdio.h>
#include <gl\glew.h>
#include <GLFW/glfw3.h>
#include <random>
const GLint WIDTH = 800, HEIGHT = 600;     //LARGURA E ALTURA GLOBAL
GLuint VAO, VBO, shaderProgram;
//VAO é tipo um array sendo o triangulo,VBO é tipo um vertice dele uma propriedade do VAO
//programa em glsl que gera o vertice
static const char* vertexShader = "           \n\
#version 330                                  \n\
                                              \n\
layout(location=0) in vec2 pos;               \n\
                                              \n\
void main() {                                 \n\
   gl_Position = vec4(pos.x, pos.y, 0.0,1.0); \n\
}                                             \n\
";
//programa em glsl que fragmenta
static const char* fragmentShader = "              \n\
#version 330                                       \n\
                                                   \n\
uniform vec3 triColor;                             \n\
out vec4 color;                                    \n\
void main() {                                      \n\
   color = vec4(triColor,1.0);                     \n\
}                                                  \n\
";


void criaTriangulo() {
	GLfloat vertices[] = {
		0.0f,1.0f, //vertice 1
		-1.0f,-1.0f,//vertice 2
		1.0f,-1.0f //vertice 3
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	  
	 glGenBuffers(1, &VBO);
	 glBindBuffer(GL_ARRAY_BUFFER, VBO);

	   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,GL_STATIC_DRAW);
	   glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	   glEnableVertexAttribArray(0); //location

	 glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}


void adicionaTriangulo(GLuint program,const char* shaderCode,GLenum type) {
	GLuint _shader = glCreateShader(type);

	//converte char para GL_char
	const GLchar* code[1];
	code[0] = shaderCode;
	
	glShaderSource(_shader, 1, code, NULL);
	glCompileShader(_shader);
	//tratar os erros

	glAttachShader(program, _shader);
}

void adicionaPrograma() {
	shaderProgram = glCreateProgram();
	if (!shaderProgram) {
		printf("erro ao criar o programa!");
		return;
	}
	adicionaTriangulo(shaderProgram,vertexShader, GL_VERTEX_SHADER);
	adicionaTriangulo(shaderProgram,fragmentShader,GL_FRAGMENT_SHADER);

	glLinkProgram(shaderProgram);
}

GLfloat random_float() {
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_real_distribution<> distr(0.0, 1.0);
	GLfloat random_float = distr(rng);
	return random_float;
}
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

	criaTriangulo();
	adicionaPrograma();

	while (!glfwWindowShouldClose(window)) {

		//GLfloat red = random_float();
	    //GLfloat green = random_float();
		//GLfloat blue = random_float();
		//aleatorio

		//cor de fundo da janela
		glClearColor(0.0f, 0.0f,0.0f,1.0f);
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);

		//altera cor do triangulo
		GLint uniformColor = glGetUniformLocation(shaderProgram, "triColor");
		glUniform3f(uniformColor, 0.0, 1.0, 0.5);
		//glUniform3f(uniformColor, red, green, blue);

		//desenhando o triangulo
		glUseProgram(shaderProgram);

		glBindVertexArray(VAO);

			glDrawArrays(GL_TRIANGLES,0, 3 );//triangulo, começando na posicao 0 ,numero de pontos 3
			glBindVertexArray(0);



		glfwSwapBuffers(window);
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}