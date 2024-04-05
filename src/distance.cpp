#include "distance.h"

distance::distance (const graph &g, const vector<vector<int>> &cout) : d_g{g}, d_cout{cout}
{}

void distance::Dikjstra(int s, int *&dist, int *&pred)
{
    /* Algorithme de Dikjstra pour le sommet s */
    /* @return tableau distance et tableau predécesseurs depuis s*/

    int ind; /* nombre d'elements qui restent a traiter */
    int i, j, k, v;
    int n = d_g.getAps(0);
    int m = d_g.getFs(0);
    pred = new int[n+1];
    dist = new int[n+1];
    bool *inS = new bool[n+1]; //sommets qui restent a traiter
    /* sert a dire quels sont les sommets qui restent a traiter inS[i] = 0 ou 1 */
    /* initialisation des tableaux d, pr et inS*/
    for (i = 1;i <= n; i++)
    {
        dist[i] = d_cout[s][i];
        inS[i] = 1;
        pred[i] = -1;
    }
    dist[s] = 0;
    pred[s] = 0;
    inS[s] = false; /* on supprime le sommet 1 */
    ind = n-1;
    while (ind >0)
    {
        /* calcule du minimum selon d des sommets de S */
        m = 100;
        for (i=1;i<=n;i++)
            if (inS[i])
                if (dist[i] < m)
                {
                    m = dist[i];
                    j = i;
                }
        if (m == 100) return; //sommet inaccessible
        inS[j] = false;
        ind--;
        k = d_g.getAps(j);
        while(d_g.getFs(k) != 0)
        {
            if (inS[d_g.getFs(k)])
            {
                v = dist[j] + d_cout[j][d_g.getFs(k)];
                if (v < dist[d_g.getFs(k)])
                {
                    dist[d_g.getFs(k)] = v;
                    pred[d_g.getFs(k)] = j;
                }
            }
            k++;
        }
    }
}


void distance::Dikjstra(int **&dist, int **&pred)
{
    /* Algorithme de Dikjstra pour tous les sommets */
    /* @return matrice distance et matrice predécesseurs */
    int n = d_g.getAps(0);
    dist = new int* [n+1];
    pred = new int* [n+1];
    for (int s=1; s<=n; s++)
        Dikjstra(s, dist[s], pred[s]);
}