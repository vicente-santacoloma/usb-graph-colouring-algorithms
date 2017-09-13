#ifndef GCS2_COLOURINGALGORITHM_H_
#define GCS2_COLOURINGALGORITHM_H_

#include "Grafo.h"

class ColouringAlgorithm {
  
protected:

    /** Compara dos nodos por grado y regresa true si el grado de i es mayor 
     * que el grado de j
     * 
     * @param i Apuntador a nodo
     * @param j Apuntador a nodo
     * @return true si i->grado > j->grado
     */
    struct comparar {

        bool operator()(Nodo * i, Nodo * j) {
            return ( i->getGrado() > j->getGrado());
        }
    } miComparar;
    
public:

    virtual int execute(Grafo &g){
      return g.getNodos()->size();
    }
};

#endif
