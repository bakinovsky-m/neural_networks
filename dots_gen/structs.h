#ifndef STRUCTS_H
#define STRUCTS_H

#include <vector>
#include <cmath>

static constexpr double EPSILON = 0.000001;

struct Color
{
  Color() : r(0), g(0), b(0) {}
  Color(double r_, double g_, double b_) : r(r_), g(g_), b(b_) {}

  double r, g, b;
};

struct Point
{
  Point() = default;
  Point(std::vector<double> c_)
  {
    c = c_;
  }
  double dist(const Point & p) const
  {
    double res = 0;
    for(size_t i = 0; i < p.c.size(); ++i)
    {
      res += pow(c[i] - p.c[i], 2);
    }
    return sqrt(res);
  }
  bool operator==(const Point &p) const
  {
    for(size_t i = 0; i < c.size(); ++i)
    {
      if(fabs(c[i] - p.c[i]) > EPSILON)
        return false;
    }
    return true;
  }

  std::vector<double> c;
};

struct Cluster
{
  Cluster() = default;
  Point center;
  std::vector<Point> points;
  Color color;
  double radius = 0;
  bool intersected = false;
};

#endif // STRUCTS_H
