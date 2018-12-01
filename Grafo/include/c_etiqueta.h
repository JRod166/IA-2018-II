#ifndef C_ETIQUETA_H
#define C_ETIQUETA_H

#include <iostream>

using namespace std;

class c_etiqueta{
public:
  int key;
  float distance;
  int come_from;

  c_etiqueta();
  c_etiqueta(int, float, int);
  ~c_etiqueta();
};

c_etiqueta::c_etiqueta(){
  this->key = 0;
  this->distance = 0;
  this->come_from = 0;
}

c_etiqueta::c_etiqueta(int _key, float _distance, int _come_from){
  this->key = _key;
  this->distance = _distance;
  this->come_from = _come_from;
}

c_etiqueta::~c_etiqueta(){}


#endif
