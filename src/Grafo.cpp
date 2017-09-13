#include "Grafo.h"

bool Grafo::agregarNodo(int id) {
  for (vector<Nodo*>::iterator i = this->nodos.begin();
  i != this->nodos.end(); ++i) {
    if (id == (*i)->getId()) {
      return false; // Ya esta agregado
    }
  }
  Nodo * n = new Nodo(id);
  this->nodos.push_back(n);
  // cout << "Nodo agregado: " << n->getId() << ", la cantidad de nodos es: " << this->nodos.size() << endl;
  return true;
};


bool Grafo::agregarLado(int fuente, int destino) {
  // cout << "Agregando arco desde " << fuente << " a " << destino << endl;
  Nodo * f, * d;
  bool fFound = false, dFound = false;
  for (vector<Nodo*>::iterator i = this->nodos.begin();
  i != this->nodos.end(); ++i) {
    //            // cout << "Voy pa la siguiente iteracion" << endl;
    if (fuente == (*i)->getId()) {
      //                // cout << "Encontre el fuente" << endl;
      f = *i; // Encontre el fuente
      //                // cout << "Asigne apuntador " << endl;
      fFound = true;
    }
    if (destino == (*i)->getId()) {
      //                // cout << "Encontre el destino" << endl;
      d = *i; // Encontre el destino
      //                // cout << "Asigne apuntador " << endl;
      dFound = true;
    }
    if (fFound && dFound) {
      //                // cout << "Encontre ambos" << endl;
      break;
    }
  }
  if (fFound && dFound) {
    if (!f->agregarAdyacente(d)) {
      return false;
    }
    if (!d->agregarAdyacente(f)) {
      f->eliminarAdyacente(d);
      return false;
    }
    //            // cout << "Adyacentes agregados" << endl;
    //            // cout << f->getId() << " tiene " << f->getAdyacentes().size() << " sucesores" << endl;
    //            // cout << d->getId() << " tiene " << d->getAdyacentes().size() << " sucesores" << endl;
    numArcos++;
    return true;
  } else {
    //            // cout << "Nodos no encontrados" << endl;
    return false;
  }
}

void Grafo::imprimir() {
  cout << endl << "Nodo\tColor\tAdyacentes" << endl;
  for (vector<Nodo*>::iterator i = this->nodos.begin();
  i != this->nodos.end(); ++i) {
    cout << (*i)->getId() << "\t" << (*i)->getColor() << "\t";
    vector<Nodo*> adyacentes = (*i)->getAdyacentes();
    for(vector<Nodo*>::iterator j = adyacentes.begin();
    j != adyacentes.end(); ++j)
    cout << (*j)->getId() << " ";
    cout << endl;
  }
  cout << endl;
}

void Grafo::clone(Grafo & g) {
  
  this->nodos.clear();
  vector<Nodo*> nodosClone = *(g.getNodos());
  int k = 0;
  for(vector<Nodo*>::iterator i = nodosClone.begin();
  i != nodosClone.end(); ++i) {
    this->agregarNodo((*i)->getId());
    (*(this->getNodos())).at(k)->setColor((*i)->getColor());
    k++;
  }
  
  for(vector<Nodo*>::iterator i = nodosClone.begin();
  i != nodosClone.end(); ++i) {
    
    vector<Nodo*> adyacentes = (*i)->getAdyacentes();
    for(vector<Nodo*>::iterator j = adyacentes.begin();
    j != adyacentes.end(); ++j) {
      this->agregarLado((*i)->getId(),(*j)->getId());
    }
  }
}

bool Grafo::cargarGrafo(string archivo) {
  
  string linea;
  string primero, segundo, tercero;
  ifstream miarchivo(archivo.c_str());
  if (miarchivo.is_open()) {
    while (miarchivo.good()) {
      getline(miarchivo, linea);
      
      istringstream lineass(linea);
      lineass >> primero >> segundo >> tercero;
      if(primero.compare("p") == 0 ){
        for(int i = 1 ; i <= atoi(tercero.c_str()) ; ++i){
          this->agregarNodo(i);
        }
      }
      if (primero.compare("e") == 0) {
        int i = atoi(segundo.c_str());
        int j = atoi(tercero.c_str());
        this->agregarLado(i, j);
      }
      
    }
    miarchivo.close();
  } else {
    return false;
  }
  return true;
}

void Grafo::descolorearGrafo() {
  for(vector<Nodo*>::iterator i = this->nodos.begin();
  i != this->nodos.end(); ++i) {
    (*i)->setColor(0);
  }
}