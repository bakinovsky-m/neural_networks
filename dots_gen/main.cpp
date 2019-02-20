#include <vector>
#include <random>
#include <array>
#include <algorithm>
#include <cmath>

#include <GL/gl.h>
#include <GL/glut.h>

using namespace std;


struct Color
{
  Color() = default;
  Color(double r_, double g_, double b_) : r(r_), g(g_), b(b_) {}
//  Color & operator=(Color& c)
//  {
//    r = c.r;
//    g = c.g;
//    b = c.b;
//  }

  double r, g, b;
};

struct Point
{
  Point() = default;
  Point(double x_, double y_) : x(x_), y(y_) {}
  double x = 0;
  double y = 0;
};


struct Cluster
{
  Cluster() = default;
  vector<Point> points;
  Color color;
};


static vector<Cluster> clusters;


Cluster genCluster(double center_x, double center_y, double radius, uint count, Color color = Color{})
{
  Cluster cluster {};

  random_device rd;
  default_random_engine eng {rd()};
  uniform_real_distribution<double> rand {0, 1};

  for(uint i = 0; i < count; ++i)
  {
    double a = rand(eng) * 2 * 3.14159265359;
    double r = radius * sqrt(rand(eng));

    double x = r * cos(a) + center_x;
    double y = r * sin(a) + center_y;
    cluster.points.push_back(Point(x, y));
  }

  cluster.color = color;

  return cluster;
}

void GLinit()
{
  glClearColor(0,0,0,1);
  glPointSize(3);
}

void GLdisplay()
{
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  for(Cluster & c : clusters)
  {
    glColor3d(c.color.r, c.color.g, c.color.b);

    glBegin(GL_POINTS);
    for (Point & p : c.points)
    {
      glVertex2d(p.x, p.y);
    }
    glEnd();
  }

  glutPostRedisplay();
  glutSwapBuffers();
}

int main(int argc, char ** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(1000, 900);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("MyOpenGL");
  GLinit();
  glutDisplayFunc(GLdisplay);

  clusters.push_back((genCluster(0.1, 0.1, 0.2, 1000, Color(0,1,0))));
  clusters.push_back((genCluster(-0.1, -0.2, 0.2, 1000, Color(0,0,1))));
  clusters.push_back((genCluster(0.2, -0.2, 0.2, 1000, Color(1,0,0))));

  glutMainLoop();
  return 0;
}
