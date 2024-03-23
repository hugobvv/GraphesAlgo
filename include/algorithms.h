#ifndef __ALGORITHMS_H__
#define __ALGORITHMS_H__

#include <iostream>
#include "graph.h"

class algorithms
{
    /* ALGORITHMES DU RANG ET DE TARJAN */
public:
    /* Constructor */
    algorithms(const graph &g);

    /* Methods */
    void rang(int *&rang);
    void tarjan(int *&CFC);

private:
    /* Methods */
    void empiler (int x);
    int* calculateDDI() const;

    /* Datas */
    int* d_pilch;
    graph d_g;
};

#endif //__ALGORITHMS_H__