#ifndef __ALGORITHMS_H__
#define __ALGORITHMS_H__

#include <iostream>
#include <stack>
#include "graph.h"

/* ALGORITHMES DU RANG ET DE TARJAN */

struct TarjanData 
{
    /* Données pour l'algorithme de Tarjan */
    int* num;
    int* ro;
    std::stack<int> pileTarj;
    bool* enTarj;
    std::vector<int> prem;
    int* CFC;
    int index;

    // Constructeur
    TarjanData(int n) {
        index = 0;
        num = new int[n + 1];
        ro = new int[n + 1];
        enTarj = new bool[n + 1];
        CFC = new int[n + 1];

        for (int i = 1; i<=n; i++)
	    {
			num[i] = 0;
			ro[i] = 0;
			enTarj[i] = false;
	    }
        pileTarj.push(0);
    }

    // Destructeur
    ~TarjanData() {
        delete[] num;
        delete[] ro;
        delete[] enTarj;
        delete[] CFC;
    }
};


class algorithms
{
public:
    /* Constructor */
    algorithms(const graph &g);

    /* Rang */
    void rang(int *&rang);
    
    /* Tarjan */
    void traversee(int s, TarjanData &td);
    void fortConnexe(int*&prem, int*&CFC);
    graph graph_reduit(int *prem, int *cfc);
    void base_Greduit(int *&br);
    void edition_bases();

private:
    /* Methods */
    void empiler (int x);
    int* calculateDDI() const;

    /* Datas */
    graph d_g;
    int* d_pilch;
    int d_numCfc;
};

#endif //__ALGORITHMS_H__
