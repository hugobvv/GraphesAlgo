#include "graph.h"

/* GRAPH CLASS */

graph::graph(const vector<int> &fs, const vector<int> &aps, const vector<string> &info, bool oriented) 
            : d_fs{fs}, d_aps{aps}, d_info{info}, d_oriented{oriented}, d_adj{}
{
    genAdj(fs, aps);
}

int graph::getFs(int i) const {return d_fs[i];}
int graph::getAps(int i) const {return d_aps[i];}

void graph::genAdj(const vector<int> &fs, const vector<int> &aps)
{
    /*Initialiser matrice d'adjacence*/
    int n = aps[0];
    d_adj = new bool*[n+1];
    for (int i = 1; i <= n; ++i) 
    {
        d_adj[i] = new bool[n+1];
        for(int j = 1; j<=n; ++j)
            d_adj[i][j] = 0;
    }

    /*Remplir matrice d'adjacence*/
    int s = 1;
    for (int i=1; i<=fs[0]; i++)
        if(fs[i]==0)
            ++s;
        else
            d_adj[s][fs[i]] = 1;
}