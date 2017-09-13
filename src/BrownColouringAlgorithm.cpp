#include "BrownColouringAlgorithm.h"

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
int BrownColouringAlgorithm::BAux(vector<Nodo*> * x,unsigned int &q , unsigned int k ,unsigned  int l){
  // Si llego al final ya encontre una mejor coloracion l
  if( k >= x->size() ){
    return l;
  }else{
    Nodo * actual = x->at(k);
    // Al encontrar un nodo pintado con q los despinto hasta el final y me regreso
    if( actual->getColor() >= q){
      this->despintar(x, k, x->size());
      return -1;
    }
    int nb;
    // Si el nodo actual no esta pintado o si ya lo esta pero no permite
    // obtener una mejor coloracion
    if( actual->getColor() == 0 ||
      (nb = BAux( x, q , k + 1 ,
                  ( actual->getColor() > l ? actual->getColor() : l ))) == -1){
      // Encuentro cuales son los colores con los que lo puedo pintar
      // El posible color debe ser menor que la posicion del x
      // y que la cota superior
      vector<unsigned int> colores = x->at(k)->encontrarColoresFactibles( x->at(k)->getColor() , ( k + 2 < q  ? k + 2 : q ) );
//         cout << colores.size() << " colores encontrados para " << k << endl;
for(vector<unsigned int>::iterator i = colores.begin() ; i != colores.end() ; ++i){
        // Pinto con un color factible e intento hallar una mejor coloracion
//         cout << "Pintando " << k << " con " << *i << endl;
        actual->setColor(*i);
        nb = BAux(x,q,k + 1, ( *i > l ? *i : l));
        if( nb != -1 ){
          return nb;
        }
      }
      // No se encontro mejor coloracion, despinto y regreso
      actual->setColor(0);
      return -1;
    }
    // Logre encontrar mejor coloracion con el color que tenia
    return nb;
  }
}

int BrownColouringAlgorithm::execute(Grafo & g) {
  vector<Nodo*> * x = g.getNodos();
  vector<Nodo*>::iterator i;
  int coloresFinales[x->size()];
  unsigned int q = x->size(); // Inicialmente puedo pintarlo con al menos x->size() colores
  int nq = -1; // Nuevo numero cromatico (encontrado en cada nueva iteracion)
  // Ordeno decrecientemente por grado del nodo para optimizar el algoritmo
  sort(x->begin(), x->end(), this->miComparar);
  // Inicializo los colores de los nodos
  this->despintar(x, 0, x->size());
  // Mientras pueda colorearlo, guardo la solucion e intento mejorarla
  while( ( nq = BAux(x,q)) != -1 ){
    this->vaciarColores(x,coloresFinales);
    q = nq - 1;
  }
  
  // Ya encontre la solucion optima, restauro la ultima solucion encontrada
  this->vaciarColores(x,coloresFinales, false);
  return q + 1;
}

