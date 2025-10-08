#include "Texture.h"

Texture::Texture() {
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = NULL;
}

Texture::Texture(char* fileLoc) {
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = fileLoc;
}

void Texture::loadTexture() {
	unsigned char* texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0); //Carrega o arquivo na memória
	if (!texData) {
		printf("Fail to load image %s\n", fileLocation);
	}

	glGenTextures(1, &textureID); //Gera uma textura e retorna o ID
	glBindTexture(GL_TEXTURE_2D, textureID); //Bind da textura na memória

		//Filtros da Imagem
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //GL_REPEAT para o eixo X
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //GL_REPEAT para o eixo Y
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //Remover os pixels se estiver muito perto
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //Remover os pixels se estiver longe

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData); //Cria a imagem na placa de vídeo
		glGenerateMipmap(GL_TEXTURE_2D); //Cria os MipMap

	glBindTexture(GL_TEXTURE_2D, 0); //Limpa a textura da memória
	stbi_image_free(texData); //Limpa o arquivo lido da memória
}

void Texture::useTexture() {
	glActiveTexture(GL_TEXTURE0); //Ativa a textura no nível 0
	glBindTexture(GL_TEXTURE_2D, textureID); //Bind da textura na memória
}

void Texture::clearTexture() {
	glDeleteTextures(1, &textureID);
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = NULL;
}

Texture::~Texture() {
	clearTexture();
}