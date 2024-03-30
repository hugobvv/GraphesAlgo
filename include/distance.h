#ifndef __DISTANCE_H__
#define __DISTANCE_H__

#include <iostream>
#include "graph.h"

class distance
{
public:
    distance (const graph &g, const vector<vector<int>> &cout);
    void Dikjstra(int s, int *&d, int *&pr);
    void Dikjstra(int **&d, int **&pred);
private:
    graph d_g;
    vector<vector<int>> d_cout; //matrice des couts
};


#endif //__DISTANCE_H__