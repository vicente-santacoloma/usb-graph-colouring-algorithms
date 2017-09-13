#ifndef GCS2_BROWNKORMANCOLOURINGALGORITHM_H_
#define GCS2_BROWNKORMANCOLOURINGALGORITHM_H_

#include <vector>
#include <algorithm> // Incluido para usar sort

#include "Grafo.h"
#include "Nodo.h"
#include "ColouringAlgorithm.h"

using namespace std;

class BrownKormanColouringAlgorithm : public ColouringAlgorithm {
  private:

    unsigned int k;
    
    // Funciones inline
    
    /** Resetea los colores de los nodos entre begin y end
    *
    * @param begin Iterador al primer nodo a despintar
    * @param end Iterador al ultimo nodo a despintar
    */
    void despintar(vector<Nodo*> * x,unsigned int from,unsigned  int to) {
      for ( unsigned int i = from ; i <= to && i < x->size() ; ++i){
        x->at(i)->setColor(0);
      }
    }
    
    
    
    /** Busca la posicion del nodo k tal que k+1 es el primer nodo en x cuyo color
    * es q. Regresa x->size() en caso de no existir tal nodo.
    */
    int buscarPuntoDeBT(vector<Nodo*> * x, const unsigned int & q){
      for(unsigned int i = 0 ; i < x->size() - 1 ; ++i){
        if(x->at(i + 1)->getColor() == q){
          return i;
        }
      }
      return x->size();
    }
    
    /** Guarda los colores de los nodos que estan en x en el arreglo colores
    */
    void guardarColoracion(vector<Nodo*> * x, int * colores){
      for(unsigned int i = 0 ; i < x->size() ; ++i){
        colores[x->at(i)->getId()] = x->at(i)->getColor();
      }
    }
    
    /** Restaura los colores guardados en colores a los nodos en x
    */
    void restaurarColoracion(vector<Nodo*> * x, int * colores){
      for(unsigned int i = 0 ; i < x->size() ; ++i){
        x->at(i)->setColor(colores[x->at(i)->getId()]);
      }
    }
    
    /** Al nodo en x[i] le asigna color
    */
    void colorear(vector<Nodo*> * x, int indice,unsigned int color){
      x->at(indice)->setColor(color);
    }
    
    /** Resetea el color del nodo x[i]
    */
    void descolorear(vector<Nodo*> * x,int indice){
      x->at(indice)->setColor(0);
    }
    
    // Funciones outline
    
    /** Busca en las posiciones [from,x->size()) de x el nodo con menor cantidad
    * de colores factibles y lo intercambia con el nodo en la posicion from.
    */
    void lookAhead(vector<Nodo*> * x, const unsigned int &q, const unsigned int & from);
    
    /** Algoritmo auxiliar recursivo que realiza el algoritmo de Brown-Korman
    */
    bool encontrarColoracion(vector<Nodo*> * x, unsigned int & q, unsigned int i , unsigned int nq, int * coloresGuardados);
    
  public:
    
    int execute(Grafo & g);
    
};

#endif
