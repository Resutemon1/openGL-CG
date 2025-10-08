#pragma once
#include <stdio.h>
#include <string>
#include <GL\glew.h>

#include <iostream>
#include <fstream>

class Shader
{
public:
	Shader();
	~Shader();
	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFile(const char* vertexLocation, const char* fragmentLocation);
	void UseProgram();

	GLuint getUniformProjection() { return uniformProjection; };
	GLuint getUniformModel() { return uniformModel; };
	GLuint getUniformView() { return uniformView; };
	GLuint getUniformAmbientIntensity() { return uniformAmbientIntensity; };
	GLuint getUniformAmbientColor() { return uniformAmbientColor; };

private:
	GLuint shaderID, uniformProjection, uniformModel, uniformView, uniformAmbientIntensity, uniformAmbientColor;
	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLenum shaderType, const char* shaderCode);
	std::string ReadFile(const char* fileLocation);
};