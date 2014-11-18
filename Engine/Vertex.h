#pragma once

namespace Engine
{
	struct Position
	{
		float x;
		float y;
	};

	struct ColorRGBA8
	{
		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;
		ColorRGBA8(GLubyte r=255, GLubyte b=255, GLubyte g=255, GLubyte a=255) :r(r), g(g), b(b), a(a)
		{
		}
	};

	struct UV
	{
		float u;
		float v;
	};

	struct Vertex
	{
		Position position;
		ColorRGBA8 color;
		UV uv;

		void SetPosition(float x, float y)
		{
			position.x=x;
			position.y=y;
		}

		void SetColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
		{
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		}

		void SetUV(float u, float v)
		{
			uv.u=u;
			uv.v=v;
		}
	};
}