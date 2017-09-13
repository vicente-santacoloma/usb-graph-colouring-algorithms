#ifndef GCS2_GRAFO_H_
#define GCS2_GRAFO_H_

#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <sstream>
#include "Nodo.h"

using namespace std;

class Grafo {
  
  private:

    /** Vector para almacenar los apuntadores a los nodos de este grafo.
    */
    vector<Nodo*> nodos;
    int numArcos;
  
  public:
    
    Grafo() {
        this->nodos.clear();
        numArcos = 0;
    };
    
    /** Agrega un nodo con id id a la lista de nodos de this. Si ya existe un
    * nodo con id id el nodo no es agregado.
    *
    * @param id id del nodo que se quiere agregar
    * @return true si el nodo se agrega, false en caso contrario.
    */
    bool agregarNodo(int id);
    
    /** Agrega un lado entre el nodo con id == fuente y el nodo con id == destino
    *
    * @param fuente id del nodo fuente
    * @param destino id del nodo destino
    * @return true si se agrego el lado, false si no existe alguno de los 2 nodos o
    *  si el lado ya estaba
    */
    bool agregarLado(int fuente, int destino);

    /** Regresa una referencia a los nodos de este grafo.
    *
    * @return el a puntador al vector de apuntadores a los nodos de este grafo.
    */
    vector<Nodo*> * getNodos(){
      return &nodos;
    };

    /** Regresa la densidad del grafo
     *
     * @return la densidad del grafo
     */
    double getDensidad(){
//       cout << "El numero de arcos es: " << numArcos << endl;
      return double(numArcos)/ (double(nodos.size()) * double((nodos.size() - 1)) / 2.0);
    }
    
    /** Imprime la informacion del grafo en un formato legible para un humano
    *
    */
    void imprimir();
    
    void clone(Grafo & g);
    
    /** Recibe la ruta a un archivo que contiene un grafo en formato DIMACS
    * y lo carga en this.
    *
    * @param archivo
    * @param g
    * @return
    */
    bool cargarGrafo(string archivo);
    
    /**
    * Descolorea todos los nodos de un grafo.
    */
    void descolorearGrafo();
    
};

#endif