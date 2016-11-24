#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <vector>

#include "Vertex.h"

namespace BorEngine
{
	enum class GlyphSortType
	{
		NONE,
		FRONT_TO_BACK,
		BACK_TO_FRONT,
		TEXTURE
	};

	class Glyph
	{
	public:
		Glyph() {};
		Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color) :
			texture(Texture),
			depth(Depth)
		{
			topLeft.color = color;
			topLeft.setPosition(destRect.x, destRect.y + destRect.w);
			topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

			bottomLeft.color = color;
			bottomLeft.setPosition(destRect.x, destRect.y);
			bottomLeft.setUV(uvRect.x, uvRect.y);

			bottomRight.color = color;
			bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
			bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

			topRight.color = color;
			topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
			topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
		};
		GLuint texture;

		float depth; // z-index

		Vertex topLeft;
		Vertex bottomLeft;
		Vertex topRight;
		Vertex bottomRight;
	};

	class RenderBatch
	{
	public:
		RenderBatch(GLuint Offset, GLuint NumVertices, GLuint Texture) :
			offset(Offset),
			numVertices(NumVertices),
			texture(Texture)
		{}

		GLuint offset;
		GLuint numVertices;
		GLuint texture;
	};

	class SpriteBatch
	{
	public:
		SpriteBatch();
		~SpriteBatch();

		void init();

		void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);
		void end();

		// void draw(glm::vec2 position, glm::vec2 size, ...);
		void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color);

		void renderBatch();

	private:
		void createRenderBatches();
		void createVertexArray();
		void sortGlyphs();

		static bool compareFrontToBack(Glyph* a, Glyph* b);
		static bool compareBackToFront(Glyph* a, Glyph* b);
		static bool compareTexture(Glyph* a, Glyph* b);

		GLuint p_vbo = 0; // vertex buffer object
		GLuint p_vao = 0; // vertex array object

		GlyphSortType p_sortType;

		std::vector<Glyph*> p_glyphPointers; // for sorting glyphs
		std::vector<Glyph> p_glyphs; // actual glyphs

		std::vector<RenderBatch> p_renderBatches;
	};
}
