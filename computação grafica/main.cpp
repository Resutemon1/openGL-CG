#include <stdio.h>
#include <gl\glew.h>
#include <GLFW/glfw3.h>
#include <random>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const GLint WIDTH = 800, HEIGHT = 600;     //LARGURA E ALTURA GLOBAL
GLuint VAO, VBO, shaderProgram,IBO;

float toRadians = 3.1415 / 180;

bool direction = false;
//variaveis pro x
float triOffset = 0.0f, trifOffsetMax = 0.7, triOffsetMin = -0.7f,triIncrement = 0.01f;
//variaveis pro tamanho
bool directionSize = false;
float triOffsetSize = 0.2f, triOffsetSizeMax = 1.2f, triOffsetSizeMin = 0.2f, triOffsetSizeIncrement = 0.01f;
float triCurrentAngle = 0.0f,triAngleIncrement = 1.0f;
//VAO é tipo um array sendo o triangulo,VBO é tipo um vertice dele uma propriedade do VAO
//programa em glsl que gera o vertice

static const char* vertexShader = "                         \n\
#version 330                                                \n\
                                                            \n\
layout(location=0) in vec3 pos;                             \n\
uniform mat4 model;                                         \n\
uniform mat4 projection;                                    \n\
out vec4 vCol;                                              \n\
								                            \n\
void main() {										        \n\
   gl_Position = projection * model * vec4(pos,1.0);        \n\
   vCol = vec4(clamp(pos,0.0f,1.0f),1.0f);				    \n\
}                                                           \n\
";
//programa em glsl que fragmenta
static const char* fragmentShader = "              \n\
#version 330                                       \n\
in vec4 vCol;                                      \n\
uniform vec3 triColor;                             \n\
out vec4 color;                                    \n\
void main() {                                      \n\
   color = vCol;                                   \n\
}                                                  \n\
";


void criaTriangulo() {
	unsigned int indices[] = {//IBO
		0,1,2,//base
		0,1,3,//esquerda
		0,2,3,// direita
		1,2,3 // Frente
	};

	GLfloat vertices[] = {
		0.0f,1.0f,0.f, //vertice 1
		-1.0f,-1.0f,0.0f,//vertice 2
		1.0f,-1.0f,0.0f,//vertice 3
		0.0f,0.0f,1.0f//vertice 4
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	 glGenBuffers(1, &IBO);
	  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	 glGenBuffers(1, &VBO);
	 glBindBuffer(GL_ARRAY_BUFFER, VBO);

	   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,GL_STATIC_DRAW);
	   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
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

	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)bufferWidth / (GLfloat)bufferHeight,0.1f,100.f );

	while (!glfwWindowShouldClose(window)) {

		//GLfloat red = random_float();
	    //GLfloat green = random_float();
		//GLfloat blue = random_float();
		//aleatorio

		//cor de fundo da janela
		glClearColor(0.5f, 0.5f,0.5f,1.0f);
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);

		//altera cor do triangulo
		GLint uniformColor = glGetUniformLocation(shaderProgram, "triColor");
		glUniform3f(uniformColor, 0.0, 1.0, 0.5);
		//glUniform3f(uniformColor, red, green, blue);

		//movimenta o triangulo
		if (!direction) {
			triOffset += triIncrement;// x e y adicionando
		}
		else {
			triOffset -= triIncrement;//x e y diminuindo
		}
		if (triOffset > trifOffsetMax || triOffset < triOffsetMin) {
			direction = !direction;//limite do x e y
		}
		triCurrentAngle += triAngleIncrement;
		if (triCurrentAngle >= 360) {
			triCurrentAngle = 0;// angulo
		}
		if (!directionSize) {
			triOffsetSize += triOffsetSizeIncrement;//incremento do tamanho
		}
		else {
			triOffsetSize -= triOffsetSizeIncrement;//diminuindo do tamanho

		}
		if (triOffsetSize > triOffsetSizeMax || triOffsetSize < triOffsetSizeMin) {
			directionSize = !directionSize;
		}

		GLint uniformModel = glGetUniformLocation(shaderProgram, "model");
		GLint uniformProjection = glGetUniformLocation(shaderProgram, "projection");
		glm::mat4 model(1.0f);

		model = glm::translate(model, glm::vec3(triOffset, triOffset, -2.5f));//x y z
		model = glm::scale(model, glm::vec3(0.4f,0.4f, 0.4f));//x y z
		model = glm::rotate(model, triCurrentAngle * toRadians , glm::vec3(0.0f, 1.0f, 1.0f));//x y z
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		//desenhando o triangulo
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		  glDrawElements(GL_TRIANGLES, 12,GL_UNSIGNED_INT,0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);


		//glBindVertexArray(VAO);

			//glDrawArrays(GL_TRIANGLES,0, 3 );//triangulo, começando na posicao 0 ,numero de pontos 3
			//glBindVertexArray(0);



		glfwSwapBuffers(window);
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}