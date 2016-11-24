#include "SpriteBatch.h"

#include <algorithm>

namespace BorEngine
{

	SpriteBatch::SpriteBatch()
	{
	}


	SpriteBatch::~SpriteBatch()
	{
	}


	void SpriteBatch::init()
	{
		createVertexArray();
	}

	void SpriteBatch::begin(GlyphSortType sortType /* GlyphSortType::TEXTURE */)
	{
		p_sortType = sortType;
		p_renderBatches.clear();
		p_glyphs.clear();
	}

	void SpriteBatch::end()
	{
		p_glyphPointers.resize(p_glyphs.size());
		for (int i = 0; i < p_glyphs.size(); i++)
		{
			p_glyphPointers[i] = &p_glyphs[i];
		}
		sortGlyphs();
		createRenderBatches();
	}

	// void draw(glm::vec2 position, glm::vec2 size, ...);
	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color)
	{
		p_glyphs.emplace_back(destRect, uvRect, texture, depth, color);
	}

	void SpriteBatch::renderBatch()
	{
		glBindVertexArray(p_vao);

		for (int i = 0; i < p_renderBatches.size(); i++)
		{
			glBindTexture(GL_TEXTURE_2D, p_renderBatches[i].texture);
			glDrawArrays(GL_TRIANGLES, p_renderBatches[i].offset, p_renderBatches[i].numVertices);
		}
		glBindVertexArray(0);
	}

	void SpriteBatch::createRenderBatches()
	{
		std::vector <Vertex> vertices;
		vertices.resize(p_glyphPointers.size() * 6); // faster thanks to allocating the exact memory we need!

		if (p_glyphPointers.empty()) {
			return;
		}

		int offset = 0; // can be replaced with "cv*6"
		int cv = 0; // current vertex

		// RenderBatch myBatch(0, 6, _glyphs[0]->texture);
		// _renderBatches.push_back(myBatch);
		p_renderBatches.emplace_back(offset, 6, p_glyphPointers[0]->texture);

		vertices[cv++] = p_glyphPointers[0]->topLeft;
		vertices[cv++] = p_glyphPointers[0]->bottomLeft;
		vertices[cv++] = p_glyphPointers[0]->bottomRight;

		vertices[cv++] = p_glyphPointers[0]->bottomRight;
		vertices[cv++] = p_glyphPointers[0]->topRight;
		vertices[cv++] = p_glyphPointers[0]->topLeft;

		for (int cg = 1; cg < p_glyphs.size(); cg++) // cg : current glyph
		{
			offset += 6;

			if (p_glyphPointers[cg]->texture != p_glyphPointers[cg - 1]->texture) {
				p_renderBatches.emplace_back(offset, 6, p_glyphPointers[cg]->texture);
			}
			else {
				p_renderBatches.back().numVertices += 6;
			}

			vertices[cv++] = p_glyphPointers[cg]->topLeft;
			vertices[cv++] = p_glyphPointers[cg]->bottomLeft;
			vertices[cv++] = p_glyphPointers[cg]->bottomRight;

			vertices[cv++] = p_glyphPointers[cg]->bottomRight;
			vertices[cv++] = p_glyphPointers[cg]->topRight;
			vertices[cv++] = p_glyphPointers[cg]->topLeft;
		}

		glBindBuffer(GL_ARRAY_BUFFER, p_vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW); // orphan the buffer
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data()); // upload the data

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void SpriteBatch::createVertexArray()
	{
		// check if init is already called (if vao and vbo already set), so that we don't call init 2 times
		if (p_vao == 0)
		{
			glGenVertexArrays(1, &p_vao);
		}
		glBindVertexArray(p_vao);

		if (p_vbo == 0)
		{
			glGenBuffers(1, &p_vbo);
		}
		glBindBuffer(GL_ARRAY_BUFFER, p_vbo);


		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		// position attrib
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		// color attrib
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		// uv attrib
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));


		glBindVertexArray(0);
	}

	void SpriteBatch::sortGlyphs()
	{
		switch (p_sortType)
		{
		case BorEngine::GlyphSortType::BACK_TO_FRONT:
			std::stable_sort(p_glyphPointers.begin(), p_glyphPointers.end(), compareBackToFront);
			break;
		case BorEngine::GlyphSortType::FRONT_TO_BACK:
			std::stable_sort(p_glyphPointers.begin(), p_glyphPointers.end(), compareFrontToBack);
			break;
		case BorEngine::GlyphSortType::TEXTURE:
			std::stable_sort(p_glyphPointers.begin(), p_glyphPointers.end(), compareTexture);
			break;
		}
	}

	bool SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b)
	{
		return (a->depth < b->depth);
	}

	bool SpriteBatch::compareBackToFront(Glyph* a, Glyph* b)
	{
		return (a->depth > b->depth);
	}

	bool SpriteBatch::compareTexture(Glyph* a, Glyph* b)
	{
		return (a->texture < b->texture);
	}
}