#include "SpriteBatch.h"

#include <algorithm>

namespace BorEngine
{

	SpriteBatch::SpriteBatch() :_vbo(0), _vao(0)
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
		_sortType = sortType;
		_renderBatches.clear();
		_glyphs.clear();
	}

	void SpriteBatch::end()
	{
		_glyphPointers.resize(_glyphs.size());
		for (int i = 0; i < _glyphs.size(); i++)
		{
			_glyphPointers[i] = &_glyphs[i];
		}
		sortGlyphs();
		createRenderBatches();
	}

	// void draw(glm::vec2 position, glm::vec2 size, ...);
	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color)
	{
		_glyphs.emplace_back(destRect, uvRect, texture, depth, color);
	}

	void SpriteBatch::renderBatch()
	{
		glBindVertexArray(_vao);

		for (int i = 0; i < _renderBatches.size(); i++)
		{
			glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);
			glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertices);
		}
		glBindVertexArray(0);
	}

	void SpriteBatch::createRenderBatches()
	{
		std::vector <Vertex> vertices;
		vertices.resize(_glyphPointers.size() * 6); // faster thanks to allocating the exact memory we need!

		if (_glyphPointers.empty()) {
			return;
		}

		int offset = 0; // can be replaced with "cv*6"
		int cv = 0; // current vertex

		// RenderBatch myBatch(0, 6, _glyphs[0]->texture);
		// _renderBatches.push_back(myBatch);
		_renderBatches.emplace_back(offset, 6, _glyphPointers[0]->texture);

		vertices[cv++] = _glyphPointers[0]->topLeft;
		vertices[cv++] = _glyphPointers[0]->bottomLeft;
		vertices[cv++] = _glyphPointers[0]->bottomRight;

		vertices[cv++] = _glyphPointers[0]->bottomRight;
		vertices[cv++] = _glyphPointers[0]->topRight;
		vertices[cv++] = _glyphPointers[0]->topLeft;

		for (int cg = 1; cg < _glyphs.size(); cg++) // cg : current glyph
		{
			offset += 6;

			if (_glyphPointers[cg]->texture != _glyphPointers[cg - 1]->texture) {
				_renderBatches.emplace_back(offset, 6, _glyphPointers[cg]->texture);
			}
			else {
				_renderBatches.back().numVertices += 6;
			}

			vertices[cv++] = _glyphPointers[cg]->topLeft;
			vertices[cv++] = _glyphPointers[cg]->bottomLeft;
			vertices[cv++] = _glyphPointers[cg]->bottomRight;

			vertices[cv++] = _glyphPointers[cg]->bottomRight;
			vertices[cv++] = _glyphPointers[cg]->topRight;
			vertices[cv++] = _glyphPointers[cg]->topLeft;
		}

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW); // orphan the buffer
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data()); // upload the data

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void SpriteBatch::createVertexArray()
	{
		// check if init is already called (if vao and vbo already set), so that we don't call init 2 times
		if (_vao == 0)
		{
			glGenVertexArrays(1, &_vao);
		}
		glBindVertexArray(_vao);

		if (_vbo == 0)
		{
			glGenBuffers(1, &_vbo);
		}
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);


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
		switch (_sortType)
		{
		case BorEngine::GlyphSortType::BACK_TO_FRONT:
			std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareBackToFront);
			break;
		case BorEngine::GlyphSortType::FRONT_TO_BACK:
			std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareFrontToBack);
			break;
		case BorEngine::GlyphSortType::TEXTURE:
			std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareTexture);
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