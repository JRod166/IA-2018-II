#ifndef C_NODO_H
#define C_NODO_H

#include <iostream>
#include <vector>

using namespace std;

class c_nodo{
public:
  float x; //Posicion determinada dentro del canvas
  float y;  //Posicion determinada dentro del canvas
  vector<int> l_adyacentes; //Lista de los nodos en conexion.
  bool visible;

  c_nodo();
  ~c_nodo();
  void add_adyacent_element(int);
  void set_positions(float, float);
};

c_nodo::c_nodo(){
  this->visible = true;
  this->x = 0;
  this->y = 0;
  this->l_adyacentes = vector<int>(0,0);
}

c_nodo::~c_nodo(){}

void c_nodo::add_adyacent_element(int coming_value){
  for(unsigned int i = 0; i < this->l_adyacentes.size(); i++){
    if(this->l_adyacentes[i] == coming_value){
      return;
    }
  }
  this->l_adyacentes.push_back(coming_value);
}

void c_nodo::set_positions(float _x, float _y){
  this->x = _x;
  this->y = _y;
}


#endif
