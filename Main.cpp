#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Texture.h"
#include<filesystem>
namespace fs = std::filesystem;


// Vertices coordinates
GLfloat vertices[] =
{	//COORDONNEES XYZ        Couleur RGb         Texture coord                        
	-0.5f, -0.5f, 0.0f,	    1.0f, 0.0f, 0.0f,	0.0f, 0.0f, //Coin bas gauche
	-0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,	0.0f, 1.0f, // Coin haut gauche
	 0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,	1.0f, 1.0f, // COin haut droit
	 0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	1.0f, 0.0f  // Coin bas droit

};

// Indices for vertices order
GLuint indices[] =
{
 0,2,1,
 0,3,2
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
		std::cout << "Echec creation fenetre" << std::endl;
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
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	//Recupere l'emplacement de l'uniforme "scale" dans le shader
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");
	


	std::string parentDir = (fs::current_path().fs::path::parent_path()).string();
	std::string texPath = "\\OpenGLSmallEngine\\Resources\\Texture\\";

	std::string fullPath = parentDir + texPath + "cat_thumbup.png";
	std::cout << "Chemin complet vers la texture : " << fullPath << std::endl;
	if (!fs::exists(fullPath)) {
		std::cerr << "ERREUR : Impossible de trouver l'image à " << fullPath << std::endl;
	}
	Texture popCat(fullPath.c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

	popCat.texUnit(shaderProgram, "tex0", 0);



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
		//On bind la texture
		popCat.Bind();
		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}



	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	popCat.Delete();
	shaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}