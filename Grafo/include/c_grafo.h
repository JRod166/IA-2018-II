#ifndef C_GRAFO_H
#define C_GRAFO_H

#include <iostream>
#include <vector>
#include "c_nodo.h"

using namespace std;

class c_grafo{
public:
  int n_x; //Nodos en el eje x;
  int n_y; //Nodos en el eje y;
  int d_x; //Largo del eje x;
  int d_y; //Largo del eje y;
  vector< vector<int> > adyacentes;

  c_grafo(int, int, int, int);
  ~c_grafo();

  void set_configuration(int); // A predeterminated edges distribution
    void buncle_1();
  void from_p_to_cord(int, int&, int&);
  int from_cord_to_p(int, int);

  //SECONDARY FUNCTIONS
  void print_adyacentes();

};

c_grafo::c_grafo(int _n_x, int _n_y, int _d_x, int _d_y){
  this->n_x = _n_x;
  this->n_y = _n_y;
  this->d_x = _d_x;
  this->d_y = _d_y;

  this->adyacentes = vector< vector<int> >(this->n_x * this->n_y, vector<int>(this->n_x * this->n_y, 0));
}

c_grafo::~c_grafo(){}


void c_grafo::set_configuration(int configuration){
  if(configuration == 1){
    this->buncle_1();
  }
}

void c_grafo::buncle_1(){
  vector<int> next_to_it;
  for(int i = 0; i < (int)this->adyacentes.size(); i++){
    int temp_x = 0;
    int temp_y = 0;
    this->from_p_to_cord(i, temp_x, temp_y);

    if( temp_x == 0){
      if( temp_y == 0){
        next_to_it.push_back(from_cord_to_p(temp_x+1, temp_y+1));
        next_to_it.push_back(from_cord_to_p(temp_x+1, temp_y));
        next_to_it.push_back(from_cord_to_p(temp_x, temp_y+1));
      }
      else if( temp_y == this->n_y-1){
        next_to_it.push_back(from_cord_to_p(temp_x+1, temp_y-1));
        next_to_it.push_back(from_cord_to_p(temp_x+1, temp_y));
        next_to_it.push_back(from_cord_to_p(temp_x, temp_y-1));
      }
      else{
        next_to_it.push_back(from_cord_to_p(temp_x, temp_y-1));
        next_to_it.push_back(from_cord_to_p(temp_x+1, temp_y-1));
        next_to_it.push_back(from_cord_to_p(temp_x+1, temp_y));
        next_to_it.push_back(from_cord_to_p(temp_x+1, temp_y+1));
        next_to_it.push_back(from_cord_to_p(temp_x, temp_y+1));
      }
    }
    else if( temp_x == this->n_x-1){
      if( temp_y == 0){
        next_to_it.push_back(from_cord_to_p(temp_x-1, temp_y+1));
        next_to_it.push_back(from_cord_to_p(temp_x-1, temp_y));
        next_to_it.push_back(from_cord_to_p(temp_x, temp_y+1));
      }
      else if( temp_y == this->n_y-1){
        next_to_it.push_back(from_cord_to_p(temp_x-1, temp_y-1));
        next_to_it.push_back(from_cord_to_p(temp_x-1, temp_y));
        next_to_it.push_back(from_cord_to_p(temp_x, temp_y-1));
      }
      else{
        next_to_it.push_back(from_cord_to_p(temp_x, temp_y-1));
        next_to_it.push_back(from_cord_to_p(temp_x-1, temp_y-1));
        next_to_it.push_back(from_cord_to_p(temp_x-1, temp_y));
        next_to_it.push_back(from_cord_to_p(temp_x-1, temp_y+1));
        next_to_it.push_back(from_cord_to_p(temp_x, temp_y+1));
      }
    }
    else{//Si x no es 0 ni esta en el limite.
      if( temp_y == 0){
        next_to_it.push_back(from_cord_to_p(temp_x-1, temp_y));
        next_to_it.push_back(from_cord_to_p(temp_x-1, temp_y+1));
        next_to_it.push_back(from_cord_to_p(temp_x, temp_y+1));
        next_to_it.push_back(from_cord_to_p(temp_x+1, temp_y+1));
        next_to_it.push_back(from_cord_to_p(temp_x+1, temp_y));
      }
      else if(temp_y == this->n_y-1){
        next_to_it.push_back(from_cord_to_p(temp_x-1, temp_y));
        next_to_it.push_back(from_cord_to_p(temp_x-1, temp_y-1));
        next_to_it.push_back(from_cord_to_p(temp_x, temp_y-1));
        next_to_it.push_back(from_cord_to_p(temp_x+1, temp_y-1));
        next_to_it.push_back(from_cord_to_p(temp_x+1, temp_y));
      }
      else{
        next_to_it.push_back(from_cord_to_p(temp_x-1, temp_y));
        next_to_it.push_back(from_cord_to_p(temp_x-1, temp_y+1));
        next_to_it.push_back(from_cord_to_p(temp_x, temp_y+1));
        next_to_it.push_back(from_cord_to_p(temp_x+1, temp_y+1));
        next_to_it.push_back(from_cord_to_p(temp_x+1, temp_y));
        next_to_it.push_back(from_cord_to_p(temp_x+1, temp_y-1));
        next_to_it.push_back(from_cord_to_p(temp_x, temp_y-1));
        next_to_it.push_back(from_cord_to_p(temp_x-1, temp_y-1));
      }
    }

    for(unsigned int j = 0; j < next_to_it.size(); j++){
      this->adyacentes[i][ next_to_it[j] ] = 1;
    }
    //cout<<next_to_it.size()<<endl;
    next_to_it.clear();
  }
}

void c_grafo::from_p_to_cord(int pos, int& x, int& y){
    x = pos / this->n_y;
    y = pos % this->n_y;
}

int c_grafo::from_cord_to_p(int x, int y){
  int answer = x*this->n_y + y;
  return answer;
}

void c_grafo::print_adyacentes(){
  for(unsigned int i = 0; i < this->adyacentes.size(); i++){
    for(unsigned int j = 0; j < this->adyacentes[0].size(); j++){
      cout<<this->adyacentes[i][j]<<"  ";
    }
    cout<<endl;
  }
}



#endif
