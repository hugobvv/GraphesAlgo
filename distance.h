#ifndef __DISTANCE_H__
#define __DISTANCE_H__

#include <iostream>
#include "graph.h"

class distance
{
    /* Algorithmes de Dikjstra et Dantzig */
public:
    /* Constructor */
    distance (const graph &g, const vector<vector<int>> &cout);

    /* Getters */
    int getCout(int i, int j) const;

    /* Methods */
    void Dikjstra(int **&d, int **&pred) const;
    string Dantzig();

private:
    /* Methods */
    bool positiveCosts() const;
    void Dikjstra(int s, int *&d, int *&pr) const;
    
    /* Datas */
    graph d_g;
    vector<vector<int>> d_cout; //matrice des couts
};


#endif //__DISTANCE_H__
