#include"Texture.h"

Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
{
	
	type = texType;

	// Taille image et nombre de canaux de couleur
	int widthImg, heightImg, numColCh;
	// Inverse l'image pour quelle soit dans le bon sens par defaut
	stbi_set_flip_vertically_on_load(true);
	// Lis l'image et stocke les donnees dans un tableau
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

	// Genere un objet texture et le lie a l'image
	glGenTextures(1, &ID);
	// Assigne l'objet a une unité de texture
	glActiveTexture(slot);
	glBindTexture(texType, ID);

	// Configure l'algorithme de filtrage de la texture (linear, nearest, etc..)
	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Configure la facon ou la texture se repete (clamp to edge, repeat, mirrored repeat, etc..)
	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);


	// Assigne l'image a l'objet texture
	glTexImage2D(texType, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
	// genere les mipmaps
	glGenerateMipmap(texType);

	// Supprime les donnees de l'image car elles sont maintenant dans l'objet texture
	stbi_image_free(bytes);

	// Unbinds la texture pour pas le modifier accidentellement
	glBindTexture(texType, 0);
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
	// Obtiens la location de l'uniforme dans le shader
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	// Active le shader avant de lui envoyer des valeurs
	shader.Activate();
	// Mets les bonne valeurs
	glUniform1i(texUni, unit);
}

void Texture::Bind()
{
	glBindTexture(type, ID);
}

void Texture::Unbind()
{
	glBindTexture(type, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}