#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"



// Vertices coordinates
GLfloat vertices[] =
{//COORDONNEES XYZ                                  // COULEURS RGB                        
	-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,		0.8f,0.3f,0.02f,// Coin bas gauche
	0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,			0.8f,0.3f,0.02f,// Coins bas droit
	0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f,		0.8f,0.45f,0.02f,// Coin haut
	-0.25f, 0.5f * float(sqrt(3)) / 6, 0.0f,		0.9f,0.45f,0.92f,// Coin interieur gauche
	0.25f , 0.5f * float(sqrt(3)) / 6, 0.0f,		0.9f,0.3f,0.92f,// Coin interieur droit
	0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f  ,		0.8f,0.3f,0.92f,// Interieur bas
};

// Indices for vertices order
GLuint indices[] =
{
	0, 3, 5, // Triangle gauche
	3, 2, 4, // Triangle droit
	5, 4, 1 // Traingle haut
};



int main()
{
	glfwInit();

	// Defini la version de openGL

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// On utlise le core profile pour avoir acces aux nouvelles fonctionnalites
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// On crée une fenetre de 800x800 pixels et on la nomme "TestOpenGL"
	GLFWwindow* window = glfwCreateWindow(800, 800, "TestOpenGL", NULL, NULL);
	// Erreur au cas ou la fenetre ne se cree pas
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);

	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, 800, 800);



	//Genere un shader
	Shader shaderProgram("default.vert", "default.frag");



	// Genere un tableau de vertices 
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indices, sizeof(indices));

	// Lie VBO to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3* sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	//Recupere l'emplacement de l'uniforme "scale" dans le shader
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");
	
	while (!glfwWindowShouldClose(window))
	{
		// Couleur du fond
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT);
		// Dis que l'on tu'iliser ShaderProgram en shader
		shaderProgram.Activate();
		//Assignbe une valeur a l'uniforme dois toujours etre fait apres avoir activer le shader
		glUniform1f(uniID, 0.5f);
		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}



	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}