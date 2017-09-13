#include "BrelazColouringAlgorithm.h"

vector<Nodo*> BrelazColouringAlgorithm::BruteForceClique(Grafo &g) {
  
  vector<Nodo*> nodos = *(g.getNodos());
  vector<Nodo*> clique;
  Nodo * n1;
  Nodo * n2;
  Nodo * n3;
  vector<Nodo*> adyacent;
  
  for(unsigned int i = 0; i < nodos.size(); ++i) {
    
    n1 = nodos.at(i);
    adyacent = n1->getAdyacentes();
    
    for(unsigned int j = 0; j < adyacent.size(); ++j) {
      n2 = adyacent.at(j); 
      for(unsigned int k = j+1; k < adyacent.size(); ++k) {
	n3 = adyacent.at(k);
	if (n2->esAdyacente(n3)) {
	  clique.push_back(n1);
	  clique.push_back(n2);
	  clique.push_back(n3);
	  return clique;     
	}
      }
    }	
  }
  return clique;
}
    
set<int> BrelazColouringAlgorithm::DetermineUk(int q, Nodo * x [], set<int> partialSolutionColors, int k) {
  
  set<int> U;
  int uk = partialSolutionColors.size();
  int minimal = min(uk+1,q-1);
  set<int> prohibitColors;
  vector<Nodo*> neighbor = x[k]->getAdyacentes();
  for(int i = 1; i < k; ++i) {
    for(unsigned int j = 0; j < neighbor.size(); ++j) {
      if(x[i]->getId() == neighbor.at(j)->getId())
	prohibitColors.insert(x[i]->getColor());
    }
  }

  //partialSolutionColors = partialSolutionAdjacentColors(partialSolutionColors, x, k);
  for(int i = 1; i <= minimal; ++i) {
    if(prohibitColors.count(i) == 0)
      U.insert(i);
  }
  return U;
}
    
set<int> BrelazColouringAlgorithm::Determineuk(int level, Nodo* x []) {
  
  set<int> partialSolutionColors;
  for(int i = 1; i <= level; ++i) {
    partialSolutionColors.insert(x[i]->getColor());
  }
  return partialSolutionColors;
}

void BrelazColouringAlgorithm::label(bool Label [], int k, Nodo* x []) {
  
  vector<int> ranks;
  vector<Nodo*> neighbor = x[k]->getAdyacentes();
  for(int i = 1; i < k; ++i) {
    for(unsigned int j = 0; j < neighbor.size(); ++j) {
      if(x[i]->getId() == neighbor.at(j)->getId())
	ranks.push_back(i);
    }
  }
 
 for(unsigned int i = 0; i < ranks.size(); ++i) {
   for(unsigned int j = i+1; j < ranks.size(); ++j) {
      if(x[ranks.at(i)]->getColor() == x[ranks.at(j)]->getColor()) {
	ranks.erase(ranks.begin()+j);
	j--;
      }
    }
  }

  for(vector<int>::iterator i = ranks.begin();
      i != ranks.end(); ++i) {
    Label[*i] = true;
  }
}

int BrelazColouringAlgorithm::maximalRank(bool Label [], int n) {
  
  for(int i = n ; i > 0; i--) {
    if(Label[i])
      return i;
  }
  return n;
}

int BrelazColouringAlgorithm::minimalRankQColoreable(Nodo* x [],unsigned  int q, int n) {
  
  for(int i = 1; i <= n; ++i) {
    if(x[i]->getColor() == q)
      return i;
  }
  return n;
}


int BrelazColouringAlgorithm::execute(Grafo &g) {
  
  vector<Nodo*> nodos = *(g.getNodos());
  Grafo graphSolution;
  bool foundSolution = false;
  int n = nodos.size();
  ColouringAlgorithm *heuristic = new DsaturColouringAlgorithm();
  int q = heuristic->execute(g); // Initial Coloration (upper bound).
  set<int> U [n+1];
  bool Label [n+1];
  Nodo* x [n+1];
  for(int i = 0; i <= n; ++i)
    Label[i] = false;
  g.descolorearGrafo();
  vector<Nodo*> clique = BruteForceClique(g);
  int w = clique.size();
  if (!clique.empty()) {
    // Finding a coloration order and coloring all the clique vertices.
    x[1] = clique.at(0);
    x[2] = clique.at(1);
    x[3] = clique.at(2);
    x[1]->setColor(1);
    x[2]->setColor(2);
    x[3]->setColor(3);
    int m = 4;
    for(vector<Nodo*>::iterator i = nodos.begin();
	i != nodos.end(); ++i) {
      if((*i)->getId() != x[1]->getId() && (*i)->getId() != x[2]->getId()
	&& (*i)->getId() != x[3]->getId()) {
	  x[m] = *i;
	  ++m;
      }
    }
    // Label all the clique vertices.
    Label[1] = true;
    Label[2] = true;
    Label[3] = true;
  } else {
    w = 1;
    for(int i = 1; i <= n; ++i)
      x[i] = nodos.at(i-1);
    x[1]->setColor(1);
    Label[1] = true;
  }
  if (w == q)
    return q;
  // BEGIN
  bool back = false;
  int k = w + 1;
  while(true) {
    if(!back) {
      // determine uk as the number of colors used for the actual partial
      // solution
      set<int> partialSolutionColors = Determineuk(k-1, x);
      // determine U(xk)
      U[k] = DetermineUk(q, x, partialSolutionColors, k);
    } else {
      U[k].erase(U[k].find(x[k]->getColor()));
      Label[k] = false;
    }
    if (!U[k].empty()) {
      int i = *(U[k].begin());  // Determine i as the minimal color in U(xk)
      x[k]->setColor(i); // Color vertex xk with color i.
      k = k + 1;
      if(k > n) {
	q = (Determineuk(n,x)).size();
	graphSolution.clone(g);
	foundSolution = true;
	if(q == w) 
	  break;
	// determine k as the minimal rank among all q-colored vertices
	k = minimalRankQColoreable(x, q, n);
	// remove all labels from the vertices xk, .. xn 
	for(int i = k; i <= n; ++i)
	  Label[i] = false;
	back = true;
      } else
	  back = false;
    } else
	back = true;
    if (back) {
      label(Label, k, x);
      // determine k as the maximal rank among all labeled vertices
      k = maximalRank(Label, n);
      if(k <= w)
	break;
    }
  }
  // END
  if (foundSolution)
    g = graphSolution;
  return q;
}