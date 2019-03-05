#include <iostream>
#include <vector>
#include <random>
#include <array>
#include <algorithm>
#include <cmath>
#include <functional>

#include <GL/gl.h>
#include <GL/glut.h>

using namespace std;

constexpr int DIM = 2;

struct Color
{
  Color() : r(0), g(0), b(0) {};
  Color(double r_, double g_, double b_) : r(r_), g(g_), b(b_) {}

  double r, g, b;
};

struct Point
{
  Point() = default;
//  Point(double x_, double y_)
//  {
//    c.push_back(x_);
//    c.push_back(y_);
//  }
  Point(vector<double> c_)
  {
    c = c_;
  }

  vector<double> c;
};

struct Cluster
{
  Cluster() = default;
  Point center;
  vector<Point> points;
  Color color;
};


static vector<Cluster> clusters;

double mult1(const double d, const int dim, const int n, const vector<double> & angles)
{
  double res = d;

  if(n == 0)
  {
    res *= cos(angles[n]);
    return res;
  }
  res *= sin(angles[n]) * mult1(res, dim, n - 1, angles);
}

Cluster genCluster(const Point center, const double radius, const uint count, const Color color)
{
  Cluster cluster {};
  cluster.center = center;

  random_device rd;
  default_random_engine eng {rd()};
  uniform_real_distribution<double> rand {0, 1};

  for(uint ccount = 0; ccount < count; ++ccount)
  {
    vector<double> angles;

    for(size_t i = 0; i < DIM-1; ++i)
    {
      angles.push_back(rand(eng) * 2 * 3.14159265359);
    }
    double r = radius * sqrt(rand(eng));

    Point p;


    for(size_t xi = 0; xi < DIM; ++xi)
    {
      p.c.push_back(r);
      p.c.back() *= mult1(p.c.back(), DIM, xi, angles);
      p.c.back() += center.c[xi];
    }

    p.c.back() = *(p.c.end() - 2);
    p.c.back() -= center.c[p.c.size() - 2];
    p.c.back() /= cos(angles[0]);
    p.c.back() *= sin(angles[0]);
    p.c.back() += center.c[p.c.size() - 1];

    cluster.points.push_back(p);
  }

  cluster.color = color;

  return cluster;
}

void GLinit(int argc, char ** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(1000, 900);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("MyOpenGL");
  glClearColor(0,0,0,1);
  glPointSize(3);
}

void GLdisplay()
{
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  for(auto & c : clusters)
  {
    glColor3d(c.color.r, c.color.g, c.color.b);

    glBegin(GL_POINTS);
    for (auto & p : c.points)
    {
      glVertex2d(*(p.c.end() - 2), *(p.c.end() - 1));
    }

    glColor3d(1, 1, 1);
    glVertex2d(c.center.c[0], c.center.c[1]);
    glEnd();

  }

  glutPostRedisplay();
  glutSwapBuffers();
}

int main(int argc, char ** argv)
{
  GLinit(argc, argv);
  glutDisplayFunc(GLdisplay);

  random_device rd;
  default_random_engine eng {rd()};
//  default_random_engine eng {};
  uniform_real_distribution<double> rand {-1, 1};
  uniform_real_distribution<double> rand_rad {0.3, 0.5};
  uniform_real_distribution<double> rand_color {0, 1};
  uniform_int_distribution<int> rand_dots {10, 1000};

  for(int i = 0; i < 10; ++i)
  {
    vector<double> dots;
    for(int k = 0; k < rand_dots(eng); ++k)
    {
      dots.push_back(rand(eng));
    }
    clusters.push_back((genCluster(Point(dots), rand_rad(eng), 1000, Color(rand_color(eng),rand_color(eng),rand_color(eng)))));
  }

  glutMainLoop();
  return 0;
}
