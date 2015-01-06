#pragma once
#include <gl/glew.h>
#include <math.h>

namespace Engine
{
	struct Position
	{
		float x;
		float y;
		float z;
		Position(float x = 0.0f, float y = 0.0f, float z = 0.0f) :x(x), y(y), z(z)
		{
		}
		void Normalize()
		{
			// calculate the length of the vector 
			float len = (float)(sqrt((x * x) + (y * y) + (z * z))); 
			// avoid division by 0 
			if (len == 0.0f) len = 1.0f; 
			// reduce to unit size 
			x /= len; 
			y /= len; 
			z /= len;
		}
	};

	struct ColorRGBA8
	{
		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;
		ColorRGBA8(GLubyte r=0, GLubyte g=0, GLubyte b=0, GLubyte a=255) :r(r), g(g), b(b), a(a)
		{
		}
	};

	struct UV
	{
		float u;
		float v;
		UV(float u = 0.0f, float v = 0.0f) :u(u), v(v)
		{
		}
	};

	struct Vertex
	{
		Position position;
		Position normal;
		ColorRGBA8 color;
		UV uv;

		void SetPosition(float x, float y, float z)
		{
			position.x=x;
			position.y=y;
			position.z=z;
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