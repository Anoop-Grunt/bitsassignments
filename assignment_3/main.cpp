#include <GL/freeglut.h>
#include <iostream>
#include <math.h>
#include <vector>
#include<cstdlib>

using namespace std;


/** global variables window_width and window_height to store width and height of the windows after reshaping */
int window_width = 640, window_height = 480;
int near_ver = -1;

class point
{
public:

    /** function lerp to calculate linear interpolation of two points at given t value */
    point lerp(point v1, point v2, double t)
    {
      point out;
      out.x = t * v1.x + (1 - t) * v2.x;
      out.y = t * v1.y + (1 - t) * v2.y;
      return out;
    }

    /** functon findoneless to reduce one control_point by applying linear interpolation between two concecutive control points */
    vector<point> findoneless(vector<point> vert, double t)
    {
     if(vert.size() != 1)
      {
        vector<point> v;
        for(double i=0;i < vert.size()-1; i++)
           v.push_back(lerp(vert[i],vert[i+1],t));
        return v;
      }
    }


    /** function de_Castlejau will return the final point at certain "t" value by using de castlejau algorithm */
    point de_Castlejau(double t)
    {
      /** if no of controlpoints is less than two, then there is no need apply algorithm */
    if(control_points.size() < 2)
      {
        point vect;
        vect.x = NULL;
        vect.y = NULL;
        return vect;
      }
    vector<point> lerp_merged;
    lerp_merged = findoneless(control_points,t);
    /** running while until we get single point */
    while(lerp_merged.size()!=1)
    {
      lerp_merged = findoneless(lerp_merged,t);
    }
    return lerp_merged[0];
    }

    /**function distance to calculate the square of distance between two points*/

    double distance(point v1, point v2)
    {
       return (v1.x-v2.x)*(v1.x-v2.x)+(v1.y-v2.y)*(v1.y-v2.y);
    }
    /** function nearest vertex to return index of the nearest vertex to the given vertex */
    int nearestvertex(double x, double y)
    {
        double minimum_distance;
        /**if no of controlpoints is 1, we can return zero */
        if(control_points.size() == 1)
        {
          return 0;
        }
     point vect;
     vect.x = x;
     vect.y = y;
     int index = 0;
     minimum_distance = distance(vect,control_points[index]);
     for (double i = 1; i < control_points.size() ; i++)
         {
         if (minimum_distance > distance(vect, control_points[i]))
            {
              minimum_distance = distance(vect, control_points[i]);
              index = i;
            }
         }
      return index;
    }
    vector<point> control_points;
    double x,y;


}obj;



/** function printmsg to print text on the window. glRasterPos2d specifies x and y co ordinates */
void printmsg(const string msg, double x, double y){
    glRasterPos2d(x, y);
    for (string::const_iterator ii = msg.begin();
        ii != msg.end();
        ++ii)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *ii);
    }
}


/** function mouse sets the mouse callback for the current window */
void mouse(int button, int state, int x,int y)
{
  /** on the left click of the mouse,
  * we will remove the nearest point to the selected point */
  if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    if(obj.control_points.size() == 0)
    {
      cout << "nothing to remove from control point" << '\n';
      return;
    }
    double x_pos = double(x)/window_width;
    double y_pos = 1-double(y)/window_height;
    int nearest_vertex_index;
    nearest_vertex_index = obj.nearestvertex(x_pos,y_pos);
    obj.control_points.erase(obj.control_points.begin()+nearest_vertex_index);
  }
  /** on the RIGHT click of the mouse,
  * we will add the nearest point to the selected point */
  else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
  {
    double x_pos = double(x)/window_width;
    double y_pos = 1-double(y)/window_height;
    point ver;
    ver.x = x_pos;
    ver.y = y_pos;
    obj.control_points.push_back(ver);
  }
  /** using middle button to move control points */
  else if(button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
  {
    if(obj.control_points.size() == 0)
    {
      cout << "no point to move" << endl ;
      return;
    }
    double x_pos = double(x)/window_width;
    double y_pos = 1-double(y)/window_height;
    int nearest_vertex_index;
    /** we can't precisely select point so we will remove nearest vertex */
    nearest_vertex_index = obj.nearestvertex(x_pos,y_pos);
    near_ver = nearest_vertex_index;
  }
  else if(button == GLUT_MIDDLE_BUTTON && state == GLUT_UP)
  {
    if(obj.control_points.size() == 0)
    {
      cout << "no point to move" << endl ;
      return;
    }
    if(near_ver == -1)
    {
      cout << "point is not selected" << endl;
      return;
    }
    double x_pos = double(x)/window_width;
    double y_pos = 1-double(y)/window_height;
    /** replacing the old points with new points at the same index */
    obj.control_points[near_ver].x = x_pos;
    obj.control_points[near_ver].y = y_pos;
    near_ver = -1;
  }
}

/** function reshape  defines what to do when the window is resized */
void reshape(int w, int h)
{
    /** Set the viewport to be the entire drawing area of the window */
    glViewport(0, 0, w, h);

    /** Save the window size */
    window_width = w;
    window_height = h;

    /** Set up the projection */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, 0.0, 1.0);
/** Always go back to model/view mode */
    glMatrixMode(GL_MODELVIEW); 
}

/** function init used to initialize the glut library */
void init()
{
  glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
  glutInitWindowSize(640,480);
  glutInitWindowPosition(50,50);
	glutCreateWindow("Bezier Curve");
}

/** function display to set the basic display modes */
void display()
{
  glClearColor(1.0,1.0,1.0,0.0);
  glClear(GL_COLOR_BUFFER_BIT);
    glColor3d(0.0, 0.0, 0.0); /**Setting the colour black */
    glPointSize(6.0);
    glBegin(GL_POINTS);
    /** Drawing all the point that are in our vector array of type point*/
    for (double i = 0; i < obj.control_points.size(); i++)
    {
        glVertex2f(obj.control_points[i].x, obj.control_points[i].y);
    }
    glPointSize(2.0);
    /** Drawing the curve */
    for (double i = 0; i < 1; i += 0.001)
    {
        point final;
        final = obj.de_Castlejau(i);
        glVertex2d(final.x, final.y);

    }
    glEnd();
    printmsg("click left button to remove control point", 0.05, 0.95);
    printmsg("click right button to add control point", 0.05, 0.90);
    printmsg("click middle button to move control point", 0.05, 0.85);
    glutSwapBuffers();
}

/** function main enters the GLUT event processing loop  */
int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  init();
  glutDisplayFunc(display);
  glutIdleFunc(display);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  
  glutMainLoop();

}
