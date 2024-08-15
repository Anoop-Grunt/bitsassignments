#include "Bresenham_Circle.h"
#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include <iostream>

///intializing circle with centre and radius of the circle
BresenCircle::BresenCircle(unsigned int x, unsigned int y, unsigned int r)
	:center_x(x), center_y(y), radius(r)
{
}

BresenCircle::~BresenCircle()
{
}
///to calculate all the points of the circle with **Bresenham_Circle_drawing**. 

///after calculating value  of (x,y) we calculate (x,-y),(x,-y),(-x,-y),(-x,y),(y,x),(y,-x),(-y,-x),(-y,x)
/// as circle is 8-way symmetric. 

///starting value of x is **0** and y is **r** and intial pk value is **1.25 minus radius**. 

///if east point(**pk<0**) is choosen then pk is incremented by 2*x+1 elseif southeast(**pk>=0**) is choosen then pk is incremented by 2*(x-y)+1. 
/// this continues until x = y and calculate (x,-y),(x,-y),(-x,-y),(-x,y),(y,x),(y,-x),(-y,-x),(-y,x)
/// as circle is 8-way symmetric.

///@note this function will store all the primitives in an array.
void BresenCircle::BresenMake()
{
    int j = 0;
    int r = radius;
    int xc = center_x;
    int yc = center_y;
    int x = 0, y = r;
    float pk = (5.0 / 4.0) - r;

    
    points[j] = xc + x;
    points[j + 1] = yc + y;
    points[j + 2] = xc + x;
    points[j + 3] = yc - y;
    points[j + 4] = xc + y;
    points[j + 5] = yc + x;
    points[j + 6] = xc + y;
    points[j + 7] = yc - x;
    points[j + 8] = xc - x;
    points[j + 9] = yc - y;
    points[j + 10] = xc - y;
    points[j + 11] = yc - x;
    points[j + 12] = xc - x;
    points[j + 13] = yc + y;
    points[j + 14] = xc - y;
    points[j + 15] = yc + x;
    j = j + 16;

    int k;
    while (x < y)
    {
        x = x + 1;
        if (pk < 0)
            pk = pk + 2 * x + 1;
        else
        {
            y = y - 1;
            pk = pk + 2 * (x - y) + 1;
        }
        points[j] = xc + x;
        points[j+1] = yc + y;
        points[j+2] = xc + x;
        points[j+3] = yc - y;
        points[j+4] = xc + y;
        points[j+5] = yc + x;
        points[j+6] = xc + y;
        points[j+7] = yc - x;
        points[j+8] = xc - x;
        points[j+9] = yc - y;
        points[j+10] = xc - y;
        points[j+11] = yc - x;
        points[j+12] = xc - x;
        points[j+13] = yc + y;
        points[j+14] = xc - y;
        points[j+15] = yc + x;
        j = j + 16;
    }
    glFlush();
}
/// this is similar to BresenMake() but it plots the values on the windows instead of storing them

///*glVertex2i()* is an function to plot points on windows
///*glFlush()* empties all commands in these buffers and forces all pending commands will to be executed immediately without waiting for uffers

void BresenCircle::draw()
{
    int j = 0;
    int r = radius;
    int xc = center_x;
    int yc = center_y;
    int x = 0, y = r;
    float pk = (5.0 / 4.0) - r;

    /* Plot the points */
    /* Plot the first point */
    glBegin(GL_POINTS);
    glVertex2i(xc + x, yc + y);
    glVertex2i(xc + x, yc - y);
    glVertex2i(xc + y, yc + x);
    glVertex2i(xc + y, yc - x);
    glVertex2i(xc - x, yc - y);
    glVertex2i(xc - y, yc - x);
    glVertex2i(xc - x, yc + y);
    glVertex2i(xc - y, yc + x);
    glEnd();
 

    int k;
    /* Find all vertices till x=y */
    while (x < y)
    {
        x = x + 1;
        if (pk < 0)
            pk = pk + 2 * x + 1;
        else
        {
            y = y - 1;
            pk = pk + 2 * (x - y) + 1;
        }
        glBegin(GL_POINTS);
        glVertex2i(xc + x, yc + y);
        glVertex2i(xc + x, yc - y);
        glVertex2i(xc + y, yc + x);
        glVertex2i(xc + y, yc - x);
        glVertex2i(xc - x, yc - y);
        glVertex2i(xc - y, yc - x);
        glVertex2i(xc - x, yc + y);
        glVertex2i(xc - y, yc + x);
        glEnd();
    }
    glFlush();
}
/// this function will use the list of primitives generated by the function BresenMake()
///to generate a vertex buffer and an index buffer and specifies the layout and finally draws circle
void BresenCircle::draw2()
{
    BresenMake();
    unsigned int indices[2000] = {};
    for (int i = 0; i < 2000;i++) {
        indices[i] = i;
    }
    VertexArray va;
    VertexBuffer vb(&points, 4000 * sizeof(unsigned int));
    VertexBufferLayout layout;
    layout.Push<unsigned int>(2);
    int i = 0;
    const auto& elements = layout.GetElements();
    const auto& element = elements[i];
    unsigned int offset = 0;
    glEnableVertexAttribArray(i);
    glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset);
    IndexBuffer ib(indices, 2000);
    ib.Bind();
    glDrawElements(GL_POINTS, 2000, GL_UNSIGNED_INT, nullptr);
    ib.Unbind(); 
    va.Unbind();
}



