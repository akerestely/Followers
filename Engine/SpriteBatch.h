#pragma once
#include <GL\glew.h>
#include "Vertex.h"
#include <glm\gtc\matrix_transform.hpp>
#include <vector>

namespace Engine
{
	enum GlyphSortType { NONE, FRONT_TO_BACK, BACK_TO_FRONT, TEXTURE};
	struct Glyph
	{
		GLuint texture;
		float depth;

		Vertex topLeft;
		Vertex bottomLeft;
		Vertex topRight;
		Vertex bottomRight;
	};

	class RenderBatch
	{
	public:
		RenderBatch(GLuint offset, GLuint numVertices, GLuint texture): offset(offset),numVertices(numVertices),texture(texture){};
		GLuint offset;
		GLuint numVertices;
		GLuint texture;
	private:
	};
	class SpriteBatch
	{
	public:
		SpriteBatch(void);
		~SpriteBatch(void);

		void Init();

		void Begin(GlyphSortType sortType = TEXTURE);
		void End();

		void Draw(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint texture, float depth, const ColorRGBA8 &color);

		void RenderBatches();
	private:
		void createRenderBatches();
		void createVertexArray();
		void bindBufferAndAttribs();
		void unbindBufferAndAttribs();
		void sortGlyphs();

		static bool compareFrontToBack(Glyph *a, Glyph *b);
		static bool compareBackToFront(Glyph *a, Glyph *b);
		static bool compareTexture(Glyph *a, Glyph *b);

		GLuint vbo;
		GLuint vao;

		GlyphSortType sortType;

		std::vector<Glyph*> glyphs;
		std::vector<RenderBatch> renderBatches;
	};
}

