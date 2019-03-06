#ifndef GL_FUNCS_H
#define GL_FUNCS_H

#include <vector>

#include <GL/gl.h>
#include <GL/glut.h>

#include "structs.h"

extern std::vector<Cluster> clusters;

void GLinit(int argc, char ** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(1000, 900);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("MyOpenGL");
  glClearColor(1, 1, 1, 1);
  glPointSize(3);
  glLineWidth(3);
}

void GLdisplay()
{
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  glPointSize(3);
#ifdef DRAW_EXTRA
  char i = 0;
#endif // DRAW_EXTRA
  for(auto & c : clusters)
  {
    glColor3d(c.color.r, c.color.g, c.color.b);

    glBegin(GL_POINTS);
    for (auto & p : c.points)
    {
      glVertex2d(p.c[0], p.c[1]);
    }

    glColor3d(0, 0, 0);
    glVertex2d(c.center.c[0], c.center.c[1]);
    glEnd();
#ifdef DRAW_EXTRA
    glBegin(GL_LINES);
    glVertex2d(c.center.c[0], c.center.c[1]);
    glVertex2d(c.center.c[0] + c.radius, c.center.c[1]);
    glEnd();
    glRasterPos2d(c.center.c[0], c.center.c[1]);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, i+48);
    ++i;
#endif //DRAW_EXTRA
  }

#ifdef DRAW_EXTRA
  glColor3d(0,0,0);
  glPointSize(7);
  glBegin(GL_POINTS);
  glVertex2d(0, 0);
  glVertex2d(0.25, 0);
  glVertex2d(0.5, 0);
  glVertex2d(0.75, 0);
  glVertex2d(-0.25, 0);
  glVertex2d(-0.5, 0);
  glVertex2d(-0.75, 0);
  glVertex2d(0, 0.25);
  glVertex2d(0, 0.5);
  glVertex2d(0, 0.75);
  glVertex2d(0, -0.25);
  glVertex2d(0, -0.5);
  glVertex2d(0, -0.75);
  glEnd();
#endif //DRAW_EXTRA

  static bool redisp = true;
  if (redisp)
  {
    glutPostRedisplay();
    glutSwapBuffers();
    redisp = false;
  }
}

void GLkeybord(unsigned char, int, int)
{
  exit(0);
}

#endif // GL_FUNCS_H
