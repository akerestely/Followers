#pragma once

struct Position
	{
		float x;
		float y;
	};

	struct Color
	{
		char r;
		char g;
		char b;
		char a;
	};

struct Vertex
{
	Position position;
	Color color;
};