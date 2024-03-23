#include <iostream>
#include "algorithms.h"
using namespace std;

algorithms::algorithms(const graph &g): d_g{g}, d_pilch{new int[g.getAps(0)+1]} {}

void algorithms::rang(int *&rang)
{
	int n = d_g.getAps(0), s, k,h,t;
	rang = new int[n+1];
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
}

void algorithms::tarjan(int *&CFC)
{
    
}

void algorithms::empiler(int x) // avec x dans {1, ... , n} 
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