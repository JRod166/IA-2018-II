#ifndef C_PROFUNDIDAD_H
#define C_PROFUNDIDAD_H

#include <iostream>
#include <vector>

using namespace std;

class c_profundidad{
public:
  int key;
  vector<int> route;

  c_profundidad();
  c_profundidad(int);
  c_profundidad(int, int);
  c_profundidad(int, vector<int>);
  ~c_profundidad();

  void add_route_member(int);
  void dominating_route(int);
  vector<int> get_route();

};

c_profundidad::c_profundidad(){
  this->key = 0;
  this->route = vector<int>(0,0);
}

c_profundidad::c_profundidad(int _key){
  this->key = _key;
  this->route = vector<int>(0,0);
}

c_profundidad::c_profundidad(int _key, int _route){
  this->key = _key;
  this->route = vector<int>(1, _route);
}

c_profundidad::c_profundidad(int _key, vector<int> _route){
  this->key = _key;
  this->route = _route;
}

c_profundidad::~c_profundidad(){}

//la ruta se lee de atras para adelante
void c_profundidad::add_route_member(int _route){
  this->route.push_back(_route);
}

void c_profundidad::dominating_route(int new_coming){
  this->route.push_back(this->key);
  this->key = new_coming;
}

vector<int> c_profundidad::get_route(){
  return this->route;
}

#endif
