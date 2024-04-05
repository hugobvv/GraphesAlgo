#include <iostream>
#include "graph.h"
#include "algorithms.h"

void testRang()
{
	vector<int> aps = {9, 1, 4, 6, 9, 10, 11, 13, 16, 20};
	vector<int> fs = {19, 2, 5, 0, 6, 0, 2, 4, 0, 8, 0, 0, 0, 3, 6, 0, 5, 9, 0, 5};
    vector<string> info = {};
    graph g = graph(fs,aps,info,true);
	int *rang;
    
    algorithms algo = algorithms(g);
    
	algo.rang(rang);
	for (int i=0; i<=aps[0]; i++)
		cout <<"rang["<<i<<"] = "<<rang[i]<<endl;
}

void testTarjan()
{
	vector<int> aps = {10, 1, 3, 6, 8, 11, 13, 18, 20, 22, 24};
	vector<int> fs = {19, 3, 0, 1, 7, 0, 2, 0, 1, 5, 0, 10, 
	0, 3, 4, 8, 9, 0, 8, 0, 3, 0, 4, 0, 9};
    vector<string> info = {};
    graph g = graph(fs,aps,info,true);
	int *cfc;
	int *prem;
    
    algorithms algo = algorithms(g);
    
	/* Affichage CFC et prem */
	algo.fortConnexe(prem, cfc);
	for (int i=0; i<=aps[0]; i++)
		cout <<"cfc["<<i<<"] = "<<cfc[i]<<endl;
	cout<<endl;

	for (int i=0; i<=prem[0]; i++)
		cout <<"prem["<<i<<"] = "<<prem[i]<<endl;
	cout<<endl;

	/* Affichage bases réduite */
	graph g1 = algo.graph_reduit(prem, cfc);
	for (int i=0; i<=g1.getFs(0); i++)
		cout <<"fs["<<i<<"] = "<<g1.getFs(i)<<endl;
}

int main()
{   /* TESTS */

	testTarjan();
    return 0;
}

