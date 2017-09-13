#include "Nodo.h"


bool Nodo::agregarAdyacente(Nodo * n) {
  // cout << "Agregando a " << this->id << " el nodo adyacente: " << n->id << endl;
  if (*this == *n) { // No admite bucles
    return false;
  }
  // Busca por nodos repetidos
  for (vector<Nodo*>::iterator i = this->adyacentes.begin(); i != this->adyacentes.end(); ++i) {
    if (*n == **i) {
      return false;
    }
  }
  this->adyacentes.push_back(n);
  this->grado++;
  //        this->imprimirAdyacentes();
  return true;
}


void Nodo::eliminarAdyacente(Nodo * n) {
  for (vector<Nodo*>::iterator i = this->adyacentes.begin();
  i != this->adyacentes.end(); ++i) {
    if (*n == **i) {
      adyacentes.erase(i);
      this->grado--;
      break;
    }
  }
}


void Nodo::incrementarGradoSaturacion() {
  
  set<int> colores;
  
  for (unsigned int i = 0; i < this->adyacentes.size(); ++i) {
    Nodo * a = this->adyacentes.at(i);
    for (unsigned int j = 0; j < a->getAdyacentes().size(); j++) {
      Nodo * b = a->getAdyacentes().at(j);
      int color = b->getColor();
      if (color != 0) {
        colores.insert(b->getColor());
      }
    }
    a->setGradoSaturacion(colores.size());
    colores.clear();
  }
  
}

int Nodo::encontrarMenorColor() {
  set<int> colores;
  for (vector<Nodo*>::iterator i = this->adyacentes.begin();
  i != this->adyacentes.end(); ++i) {
    if((*i)->getColor() != 0)
      colores.insert((*i)->getColor());
  }
  int color = 1;
  for (set<int>::iterator i = colores.begin();
  i != colores.end(); ++i) {
    if (color < *i)
      break;
    else
      color++;
  }
  return color;
}

bool Nodo::esAdyacente(Nodo * a) {
  
  for(vector<Nodo*>::iterator i = this->adyacentes.begin();
	i != this->adyacentes.end(); ++i) {
    if((*i)->getId() == a->getId())
      return true;
  }
  return false;
}

void Nodo::imprimirAdyacente() {
  for (vector<Nodo*>::iterator it = this->adyacentes.begin();
  it != this->adyacentes.end(); ++it) {
    cout << (*it)->getId() << " ";
  }
}

vector<unsigned int> Nodo::encontrarColoresFactibles(unsigned int lowerBound,unsigned  int upperBound) {
  vector<unsigned int> colores;
  set<unsigned int> ocupados;
  vector<Nodo*> sucesores = this->adyacentes;
  vector<Nodo*>::iterator sucesor;
  for(sucesor = sucesores.begin() ; sucesor != sucesores.end() ; ++sucesor){
    if((*sucesor)->getColor() != 0){
      ocupados.insert((*sucesor)->getColor());
    }
  }
  set<unsigned int>::iterator colorOcupado = ocupados.begin();
  for(unsigned int i = lowerBound + 1 ; i < upperBound ; ++i){
    if(colorOcupado != ocupados.end()){
      if( *colorOcupado != i ){
        colores.push_back(i);
      }else{
        colorOcupado++;
      }
    }else{
      colores.push_back(i);
    }
  }
  return colores;
}

int Nodo::encontrarCantidadDeColoresFactibles(unsigned int lowerBound,unsigned  int upperBound) {
  int tamano = upperBound - lowerBound - 1;
  unsigned int colores = tamano;
  bool ocupados[tamano];
  memset(ocupados, 0, tamano);
  vector<Nodo*> sucesores = this->adyacentes;
  vector<Nodo*>::iterator sucesor;
  for(sucesor = sucesores.begin() ; sucesor != sucesores.end() ; ++sucesor){
    if((*sucesor)->getColor() != 0 && (*sucesor)->getColor() > lowerBound &&
      (*sucesor)->getColor() < upperBound){
      if(!ocupados[(*sucesor)->getColor() - lowerBound - 1]) --colores;
      ocupados[(*sucesor)->getColor() - lowerBound - 1] = true;
    }
  }
  return colores ;
}