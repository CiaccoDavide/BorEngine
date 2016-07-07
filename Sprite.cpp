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
	vertexData[0].position.x = x + width / 2.0f;
	vertexData[0].position.y = y + height / 2.0f;

	vertexData[1].position.x = x - width / 2.0f;
	vertexData[1].position.y = y + height / 2.0f;

	vertexData[2].position.x = x - width / 2.0f;
	vertexData[2].position.y = y - height / 2.0f;
	// second triangle
	vertexData[3].position.x = x - width / 2.0f;
	vertexData[3].position.y = y - height / 2.0f;

	vertexData[4].position.x = x + width / 2.0f;
	vertexData[4].position.y = y - height / 2.0f;

	vertexData[5].position.x = x + width / 2.0f;
	vertexData[5].position.y = y + height / 2.0f;

	for (int i = 0; i < 6; i++)
	{
		vertexData[i].color.r = 255;
		vertexData[i].color.g = 228;
		vertexData[i].color.b = 140;
		vertexData[i].color.a = 255;
	}

	vertexData[1].color.r = 228;
	vertexData[1].color.g = 255;
	vertexData[1].color.b = 140;

	vertexData[4].color.r = 150;
	vertexData[4].color.g = 90;
	vertexData[4].color.b = 250;

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
	//color attrib
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE,GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}