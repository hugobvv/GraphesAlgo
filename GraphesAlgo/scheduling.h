#ifndef SCHEDULING_H_INCLUDED
#define SCHEDULING_H_INCLUDED

#include <iostream>
#include "graph.h"
#include <vector>


using namespace std;


struct task
{
    /* Data for scheduling*/
    string name;
    vector<int> predecessors;
    vector<int> successors;
    int earliest_start = 0;
    int latest_start = 0;
    int duration = 0;
};

class scheduling
{
public:
    /* Constructor */
    scheduling(const graph &graphe);

    /* Methods */
    void calculateEarliestStart();
    void calculateLatestStart();
    void findCriticalPath(int current_task, vector<string>& current_path);
    void calculateCriticalPath();
     void enterduration(vector<int> &t);

    /* Getters */
    vector<task> getTask() const;
    vector<vector<string>> getCritical_paths() const;

private:
    /* Datas */
    graph d_graphe;
    vector<task> d_task;
    vector<vector<string>> critical_paths;
};


#endif // SCHEDULING_H_INCLUDED
