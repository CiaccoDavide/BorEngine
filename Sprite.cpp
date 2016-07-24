#include "Sprite.h"
#include "Vertex.h"

#include <cstddef>

Sprite::Sprite()
{
	_vboID = 0;
}


Sprite::~Sprite()
{
	if (_vboID != 0)
	{
		glDeleteBuffers(1, &_vboID);
	}
}


void Sprite::init(float x, float y, float width, float height)
{
	_x = x;
	_y = y;
	_width = width;
	_height = height;

	if (_vboID == 0) // if the vbo is still to be created
	{
		glGenBuffers(1, &_vboID);
	}

	Vertex vertexData[6]; // 6x 6y coords
	// first triangle
	vertexData[0].setPosition(x + width / 2.0f, y + height / 2.0f);
	vertexData[0].setUV(1.0f, 1.0f);
	vertexData[1].setPosition(x - width / 2.0f, y + height / 2.0f);
	vertexData[1].setUV(0.0f, 1.0f);
	vertexData[2].setPosition(x - width / 2.0f, y - height / 2.0f);
	vertexData[2].setUV(0.0f, 0.0f);
	// second triangle
	vertexData[3].setPosition(x - width / 2.0f, y - height / 2.0f);
	vertexData[3].setUV(0.0f, 0.0f);
	vertexData[4].setPosition(x + width / 2.0f, y - height / 2.0f);
	vertexData[4].setUV(1.0f, 0.0f);
	vertexData[5].setPosition(x + width / 2.0f, y + height / 2.0f);
	vertexData[5].setUV(1.0f, 1.0f);

	for (int i = 0; i < 6; i++)
	{
		vertexData[i].setColor(255, 228, 140, 255);
	}
	vertexData[1].setColor(228, 255, 140, 255);
	vertexData[4].setColor(150,90,250,255);

	glBindBuffer(GL_ARRAY_BUFFER, _vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Sprite::draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, _vboID);
	glEnableVertexAttribArray(0);

	// position attrib
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	// color attrib
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	// uv attrib
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}