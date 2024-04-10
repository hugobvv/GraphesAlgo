#include "graphWithCosts.h"
#include <QDebug>

graphWithCosts::graphWithCosts (const graph &g, const vector<vector<int>> &cout) : d_g{g}, d_cout{cout}
{}

int graphWithCosts::getCout(int i, int j) const
{
    return d_cout[i][j];
}

bool graphWithCosts::positiveCosts() const
{
    for (int i=1; i<d_cout.size(); i++)
        for (int j=1; j<d_cout.size(); j++)
            if (d_cout[i][j] < 0)
                return false;
    return true;
}

void graphWithCosts::Dikjstra(int s, int *&dist, int *&pred) const
{
    /* Algorithme de Dikjstra pour le sommet s */
    /* @return tableau distance et tableau predécesseurs depuis s*/
    /* les coûts sont >= 0 */

    int ind; /* nombre d'elements qui restent a traiter */
    int i, j, k, v, m;
    int n = d_g.getAps(0);
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
        if (m == 100)
        {
            delete[] inS;
            return; //sommet inaccessible
        }
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
    delete[] inS;
}


void graphWithCosts::Dikjstra(int **&dist, int **&pred) const
{
    /* Algorithme de Dikjstra pour tous les sommets */
    /* @return matrice distance et matrice predécesseurs */

    int n = d_g.getAps(0);
    dist = new int* [n+1];
    pred = new int* [n+1];
    for (int s=1; s<=n; s++)
        Dikjstra(s, dist[s], pred[s]);
}

string graphWithCosts::Dantzig()
{
    /* Modifie la matrice des couts en trouvant les chemins les plus courts */
    /* Les coûts peuvent être négatifs */
    /* Retourne un message si présence d'un circuit absorbant */
    int x;
    for (int k=1; k<d_cout.size()-1; k++)
    {
        for (int i=1; i<=k; i++)
        {
            for (int j=1; j<=k; j++)
                if((x = d_cout[i][j] + d_cout[j][k+1]) < d_cout[i][k+1])
                    d_cout[i][k+1] = x;
            for (int j=1; j<=k; j++)
                if((x = d_cout[k+1][j] + d_cout[j][i]) < d_cout[k+1][i])
                    d_cout[k+1][i] = x;
            if (d_cout[i][k+1] + d_cout[k+1][i] < 0)
                return "Existence d'un circuit absorbant passant par les sommets " + to_string(i) + " et " + to_string(k+1);
        }
        for (int i=1; i<=k; i++)
            for (int j=1; j<=k; j++)
                if ((x = d_cout[i][k+1]+d_cout[k+1][j]) < d_cout[i][j])
                    d_cout[i][j] = x;
    }
    return "";
}
