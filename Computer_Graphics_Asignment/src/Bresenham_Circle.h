#pragma once

#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

class BresenCircle
{
public:
	BresenCircle(unsigned int x, unsigned int y, unsigned int r);
	~BresenCircle();
	void BresenMake();
	void draw();
	void draw2();

private:
	unsigned int center_x;
	unsigned int center_y;
	unsigned int radius;
	unsigned int points[4000] = {};
};

