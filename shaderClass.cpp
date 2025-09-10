#include"shaderClass.h"

std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);
	
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();
	
	//Creation d'un Shader object  et sa reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	
	//attache vertex shader source a vertex shader object
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	//Compile le vertex shader
	glCompileShader(vertexShader);
	
	//Creation d'un Shader object  et sa reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	
	//attache fragment shader source a fragment shader object
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	//Compile le fragment shader
	glCompileShader(fragmentShader);
	
	//Creation de shader program et sa reference
	 ID = glCreateProgram();


	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	
	
	//lie les shaders ensemble dans shader program
	glLinkProgram(ID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Activate()
{
	glUseProgram(ID);
}

void Shader::Delete()
{
	glDeleteProgram(ID);
}