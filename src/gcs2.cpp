#include <cstdlib>
#include <vector>
#include <string>
#include <cstring> // memset
#include <sys/time.h>
#include <signal.h>
#include "ColouringAlgorithm.h"
#include "BrownColouringAlgorithm.h"
#include "DsaturColouringAlgorithm.h"
#include "BrownKormanColouringAlgorithm.h"
#include "BrelazColouringAlgorithm.h"
#include "Grafo.h"
#include "Nodo.h"


using namespace std;


/** Analiza los parametros de entrada recibidos en argv. Ajusta los parametros
 * archivo, algorithm y tiempo.
 * 
 * @param argc tamanio del parametro argv
 * @param argv arreglo de caracteres con los parametros de entrada del programa
 * @param archivo Variable que contendra la ruta del archivo donde se especifica la instancia del problema en formato DIMACS
 * @param algorithm Variable que contendra el algoritmo a usar por el programa. DsaturColouringAlgorithm por defecto
 * @param tiempo Variable que contendra el tiempo maximo de ejecucion permitido para el algoritmo. 15 minutos por defecto
 * @return true si no hay errores en los parametros, false en caso contrario.
 */
bool parametros_ok(int argc, char** argv, string & archivo, ColouringAlgorithm *& algorithm, int &tiempo);

/** Manejador de interrupcion SIGALRM para detener la ejecucion del algoritmo
 * si pasa el tiempo maximo permitido
 */
void detenerEjecucion(int sig);

bool coloracionFactible(Grafo * g) {
  vector<Nodo*> * nodos = g->getNodos();
  for(vector<Nodo*>::iterator i = nodos->begin() ;
          i != nodos->end() ; i++){
            vector<Nodo*> sucs = (*i)->getAdyacentes();
            for(vector<Nodo*>::iterator j = sucs.begin() ;
                j != sucs.end() ; j++){
                  if( (*i)->getColor() == (*j)->getColor() ){
                    return false;
                  }
                }
  }
  
  return true;
  
}

/* retorna "a - b" en segundos */
double timeval_diff(struct timeval *a, struct timeval *b)
{
  return
  (double)(a->tv_sec + (double)a->tv_usec/1000000) -
  (double)(b->tv_sec + (double)b->tv_usec/1000000);
}

/** Programa Principal. Recibe como entrada la ruta a un archivo conteniendo 
 * la información de un grafo siguiendo el formato DIMACS y le aplica un algoritmo
 * para hallar la coloración mínima del grafo usando el algoritmo de J.R. Brown
 * el cual halla la solución exacta, i.e. el número cromático del grafo, y el 
 * algoritmo de Brelaz (conocido como Dsatur) para aproximarlo. 
 * El programa se llama usando el comando:
 * 
 *   gcs1 [opciones] archivo
 * 
 * donde [opciones] puede ser:
 * 
 *      -d              Usar el algoritmo Dsatur
 *      -b              Usar el algoritmo de Brown
 *      -t <seg>        tiempo máximo de ejecución del algoritmo expresado en segundos
 *      archivo         Ruta al archivo que contiene la descripción del grafo en formato DIMACS
 * 
 * La opción -d y -b no pueden usarse juntas. En caso de que no se especifique la
 * opción -t el tiempo por defecto será de 15 minutos. En caso de no especificar
 * la opción -d o -b se usará por defecto el algoritmo Dsatur.
 * 
 * @param argc numero de argumentos recibidos 
 * @param argv arreglo de strings conteniendo los parametros de entrada del programa.
 * (En la primera posición debe estar gcs1 que es el nombre del programa).
 * @return 0 si no hubieron errores durante la ejecución del programa. 1 si las
 * opciones son incorrectas. 2 Si la ruta al archivo es inválida o el formato es
 * incorrecto. 
 */
int main(int argc, char** argv) {
    Grafo g;
    int numeroCromatico, tiempoMaximo;
    string archivo;
    ColouringAlgorithm *algoritmo;
    // Para tomar el tiempo
    struct timeval t_ini, t_fin;
    double secs;

    // Checkear parametros
    if (!parametros_ok(argc, argv, archivo, algoritmo, tiempoMaximo)) {
        // formato de parametros incorrectos.
        cout << "Los parametros introducidos son incorrectos" << endl;
        exit(1);
    }
    // Cargar grafo
    if (!g.cargarGrafo(archivo)) {
        // archivo invalido
        cout << "El archivo de entrada es invalido" << endl;
        exit(2);
    }
    
//     cout << archivo << ",";
    signal(SIGALRM, detenerEjecucion);
    alarm(tiempoMaximo);
    
    gettimeofday(&t_ini, NULL);
    
    numeroCromatico = algoritmo->execute(g);
    
    gettimeofday(&t_fin, NULL);
    
    secs = timeval_diff(&t_fin, &t_ini);
//     cout << g.getNodos()->size() << "," << numeroCromatico << "," << secs << "," << (coloracionFactible(&g) ? "c" : "i" );
    cout << "nodos " << g.getNodos()->size() << endl;
    cout << "densidad " << g.getDensidad() << endl;
    cout << "colores " << numeroCromatico << endl;
    cout << "tiempo " << secs << endl;
    g.imprimir();
    cout << endl;
//     cout << numeroCromatico << "\t" << secs << "\t" << (coloracionFactible(&g) ? "c" : "i" ) << endl;
    return 0;
}

bool parametros_ok(int argc, char** argv, string & archivo,
        ColouringAlgorithm *& algorithm, int &tiempo) {
    tiempo = 60*60;
    algorithm = NULL;
    bool a = false;
    int x = 1;
    while (x < argc) {
        if (argv[x][0] != '-') {
            if (archivo.compare("") == 0) {
                archivo = argv[x];
                break;
            } else {
                cout << "Error de Formato" << endl;
                return false;
            }
        } else {
            switch (argv[x][1]) {
                case 'd':
                    if(a)
                        return false;
                    a = true;
                    algorithm = new DsaturColouringAlgorithm();
                    break;
                case 'b':
                    if(a)
                        return false;
                    a = true;
                    algorithm = new BrownColouringAlgorithm();
                    break;
		case 'z':
                    if(a)
                        return false;
                    a = true;
                    algorithm = new BrelazColouringAlgorithm();
                    break;
                case 'k':
                  if(a)
                    return false;
                  a = true;
                  algorithm = new BrownKormanColouringAlgorithm();
                  break;
                case 't':
                    x++;
                    if (x == argc) {
                        cout << "La opcion -t se usa con un parametro <tiempo>" << endl;
                        return false;
                    }
                    tiempo = atoi(argv[x]);
                    if (tiempo <= 0) {
                        cout << "Tiempo Invalido" << endl;
                        return false;
                    }
                    break;
                default:
                    return false;
            }
        }
        x++;
    }
   if(algorithm == NULL){
       algorithm = new DsaturColouringAlgorithm();
   }
    return true;
}

void detenerEjecucion(int sig){
    cout << "-1" << endl;
    exit(sig);
}