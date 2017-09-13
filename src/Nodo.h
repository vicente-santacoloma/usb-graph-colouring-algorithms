#ifndef GCS2_NODO_H_
#define GCS2_NODO_H_

#include <iostream>
#include <vector>
#include <set>
#include <cstring> // memset

using namespace std;

class Nodo {
  private:
    
    int id;
    unsigned int color; // 0 denota no coloreado
    int grado;
    int gradoSaturacion;
    vector<Nodo*> adyacentes;
  
  public:
    
    Nodo(int id){
      this->id = id;
      this->color = 0;
      this->grado = 0;
      this->gradoSaturacion = 0;
      this->adyacentes.clear();
    }
    
    
    // Getters y Setters
    
    vector<Nodo*> getAdyacentes()  {
      return this->adyacentes;
    }
    
    void setAdyacentes(vector<Nodo*> adyacentes) {
      this->adyacentes = adyacentes;
    }
    
    unsigned int getColor() const {
      return color;
    }
    
    void setColor(unsigned int color) {
      this->color = color;
    }
    
    int getGrado() const {
      return grado;
    }
    
    void setGrado(int grado) {
      this->grado = grado;
    }
    
    int getGradoSaturacion() const {
      return gradoSaturacion;
    }
    
    void setGradoSaturacion(int gradoSaturacion) {
      this->gradoSaturacion = gradoSaturacion;
    }
    
    int getId() const {
      return id;
    }
    
    void setId(int id) {
      this->id = id;
    }


    /** Agrega n como nodo adyacente de this ssi no existe previamente y
    * es diferente de this. La comparacion se hace mediante el operador ==
    *
    * @param n Referencia al nodo que se desea insertar como adyacente
    * @return true si lo agrega, false si no.
    */
    bool agregarAdyacente(Nodo * n);
    
    /** Borra de la lista de nodos adyacentes de this el nodo que evalue true
    * al compararlo usando == con el nodo n. Si ninguno evalua true, no se modifica
    * la lista.
    *
    * @param n Nodo que se desea eliminar.
    */
    void eliminarAdyacente(Nodo * n);
    
    /**
    *
    */
    void incrementarGradoSaturacion();
    
    /** Consulta entre los nodos adyacentes a this cual es el menor color
    * que no aun no ha sido usado por ellos.
    *
    * @return Un color factible para this
    */
    int encontrarMenorColor();
    
    /**
    * Indica si este nodo es adyacente a otro
    *
    * @param a nodo a comparar con el actual
    * @return true si los dos nodos son adyacentes, false si no lo son
    *
    */
    bool esAdyacente(Nodo * a);
    
    
    void imprimirAdyacente();

    /** Busca colores factibles para el este nodo que se encuentren en el intervalo
    * (lowerBound,upperBound). Un color factible para this es aquel que no este
    * siendo usado por ninguno de sus nodos adyacentes.
    *
    * @param lowerBound cota inferior para los colores
    * @param upperBound cota superior para los colores
    * @return Un vector de enteros representando los colores factibles entre
    * lowerBound y upperBound para n (sin incluir lowerBound ni upperBound)
    */
    vector<unsigned int> encontrarColoresFactibles(unsigned int lowerBound,unsigned  int upperBound);

    /** Busca la cantidad de colores factibles para el este nodo que se encuentren en el intervalo
    * (lowerBound,upperBound). Un color factible para this es aquel que no este
    * siendo usado por ninguno de sus nodos adyacentes.
    *
    * @param lowerBound cota inferior para los colores
    * @param upperBound cota superior para los colores
    * @return Un vector de enteros representando los colores factibles entre
    * lowerBound y upperBound para n (sin incluir lowerBound ni upperBound)
    */
    int encontrarCantidadDeColoresFactibles(unsigned int lowerBound,unsigned  int upperBound);
    
    bool operator==(const Nodo & n) const {
      return this->id == n.id;
    }
    
};

#endif