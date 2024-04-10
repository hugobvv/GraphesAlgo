#ifndef __GRAPHWITHCOSTS_H__
#define __GRAPHWITHCOSTS_H__

#include <iostream>
#include "graph.h"

class graphWithCosts
{
    /* Algorithmes de Dikjstra et Dantzig */
public:
    /* Constructor */
    graphWithCosts(const graph &g, const vector<vector<int>> &cout);

    /* Getters */
    int getCout(int i, int j) const;

    /* Methods */
    bool positiveCosts() const;
    void Dikjstra(int **&dist, int **&pred) const;
    string Dantzig();

private:
    /* Methods */
    void Dikjstra(int s, int *&d, int *&pr) const;
    
    /* Datas */
    graph d_g;
    vector<vector<int>> d_cout; //matrice des couts
};


#endif //__GRAPHWITHCOSTS_H__
