#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class graph
{
public:
    /* Constructor */
    graph(const vector<int> &fs, const vector<int> &aps, const vector<string> &info, bool oriented);

    /* Getters */
    int getFs(int i) const;
    int getAps(int i) const;

    /* Methods */

private:
    /* Methods */
    void genAdj(const vector<int> &fs, const vector<int> &aps);

    /* Data */
    vector<int> d_fs;
    vector<int> d_aps;
    vector<string> d_info;
    bool d_oriented;
    bool **d_adj;
};

#endif //__GRAPH_H__