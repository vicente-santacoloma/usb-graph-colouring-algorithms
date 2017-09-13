
#ifndef GCS2_BROWNCOLOURINGALGORITHM__H_
#define GCS2_BROWNCOLOURINGALGORITHM__H_

#include <vector>
#include <algorithm> // Incluido para usar sort

#include "Grafo.h"
#include "Nodo.h"
#include "ColouringAlgorithm.h"

using namespace std;

class BrownColouringAlgorithm : public ColouringAlgorithm {

  private:

    // Metodos inline

    /** Resetea los colores de los nodos entre begin y end
    *
    * @param begin Iterador al primer nodo a despintar
    * @param end Iterador al ultimo nodo a despintar
    */
    void despintar(vector<Nodo*> * x, unsigned int from, unsigned int to) {
      for (unsigned int i = from ; i <= to && i < x->size() ; ++i){
        x->at(i)->setColor(0);
      }
    }
    
    
    /** Si sentido == true almacena en coloresFinales los colores de los nodos
    * en this->x. Si sentido == false le asigna a los nodos en this->x los
    * colores que contiene el arreglo coloresFinales.
    *
    * @param coloresFinales arreglo de enteros de tamanio this->x->size()
    * @param sentido true para almacenar en coloresFinales, false para revertir
    * la coloracion
    */
    void vaciarColores(vector<Nodo*> * x, int * coloresFinales, bool sentido = true){
      for(vector<Nodo*>::iterator i = x->begin() ; i != x->end() ; ++i){
        if(sentido){
          coloresFinales[i - x->begin()] = (*i)->getColor();
        }else{
          (*i)->setColor(coloresFinales[i - x->begin()]);
        }
      }
    }

    // Metodos outline
    
    /** Procedimiento recursivo auxiliar para el algoritmo de Brown. Busca
    * mejorar la coloracion obtenida de this->x usando como cota superior q.
    * k indica la posicion del nodo que se va a analizar y l la nueva cota superior obtenida al finalizar
    * el procedimiento.
    *
    * @param k posicion de un nodo en this->x que se va a analizar
    * @param l coloracion obtenida durante la ejecucion del procedimiento
    * @return -1 si no se logra encontrar una coloracion mejor; el numero de
    * colores usados en caso contrario.
    *
    */
    int BAux(vector<Nodo*> * x, unsigned int & q , unsigned int k = 0 , unsigned int l = 0);
    
    
  public:
    
    int execute(Grafo & g);
    
};

#endif