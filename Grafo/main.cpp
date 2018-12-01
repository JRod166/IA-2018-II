#define GLUT_DISABLE_ATEXIT_HACK
#include <iostream>
#include <windows.h>
#include <math.h>
#include <vector>
#include <GL/glut.h>
#include <fstream>
#include "c_nodo.h"
#include "c_graph.h"

#define KEY_ESC 27

#define PI 3.14159265f


int extern_n_x = 50;
int extern_n_y = 50;
float extern_d_y = 2000;
float extern_d_x = 2000;

bool bf_controller = false;
bool climbing_controller = false;
vector<float> mouse_click_controller_x = vector<float>(0,0);
vector<float> mouse_click_controller_y = vector<float>(0,0);

c_graph grafo_list(extern_n_x, extern_n_y, extern_d_x, extern_d_y);

using namespace std;

void init_GL(void);
void glPaint(void);
void OnMouseClick(int, int, int, int);
void OnMouseMotion(int, int);
void idle();
GLvoid window_redraw(GLsizei, GLsizei);
GLvoid window_key(unsigned char, int, int);

void translate_points(float&, float&);//Adapt incoming data to the new format needed in opengl map
void get_point(float&, float&);

void points_distribution();
void DrawCircle(float, float, int);
void draw_points();
void draw_lines();
void draw_routes();
void draw_main_points();


int main(int argc, char** argv){
  //grafo.set_configuration(1);
  grafo_list.set_configuration(1);
  //grafo_list.general_view();
  //grafo_list.alg_profundidad();
  //grafo_list.alg_climbing();
  //points_distribution();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1300, 700); //tama o de la ventana
	glutInitWindowPosition(0, 0); //posicion de la ventana
	glutCreateWindow("Grafo Simulation"); //titulo de la ventana

	init_GL();
	glutDisplayFunc(glPaint);
	glutReshapeFunc(&window_redraw);

	glutKeyboardFunc(&window_key);
  glutMouseFunc(&OnMouseClick);
	glutMotionFunc(&OnMouseMotion);
  glutIdleFunc(&idle);
  glutMainLoop(); //bucle de rendering
  return 0;
}

void points_distribution(){
}

void draw_points(){
  glPointSize(3);
  glColor3f(9.0f, 0.0f, 0.0f);
  glBegin(GL_POINTS);

  for(unsigned int i = 0; i < grafo_list.adyacentes.size(); i++){
    if(grafo_list.adyacentes[i].visible){
      glVertex2f(grafo_list.adyacentes[i].y, grafo_list.adyacentes[i].x);
    }
    //glVertex2f(points_distribution_y[i], points_distribution_x[i]);
  }
  //for(unsigned int i = 0; i < points_distribution_x.size(); i++){
    //DrawCircle(points_distribution_x[i], points_distribution_y[i], 1);
    //glVertex2f(points_distribution_y[i], points_distribution_x[i]);
  //}
  glEnd();
}

void draw_lines(){
  glColor3f( 0.329412, 0.329412, 0.329412);
  glBegin(GL_LINES);
  for(unsigned int i = 0; i < grafo_list.adyacentes.size(); i++){
    for(unsigned int j = 0; j < grafo_list.adyacentes[i].l_adyacentes.size(); j++){
      glVertex2f( grafo_list.adyacentes[i].y, grafo_list.adyacentes[i].x);
      glVertex2f( grafo_list.adyacentes[grafo_list.adyacentes[i].l_adyacentes[j]].y, grafo_list.adyacentes[grafo_list.adyacentes[i].l_adyacentes[j]].x);
    }
  }

  glEnd();
}

void DrawCircle(float cx, float cy, int r){
}

void translate_points(float& _x, float& _y){
  _x = (_x * extern_d_x) / glutGet(GLUT_WINDOW_HEIGHT);
  _y = (_y * extern_d_y) / glutGet(GLUT_WINDOW_WIDTH);
}

void draw_routes(){
  if(bf_controller){
    glColor3f(0.0f, 9.0f, 0.0f);
    glBegin(GL_LINES);
    for(unsigned int i = 1; i < grafo_list.bf_route.size(); i++){
      glVertex2f(grafo_list.adyacentes[grafo_list.bf_route[i-1]].y , grafo_list.adyacentes[grafo_list.bf_route[i-1]].x );
      glVertex2f(grafo_list.adyacentes[grafo_list.bf_route[i]].y , grafo_list.adyacentes[grafo_list.bf_route[i]].x );
    }
    glEnd();
  }
  if(climbing_controller){
    glColor3f(0.0f, 0.0f, 9.0f);
    glBegin(GL_LINES);
    for(unsigned int i = 1; i < grafo_list.climbing_route.size(); i++){
      glVertex2f(grafo_list.adyacentes[grafo_list.climbing_route[i-1]].y , grafo_list.adyacentes[grafo_list.climbing_route[i-1]].x );
      glVertex2f(grafo_list.adyacentes[grafo_list.climbing_route[i]].y , grafo_list.adyacentes[grafo_list.climbing_route[i]].x );
    }
    glEnd();
  }
}

void draw_main_points(){
  glPointSize(5);
  glColor3f(10.0f, 10.0f, 10.0f);
  glBegin(GL_POINTS);
      //glVertex2f(grafo_list.adyacentes[i].y, grafo_list.adyacentes[i].x);
      glVertex2f(grafo_list.adyacentes[grafo_list.initial_point].y, grafo_list.adyacentes[grafo_list.initial_point].x);
      glVertex2f(grafo_list.adyacentes[grafo_list.final_point].y, grafo_list.adyacentes[grafo_list.final_point].x);
  glEnd();
}

void get_point(float& a, float& b){
  a = a / ( (float)glutGet(GLUT_WINDOW_WIDTH)/extern_d_x);
	//b = ( (float)glutGet(GLUT_WINDOW_HEIGHT) - b ) / ( (float)glutGet(GLUT_WINDOW_HEIGHT) / extern_d_y);
  b = b / ((float)glutGet(GLUT_WINDOW_HEIGHT) / extern_d_y);
}

void init_GL(void) {
	//Color del fondo de la escena
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //(R, G, B, transparencia) en este caso un fondo negro
	//modo projeccion
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

void glPaint(void) {
	//El fondo de la escena al color initial
	glClear(GL_COLOR_BUFFER_BIT); //CAMBIO
	glLoadIdentity();
	//glOrtho(-300.0f,  300.0f,-300.0f, 300.0f, -1.0f, 1.0f);//Setting up configuration of  inner windows
  //glOrtho( 0.0f, windowWidth, windowHeight,  0.0f, 0.0f, 1.0f);//Setting up configuration of  inner windows
  glOrtho( 0.0f, extern_d_y, extern_d_x,  0.0f, 0.0f, 1.0f);//Setting up configuration of  inner windows
  //glOrtho( 0.0f, 1300.0f, 700.0f,  0.0f, 0.0f, 1.0f);//Setting up configuration of  inner windows
  draw_points();
  draw_lines();
  draw_routes();
  draw_main_points();
  //DrawCircle(0, 0, 1);
  glutSwapBuffers();
}


void OnMouseClick(int button, int state, int x, int y){
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
	float x_2 = (float)x;
	float y_2 = (float)y;
  get_point(x_2, y_2);
  //cout<<x_2<<";"<<y_2<<" -- "<<grafo_list.from_cord_to_p(x_2, y_2)<<endl;
  grafo_list.set_points(y_2, x_2);
  //cout<<grafo_list.initial_point<<" - "<<grafo_list.final_point<<endl;
  }

  if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
  //  cout<<"Click para eliminar"<<endl;
    float x_2 = (float)x;
    float y_2 = (float)y;
    get_point(x_2, y_2);
    if(mouse_click_controller_x.size() == 2){
      //for(unsigned int i = 0; i < mouse_click_controller_x.size(); i++){
        //cout<<mouse_click_controller_x[i]<<" - "<<mouse_click_controller_y[i]<<endl;
      //}
      grafo_list.rec_erase(mouse_click_controller_y[0], mouse_click_controller_x[0], mouse_click_controller_y[1], mouse_click_controller_x[1] );
      mouse_click_controller_x.clear();
      mouse_click_controller_y.clear();
    }
    else{
      mouse_click_controller_x.push_back(x_2);
      mouse_click_controller_y.push_back(y_2);
    }

  }
}

void OnMouseMotion(int x, int y){
}

void idle(){ // AGREGAR ESTA FUNCION
	glutPostRedisplay();
}

GLvoid window_key(unsigned char key, int x, int y) {
	switch (key) {
	case KEY_ESC:
		exit(0);
		break;
  case 'b':
    bf_controller = !bf_controller;
    if(bf_controller){
      grafo_list.alg_profundidad();
    }
    break;
  case 'c':
    climbing_controller = !climbing_controller;
    if(climbing_controller){
      grafo_list.alg_climbing();
    }
    break;
	default:
		break;
	}
}

GLvoid window_redraw(GLsizei width, GLsizei height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
  glOrtho( 0.0f, extern_d_y, extern_d_x,  0.0f, 0.0f, 1.0f);
}
