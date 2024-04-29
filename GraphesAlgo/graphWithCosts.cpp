#include "graphWithCosts.h"
#include <QDebug>

graphWithCosts::graphWithCosts (const graph &g, const vector<vector<int>> &cout) : d_g{g}, d_cout{cout}
{}

int graphWithCosts::getCout(int i, int j) const
{
    return d_cout[i][j];
}

int graphWithCosts::getNbAreteKruskal() const
{
    return d_gk.a.size();
}

arete graphWithCosts::getArete(int i) const
{
    return d_gk.a[i];
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

void graphWithCosts::Kruskal()
{
    graphKruskal g = convertToKruskal();

    graphKruskal gMin;
    gMin.n = g.n;
    int *prem = new int [g.n+1];
    int *pilch = new int [g.n+1];
    int *cfc = new int [g.n+1];
    for (int i=1; i<=g.n; ++i)
    {
        prem[i] = i;
        pilch[i] = 0;
        cfc[i] = i;
    }
    pilch[0] = 0;
    int i = 0;
    while (gMin.a.size() < g.n-1)
    {
        arete d = g.a[i];
        if(cfc[d.s]!=cfc[d.t])
        {
            gMin.a.push_back(d);
            fusion(d.s,d.t,prem,pilch,cfc);
        }
        ++i;
    }

    d_gk = gMin;
    delete[] cfc;
    delete[] pilch;
    delete[] prem;
}

void graphWithCosts::fusion(int s, int t, int* prem, int* &pilch, int* &cfc) const
{
    int cs = cfc[s];
    int ct = cfc[t];
    int x = prem[ct];
    while (x!=0)
    {
        cfc[x] = cs;
        x = pilch[x];
    }
    x = prem[cs];
    while(pilch[x]!=0)
        x = pilch[x];
    pilch[x] = prem[ct];
}

graphKruskal graphWithCosts::convertToKruskal() const
{
    graphKruskal kruskalGraph;
    kruskalGraph.n = d_g.getAps(0); // Initialisation de kruskalGraph.n avec le nombre de sommets de g

    /* Ajout des aretes */
    int t;
    for (int s=1; s < d_g.getAps(0); ++s)
        for (int indFs = d_g.getAps(s); (t = d_g.getFs(indFs))!=0; ++indFs)
            if (t >= s) //comme on parcours le graphe dans l'ordre, cela permet de ne pas ajouter 2x la même arete
                kruskalGraph.a.push_back(arete(s,t,d_cout[s][t]));

    /* Tri des aretes selon leur poids */
    sort(kruskalGraph.a.begin(), kruskalGraph.a.end(), compareArete);

    return kruskalGraph;
}

bool graphWithCosts::compareArete(const arete& a1, const arete& a2)
{
    /* Fonction de comparaison personnalisée pour trier les arêtes */
    // Compare d'abord les poids
    if (a1.p != a2.p) {
        return a1.p < a2.p;
    }
    // Si les poids sont égaux, compare s
    if (a1.s != a2.s) {
        return a1.s < a2.s;
    }
    // Si s est égal, compare t
    return a1.t < a2.t;
}

