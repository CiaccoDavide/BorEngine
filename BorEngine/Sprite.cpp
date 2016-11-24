#include "ResourcesManager.h"
#include "Sprite.h"
#include "Vertex.h"

#include <cstddef>

namespace BorEngine
{
	Sprite::Sprite()
	{
		p_vboID = 0;
	}


	Sprite::~Sprite()
	{
		if (p_vboID != 0)
		{
			glDeleteBuffers(1, &p_vboID);
		}
	}


	void Sprite::init(float x, float y, float width, float height, std::string texturePath)
	{
		p_x = x;
		p_y = y;
		p_width = width;
		p_height = height;

		p_texture = ResourcesManager::getTexture(texturePath);

		if (p_vboID == 0) // if the vbo is still to be created
		{
			glGenBuffers(1, &p_vboID);
		}

		Vertex vertexData[6]; // 6x 6y coords
		//// first triangle
		//vertexData[0].setPosition(x + width / 2.0f, y + height / 2.0f);
		//vertexData[0].setUV(1.0f, 1.0f);
		//vertexData[1].setPosition(x - width / 2.0f, y + height / 2.0f);
		//vertexData[1].setUV(0.0f, 1.0f);
		//vertexData[2].setPosition(x - width / 2.0f, y - height / 2.0f);
		//vertexData[2].setUV(0.0f, 0.0f);
		//// second triangle
		//vertexData[3].setPosition(x - width / 2.0f, y - height / 2.0f);
		//vertexData[3].setUV(0.0f, 0.0f);
		//vertexData[4].setPosition(x + width / 2.0f, y - height / 2.0f);
		//vertexData[4].setUV(1.0f, 0.0f);
		//vertexData[5].setPosition(x + width / 2.0f, y + height / 2.0f);
		//vertexData[5].setUV(1.0f, 1.0f);



		//// first triangle
		vertexData[0].setPosition(x + width, y + height);
		vertexData[0].setUV(1.0f, 1.0f);
		vertexData[1].setPosition(x, y + height);
		vertexData[1].setUV(0.0f, 1.0f);
		vertexData[2].setPosition(x, y);
		vertexData[2].setUV(0.0f, 0.0f);
		// second triangle
		vertexData[3].setPosition(x, y);
		vertexData[3].setUV(0.0f, 0.0f);
		vertexData[4].setPosition(x + width, y);
		vertexData[4].setUV(1.0f, 0.0f);
		vertexData[5].setPosition(x + width, y + height);
		vertexData[5].setUV(1.0f, 1.0f);




		for (int i = 0; i < 6; i++)
		{
			vertexData[i].setColor(255, 228, 140, 255);
		}
		vertexData[1].setColor(228, 255, 140, 255);
		vertexData[4].setColor(150, 90, 250, 255);

		glBindBuffer(GL_ARRAY_BUFFER, p_vboID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Sprite::draw()
	{
		glBindTexture(GL_TEXTURE_2D, p_texture.id);

		glBindBuffer(GL_ARRAY_BUFFER, p_vboID);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		// position attrib
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		// color attrib
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		// uv attrib
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}