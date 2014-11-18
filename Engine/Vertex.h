#pragma once

namespace Engine
{
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
		Color(char r=255, char b=255, char g=255, char a=255)
		{
			this->r=r;
			this->g=g;
			this->b=b;
			this->a=a;
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
		Color color;
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