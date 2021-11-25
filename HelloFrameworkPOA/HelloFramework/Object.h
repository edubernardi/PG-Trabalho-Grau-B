#pragma once

#include <iostream>
#include <string>
#include <assert.h>
#include <cmath>

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

using namespace std;

class Object
{
public:

	Object() {};
	~Object() {};

	void initialize(); //equivalente ao setupGeometry
	void update();
	void draw();
	 
	inline void setPosition(glm::vec3 position) { this->position = position; }
	inline void setDimensions(glm::vec3 scale) { this->scale = scale; }
	inline void setAngle(float angle) { this->angle = angle; }
	inline void setTexture(GLuint texID) { this->texID = texID; }
	inline void setShader(Shader* shader) { this->shader = shader; }
	inline Shader* getShader() { return shader; }
	glm::vec3 getTopRightVertex(); //get para posição do vértice superior direito do quadrilátero, para detecção de colisão
	glm::vec3 getBottomLeftVertex(); //get para posição do vértice inferior esquerdo do quadrilátero, para detecção de colisão
	inline glm::vec3 getDimensions() { return scale; };
	inline float getAngle() { return angle; };

protected:

	void setRotation(float angle, glm::vec3 axis, bool reset = false);
	void setTranslation(glm::vec3 displacements, bool reset = false);
	void setScale(glm::vec3 scaleFactors, bool reset = false);

	GLuint VAO; //buffer de geometria
	glm::mat4 transform; //matriz de transformação
	GLuint texID; //id de textura

	Shader* shader; //ponteiro para o shader 

	glm::vec3 position, scale; 
	float angle;

	bool first;

};

