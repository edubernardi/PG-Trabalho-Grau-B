/* Hello Triangle - código adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle
 *
 * Adaptado por Rossana Baptista Queiroz
 * para a disciplina de Processamento Gráfico - Jogos Digitais - Unisinos
 * Versão inicial: 7/4/2017
 * Última atualização em 09/08/2021
 *
 */



#include <iostream>
#include <string>
#include <assert.h>
#include <cmath>


using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"

#include "Shader.h"

#include "Object.h"
#include "Sprite.h"

#include <vector>

using namespace std;

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

// Protótipos das funções
int setupGeometry();
int loadTexture(string path);
int loadTextureBlur(string path);
int loadTextureMirror(string path);
GLuint createSprite();

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 800, HEIGHT = 600;
const int nPoints = 100 + 1 + 1; //+centro +cópia do primeiro
const float pi = 3.14159;
Object lenas[9];
Object lenaGrande;
Object stickers[7];
bool moving[7];
string imagem;

// Função MAIN
int main()
{
	// Inicialização da GLFW
	glfwInit();

	//Muita atenção aqui: alguns ambientes não aceitam essas configurações
	//Você deve adaptar para a versão do OpenGL suportada por sua placa
	//Sugestão: comente essas linhas de código para desobrir a versão e
	//depois atualize (por exemplo: 4.5 com 4 e 5)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Essencial para computadores da Apple
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Hello PG Framework!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);

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

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Solicitando imagem
	GLuint texID = 0;
	while (texID == 0) {
		cout << "Insira a imagem a ser carregada: ";
		cin >> imagem;
		texID = loadTexture("./textures/" + imagem);
	}

	// Compilando e buildando o programa de shader
	Shader* shader = new Shader("./shaders/sprite.vs", "./shaders/sprite.fs");
	//Shader* sprShader = new Shader("./shaders/animatedsprites.vs", "./shaders/animatedsprites.fs");
	

	Shader* shaders[7];
	shaders[0] = new Shader("./shaders/sprite.vs", "./shaders/sprite a.fs");
	shaders[1] = new Shader("./shaders/sprite.vs", "./shaders/sprite b.fs");
	shaders[2] = new Shader("./shaders/sprite.vs", "./shaders/sprite d.fs");
	shaders[3] = new Shader("./shaders/sprite.vs", "./shaders/sprite e.fs");
	shaders[4] = new Shader("./shaders/sprite.vs", "./shaders/sprite f.fs");
	shaders[5] = new Shader("./shaders/sprite.vs", "./shaders/sprite g.fs");
	shaders[6] = new Shader("./shaders/sprite.vs", "./shaders/sprite h.fs");
	
	for (int i = 0; i < 7; i++) {
		lenas[i].initialize();
		lenas[i].setPosition(glm::vec3(44.0 + 89.0 * i, 80.0, 0.0));
		lenas[i].setDimensions(glm::vec3(85.0, 85, 1.0));
		lenas[i].setTexture(texID);
		lenas[i].setShader(shaders[i]);
	}

	lenas[7].initialize();
	lenas[7].setPosition(glm::vec3(44.0 + 89.0 * 7, 80.0, 0.0));
	lenas[7].setDimensions(glm::vec3(85.0, 85, 1.0));
	lenas[7].setTexture(loadTextureBlur("./textures/" + imagem));
	lenas[7].setShader(shader);

	lenas[8].initialize();
	lenas[8].setPosition(glm::vec3(44.0 + 89.0 * 8, 80.0, 0.0));
	lenas[8].setDimensions(glm::vec3(85.0, 85, 1.0));
	lenas[8].setTexture(loadTextureMirror("./textures/" + imagem));
	lenas[8].setShader(shader);

	lenaGrande.initialize();
	lenaGrande.setPosition(glm::vec3(400.0, 300.0, 0.0));
	lenaGrande.setDimensions(glm::vec3(300.0, 300, 1.0));
	lenaGrande.setTexture(texID);
	lenaGrande.setShader(shader);

	//Carregando stickers
	for (int i = 0; i < 7; i++) {
		stickers[i].initialize();
		stickers[i].setPosition(glm::vec3(70.0 + 110.0 * i, 525.0, 0.0));
		stickers[i].setDimensions(glm::vec3(100.0, 100, 1.0));
		string path = "./textures/sticker" + std::to_string(i);
		stickers[i].setTexture(loadTexture(path + ".png"));
		stickers[i].setShader(shader);
	}
	
	// Gerando um buffer simples, com a geometria de um triângulo
	//GLuint VAO = setupGeometry();
	GLuint VAO = createSprite();

	//Ativando o shader que será usado

	//shader->Use();

	// Enviando a cor desejada (vec4) para o fragment shader
	// Utilizamos a variáveis do tipo uniform em GLSL para armazenar esse tipo de info
	// que não está nos buffers

	GLint projLoc = glGetUniformLocation(shader->Program, "projection");
	assert(projLoc > -1);

	glUniform1i(glGetUniformLocation(shader->Program, "tex1"), 0);

	glm::mat4 ortho = glm::mat4(1); //inicializa com a matriz identidade

	glm::mat4 model = glm::mat4(1);

	//shader->setMat4("projection", glm::value_ptr(ortho)),

	double xmin = 0.0, xmax = WIDTH, ymin = 0.0, ymax = HEIGHT;

	//shader->Use();
	//shader->setVec4("corColorizadora", 0.5, 0.0, 0.5, 1.0);

// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		ortho = glm::ortho(xmin, xmax, ymin, ymax, -1.0, 1.0);

		// Limpa o buffer de cor
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (int i = 0; i < 7; i++) {
			shaders[i]->Use();
			//Enviar a matriz de projeção ortográfica para o shader
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(ortho));

			lenas[i].update();
			lenas[i].draw();
		}

		lenas[7].update();
		lenas[7].draw();

		lenas[8].update();
		lenas[8].draw();


		shader->Use();
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(ortho));
		lenaGrande.update();
		lenaGrande.draw();

		for (int i = 0; i < 7; i++) {
			shader->Use();
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(ortho));

			if (moving[i]) {
				double xpos, ypos;
				glfwGetCursorPos(window, &xpos, &ypos);

				//Transformando as posições x e y do cursor para que as seleções sejam pecisas mesmo após redimensionamento da janela
				int width, height;
				glfwGetFramebufferSize(window, &width, &height);
				ypos = ypos * HEIGHT / (double)height;
				ypos -= HEIGHT;
				ypos *= -1;
				xpos = xpos * WIDTH / (double)width;
				stickers[i].setPosition(glm::vec3(xpos, ypos, 0));
			}
			

			stickers[i].update();
			stickers[i].draw();
		}
		

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Função de callback de teclado - só pode ter uma instância (deve ser estática se
// estiver dentro de uma classe) - É chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		for (int i = 0; i < 7; i++) {
			if (moving[i]) {
				stickers[i].setDimensions(glm::vec3(stickers[i].getDimensions().x * 1.1, stickers[i].getDimensions().y * 1.1 ,1.0));
			}
		}
	}

	if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		for (int i = 0; i < 7; i++) {
			if (moving[i]) {
				stickers[i].setDimensions(glm::vec3(stickers[i].getDimensions().x * 0.9, stickers[i].getDimensions().y * 0.9, 1.0));
			}
		}
	}

	if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		for (int i = 0; i < 7; i++) {
			if (moving[i]) {
				stickers[i].setAngle(stickers[i].getAngle() - 0.1);
			}
		}
	}

	if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		for (int i = 0; i < 7; i++) {
			if (moving[i]) {
				stickers[i].setAngle(stickers[i].getAngle() + 0.1);
			}
		}
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	//Transformando as posições x e y do cursor para que as seleções sejam pecisas mesmo após redimensionamento da janela
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	ypos = ypos * HEIGHT / (double)height;
	ypos -= HEIGHT;
	ypos *= -1;
	xpos = xpos * WIDTH / (double)width;

	//Verifica se alguma imagem modelo foi clicada
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		for (int i = 0; i < 9; i++) {
			if (lenas[i].getBottomLeftVertex().x  < xpos && lenas[i].getTopRightVertex().x > xpos) {
				if ((lenas[i].getBottomLeftVertex().y) < ypos && (lenas[i].getTopRightVertex().y) > ypos) {
					if (i == 8) {
						lenaGrande.setTexture(loadTextureMirror("./textures/" + imagem));
					} else if (i == 7) {
						lenaGrande.setTexture(loadTextureBlur("./textures/" + imagem));
					}
					else {
						lenaGrande.setTexture(loadTexture("./textures/" + imagem));
						lenaGrande.setShader(lenas[i].getShader());
					}
				}
			}
		}
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		for (int i = 6; i >= 0; i--) {
			if (stickers[i].getBottomLeftVertex().x  < xpos && stickers[i].getTopRightVertex().x > xpos) {
				if ((stickers[i].getBottomLeftVertex().y) < ypos && (stickers[i].getTopRightVertex().y) > ypos) {
					bool alreadyMoving = false;
					for (int j = 0; j < 7; j++) {
						if (moving[j]) {
							alreadyMoving = true;
							break;
						}
					}
					if (!alreadyMoving) {
						moving[i] = true;
					}
				}
			}
		}
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		for (int i = 0; i < 7; i++) {
			if (stickers[i].getBottomLeftVertex().x  < xpos && stickers[i].getTopRightVertex().x > xpos) {
				if ((stickers[i].getBottomLeftVertex().y) < ypos && (stickers[i].getTopRightVertex().y) > ypos) {
					moving[i] = false;
				}
			}
		}
	}
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{

}

// Esta função está bastante harcoded - objetivo é criar os buffers que armazenam a 
// geometria de um triângulo
// Apenas atributo coordenada nos vértices
// 1 VBO com as coordenadas, VAO com apenas 1 ponteiro para atributo
// A função retorna o identificador do VAO
int setupGeometry()
{
	// Aqui setamos as coordenadas x, y e z do triângulo e as armazenamos de forma
	// sequencial, já visando mandar para o VBO (Vertex Buffer Objects)
	// Cada atributo do vértice (coordenada, cores, coordenadas de textura, normal, etc)
	// Pode ser arazenado em um VBO único ou em VBOs separados
	GLfloat* vertices;

	vertices = new GLfloat[nPoints * 3];

	float angle = 0.0;
	float deltaAngle = 2 * pi / (float)(nPoints - 2);
	float radius = 0.5;

	//Adicionar o centro
	vertices[0] = 0.0; // x
	vertices[1] = 0.0; // y
	vertices[2] = 0.0; // z sempre zero 

	for (int i = 3; i < nPoints * 3; i += 3)
	{
		vertices[i] = radius * cos(angle);
		vertices[i + 1] = radius * sin(angle);
		vertices[i + 2] = 0.0;

		angle += deltaAngle;
	}

	GLuint VBO, VAO;

	//Geração do identificador do VBO
	glGenBuffers(1, &VBO);
	//Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, (nPoints * 3) * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice 
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}

int loadTexture(string path)
{
	GLuint texID;

	// Gera o identificador da textura na memória 
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	//Ajusta os parâmetros de wrapping e filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Carregamento da imagem
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		for (int i = 0; i < width * height * nrChannels; i += nrChannels)
		{
			//float media = data[i] * 0.2125 + data[i + 1] * 0.7154 + data[i + 2] * 0.0721;
			//data[i] = media ; // R
			//data[i + 1] = media; //G
			//data[i + 2] = media; //B
		//	//data[i+3] é o canal alpha, se houver

			/*char corColorizadora[3];
			corColorizadora[0] = 200;
			corColorizadora[1] = 0;
			corColorizadora[2] = 200;

			data[i] = data[i] | corColorizadora[0];
			data[i + 1] = data[i + 1] | corColorizadora[1];
			data[i + 2] = data[i + 2] | corColorizadora[2];*/

			/*data[i] = data[i] ^ 255;
			data[i + 1] = data[i + 1] ^ 255;
			data[i + 2] = data[i + 2] ^ 255;*/
		}

		if (nrChannels == 3) //jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else //png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		texID = 0;
	}

	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}

int loadTextureBlur(string path)
{
	GLuint texID;

	// Gera o identificador da textura na memória 
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	//Ajusta os parâmetros de wrapping e filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Carregamento da imagem
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		for (int i = 1; i < height - 1; i += 1)
		{
			for (int j = 1; j < width -1; j += 1) {
				float meanR = 0;
				float meanG = 0;
				float meanB = 0;

				for (int x = -1; x < 2; x++) {
					for (int y = -1; y < 2; y++) {
						meanR += data[((i + x) * width * nrChannels) + (nrChannels * (j + y))];
						meanG += data[((i + x) * width * nrChannels) + (nrChannels * (j + y)) + 1];
						meanB += data[((i + x) * width * nrChannels) + (nrChannels * (j + y)) + 2];
					}
				}
				meanR /= 9;
				meanG /= 9;
				meanB /= 9;
				for (int x = -1; x < 2; x++) {
					for (int y = -1; y < 2; y++) {
						data[((i + x) * width * nrChannels) + (nrChannels * (j + y))] = meanR;
						data[((i + x) * width * nrChannels) + (nrChannels * (j + y)) + 1] = meanG;
						data[((i + x) * width * nrChannels) + (nrChannels * (j + y)) + 2] = meanB;
					}
				}
			}
		}

		if (nrChannels == 3) //jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else //png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		texID = 0;
	}

	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}

int loadTextureMirror(string path)
{
	GLuint texID;

	// Gera o identificador da textura na memória 
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	//Ajusta os parâmetros de wrapping e filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Carregamento da imagem
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	unsigned char* dataTemp = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		for (int i = 0; i < height; i += 1)
		{
			for (int j = 0; j < width; j += 1) {
				data[(i * width * nrChannels) + (nrChannels * j)] = dataTemp[(i * width * nrChannels) + (nrChannels * (width - j))];
				data[(i * width * nrChannels) + (nrChannels * j) + 1] = dataTemp[(i * width * nrChannels) + (nrChannels * (width - j)) + 1];
				data[(i * width * nrChannels) + (nrChannels * j) + 2] = dataTemp[(i * width * nrChannels) + (nrChannels * (width - j)) + 2];
			}
		}

		if (nrChannels == 3) //jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else //png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		texID = 0;
	}

	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}

GLuint createSprite()
{
	GLuint VAO;
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

	return VAO;
}

