#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>



// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.2f, 0.8f, 0.02f, 1.0f);\n"
"}\n\0";



int main()
{
	glfwInit();
	//permet de choisir la version d'opengl ici le 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//le CORE PROFILE permet l'utilisation de la version moderne de opengl
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	//vertices coordonates
	GLfloat vertices[] =
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // Upper corner
		-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner left
		0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner right
		0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f // Inner down
	};

	// Indices for vertices order
	GLuint indices[] =
	{
		0, 3, 5, // Lower left triangle
		3, 2, 4, // Upper triangle
		5, 4, 1 // Lower right triangle
	};


	GLFWwindow* window = glfwCreateWindow(800, 800 ,"OPENGLTestProject", NULL, NULL);
	if (window == NULL)
	{

		std::cout << "Failed to create GLFW windows" << std::endl;

		glfwTerminate();
		return -1;

	}

	glfwMakeContextCurrent(window);


	gladLoadGL();

	glViewport(0, 0, 800, 800);
	
	//Creation d'un Shader object  et sa reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//attache vertex shader source a vertex shader object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);



	GLuint shaderProgram = glCreateProgram();
	//attach vertex et fragment shaders a shader program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	//lie les shaders ensemble dans shader program
	glLinkProgram(shaderProgram);

	//On les supprime car on en a plus besoin 
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//Creation de references  pour le vertex array object et vertex buffer object 
	GLuint VAO,VBO,EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);



	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Configuration des attributs des vertex pour que opengl comprennnent comment  lire VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//On les bind a 0 pour pas modifier VBO et VAO accidentellement 
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);






	glClearColor(0.07f, 0.13f, 0.17f, 1.0f); //background color
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);


	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f); //background color
		glClear(GL_COLOR_BUFFER_BIT);
		//Permet de dire a openGL le shader que l'on utilise
		glUseProgram(shaderProgram);

		glBindVertexArray(VAO);
		//Dessine la forme de l'array
		glDrawElements(GL_TRIANGLES, 9,GL_UNSIGNED_INT,0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//On supprime tt les objets 
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1,&VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);


	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}