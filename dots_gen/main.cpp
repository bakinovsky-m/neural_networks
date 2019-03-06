#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <sstream>
#include <chrono>
#include <thread>

#include <cxxopts.hpp>

#include <GL/gl.h>
#include <GL/glut.h>


#include "structs.h"
#include "gl_funcs.h"

using namespace std;

vector<Cluster> clusters;
static random_device rd;
static default_random_engine eng (rd());

Point genPoint(const Point &center, const double radius)
{
  double DIM = center.c.size();
  uniform_real_distribution<double> rand(0, 1);
  vector<double> angles;

  for(size_t i = 0; i < DIM - 1; ++i)
  {
    angles.push_back(rand(eng) * 3.1415);
  }
  angles[0] *= 2;

  Point p;

  for(size_t xi = 0; xi < DIM; ++xi)
  {
    double to_p = radius;

    for(size_t kk = xi; kk < DIM - 1; ++kk)
    {
      to_p *= sin(angles[kk]);
    }
    if(xi != 0)
      to_p *= cos(angles[xi - 1]);

    to_p += center.c[xi];

    p.c.push_back(to_p);
  }

  return p;
}

Cluster genCluster(const Point &center, const double radius, const uint count, const Color color)
{
  Cluster cluster {};
  cluster.center = center;
  cluster.radius = radius;

  uniform_real_distribution<double> rand {0, 1};

  for(uint ccount = 0; ccount < count; ++ccount)
  {
    double r = radius * rand(eng);

    Point p = genPoint(center, r);

    cluster.points.push_back(p);
  }

  cluster.color = color;

  return cluster;
}

int main(int argc, char ** argv)
{
  cxxopts::Options opts ("dots_gen", "");
  opts.add_options()
      ("clusters",  "Clusters count",                 cxxopts::value<size_t>()->    default_value("2"))
      ("dim",       "Dimensions",                     cxxopts::value<size_t>()->    default_value("2"))
      ("min_dots",  "Min dots in cluster",            cxxopts::value<uint32_t>()->  default_value("10"))
      ("max_dots",  "Max dots in cluster",            cxxopts::value<uint32_t>()->  default_value("10"))
      ("min_rad",   "Min radius of cluster",          cxxopts::value<double>()->    default_value("0.2"))
      ("max_rad",   "Max radius of cluster",          cxxopts::value<double>()->    default_value("0.5"))
      ("intersecs",   "Number of intersections",      cxxopts::value<uint32_t>()->    default_value("0"))
      ("int_percent",   "Percent of intersections",   cxxopts::value<double>()->    default_value("0"))
      ;
  auto opt_res = opts.parse(argc, argv);
  // без этого одинаковые значения в нулевом кластере (???)
//  if(opt_res.arguments().size() == 0)
  cout << opts.help() << endl;

  size_t clusters_count = opt_res["clusters"].as<size_t>();
  size_t DIM = opt_res["dim"].as<size_t>();
  if (DIM == 1) throw invalid_argument("can't do it in 1-dim");
  uint32_t min_dots = opt_res["min_dots"].as<uint32_t>();
  uint32_t max_dots = opt_res["max_dots"].as<uint32_t>();
  double min_rad = opt_res["min_rad"].as<double>();
  double max_rad = opt_res["max_rad"].as<double>();
  uint32_t intersecs = opt_res["intersecs"].as<uint32_t>();
  double int_percent = opt_res["int_percent"].as<double>();

  uniform_real_distribution<double> rand_for_center {-1, 1};
  uniform_real_distribution<double> rand_rad {min_rad, max_rad};
  uniform_real_distribution<double> rand_color {0, 1};
  uniform_int_distribution<uint> rand_dots {min_dots, max_dots};

  auto begin = chrono::steady_clock::now();

  for(size_t i = 0; i < clusters_count; ++i)
  {
    vector<double> dots;
    Point maybe_center;
    double radius = 0;

    bool found = false;
    int counter = 0;
    while(!found)
    {
      counter++;
      if (counter >= 100000)
      {
        cout << "plz, set smaller radius or less clusters, dimension is too small" << endl;
        return -1;
      }
      dots.clear();
      for(size_t k = 0; k < DIM; ++k)
      {
        dots.push_back(rand_for_center(eng));
      }
      maybe_center = Point(dots);
      radius = rand_rad(eng);

      found = true;
//      int j = 0;
      for (auto &c : clusters)
      {
//        cout << "cluster " << i << " with ";
//        cout << "cluster " << j << endl;
//        cout << c.center.dist(maybe_center) << endl;
        if (c.center.dist(maybe_center) <= c.radius + radius)
        {
//          cout << "ALARM! " << c.center.dist(maybe_center) << " <= " << c.radius << endl;
          found = false;
          break;
        }
//        j++;
      }
    }

    Cluster cl = genCluster(maybe_center, radius, rand_dots(eng), Color(rand_color(eng),rand_color(eng),rand_color(eng)));
    clusters.push_back(cl);
    cout << "clusters size: " << clusters.size() << "\r";
    cout.flush();
  }

  auto end = chrono::steady_clock::now();

  cout << "generating: " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << " msec" << endl;

//  int i = 0;
//  for(auto & c: clusters)
//  {
//    cout << i << ": " << c.points.size() << " " << c.center.c[0] << ":" << c.center.c[1] << " " << c.radius << endl;
//    ++i;
//  }

  if (DIM <= 2){
    GLinit(argc, argv);
    glutDisplayFunc(GLdisplay);
    glutKeyboardFunc(GLkeybord);
    glutMainLoop();
  }
  return 0;
}
