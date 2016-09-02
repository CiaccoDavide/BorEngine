#pragma once

#include <GL/glew.h>

// we don't need a cpp file because there is no method, only data

namespace BorEngine
{
	struct Position
	{
		float x;
		float y;
	};

	struct ColorRGB8
	{
		ColorRGB8() : r(0), g(0), b(0) {}
		ColorRGB8(GLubyte R, GLubyte G, GLubyte B) : r(R), g(G), b(B) {}
		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;
	};

	struct UV
	{
		float u;
		float v;
	};

	struct Vertex
	{
		Position position;

		ColorRGB8 color;

		UV uv;

		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
		{
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		}

		void setUV(float u, float v)
		{
			uv.u = u;
			uv.v = v;
		}

		void setPosition(float x, float y)
		{
			position.x = x;
			position.y = y;
		}
	};
}
