#include "Sprite.h"


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

	float vertexData[12]; // 6x 6y coords
	// first triangle
	vertexData[0] = x - width / 2.0f;
	vertexData[1] = y + height / 2.0f;

	vertexData[2] = x + width / 2.0f;
	vertexData[3] = y + height / 2.0f;

	vertexData[4] = x - width / 2.0f;
	vertexData[5] = y - height / 2.0f;
	// second triangle
	vertexData[6] = x - width / 2.0f;
	vertexData[7] = y - height / 2.0f;

	vertexData[8] = x + width / 2.0f;
	vertexData[9] = y + height / 2.0f;

	vertexData[10] = x + width / 2.0f;
	vertexData[11] = y - height / 2.0f;

	glBindBuffer(GL_ARRAY_BUFFER, _vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Sprite::draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, _vboID);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}