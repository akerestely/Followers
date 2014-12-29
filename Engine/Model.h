#pragma once
class Model
{
public:
	Model(void);
	~Model(void);

	void Render();
private:
	unsigned int vboId,ibo_cube_elements;
};

