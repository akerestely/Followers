#pragma once
#include <gl/glew.h>
#include <math.h>

#define NUM_BONES_PER_VEREX 4

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
		Position operator + (Position p)
		{
			p.x+=x;
			p.y+=y;
			p.z+=z;
			return p;
		}
		Position operator - (Position p)
		{
			p.x-=x;
			p.y-=y;
			p.z-=z;
			return p;
		}
		Position operator * (const float d)
		{
			Position p = *this;
			p.x*=d;
			p.y*=d;
			p.z*=d;
			return p;
		}
		Position operator / (const float d)
		{
			Position p = *this;
			p.x/=d;
			p.y/=d;
			p.z/=d;
			return p;
		}
		Position& operator /= (const float d)
		{
			this->x/=d;
			this->y/=d;
			this->z/=d;
			return *this;
		}
		void Normalize()
		{
			// calculate the length of the vector 
			float len = (float)(sqrt((x * x) + (y * y) + (z * z))); 
			// avoid division by 0 
			if (len == 0.0f) len = 1.0f; 
			// reduce to unit size 
			*this /= len;
		}
		static Position Normal(Engine::Position p1, Engine::Position p2, Engine::Position p3)
		{
			Engine::Position a, b;
			// calculate the vectors A and B
			// note that p[3] is defined with counterclockwise winding in mind
			a = p1 - p2; 
			b = p2 - p3; 

			// calculate the cross product
			Engine::Position normal;
			normal.x = (a.y * b.z) - (a.z * b.y); 
			normal.y = (a.z * b.x) - (a.x * b.z); 
			normal.z = (a.x * b.y) - (a.y * b.x); 

			return normal;
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

	struct BoneData
	{
		unsigned int ids[NUM_BONES_PER_VEREX];
		float weights[NUM_BONES_PER_VEREX];
		
		BoneData()
		{
			for (unsigned int i=0; i < NUM_BONES_PER_VEREX; i++)
				weights[i] = ids[i] = 0;
		}

		void Add(unsigned int boneId, float weight)
		{
			for (unsigned int i = 0 ; i < NUM_BONES_PER_VEREX ; i++) 
				if (weights[i] == 0.0)
				{
					ids[i] = boneId;
					weights[i] = weight;
					return;
				}        
		}
	};

	// Vertex, containing position, normal, uv coordinates
	struct Vertex
	{
		Position position;
		Position normal;
		UV uv;

		void SetPosition(float x, float y, float z)
		{
			position.x=x;
			position.y=y;
			position.z=z;
		}

		void SetNormal(float x, float y, float z)
		{
			normal.x=x;
			normal.y=y;
			normal.z=z;
		}

		void SetUV(float u, float v)
		{
			uv.u=u;
			uv.v=v;
		}
	};

	// Vertex, containing bone data
	struct BonedVertex
	{
		BoneData boneData;
		Position position;
		Position normal;
		UV uv;

		void SetPosition(float x, float y, float z)
		{
			position.x=x;
			position.y=y;
			position.z=z;
		}

		void SetNormal(float x, float y, float z)
		{
			normal.x=x;
			normal.y=y;
			normal.z=z;
		}

		void SetUV(float u, float v)
		{
			uv.u=u;
			uv.v=v;
		}
	};

	// Vertex, containing only position
	struct SimpleVertex
	{
		Position position;

		void SetPosition(float x, float y, float z)
		{
			position.x=x;
			position.y=y;
			position.z=z;
		}
	};
}