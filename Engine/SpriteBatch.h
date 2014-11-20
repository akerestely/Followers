#pragma once
#include <GL\glew.h>
#include "Vertex.h"
#include <glm\gtc\matrix_transform.hpp>
#include <vector>

namespace Engine
{
	enum GlyphSortType { NONE, FRONT_TO_BACK, BACK_TO_FRONT, TEXTURE};
	class Glyph
	{
	public:
		Glyph() {}
		Glyph(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint texture, float depth, const ColorRGBA8 &color):texture(texture), depth(depth)
		{
			topLeft.color = color;
			topLeft.SetPosition(destRect.x, destRect.y + destRect.w);
			topLeft.SetUV(uvRect.x, uvRect.y + uvRect.w);

			bottomLeft.color = color;
			bottomLeft.SetPosition(destRect.x, destRect.y);
			bottomLeft.SetUV(uvRect.x, uvRect.y);

			bottomRight.color = color;
			bottomRight.SetPosition(destRect.x + destRect.z, destRect.y);
			bottomRight.SetUV(uvRect.x + uvRect.z, uvRect.y);

			topRight.color = color;
			topRight.SetPosition(destRect.x + destRect.z, destRect.y + destRect.w);
			topRight.SetUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
		}
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

		std::vector<Glyph*> glyphPointers; /// for sorting
		std::vector<Glyph> glyphs; /// actual glyphs
		std::vector<RenderBatch> renderBatches;
	};
}

