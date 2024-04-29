#ifndef __GRAPHWITHCOSTS_H__
#define __GRAPHWITHCOSTS_H__

#include <iostream>
#include "graph.h"

struct arete
{
    int s, t, p; //extrémités et poids
    arete(int s, int t, int p) : s(s), t(t), p(p) {}
};

struct graphKruskal
{
    int n; //nb sommets
    vector<arete> a; //tableau d'aretes
};

class graphWithCosts
{
    /* Algorithmes de Dikjstra, Dantzig et Kruskal */
public:
    /* Constructor */
    graphWithCosts(const graph &g, const vector<vector<int>> &cout);

    /* Getters */
    int getCout(int i, int j) const;
    int getNbAreteKruskal() const;
    arete getArete(int i) const;

    /* Methods */
    bool positiveCosts() const;
    void Dikjstra(int **&dist, int **&pred) const;
    string Dantzig();
    void Kruskal();

private:
    /* Methods */
    void Dikjstra(int s, int *&d, int *&pr) const;
    void fusion(int s, int t, int* prem, int* &pilch, int* &cfc) const;
    graphKruskal convertToKruskal() const;
    static bool compareArete(const arete& a1, const arete& a2);
    
    /* Datas */
    graph d_g;
    vector<vector<int>> d_cout; //matrice des couts
    graphKruskal d_gk; //Graphe recouvrant minimal
};


#endif //__GRAPHWITHCOSTS_H__
