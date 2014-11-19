#include "SpriteBatch.h"
#include <algorithm>

namespace Engine
{
	SpriteBatch::SpriteBatch(void):
		vbo(0),
		vao(0)
	{
	}


	SpriteBatch::~SpriteBatch(void)
	{
	}

	void SpriteBatch::Init()
	{
		createVertexArray();
	}

	void SpriteBatch::Begin(GlyphSortType sortType)
	{
		this->sortType = sortType;
		renderBatches.clear();
		glyphs.clear();
	}

	void SpriteBatch::End()
	{
		glyphPointers.resize(glyphs.size());
		for(int i=0; i<glyphs.size(); i++)
			glyphPointers[i] = &glyphs[i];

		sortGlyphs();
		createRenderBatches();
	}

	void SpriteBatch::Draw(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint texture, float depth, const ColorRGBA8 &color)
	{
		Glyph newGlyph(destRect, uvRect, texture, depth, color);

		glyphs.emplace_back(newGlyph);
	}

	void SpriteBatch::RenderBatches()
	{
		/*opengl > 3*/ //glBindVertexArray(vao);
		/*opengl 2.1*/ bindBufferAndAttribs();
		for (int i=0; i< renderBatches.size(); i++)
		{
			glBindTexture(GL_TEXTURE_2D, renderBatches[i].texture);
			glDrawArrays(GL_TRIANGLES, renderBatches[i].offset, renderBatches[i].numVertices);
		}
		/*opengl > 3*/ //glBindVertexArray(0);
		/*opengl 2.1*/ unbindBufferAndAttribs();
	}

	void SpriteBatch::createRenderBatches()
	{
		std::vector<Vertex> vertices;
		vertices.resize(glyphPointers.size() * 6);

		if(glyphPointers.empty())
			return;
		
		int offset = 0;
		int cv = 0;

		RenderBatch rb(offset, 6, glyphPointers[0]->texture);
		renderBatches.emplace_back(rb);
		vertices[cv++] = glyphPointers[0]->topLeft;
		vertices[cv++] = glyphPointers[0]->bottomLeft;
		vertices[cv++] = glyphPointers[0]->bottomRight;
		vertices[cv++] = glyphPointers[0]->bottomRight;
		vertices[cv++] = glyphPointers[0]->topRight;
		vertices[cv++] = glyphPointers[0]->topLeft;
		offset += 6;

		for(int cg=1; cg<glyphPointers.size(); cg++)
		{
			if(glyphPointers[cg]->texture != glyphPointers[cg-1]->texture)
			{
				RenderBatch rb(offset, 6, glyphPointers[cg]->texture);
				renderBatches.emplace_back(rb);
			}
			else
			{
				renderBatches.back().numVertices += 6;
			}
			vertices[cv++] = glyphPointers[cg]->topLeft;
			vertices[cv++] = glyphPointers[cg]->bottomLeft;
			vertices[cv++] = glyphPointers[cg]->bottomRight;
			vertices[cv++] = glyphPointers[cg]->bottomRight;
			vertices[cv++] = glyphPointers[cg]->topRight;
			vertices[cv++] = glyphPointers[cg]->topLeft;
			offset += 6;
		}
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		//orphan the buffer
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER,0,vertices.size() * sizeof(Vertex), vertices.data());

		glBindBuffer(GL_ARRAY_BUFFER,0);
	}

	void SpriteBatch::createVertexArray()
	{
		/* use for opengl > 3
		if(!vao)
			glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);*/

		if (!vbo)
			glGenBuffers(1,&vbo);

		/* use for opengl > 3
		bindBufferAndAttribs();

		glBindVertexArray(0);*/
	}
void SpriteBatch::bindBufferAndAttribs()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 2, GL_FLOAT ,GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,position));
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex,color));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,uv));
}
void SpriteBatch::unbindBufferAndAttribs()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

	void SpriteBatch::sortGlyphs()
	{
		switch(sortType)
		{
		case BACK_TO_FRONT:
			std::stable_sort(glyphPointers.begin(), glyphPointers.end(),compareBackToFront);
			break;
		case FRONT_TO_BACK:
			std::stable_sort(glyphPointers.begin(), glyphPointers.end(),compareFrontToBack);
			break;
		case TEXTURE:
			std::stable_sort(glyphPointers.begin(), glyphPointers.end(),compareTexture);
			break;
		}
	}

	bool SpriteBatch::compareFrontToBack(Glyph *a, Glyph *b)
	{
		return (a->depth < b->depth);
	}
	bool SpriteBatch::compareBackToFront(Glyph *a, Glyph *b)
	{
		return (a->depth > b->depth);
	}
	bool SpriteBatch::compareTexture(Glyph *a, Glyph *b)
	{
		return (a->texture < b->texture);
	}
}
