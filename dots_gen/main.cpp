#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <sstream>
#include <chrono>

#include <cxxopts.hpp>

#include <GL/gl.h>
#include <GL/glut.h>

using namespace std;

static size_t DIM = 2;

struct Color
{
  Color() : r(0), g(0), b(0) {}
  Color(double r_, double g_, double b_) : r(r_), g(g_), b(b_) {}

  double r, g, b;
};

struct Point
{
  Point() = default;
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

double mult1(const double d, const size_t dim, const size_t n, const vector<double> & angles)
{
  double res = d;

  if(n == 0)
  {
    res *= cos(angles[n]);
    return res;
  }
  res *= sin(angles[n]) * mult1(res, dim, n - 1, angles);
  return 0;
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

    for(size_t i = 0; i < DIM; ++i)
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
  glClearColor(1,1,1,1);
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
      if (p.c.size() == 1)
      {
        glVertex2d(*(p.c.end() - 1), 0);
      }else {
        glVertex2d(*(p.c.end() - 2), *(p.c.end() - 1));
      }

//      glVertex2d(*(p.c.end() - 1), *(p.c.end() - 2));
    }

    glColor3d(1, 1, 1);
//    glVertex2d(c.center.c[1], c.center.c[0]);
    glVertex2d(c.center.c[0], c.center.c[1]);
    glEnd();

  }

  glutPostRedisplay();
  glutSwapBuffers();
}

void GLkeybord(unsigned char, int, int)
{
  exit(0);
}

int main(int argc, char ** argv)
{
  cxxopts::Options opts ("dots_gen", "");
  opts.add_options()
      ("clusters",  "Clusters count",             cxxopts::value<size_t>()->    default_value("10"))
      ("dim",       "Dimensions",                 cxxopts::value<size_t>()->    default_value("2"))
      ("min_dots",  "Min dots in cluster",        cxxopts::value<uint32_t>()->  default_value("100"))
      ("max_dots",  "Max dots in cluster",        cxxopts::value<uint32_t>()->  default_value("1000"))
      ("min_rad",   "Min radius of cluster",      cxxopts::value<double>()->    default_value("0.2"))
      ("max_rad",   "Max radius of cluster",      cxxopts::value<double>()->    default_value("0.7"))
      ;
  auto opt_res = opts.parse(argc, argv);
  if(opt_res.arguments().size() == 0)
    cout << opts.help() << endl;

  size_t clusters_count = opt_res["clusters"].as<size_t>();
  DIM = opt_res["dim"].as<size_t>();
  uint32_t min_dots = opt_res["min_dots"].as<uint32_t>();
  uint32_t max_dots = opt_res["max_dots"].as<uint32_t>();
  double min_rad = opt_res["min_rad"].as<double>();
  double max_rad = opt_res["max_rad"].as<double>();

  random_device rd;
  default_random_engine eng {rd()};
//  default_random_engine eng {};
  uniform_real_distribution<double> rand_for_center {-1, 1};
  uniform_real_distribution<double> rand_rad {min_rad, max_rad};
  uniform_real_distribution<double> rand_color {0, 1};
  uniform_int_distribution<uint> rand_dots {min_dots, max_dots};

  auto begin = chrono::steady_clock::now();

  for(size_t i = 0; i < clusters_count; ++i)
  {
    vector<double> dots;
    for(size_t k = 0; k < DIM; ++k)
    {
      dots.push_back(rand_for_center(eng));
    }
    clusters.push_back((genCluster(Point(dots), rand_rad(eng), rand_dots(eng), Color(rand_color(eng),rand_color(eng),rand_color(eng)))));
  }

  auto end = chrono::steady_clock::now();

  if (DIM <= 2){
    GLinit(argc, argv);
    glutDisplayFunc(GLdisplay);
    glutKeyboardFunc(GLkeybord);
    glutMainLoop();
  }
  return 0;
}
