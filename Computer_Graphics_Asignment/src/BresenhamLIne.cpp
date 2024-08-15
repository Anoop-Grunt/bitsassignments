#include "BresenhamLIne.h"
#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include <iostream>

///just initializes the endpoints values to the parameters received
BresenLine::BresenLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
	:x_co1(x1), y_co1(y1), x_co2(x2), y_co2(y2), m_RendererID(0)
{
	
}
/// destroys the line object, although this isn't explicitly called, when the object goes out of scope  it gets destroyed.
BresenLine::~BresenLine()
{
	
}

///the points generated in BresenMake() are used to generate a vertex buffer, an index buffer, and a vertex buffer layout is automatically specified assuming that all the primitives are in pixel space, and the offset to the next vertex is 2*sizeof(unsigned int), because other primitives like color etc are being sent to the fragment shader via uniforms
void BresenLine::draw2()
{
	BresenMake();
	unsigned int positions[] = {
		x_co1, y_co1,
		x_co2, y_co2
	};
	unsigned int indices[1250] = {};
	for (int i = 0; i < 1250;i++) {
		indices[i] = i;
	}
	VertexArray va;
	VertexBuffer vb(&bresenham_points, 2500 * sizeof(unsigned int));
	VertexBufferLayout layout;
	layout.Push<unsigned int>(2);
	int i = 0;
	const auto& elements = layout.GetElements();
	const auto& element = elements[i];
	unsigned int offset = 0;
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset);
	IndexBuffer ib(indices, 1250);
	ib.Bind();
	glDrawElements(GL_POINTS, 1250, GL_UNSIGNED_INT, nullptr);
	ib.Unbind();
	va.Unbind();
	/*std::cout << bresenham_points[3] << std::endl;*/

}
///similar to drawline() method but instead of plotting the line it will store the points in a buffer.
void BresenLine::BresenMake()
{
	unsigned int j = 0;
	int x1 = x_co1;
	int x2 = x_co2;
	int y1 = y_co1;
	int y2 = y_co2;

	int dx, dy, i, e;

	int incx, incy, inc1, inc2;

	int x, y;



	dx = x2 - x1;

	dy = y2 - y1;



	if (dx < 0) dx = -dx;

	if (dy < 0) dy = -dy;

	incx = 1;

	if (x2 < x1) incx = -1;

	incy = 1;

	if (y2 < y1) incy = -1;

	x = x1; y = y1;

	if (dx > dy) {

		/*glBegin(GL_POINTS);
		glVertex2i(x, y);
		glEnd();*/
		bresenham_points[j] = x;
		bresenham_points[j + 1] = y;
		j = j + 2;

		e = 2 * dy - dx;

		inc1 = 2 * (dy - dx);

		inc2 = 2 * dy;

		for (i = 0; i < dx; i++) {

			if (e >= 0) {

				y += incy;

				e += inc1;

			}

			else

				e += inc2;

			x += incx;

			bresenham_points[j] = x;
			bresenham_points[j + 1] = y;
			j = j + 2;

		}



	}
	else {

		bresenham_points[j] = x;
		bresenham_points[j + 1] = y;
		j = j + 2;

		e = 2 * dx - dy;

		inc1 = 2 * (dx - dy);

		inc2 = 2 * dx;

		for (i = 0; i < dy; i++) {

			if (e >= 0) {

				x += incx;

				e += inc1;

			}

			else

				e += inc2;

			y += incy;

			bresenham_points[j] = x;
			bresenham_points[j + 1] = y;
			j = j + 2;

		}

	}
}

/// draw_line is the method to draw line using BresenLine drawing algorithm

///here we are starting from the point (x1,y1) so if x2 and y2 are less than x1 and y1 then we set the increment value to be **-1**. 
///*case1:* if dx > dy then slope is less than 1 so only one pixel has to be picked along y-axis. if e value is less tha zero it incremented by 2*dy else by 2*(dy-dx) and y is incremented by 1. 
///
///
///*case2:* if dx<=dy then slope is greater then 1  so only one pixel has to be picked along y-axis. if e value is greatrer tha zero it incremented by 2*dx else by 2*(dx-dy) and x is incremented by 1. 
///in case1 and case2 the value of x and y are interchanged.

void BresenLine::draw_line() {

	int x1 = x_co1;
	int x2 = x_co2;
	int y1 = y_co1;
	int y2 = y_co2;

	int dx, dy, i, e;

	int incx, incy, inc1, inc2;

	int x, y;



	dx = x2 - x1;

	dy = y2 - y1;



	if (dx < 0) dx = -dx;

	if (dy < 0) dy = -dy;

	incx = 1;

	if (x2 < x1) incx = -1;

	incy = 1;

	if (y2 < y1) incy = -1;

	x = x1; y = y1;

	if (dx > dy) {

		glBegin(GL_POINTS);
			glVertex2i(x,y);
		glEnd();

		e = 2 * dy - dx;

		inc1 = 2 * (dy - dx);

		inc2 = 2 * dy;

		for (i = 0; i < dx; i++) {

			if (e >= 0) {

				y += incy;

				e += inc1;

			}

			else

				e += inc2;

			x += incx;

			glBegin(GL_POINTS);
			glVertex2i(x, y);
			glEnd();

		}



	}
	else {

		glBegin(GL_POINTS);
		glVertex2i(x, y);
		glEnd();

		e = 2 * dx - dy;

		inc1 = 2 * (dx - dy);

		inc2 = 2 * dx;

		for (i = 0; i < dy; i++) {

			if (e >= 0) {

				x += incx;

				e += inc1;

			}

			else

				e += inc2;

			y += incy;

			glBegin(GL_POINTS);
			glVertex2i(x, y);
			glEnd();

		}

	}

}

