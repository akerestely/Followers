#include "Sun.h"

#include "Vertex.h"

#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define PI 3.14159265358979323846264338327950288

const glm::vec3 OX(1.0f, 0.0f, 0.0f);
const glm::vec3 OY(0.0f, 1.0f, 0.0f);
const glm::vec3 baseColor(0.0f, 0.5f, 1.0f);
const glm::mat4 biasMatrix(
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 0.5, 0.0,
	0.5, 0.5, 0.5, 1.0);

namespace Engine
{
	Sun::Sun(int screenWidth, int screenHeight) : 
		program(nullptr),
		depthTestProgram(nullptr),
		radialBlurProgram(nullptr),
		sunPosition(glm::vec3(3000, 0, 0)),
		position(sunPosition)
	{
		buildModel();
		buildRect();

		//generate id
		glGenFramebuffers(1, &fboId);
		glGenTextures(2, texIdDepth);
		glGenTextures(2, texIdColor);

		//build textures
		for(unsigned int i=0; i<2; i++)
		{
			glBindTexture(GL_TEXTURE_2D, texIdDepth[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, screenWidth, screenHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		for(unsigned int i=0; i<2; i++)
		{
			glBindTexture(GL_TEXTURE_2D, texIdColor[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	Sun::~Sun(void)
	{
		if (program)
			delete program;
		if (depthTestProgram)
			delete depthTestProgram;
		if (radialBlurProgram)
			delete radialBlurProgram;
	}
	
	void Sun::Update(const CameraSpectator &camera)
	{
		sunPosition = glm::vec3(glm::rotate(sunPosition,0.01f,glm::vec3(0.0f, 0.0f, 1.0f)));
		position = -camera.GetPosition() + sunPosition;
	}

	void Sun::Render(const CameraSpectator &camera)
	{
		if (!program)
			initShaders();

		glm::mat4 biasMVP = biasMatrix * camera.GetCameraMatrix();
		glm::vec4 biassedPosition =  biasMVP * glm::vec4(position,1.0);
		biassedPosition /= biassedPosition.w;

		//copy existing depth buffer to depth texture
		glBindTexture(GL_TEXTURE_2D, texIdDepth[0]);
		glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, camera.GetScreenWidth(),  camera.GetScreenHeight());

		//draw sun
		glBindFramebuffer(GL_FRAMEBUFFER, fboId);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texIdColor[0], 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texIdDepth[1], 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderSun(camera);

		//apply depth test
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texIdColor[1], 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);

		glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, texIdColor[0]);
		glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, texIdDepth[1]);
		glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, texIdDepth[0]);
		depthTestProgram->Use();
		renderRect();
		depthTestProgram->UnUse();

		//apply radial blur and render to screen
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
		glEnable(GL_BLEND);
		glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, texIdColor[1]);
		radialBlurProgram->Use();
		glUniform2f(radialBlurProgram->GetUniformLocation("sourcePos"), biassedPosition.x, biassedPosition.y);
		renderRect();		
		radialBlurProgram->UnUse();
		glDisable(GL_BLEND);
	}

	void Sun::buildModel()
	{
		const unsigned int nVertices = 32;
		SimpleVertex center;
		const float radius = 100.0f;

		std::vector<SimpleVertex> vertices(1+nVertices);
		std::vector<unsigned int> indices(nVertices*3);
		vertices[0] = center;

		//generate vertices
		const float slice = 2*PI/nVertices;
		for (unsigned int i=1, iVert=1; i<=nVertices; i++, iVert++)
		{
			float crtSlice = slice*i;
			vertices[iVert].SetPosition(center.position.x + cos(crtSlice) * radius, 
				center.position.y + sin(crtSlice) * radius, center.position.z);
		}

		//generate indices
		unsigned int k=0;
		for (unsigned int i=1; i<=nVertices; i++)
		{
			indices[k++] = 0;
			indices[k++] = i;
			indices[k++] = i+1;
		}
		//close circle
		indices[k-1] = 1;

		//upload to GPU
		glGenBuffers(1, &vboId);
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(SimpleVertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &iboId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
	}

	void Sun::buildRect()
	{
		//rectangle vertices
		SimpleVertex vertices[4];
		vertices[0].SetPosition(-1.0f, -1.0f, 0.0f);
		vertices[1].SetPosition(-1.0f,  1.0f, 0.0f);
		vertices[2].SetPosition( 1.0f,  1.0f, 0.0f);
		vertices[3].SetPosition( 1.0f, -1.0f, 0.0f);
		//indices
		unsigned int indices[] = {
			0, 1, 2,
			0, 2, 3
		};
		//upload to GPU
		glGenBuffers(1, &vboIdRect);
		glBindBuffer(GL_ARRAY_BUFFER, vboIdRect);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &iboIdRect);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboIdRect);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);
	}

	void Sun::initShaders()
	{
		//for drawing circle
		program = new GLSLProgram;
		program->CompileShaders("shaders/sunShading.vert","shaders/sunShading.frag");
		program->AddAttribute("vertexPosition");
		program->LinkShader();

		//for depth test
		depthTestProgram = new GLSLProgram;
		depthTestProgram->CompileShaders("shaders/depthTest.vert", "shaders/depthTest.frag");
		depthTestProgram->AddAttribute("vertexPosition");
		depthTestProgram->LinkShader();
		depthTestProgram->Use();
		glUniform1i(depthTestProgram->GetUniformLocation("texture"), 0);
		glUniform1i(depthTestProgram->GetUniformLocation("textureDepth"), 1);
		glUniform1i(depthTestProgram->GetUniformLocation("sceneDepth"), 2);
		depthTestProgram->UnUse();

		//for radial blur
		radialBlurProgram = new GLSLProgram;
		radialBlurProgram->CompileShaders("shaders/radialBlur.vert", "shaders/radialBlur.frag");
		radialBlurProgram->AddAttribute("vertexPosition");
		radialBlurProgram->LinkShader();
		radialBlurProgram->Use();
		glUniform1i(radialBlurProgram->GetUniformLocation("image"), 0);
		radialBlurProgram->UnUse();
	}

	void Sun::renderSun(const CameraSpectator &camera)
	{
		program->Use();

		//setting model-view matrix
		glm::vec3 rotation = camera.GetRotation();
		glm::mat4 mvp = glm::translate(camera.GetCameraMatrix(), position);
		mvp = glm::rotate(mvp, -rotation.y, OY);
		mvp = glm::rotate(mvp, -rotation.x, OX);
		glUniformMatrix4fv(program->GetUniformLocation("MVP"), 1, GL_FALSE, &mvp[0][0]);
		//calculate color
		glm::vec3 direction = glm::normalize(position);
		float refractionFactor = 1.0f - sqrt(max(0.0f, direction.y));
		glm::vec3 sunColor = 1.0f - baseColor * refractionFactor;
		glUniform3fv(program->GetUniformLocation("sunColor"), 1, &sunColor[0]);

		//render sun
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glVertexAttribPointer(0, 3, GL_FLOAT ,GL_FALSE, sizeof(Engine::SimpleVertex), (void*)offsetof(Engine::SimpleVertex,position));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
		int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

		glDrawElements(GL_TRIANGLES, size/sizeof(GLuint), GL_UNSIGNED_INT, 0);
		glDisableVertexAttribArray(0);

		program->UnUse();
	}

	void Sun::renderRect()
	{
		glBindBuffer(GL_ARRAY_BUFFER, vboIdRect);
		glVertexAttribPointer(0, 3, GL_FLOAT ,GL_FALSE, sizeof(Engine::SimpleVertex), (void*)offsetof(Engine::SimpleVertex,position));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboIdRect);
		int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

		glDrawElements(GL_TRIANGLES, size/sizeof(GLuint), GL_UNSIGNED_INT, 0);
		glDisableVertexAttribArray(0);
	}
}
