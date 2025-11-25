#include"Texture.h"

Texture::Texture(const char* image, const char* texType, GLuint slot)
{
	
	type = texType;

	//stock la taille et les canaux de couleurs de l'image
	int widthImg, heightImg, numColCh;
	// Inverse l'image pour qu'elle ne soit pas à l'envers
	stbi_set_flip_vertically_on_load(true);
	// Transfer l'image en un tableau de bytes
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

	// Cree un objet texture pour openGl
	glGenTextures(1, &ID);
	// Assigne l'objet texture a une unité de texture
	glActiveTexture(GL_TEXTURE0 + slot);
	unit = slot;
	glBindTexture(GL_TEXTURE_2D, ID);

	// Algo de rescale de la texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Configuration de la repetition de la texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	//Charge la texture en fonction du nombre de canaux de couleurs (RGB,RGBA, R)

	//4 canaux de couleurs (RGBA) pour des png avec transparence
	if (numColCh == 4)
		glTexImage2D
		(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			widthImg,
			heightImg,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			bytes
		);

	//3 canaux de couleurs (RGB) pour des jpg sans transparence
	else if (numColCh == 3)
		glTexImage2D
		(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			widthImg,
			heightImg,
			0,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			bytes
		);
	//1 canal de couleur (R) pour des images en niveaux de gris
	else if (numColCh == 1)
		glTexImage2D
		(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			widthImg,
			heightImg,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			bytes
		);
	else
		throw std::invalid_argument("Automatic Texture type recognition failed");

	// Genere les mipmaps (version plus petites de l'image pour les objets loins de la camera)
	glGenerateMipmap(GL_TEXTURE_2D);

	// Supprime le tableau de bytes de l'image car elle a été transferée dans l'objet texture OpenGL
	stbi_image_free(bytes);

	// Unbind la texture pour pas qu'elle soit modifiée 
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
	// Recupere l'emplacement de l'uniforme  dans le shader (envoie la valeur vers le shader)
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	//Active le shader avant de lui envoyer la valeur
	shader.Activate();
	// Mets la valeur de l'uniforme
	glUniform1i(texUni, unit);
}

void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}