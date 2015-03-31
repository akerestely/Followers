#include "SkyDome.h"

#include "Vertex.h"

#include <vector>

#include <glm/gtc/matrix_transform.hpp>

#define PI (float)3.14159265358979323846264338327950288

const unsigned int levels = 6U;
const unsigned int nVertices = 10U; //per level
const float bigRadius = 1.0f; 
const float height = 0.25f;
const float bottom = 10.0f;  //bottom of dome

namespace Engine
{
	SkyDome::SkyDome() : skyProgram(nullptr), InnerRadius(10.0f)
	{
		SimpleVertex center;
		center.position.y = bottom;		

		std::vector<SimpleVertex> vertices(1+nVertices*levels);
		std::vector<unsigned int> indices(nVertices*3+nVertices*3*2*(levels-1));
		vertices[0] = center;
		vertices[0].position.y += height; //top of dome

		//generate vertices
		const float slice = 2.0f*PI/nVertices;
		for(unsigned int level = 1, iVert=1; level<=levels; level++)
		{
			const float radius = bigRadius * sin(PI/2.0f * level/levels);
			const float circleHeight = center.position.y + height * cos(PI/2 * level/levels);
			for (unsigned int i=1; i<=nVertices; i++,iVert++)
			{
				float crtSlice = slice*i;
				vertices[iVert].SetPosition(center.position.x + cos(crtSlice) * radius,
					circleHeight, center.position.z + sin(crtSlice) * radius);
			}
		}

		//generate indices
		//top of dome
		unsigned int k=0;
		for (unsigned int i=1; i<=nVertices; i++)
		{
			indices[k++] = 0;
			indices[k++] = i;
			indices[k++] = i+1;
		}
		//close circle
		indices[k-1] = 1;

		//rest of dome
		for (unsigned int level = 0; level<levels-1; level++)
		{
			for (unsigned int i=1; i<nVertices; i++)
			{
				indices[k++] = level * nVertices + i;
				indices[k++] = (level+1) * nVertices + i;
				indices[k++] = level * nVertices + i + 1;

				indices[k++] = level * nVertices + i + 1;
				indices[k++] = (level+1) * nVertices + i;
				indices[k++] = (level+1) * nVertices + i + 1;
			}
			//close circle
			indices[k++] = (level+1) * nVertices;
			indices[k++] = (level+2) * nVertices;
			indices[k++] = level * nVertices + 1;

			indices[k++] = level * nVertices + 1;
			indices[k++] = (level+2) * nVertices;
			indices[k++] = (level+1) * nVertices + 1;
		}

		//upload to GPU
		glGenBuffers(1, &vboId);
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(SimpleVertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &iboId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);

		//build bottom circle
		buildBottom();
	}

	SkyDome::~SkyDome()
	{
		if(skyProgram)
			delete skyProgram;
		if(skyBottomProgram)
			delete skyBottomProgram;
	}

	void SkyDome::Render(const CameraSpectator &camera, const glm::vec3 sunPos)
	{
		if(!skyProgram)
			initShader();

		glDisable(GL_DEPTH_TEST);
		skyProgram->Use();

		//setting model-view matrix
		glm::vec3 negativeCamPos = -camera.GetPosition();
		negativeCamPos.y -= InnerRadius;
		glm::mat4 mvp = glm::translate(camera.GetCameraMatrix(), negativeCamPos);
		glUniformMatrix4fv(skyProgram->GetUniformLocation("MVP"), 1, GL_FALSE, &mvp[0][0]);

		//setting sun position
		glm::vec3 sunDir = glm::normalize(sunPos);
		glUniform3fv(skyProgram->GetUniformLocation("v3LightPos"), 1, &sunDir[0]);

		//render sky dome
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glVertexAttribPointer(0, 3, GL_FLOAT ,GL_FALSE, sizeof(Engine::SimpleVertex), (void*)offsetof(Engine::SimpleVertex,position));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
		int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

		glDrawElements(GL_TRIANGLES, size/sizeof(GLuint), GL_UNSIGNED_INT, 0);

		skyProgram->UnUse();

		skyBottomProgram->Use();
		glUniform3fv(skyBottomProgram->GetUniformLocation("sunDir"), 1, &sunDir[0]);
		glUniformMatrix4fv(skyBottomProgram->GetUniformLocation("MVP"), 1, GL_FALSE, &mvp[0][0]);
		//render sky bottom
		glBindBuffer(GL_ARRAY_BUFFER, vboIdBottom);
		glVertexAttribPointer(0, 3, GL_FLOAT ,GL_FALSE, sizeof(Engine::SimpleVertex), (void*)offsetof(Engine::SimpleVertex,position));
		glDrawArrays(GL_TRIANGLE_FAN, 0, nVertices+2);
		skyBottomProgram->UnUse();

		glEnable(GL_DEPTH_TEST);
	}

	void SkyDome::initShader()
	{
		skyProgram = new GLSLProgram;

		//links shaders
		skyProgram->CompileShaders("shaders/skyShading.vert","shaders/skyShading.frag");
		skyProgram->AddAttribute("vertexPosition");
		skyProgram->LinkShader();

		skyProgram->Use();
		//constants for uniforms
		//for details on constants see: http://http.developer.nvidia.com/GPUGems2/gpugems2_chapter16.html
		const float Kr = 0.0030f;
		const float Km = 0.0015f;
		const float ESun = 16.0f;
		const float g = -0.75f;
		const float OuterRadius = 10.25f;
		const float Scale = 1.0f / (OuterRadius - InnerRadius);
		const float ScaleDepth = 0.25f;
		const float ScaleOverScaleDepth = Scale / ScaleDepth;

		//adding uniforms to shader
		glUniform3f(skyProgram->GetUniformLocation("v3CameraPos"), 0.0f, InnerRadius, 0.0f);
		glUniform3f(skyProgram->GetUniformLocation("v3InvWavelength"), 1.0f / powf(0.650f, 4.0f), 1.0f / powf(0.570f, 4.0f), 1.0f / powf(0.475f, 4.0f));
		glUniform1f(skyProgram->GetUniformLocation("fCameraHeight"), InnerRadius);
		glUniform1f(skyProgram->GetUniformLocation("fInnerRadius"), InnerRadius);
		glUniform1f(skyProgram->GetUniformLocation("fKrESun"), Kr * ESun);
		glUniform1f(skyProgram->GetUniformLocation("fKmESun"), Km * ESun);
		glUniform1f(skyProgram->GetUniformLocation("fKr4PI"), Kr * 4.0f * PI);
		glUniform1f(skyProgram->GetUniformLocation("fKm4PI"), Km * 4.0f * PI);
		glUniform1f(skyProgram->GetUniformLocation("fScale"), Scale);
		glUniform1f(skyProgram->GetUniformLocation("fScaleDepth"), ScaleDepth);
		glUniform1f(skyProgram->GetUniformLocation("fScaleOverScaleDepth"), ScaleOverScaleDepth);
		glUniform1f(skyProgram->GetUniformLocation("g"), g);
		glUniform1f(skyProgram->GetUniformLocation("g2"), g * g);
		glUniform1i(skyProgram->GetUniformLocation("Samples"), 4);
		skyProgram->UnUse();

		//skyBottomShading
		skyBottomProgram = new GLSLProgram;
		
		//links shaders
		skyBottomProgram->CompileShaders("shaders/skyBottomShading.vert","shaders/skyBottomShading.frag");
		skyBottomProgram->AddAttribute("vertexPosition");
		skyBottomProgram->LinkShader();
	}

	void SkyDome::buildBottom()
	{
		SimpleVertex center;
		center.position.y = bottom;		

		std::vector<SimpleVertex> vertices(1+nVertices+1);
		std::vector<unsigned int> indices(nVertices*3);
		vertices[0] = center;
		vertices[0].position.y -= 0.150; // subtract a little more than near clipping

		//generate vertices
		const float slice = 2.0f*PI/nVertices;
		const float circleHeight = center.position.y;
		for (unsigned int i=0; i<=nVertices; i++)
		{
			float crtSlice = slice*i;
			vertices[i+1].SetPosition(center.position.x + cos(crtSlice) * bigRadius,
				circleHeight, center.position.z + sin(crtSlice) * bigRadius);
		}

		//upload to GPU
		glGenBuffers(1, &vboIdBottom);
		glBindBuffer(GL_ARRAY_BUFFER, vboIdBottom);
		glBufferData(GL_ARRAY_BUFFER, sizeof(SimpleVertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	}
}
