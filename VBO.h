#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include<glm/glm.hpp>
#include <glad/glad.h>
#include <vector>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;

	glm::vec3 Color;
	glm::vec2 TexUV;
};



class VBO
{
	public:
	GLuint ID;
	VBO(std::vector<Vertex>& vertices );
	void Bind();
	void Unbind();
	void Delete();
};

#endif 
