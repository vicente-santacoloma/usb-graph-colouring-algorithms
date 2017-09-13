#ifndef GCS2_DSATURCOLOURINGALGORITHM_H_
#define GCS2_DSATURCOLOURINGALGORITHM_H_

#include <vector>
#include <algorithm> // Incluido para usar sort
#include "Grafo.h"
#include "Nodo.h"
#include "ColouringAlgorithm.h"

using namespace std;

class DsaturColouringAlgorithm : public ColouringAlgorithm {
  
  private:
    
    /**
     * Devuelve un vector con el (los) nodo(s) de mayor grado de saturacion
     */
    vector<Nodo*> maximoGradoSaturacion(vector<Nodo*> nodos);
    
    /**
     * Devuelve un vector con el (los) nodo(s) de mayor grado
     */
    vector<Nodo*> maximoGrado(vector<Nodo*> nodos);
    
  public:
    
    /**
     * Ejecuta el algoritmo DsaturColouringAlgorithm
     */
    int execute(Grafo &g);
};

#endif