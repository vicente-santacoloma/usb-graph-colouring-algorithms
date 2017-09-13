#ifndef GCS2_BRELAZCOLOURINGALGORITHM_H_
#define GCS2_BRELAZCOLOURINGALGORITHM_H_

#include <vector>
#include <algorithm> // Incluido para usar sort
#include "Grafo.h"
#include "Nodo.h"
#include "ColouringAlgorithm.h"
#include "DsaturColouringAlgorithm.h"

using namespace std;

class BrelazColouringAlgorithm : public ColouringAlgorithm {
  
  private:
    
    /**
     * Encuentra y devuelve un vector con los nodos que conforman un clique
     * de tamano 3. En caso de no conseguirlo devuelve un vector vacio.
     */
    vector<Nodo*> BruteForceClique(Grafo &g);
    
    /**
     * Calcula el conjunto de colores desde [1,...,min(uk+1,q-1)] que no son usados para la
     * actual solucion parcial de k-1 niveles para los vecinos de x[k].
     */
    set<int> DetermineUk(int q, Nodo * x [], set<int> partialSolutionColors, int k);
    
    /**
     * Devuelve el conjunto de colores usados por actual solucion parcial de k-1 niveles.
     */
    set<int> Determineuk(int level, Nodo* x []);
       
    /**
     * Marca los vertices que no esten marcados que cumplan:
     * 	(i)   Menor rango que x[k]
     *  (ii)  Adyacentes a x[k]
     * 	(iii) Menor rango para los vertices adyacentes a x[k] que tenga igual color.
     */
    void label(bool Label [], int k, Nodo* x []);
    
    /**
     * Devuelve el maximo rango de un vertice que este marcado
     */
    int maximalRank(bool Label [], int n);
    
    /**
     * Devuelve el menor rango de los vertices que tengan el color q
     */
    int minimalRankQColoreable(Nodo* x [], unsigned int q, int n);
    
  public:
    
    /**
     * Ejecuta el algoritmo BrelazColouringAlgorithm
     */
    int execute(Grafo &g);
};

#endif