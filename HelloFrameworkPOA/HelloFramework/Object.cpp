#include "Object.h"

void Object::initialize()
{
	GLuint VBO, EBO;

	float vertices[] = {
		// positions          // colors          // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0, // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0  // top left 
	};
	unsigned int indices[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); //unbind

	transform = glm::mat4(1);
	position = glm::vec3(0.0, 0.0, 0.0);
	scale = glm::vec3(1.0, 1.0, 1.0);
	angle = 0.0;
	shader = NULL;
	texID = 0;

	first = true;
}

void Object::update()
{
	shader->Use();

	//atualizar a matriz de transformação do objeto
	setTranslation(position,true);
	setRotation(angle, glm::vec3(0.0, 0.0, 1.0));
	setScale(scale);

	shader->setMat4("model", glm::value_ptr(transform));
}

void Object::draw()
{
	shader->Use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Object::setRotation(float angle, glm::vec3 axis, bool reset)
{
	if (reset)
	{
		transform = glm::mat4(1); //inicializa com matriz identidade
	}
	transform = glm::rotate(transform, angle, axis);
}

void Object::setTranslation(glm::vec3 displacements, bool reset)
{
	if (reset)
	{
		transform = glm::mat4(1); //inicializa com matriz identidade
	}
	transform = glm::translate(transform,displacements);
	
}

void Object::setScale(glm::vec3 scaleFactors, bool reset)
{
	if (reset)
	{
		transform = glm::mat4(1); //inicializa com matriz identidade
	}
	transform = glm::scale(transform,scaleFactors);
}

glm::vec3 Object::getTopRightVertex()
{
	return glm::vec3(position.x + (scale.x / 2), position.y + (scale.y / 2), 1);
}

glm::vec3 Object::getBottomLeftVertex()
{
	return glm::vec3(position.x - (scale.x / 2), position.y - (scale.y / 2), 1);
}