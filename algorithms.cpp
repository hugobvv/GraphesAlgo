#include <iostream>
#include <stack>
#include <vector>
#include "algorithms.h"
using namespace std;

algorithms::algorithms(const graph &g): d_g{g}, d_pilch{new int[g.getAps(0)+1]}, d_numCfc{0}
{
	d_pilch[0] = 0;
}

void algorithms::rang(int *&rang)
{
	int n = d_g.getAps(0), s, k,h,t;
	rang = new int[n+1];
	rang[0] = n+1;
	int *ddi= calculateDDI();
	int *prem= new int[n+1];
    
	d_pilch[0]=0;
	for(s = 1; s <= n; s++)
	{
		rang[s] = -1; // n : nombre de sommets de G represente l'infini
		if (ddi[s] == 0) empiler(s);
	}

	k=-1;
	s=d_pilch[0];
	prem[0] = s;
	while (d_pilch[0] > 0)
	{
		k++;
		d_pilch[0] = 0;
		while (s > 0)
		{
			rang[s] = k;
			h = d_g.getAps(s);
            t = d_g.getFs(h);
			while (t > 0)
			{
				ddi[t]--;
				if (ddi[t] == 0) empiler(t);
				h++;
				t=d_g.getFs(h);
			}
			s = d_pilch[s];
		}
		s = d_pilch[0];
		prem[k+1] = s;
	}
	d_pilch[0] = 0;
}

void algorithms::traversee(int s, TarjanData &td)
{
	/* Determination des CFC des successeurs de s */
	int t;
	td.index++;
	td.num[s] = td.index; 
	td.ro[s] = td.index;	 	 // numérote s et initialise ro[s]
	td.pileTarj.push(s); 
	td.enTarj[s] = true;
	for (int k=d_g.getAps(s); (t=d_g.getFs(k)) != 0 ; k++)
	{ 
		if (td.num[t] == 0)	 	 	 // si t n'est pas encore numéroté
		{ 
			traversee(t, td);
			if (td.ro[t] < td.ro[s]) 
				td.ro[s] = td.ro[t] ;	 	 	 // Z1
		}
		else if ((td.num[t] < td.ro[s]) && td.enTarj[t]) 
			td.ro[s] = td.num[t]; // Z2
	}
	if (td.ro[s] == td.num[s])
	{
		d_numCfc++;
		int sommetPile;
		do
		{	 	 	 	 	 	 	 	 	 // Z3
			sommetPile = td.pileTarj.top();
            td.pileTarj.pop();
			td.enTarj[sommetPile] = false;
			empiler(sommetPile);
			td.CFC[sommetPile] = d_numCfc;
		} while (sommetPile != s);
		td.prem.push_back(d_pilch[0]);
		d_pilch[0] = 0;
	}
	td.CFC[0] = d_g.getAps(0);
}

void algorithms::fortConnexe(int*&prem, int*&CFC)
{
	/* Determination des CFC et de prem du graphe courant */
	int n = d_g.getAps(0);
	TarjanData data(n);
	d_pilch[0] = 0;
	d_numCfc=0;

	for(int s = 1; s<=n; s++)
		if (data.num[s] == 0) traversee(s, data);

	/* Recopie afin de garder uniquement les élements utiles (prem et CFC) */
	prem = new int [n+1];
	CFC = new int[n+1];

	for (int i=1; i<=n; i++)
	{
		prem[i]=data.prem[i-1];
		CFC[i]=data.CFC[i];
	}
	CFC[0]=data.CFC[0];
	prem[0] = data.prem.size();
}


graph algorithms::graph_reduit(int *prem, int *cfc)
{
	int s, kr = 1, t, nbCfc = prem[0];
	bool *deja_mis = new bool [nbCfc+1];
	vector<int> fsr;
	vector<int> apsr;
	apsr.push_back(nbCfc);
	fsr.push_back(0);
	for(int i=1; i<=nbCfc; i++)
	{
		apsr.push_back(kr);
		for(int i=1; i<=nbCfc; i++)
			deja_mis[i]=false;
		deja_mis[i] = true;
		s = prem[i];
		while(s != 0)
		{
			for(int k=d_g.getAps(s); (t=d_g.getFs(k)) !=0; k++)
			{
				if(deja_mis[cfc[t]] == false)
				{
					fsr.push_back(cfc[t]);
					deja_mis[cfc[t]] = true;
                    kr++;
				}
			}
			s=d_pilch[s];
		}
		fsr.push_back(0);

	}
    fsr[0]=fsr.size()-1;

	delete[] deja_mis;
	return graph(fsr, apsr);
}

void algorithms::base_Greduit(int *&br)
{
	/* A utiliser sur le graphe réduit */
	int nr = d_g.getAps(0);
	br = new int[nr+1];
	int *ddir = new int[nr+1];
	for (int i=0; i<=nr; i++)
		ddir[i] = 0;
	for (int kr=1; kr <= d_g.getFs(kr); kr++)
		ddir[d_g.getFs(kr)]++;
	int nb = 0;
	for (int c = 1; c <= nr; c++)
		if (ddir[c] == 0) 
			br[++nb] = c;
	br[0] = nb;
}

void algorithms::edition_bases()
{
	int *br, *prem, *CFC;
	base_Greduit(br);
	fortConnexe(prem, CFC);
	int nb = br[0]; // Nombre de CFC de l’unique base du graphe réduit
	int *Base = new int[nb+1]; // pile qui mémorise les sommets des bases du graphe initial
	Base[0] = nb;
	int p = 1;
	int som = prem[br[1]]; // premier sommet de la première CFC
	while (p >= 1)
	{
	 	if ((p<= nb) && (som != 0))
	 	{
	 	 	Base[p] = som;
	 	 	p++;
	 	 	if (p <= nb)
	 	 	 	som = prem[br[p]];
	 	 	else
			{// Afficher le contenu du tableau Base contenant les sommets d’une base du graphe initial
			// (QT)
			}
				
	 	}
	 	else
	 	{
	 	 	p--;
	 	 	som = d_pilch[Base[p]];
	 	}
	}
}

void algorithms::empiler(int x) // x € [1,n] 
{ 
	d_pilch[x] = d_pilch[0]; 
	d_pilch[0] = x; 
}

int* algorithms::calculateDDI() const
{
    int n = d_g.getAps(0), taillefs = d_g.getFs(0), s;
    int *ddi= new int[n+1];
    for(int i=1; i <=n ; i++) 
        ddi[i]=0;
	
    for(int i=1; i <=taillefs ; i++) 
	{
		s=d_g.getFs(i);
		if (s >0) 
            ddi[s]++;
	}
    return ddi;
}
