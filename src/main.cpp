#include <iostream>
#include "graph.h"
#include "algorithms.h"

int main()
{   /* TESTS */
	/*
	int n,m;
	cout << "Donnez le nombre de sommets de votre graphe : " ;
	cin >> n;
	cout << "Donnez le nombre d'arcs de votre graphe     : " ;
	cin >> m;
	int *aps = new int[n+1];
	int *fs = new int[n+m+1];
	aps[0] = n;
	fs[0] = n+m;
	cout << "donnez le tableau aps " << endl;
	for (int i=1; i<=n; i++) cin >> aps[i];
	cout << "donnez le tableau fs " << endl;
	for (int i=1; i<=n+m; i++) cin >> fs[i];
*/
	vector<int> aps = {12, 1, 4, 7, 11, 15, 18, 21, 23, 25, 26, 28, 31};
	vector<int> fs = {31, 2, 3, 0, 4, 9, 0, 4, 6, 8, 0, 8, 9, 11, 0, 3, 6, 0, 7, 8, 0, 8, 0, 10, 0, 0, 11, 0, 10, 12, 0, 0};
    vector<string> info = {};
    graph g = graph(fs,aps,info,true);
	int *rang;
    
    algorithms algo = algorithms(g);
    
	algo.rang(rang);
	for (int i=1; i<=aps[0]; i++)
		cout <<"rang["<<i<<"] = "<<rang[i]<<endl;
    
}