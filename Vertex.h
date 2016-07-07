#pragma once

#include <GL/glew.h>

// we don't need a cpp file because there is no method, only data

struct Position
{
	float x;
	float y;
};

struct Color
{
	GLubyte r;
	GLubyte g;
	GLubyte b;
	GLubyte a;
};

struct Vertex
{
	Position position;

	Color color;
};