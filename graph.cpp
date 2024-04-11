#include "graph.h"

/* GRAPH CLASS */

graph::graph(const vector<int> &fs, const vector<int> &aps, const vector<string> &info, bool oriented) 
            : d_fs{fs}, d_aps{aps}, d_info{info}, d_oriented{oriented}, d_adj{}
{
    genAdj(fs, aps);
}

int graph::getFs(int i) const {return d_fs[i];}
int graph::getFsSize() const {return d_fs.size();}
int graph::getAps(int i) const {return d_aps[i];}

bool graph::isTree() const
{
  return (d_aps[0] - 1)*2 == d_fs[0] - d_aps[0] + 1;
}

vector<int> graph::prufer() const
{
  vector<int> result;

  if(!isTree()) return result;

  int n = d_aps[0];
  int* degrees = new int[n];
  for (int i = 0; i < n; i++) degrees[i] = 0;

  for (int i = 1; i <= n; i++) 
    for (int j = 1; j <= n; j++) 
      if (d_adj[i][j]) 
        degrees[i]++;
        
  int leaves = n - 2;

  while (leaves > 0)
  {
    int u = -1;
    for (int i = 0; i < n; i++) 
      if (degrees[i] == 1)
      {
        u = i;
        break;
      }
    
    int v = -1;
    for (int i = 1; i <= n; i++) 
      if (d_adj[u][i] && i != u)
      {
        v = i;
        break;
      }
      
    result.push_back(v);
    degrees[u] = 0;
    degrees[v]--;
    leaves--;
  }

  return result;
}
        
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
