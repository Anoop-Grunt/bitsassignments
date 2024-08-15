#pragma once
#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

class BresenLine
{
public:
	BresenLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
	~BresenLine();

	void draw2();
	void BresenMake();
	void draw_line();

private:

	unsigned int m_RendererID;
	unsigned int x_co1, y_co1, x_co2, y_co2;
	unsigned int bresenham_points[2500] = {};
	
};
