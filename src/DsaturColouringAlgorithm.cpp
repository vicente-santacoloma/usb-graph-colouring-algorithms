#include "DsaturColouringAlgorithm.h"

vector<Nodo*> DsaturColouringAlgorithm::maximoGradoSaturacion(vector<Nodo*> nodos) {

    vector<Nodo*> nodosMaximo;
    nodosMaximo.clear();
    Nodo * max = nodos.at(0);
    nodosMaximo.push_back(max);
    for (unsigned int i = 1; i < nodos.size(); ++i) {
	Nodo * aux = nodos.at(i);
	if (max->getGradoSaturacion() < aux->getGradoSaturacion()) {
	    nodosMaximo.clear();
	    nodosMaximo.push_back(aux);
	    max = aux;
	} else if (max->getGradoSaturacion() == aux->getGradoSaturacion()) {
	    nodosMaximo.push_back(aux);
	}
    }
    return nodosMaximo;

}

vector<Nodo*> DsaturColouringAlgorithm::maximoGrado(vector<Nodo*> nodos) {

    vector<Nodo*> nodosMaximo;
    nodosMaximo.clear();
    Nodo * max = nodos.at(0);
    nodosMaximo.push_back(max);
    for (unsigned int i = 1; i < nodos.size(); ++i) {
	Nodo * aux = nodos.at(i);
	if (max->getGrado() < aux->getGrado()) {
	    nodosMaximo.clear();
	    nodosMaximo.push_back(aux);
	    max = aux;
	} else if (max->getGrado() == aux->getGrado()) {
	    nodosMaximo.push_back(aux);
	}
    }
    return nodosMaximo;
}

int DsaturColouringAlgorithm::execute(Grafo &g) {

    set<int> colores;
    vector<Nodo*> nodos = *(g.getNodos());
    vector<Nodo*> gradoSaturacion;
    vector<Nodo*> grado;
    int nodosPorColorear = nodos.size() - 1;
    // PASO 1: Arreglar los vertices por orden decreciente de grado 
    sort(nodos.begin(), nodos.end(), this->miComparar);
    Nodo * n = nodos.at(0);
    // PASO 2: Colorear el vertice con mayor grado con 1
    n->setColor(1);
    colores.insert(1);
    n->incrementarGradoSaturacion();
    nodos.erase(nodos.begin());

    for (int i = 0; i < nodosPorColorear; ++i) {
	gradoSaturacion = maximoGradoSaturacion(nodos);
	Nodo * nodoColorear;
	if (gradoSaturacion.size() == 1) {
	    nodoColorear = gradoSaturacion.at(0);
	    gradoSaturacion.pop_back();
	} else {
	    grado = maximoGrado(gradoSaturacion);
	    if (grado.size() == 1) {
		nodoColorear = grado.at(0);
		grado.pop_back();
	    } else {
        for (unsigned int i = 0; i < nodos.size(); ++i) {
		    Nodo * a = nodos.at(i);
        for (unsigned int j = 0; j < grado.size(); ++j) {
			Nodo * b = grado.at(j);
			if (a->getId() == b->getId()) {
			    nodoColorear = a;
			    nodos.erase(nodos.begin() + i);
			    grado.clear();
			    break;
			}
		    }
		}
	    }
	}
	for (unsigned int i = 0; i < nodos.size(); ++i) {
	    if (nodos.at(i)->getId() == nodoColorear->getId()) {
		nodos.erase(nodos.begin() + i);
		break;
	    }
	}
	gradoSaturacion.clear();
	nodoColorear->setColor(nodoColorear->encontrarMenorColor());
	colores.insert(nodoColorear->getColor());
    }

    return colores.size();
}