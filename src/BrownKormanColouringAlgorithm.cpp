#include "BrownKormanColouringAlgorithm.h"
#include <sys/time.h>

/* retorna "a - b" en segundos */
double timeval_diff2(struct timeval *a, struct timeval *b)
{
  return
  (double)(a->tv_sec + (double)a->tv_usec/1000000) -
  (double)(b->tv_sec + (double)b->tv_usec/1000000);
}

void BrownKormanColouringAlgorithm::lookAhead(vector<Nodo*> * x, const unsigned int &q, const unsigned int & from){
//  struct timeval t_ini, t_fin;
//  gettimeofday(&t_ini, NULL);
  // Busco el minimo
  int min = x->size() * 2, minPos = from;
  for(unsigned int i = from ; i < x->size() ; ++i){
    int colores = x->at(i)->encontrarCantidadDeColoresFactibles( x->at(i)->getColor() , ( i + 2 < q ? i + 2 : q ) );
    if( colores < min ){ // Si es mejor que el actual
      minPos = i;
      min = colores;
    }
    break;
  }
  Nodo * temp = x->at(minPos);
  x->at(minPos) = x->at(from);
  x->at(from) = temp;
//  gettimeofday(&t_fin, NULL);
//  cout << timeval_diff2(&t_fin, &t_ini) << endl;
}

bool BrownKormanColouringAlgorithm::encontrarColoracion(vector<Nodo*> * x, unsigned int &q , unsigned int i , unsigned int nq, int * coloracionGuardada){
  if( i == x->size() ){
    q = nq;
    k = buscarPuntoDeBT(x,q);
    guardarColoracion(x,coloracionGuardada);
    //       cout << "Nueva coloracion encontrada con " << q << " colores. El k vale " << k << endl;
    return true;
  }else{
    lookAhead(x,q,i);
    vector<unsigned int> colores = x->at(i)->encontrarColoresFactibles( x->at(i)->getColor() , ( i + 2 < q ? i + 2 : q ) );
    for(vector<unsigned int>::iterator c = colores.begin() ; c != colores.end(); ++c){
      if( *c >= q ){ // Si se encuentra coloracion los nodos menores o iguales a k van a seguir probando colores que no son factibles
        break;
      }
      
      colorear(x,i,*c);
      
      if( encontrarColoracion( x, q,  i + 1 , ( *c > nq ? *c : nq ) , coloracionGuardada) ){
        if( i > k ){
          // Si encontre una coloracion  y estoy regresandome
          descolorear(x,i);
          return true;
        }
      }
    }
    descolorear(x,i);
    return false;
  }
}

int BrownKormanColouringAlgorithm::execute(Grafo & g) {
  vector<Nodo*> * x = g.getNodos();
  unsigned int tamano = x->size();
  unsigned int q = tamano;
  k = -1;
  
  int * coloresGuardados = (int*)calloc(tamano + 1, sizeof(int));
//   int * primerColorFactible = (int*)calloc(tamano + 1, sizeof(int));
  despintar(x, 0, tamano);
  // Me traigo el que tenga mayor grado para reducir la coloracion en alguito
  
  encontrarColoracion(x,q,0,0, coloresGuardados);
  restaurarColoracion(x, coloresGuardados);
  return q;
}
