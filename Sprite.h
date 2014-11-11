#pragma once
#include <gl\glew.h>

class Sprite
{
public:
	Sprite(void);
	void Init(float x,float y,float width,float height);
	void Draw();
	~Sprite(void);
private:
	GLuint vboId;
	float x,y,width,height;
};

