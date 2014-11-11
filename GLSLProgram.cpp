#include "GLSLProgram.h"

#include <fstream>
#include <string>
#include <vector>

#include "Errors.h"

GLSLProgram::GLSLProgram(void): 
	numAtrtrib(0), programId(0), vertexShaderId(0), fragmentShaderId(0)
{
}

GLSLProgram::~GLSLProgram(void)
{
}

void GLSLProgram::CompileShaders(const char* vertexShaderFilePath, const char* fragmentShaderFilePath)
{
	vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	if(!vertexShaderId)
		fatalError("Vertex shader failed to be created!");

	fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	if(!fragmentShaderId)
		fatalError("Fragment shader failed to be created!");

	compileShader(vertexShaderFilePath, vertexShaderId);
	compileShader(fragmentShaderFilePath, fragmentShaderId);

	//Vertex and fragment shaders are successfully compiled.
	//Get a program object.
	programId = glCreateProgram();
}

void GLSLProgram::LinkShader()
{
	//Attach our shaders to our program
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);

	//Link our program
	glLinkProgram(programId);

	//Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(programId, GL_LINK_STATUS, (int *)&isLinked);
	if(isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<char> errorLog(maxLength);
		glGetProgramInfoLog(programId, maxLength, &maxLength, &errorLog[0]);

		//We don't need the program anymore.
		glDeleteProgram(programId);
		//Don't leak shaders either.
		glDeleteShader(vertexShaderId);
		glDeleteShader(fragmentShaderId);

		//Use the infoLog as you see fit.

		std::printf("%s\n",&errorLog[0]);
		fatalError("Shader faild to link");

		//In this simple program, we'll just leave
		return;
	}

	//Always detach shaders after a successful link.
	glDetachShader(programId, vertexShaderId);
	glDetachShader(programId, fragmentShaderId);
	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);
}

void GLSLProgram::AddAttribute(const char* attribName)
{
	glBindAttribLocation(programId, numAtrtrib++, attribName);
}

GLuint GLSLProgram::GetUniformLocation( const char* uniformName )
{
	GLuint location=glGetUniformLocation(programId, uniformName);
	if(location == GL_INVALID_INDEX)
		fatalError ( "Uniform not found in shader");
	return location;
}


void GLSLProgram::Use()
{
	glUseProgram(programId);
	for(int i=0;i<numAtrtrib;i++)
		glEnableVertexAttribArray(i);
}
void GLSLProgram::UnUse()
{
	glUseProgram(0);
	for(int i=0;i<numAtrtrib;i++)
		glDisableVertexAttribArray(i);
}

void GLSLProgram::compileShader(const char* filePath, GLuint id)
{
	std::ifstream file(filePath);
	if(file.fail())
	{
		perror(filePath);
		fatalError("Failed to open shader file");
	}

	std::string fileContents;

	fileContents.assign((std::istreambuf_iterator<char> (file) ), std::istreambuf_iterator<char> () );

	file.close();

	const GLchar* chShaderSource = fileContents.c_str() + '\0';
    //printf("%s", chShaderSource);
	glShaderSource(id, 1, (const GLchar**) &chShaderSource, nullptr);

	glCompileShader(id);

	GLint isCompiled = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &isCompiled);

	if(isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<char> errorLog(maxLength);
		glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

		//Provide the infolog in whatever manor you deem best.
		//Exit with failure.
		glDeleteShader(id); //Don't leak the shader.

		std::printf("%s\n",&errorLog[0]);
		fatalError("Shader failed to compile!");

		return;
	}
}