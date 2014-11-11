#include "Sprite.h"


Sprite::Sprite(void)
{
	vboId=1;
}

void Sprite::Init(float x,float y,float width,float height)
{
	this->x=x;
	this->y=y;
	this->width=width;
	this->height=height;

	if (!vboId)
	{
		glGenBuffers(1,&vboId);
	}

	float vertexData[12];

	vertexData[0] = x + width;
	vertexData[1] = y + height;
	vertexData[2] = x;
	vertexData[3] =	y + height;
	vertexData[4] = x;
	vertexData[5] = y;

	vertexData[6] = x;
	vertexData[7] = y;
	vertexData[8] =	x + width;
	vertexData[9] =	y;
	vertexData[10] = x + width;
	vertexData[11] = y + height;

	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData),vertexData,GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Sprite::Draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,0);
	glDrawArrays(GL_TRIANGLES,0,6);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Sprite::~Sprite(void)
{
	if(vboId)
		glDeleteBuffers(1,&vboId);
}
