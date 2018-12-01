#ifndef C_GRAPH_H
#define C_GRAPH_h

#include <iostream>
#include <vector>
#include <queue>
#include <math.h>
#include "c_nodo.h"
#include "c_profundidad.h"
#include "c_etiqueta.h"

using namespace std;

class comparison{
public:
  bool operator () (const c_etiqueta& a, const c_etiqueta& b) const {
    return  (a.distance > b.distance);
  }
};

class c_graph{
public:
  int n_x;
  int n_y;
  int d_x;
  int d_y;
  int initial_point;
  int final_point;
  vector< c_nodo > adyacentes; //Lista de TODOS los nodos
  int points_controller = true;

  vector<int> bf_route;
  vector<int> climbing_route;

  c_graph(int, int, int, int);
  ~c_graph();

  void set_configuration(int);
    void buncle_1();

  void from_p_to_cord(int, int&, int&);
  int from_cord_to_p(int, int);
  void eliminar(int);//Eliminamos la visibilidad del n miembro de los adyacentes;

  void general_view();

  void alg_profundidad();
    void build_route_v1(vector< c_profundidad >);
  void alg_climbing();

  float distance(int, int);
  float distance_v2(int, float, float);
  void set_points(float, float);
  void rec_erase(float, float, float, float);
  bool rec_erase_val(int, float, float, float, float);
};

c_graph::c_graph(int _n_x, int _n_y, int _d_x, int _d_y){
  this->n_x = _n_x;
  this->n_y = _n_y;
  this->d_x = _d_x;
  this->d_y = _d_y;

  this->adyacentes = vector< c_nodo > (this->n_x * this->n_y, c_nodo());

  this->initial_point = 0;
  this->final_point = 0;
  this->bf_route = vector<int> (0,0);
  this->climbing_route = vector<int> (0,0);
  this->points_controller = true;
}

c_graph::~c_graph(){}

void c_graph::set_configuration(int configuration){
  if(configuration == 1){
    this->buncle_1();
  }
}

void c_graph::buncle_1(){
  //vector<int> next_to_it;
  int x_counter = 0; //Señala con que miembro del array tratamos
  int y_counter = 0;
  float x_jump = this->d_x/(this->n_x - 1) -0.3; //Distancia entre los puntos en x (VERTICAL)
  float y_jump = this->d_y/(this->n_y - 1) -0.3; //Distancia entre los puntos en y (HORIZONTAL)
  float x_pointer = 0.3; //Contador para los posiciones de los puntos en x
  float y_pointer = 0.3; //Contador para las posiciones de los puntos en y

  for(int i = 0; i < (int)this->adyacentes.size(); i++){
    //cout<<"-> "<<i<<endl;
    //cout<<x_pointer<<" - "<<y_pointer<<endl;
/////////////SETTING UP METRICAL POSITIONS.
    this->adyacentes[i].set_positions(x_pointer, y_pointer);
    y_counter++;//No afecta en nada a las posiciones, solo indica en que zona Y (horizontal) estoy.
    y_pointer += y_jump;

    if(y_counter >= this->n_y){
      y_pointer = 0.3;
      x_counter++;
      x_pointer += x_jump;
      y_counter = 0;
    }

    //if(x_counter >= this->n_x){
      //cout<<"AAAAAALTO"<<endl;
      //x_pointer = 0.3;
      //x_counter = 0;
    //}
////////////////////////////

    int temp_x = 0;
    int temp_y = 0;
    this->from_p_to_cord(i, temp_x, temp_y);//Convertimos nuestro posicion a coordenada

    if( temp_x == 0){
      if( temp_y == 0){
        this->adyacentes[i].add_adyacent_element(from_cord_to_p(temp_x+1, temp_y+1));
        this->adyacentes[i].add_adyacent_element(from_cord_to_p(temp_x+1, temp_y));
        this->adyacentes[i].add_adyacent_element(from_cord_to_p(temp_x, temp_y+1));
      }
      else if( temp_y == this->n_y-1){
        this->adyacentes[i].add_adyacent_element(from_cord_to_p(temp_x+1, temp_y-1));
        this->adyacentes[i].add_adyacent_element(from_cord_to_p(temp_x+1, temp_y));
        this->adyacentes[i].add_adyacent_element(from_cord_to_p(temp_x, temp_y-1));
      }
      else{
        this->adyacentes[i].add_adyacent_element(from_cord_to_p(temp_x, temp_y-1));
        this->adyacentes[i].add_adyacent_element(from_cord_to_p(temp_x+1, temp_y-1));
        this->adyacentes[i].add_adyacent_element(from_cord_to_p(temp_x+1, temp_y));
        this->adyacentes[i].add_adyacent_element(from_cord_to_p(temp_x+1, temp_y+1));
        this->adyacentes[i].add_adyacent_element(from_cord_to_p(temp_x, temp_y+1));
      }
    }
    else if( temp_x == this->n_x-1){
      if( temp_y == 0){
        this->adyacentes[i].add_adyacent_element(from_cord_to_p(temp_x-1, temp_y+1));
        this->adyacentes[i].add_adyacent_element(from_cord_to_p(temp_x-1, temp_y));
        this->adyacentes[i].add_adyacent_element(from_cord_to_p(temp_x, temp_y+1));
      }
      else if( temp_y == this->n_y-1){
        this->adyacentes[i].add_adyacent_element(from_cord_to_p(temp_x-1, temp_y-1));
        this->adyacentes[i].add_adyacent_element(from_cord_to_p(temp_x-1, temp_y));
        this->adyacentes[i].add_adyacent_element(from_cord_to_p(temp_x, temp_y-1));
      }
      else{
        this->adyacentes[i].add_adyacent_element(from_cord_to_p(temp_x, temp_y-1));
        this->adyacentes[i].add_adyacent_element(from_cord_to_p(temp_x-1, temp_y-1));
        this->adyacentes[i].add_adyacent_element(from_cord_to_p(temp_x-1, temp_y));
        this->adyacentes[i].add_adyacent_element(from_cord_to_p(temp_x-1, temp_y+1));
        this->adyacentes[i].add_adyacent_element(from_cord_to_p(temp_x, temp_y+1));
      }
    }
    else{//Si x no es 0 ni esta en el limite.
      if( temp_y == 0){
        this->adyacentes[i].add_adyacent_element(from_cord_to_p(temp_x-1, temp_y));
        this->adyacentes[i].add_adyacent_element(from_cord_to_p(temp_x-1, temp_y+1));
        this->adyacentes[i].add_adyacent_element(from_cord_to_p(temp_x, temp_y+1));
        this->adyacentes[i].add_adyacent_element(from_cord_to_p(temp_x+1, temp_y+1));
        this->adyacentes[i].add_adyacent_element(from_cord_to_p(temp_x+1, temp_y));
      }
      else if(temp_y == this->n_y-1){
        this->adyacentes[i].add_adyacent_element(from_cord_to_p(temp_x-1, temp_y));
        this->adyacentes[i].add_adyacent_element(from_cord_to_p(temp_x-1, temp_y-1));
        this->adyacentes[i].add_adyacent_element(from_cord_to_p(temp_x, temp_y-1));
        this->adyacentes[i].add_adyacent_element(from_cord_to_p(temp_x+1, temp_y-1));
        this->adyacentes[i].add_adyacent_element(from_cord_to_p(temp_x+1, temp_y));
      }
      else{
        this->adyacentes[i].add_adyacent_element(from_cord_to_p(temp_x-1, temp_y));
        this->adyacentes[i].add_adyacent_element(from_cord_to_p(temp_x-1, temp_y+1));
        this->adyacentes[i].add_adyacent_element(from_cord_to_p(temp_x, temp_y+1));
        this->adyacentes[i].add_adyacent_element(from_cord_to_p(temp_x+1, temp_y+1));
        this->adyacentes[i].add_adyacent_element(from_cord_to_p(temp_x+1, temp_y));
        this->adyacentes[i].add_adyacent_element(from_cord_to_p(temp_x+1, temp_y-1));
        this->adyacentes[i].add_adyacent_element(from_cord_to_p(temp_x, temp_y-1));
        this->adyacentes[i].add_adyacent_element(from_cord_to_p(temp_x-1, temp_y-1));
      }
    }

    /*for(unsigned int j = 0; j < next_to_it.size(); j++){
      //this->adyacentes[i].l_adyacentes.push_back(next_to_it[j]);
      this->adyacentes[i].add_adyacent_element(next_to_it[j]);
    }*/

    //next_to_it.clear();
  }
}

void c_graph::from_p_to_cord(int pos, int& x, int& y){
  x = pos / this->n_y;
  y = pos % this->n_y;
}

int c_graph::from_cord_to_p(int x, int y){
  int answer = x*this->n_y + y;
  return answer;
}


void c_graph::eliminar(int pos){
  this->adyacentes[pos].visible = false;
  this->adyacentes[pos].l_adyacentes.clear();
}

void c_graph::alg_profundidad(){
  this->bf_route.clear();
  vector< c_profundidad > roles_nodes = vector< c_profundidad > (0, c_profundidad());
  vector< bool > visited = vector< bool >(this->adyacentes.size(), 0);

  bool end_look = false;
  bool founded_route = false;

  //Añadimos los cercanos al nodo de partida;
  visited[this->initial_point] = true;//Marcando el punto incial

  for(unsigned int i = 0; i < this->adyacentes[this->initial_point].l_adyacentes.size(); i++){
    c_profundidad temp(this->adyacentes[this->initial_point].l_adyacentes[i], this->initial_point);
    roles_nodes.push_back(temp);
  }

  while(!end_look){
    //Extraemos los datos del ultimo elemento en el vector
    int look_in = roles_nodes[ roles_nodes.size() - 1 ].key; //Obtenemos al ultimo elemento
    vector<int> sub_route = roles_nodes[roles_nodes.size() - 1].get_route(); //Obtenemos la ruta del ultimo elemento
    sub_route.push_back(look_in); //Añadimos el ultimo key a esta ruta;

    roles_nodes.pop_back();
    visited[look_in] = true;

    for(unsigned int i = 0; i < this->adyacentes[look_in].l_adyacentes.size(); i++){
      if(! visited[this->adyacentes[look_in].l_adyacentes[i]]){
        c_profundidad temp(this->adyacentes[look_in].l_adyacentes[i], sub_route); //key - route
        roles_nodes.push_back(temp);
      }

      if(this->adyacentes[look_in].l_adyacentes[i] == this->final_point){
        end_look = true;
        founded_route = true;
        i = this->adyacentes[look_in].l_adyacentes.size(); //Aceleramos el for;
      }
    }

    if(roles_nodes.empty()){
      end_look = true;
    }
  }

  if(founded_route){
    cout<<"Se hallo la ruta"<<endl;
    this->build_route_v1(roles_nodes);
  }
  else{
    cout<<"No hay ruta"<<endl;
  }

}

void c_graph::build_route_v1(vector<c_profundidad> camino){
  for(unsigned int i = 0; i < camino[camino.size()-1].route.size(); i++){
    this->bf_route.push_back(camino[camino.size()-1].route[i]);
  }
  this->bf_route.push_back(camino[camino.size()-1].key);

  for(unsigned int i = 0; i < this->bf_route.size(); i++){
    cout<<this->bf_route[i]<<" ";
  }
  cout<<endl;
}


void c_graph::alg_climbing(){
  this->climbing_route.clear();

  cout<<"Starting"<<endl;
  priority_queue<c_etiqueta, vector<c_etiqueta>, comparison> p_list;
  bool end_look = false;
  bool founded_route = false;
  vector<bool> visited = vector<bool> (this->adyacentes.size(), 0);
  //Insertando el punto inicial y la distancia de este al punto final;
  p_list.push(c_etiqueta(this->initial_point, this->distance(this->initial_point, this->final_point), this->initial_point ));
  visited[this->initial_point] = 1;

  for(unsigned int i = 0; i < this->adyacentes[this->initial_point].l_adyacentes.size(); i++){
    p_list.push( c_etiqueta( this->adyacentes[this->initial_point].l_adyacentes[i] , this->distance(this->adyacentes[this->initial_point].l_adyacentes[i], this->final_point) , this->initial_point) );
  }

  while (!end_look) {
    cout<<" --> "<<p_list.top().key<<endl;
    if(p_list.top().key == this->final_point){
      end_look = true;
      founded_route = true;
    }
    else{
      if(!p_list.empty()){
        //Si el mas corto ya no tiene por donde ir;
        visited[p_list.top().key] = 1;
        if(this->adyacentes[ p_list.top().key ].l_adyacentes.size() == 0){
          p_list.pop();
        }
        else{
          int actual_selector = p_list.top().key;
          for(unsigned int i = 0; i < this->adyacentes[actual_selector].l_adyacentes.size(); i++){
            if( !visited[this->adyacentes[actual_selector].l_adyacentes[i] ]){
                p_list.push( c_etiqueta(this->adyacentes[actual_selector].l_adyacentes[i] ,  this->distance(this->adyacentes[actual_selector].l_adyacentes[i], this->final_point) , actual_selector ) );
            }
          }
        }
      }
      else{
        end_look = true;
      }
    }
  }

  if(founded_route == true){
    cout<<"Ruta encontrada"<<endl;
    //Recuperacion de la ruta;

    //int next = p_list.top().come_from;
    //this->climbing_route.push_back(this->final_point);
    //while(p_list.top().key != this->initial_point){
      //cout<<"Extrayendo"<<endl;
      //if(p_list.top().key == next){
        //this->climbing_route.push_back(next);
        //next = p_list.top().come_from;
    //  }
      //p_list.pop();
    //}

    bool continue_ = true;
    vector<int> ruta_d = vector<int>(this->adyacentes.size(), 0);
    while(!p_list.empty()){
      ruta_d[p_list.top().key] = p_list.top().come_from;
      p_list.pop();
    }

    this->climbing_route.push_back(this->final_point);
    int temp =  ruta_d[this->final_point];

    while(continue_){
      if(temp == this->initial_point){
        continue_ = false;
      }
      else{
        this->climbing_route.push_back(temp);
        temp = ruta_d[temp];
      }
    }

    this->climbing_route.push_back(this->initial_point);

    for(unsigned int i = 0; i < this->climbing_route.size(); i++){
      cout<<climbing_route[i]<<" ";
    }
    cout<<endl;

  }
  else{
    cout<<"Ruta no encontrada"<<endl;
  }
}

void c_graph::general_view(){

  for(unsigned int i = 0; i < this->adyacentes.size(); i++){
    cout<<i<<": "<<this->adyacentes[i].l_adyacentes.size()<<" :: "<<this->adyacentes[i].x<<" -- "<<this->adyacentes[i].y<<endl;
  }

}


float c_graph::distance(int a, int b){
  float x_1 = this->adyacentes[a].x;
  float y_1 = this->adyacentes[a].y;

  float x_2 = this->adyacentes[b].x;
  float y_2 = this->adyacentes[b].y;

  float answer = sqrt(pow(x_2 - x_1, 2) + pow(y_2 - y_1, 2));
  return answer;
}

float c_graph::distance_v2(int a, float x_2, float y_2){
  float x_1 = this->adyacentes[a].x;
  float y_1 = this->adyacentes[a].y;

  float answer = sqrt(pow(x_2 - x_1, 2) + pow(y_2 - y_1, 2));
  return answer;
}


void c_graph::set_points(float _x, float _y){
  int value = 0;
  float min = 99999999;
  float temp = 0;
  for(unsigned int i = 0; i < this->adyacentes.size(); i++){
    temp = distance_v2(i, _x, _y);
    if(temp < min){
      min = temp;
      value = i;
    }
  }
  if(this->points_controller){
    this->initial_point = value;
    this->points_controller = !this->points_controller;
  }
  else{
    this->final_point = value;
    this->points_controller = !this->points_controller;
  }
}


void c_graph::rec_erase(float _x1, float _y1, float _x2, float _y2){
  float min_x = 0;
  float min_y = 0;
  float max_x = 0;
  float max_y = 0;

  if(_x1  < _x2){
    min_x = _x1;
    max_x = _x2;
  }
  else{
    min_x = _x2;
    max_x = _x1;
  }

  if(_y1 < _y2){
    min_y = _y1;
    max_y = _y2;
  }
  else{
    min_y = _y2;
    max_y = _y1;
  }


  for(unsigned int i = 0; i < this->adyacentes.size(); i++){
    if( this->rec_erase_val(i, min_x, min_y, max_x, max_y)){
      this->eliminar(i);
    }
  }

}

bool c_graph::rec_erase_val(int val, float min_x, float min_y, float max_x, float max_y){
  if(this->adyacentes[val].x < min_x or this->adyacentes[val].x > max_y){
    return false;
  }
  else if(this->adyacentes[val].y < min_y or this->adyacentes[val].y > max_y){
    return false;
  }
  else{
    return true;
  }
}

#endif
