#include "scheduling.h"
#include <vector>
#include <algorithm>
#include <functional>


using namespace std;


scheduling::scheduling(const graph &graphe): d_graphe{graphe}
{
    int n = d_graphe.getAps(0);
    d_task.resize(n);
    for(int i = 0; i < n; i++)  
        d_task[i].name = d_graphe.getInfo(i+1);

    int current_task = 1;

    for (int i = 1; i < d_graphe.getFsSize(); ++i)
    {
        int fs_value = d_graphe.getFs(i);

        if (fs_value == 0)
        {
            ++current_task;
            continue;
        }

        d_task[current_task - 1].successors.push_back(fs_value);

        d_task[fs_value - 1].predecessors.push_back(current_task);
    }
}


void scheduling::calculateEarliestStart()
{

    for (int i = 0; i < d_task.size(); ++i)
    {
        int max_earliest = 0;
        for (const auto& pred : d_task[i].predecessors)
        {
            max_earliest = max(max_earliest, d_task[pred - 1].earliest_start + d_task[pred - 1].duration);
        }
        d_task[i].earliest_start = max_earliest;
    }
}

void scheduling::calculateLatestStart()
{
    for (auto& task : d_task)
        task.latest_start = INT_MAX;

    d_task.back().latest_start = d_task.back().earliest_start;
    for (int i = d_task.size() - 2; i >= 0; --i)
    {
        int min_latest = INT_MAX;
        for (const auto& succ : d_task[i].successors)
        {
            min_latest = min(min_latest, d_task[succ - 1].latest_start - d_task[i].duration);
        }
        d_task[i].latest_start = min_latest;
    }
}


vector<task> scheduling::getTask() const
{
    return d_task;
}

void scheduling::enterduration(vector<int> &t)
{
    for(int i = 0; i < t.size(); i++)
    {
        d_task[i].duration = t[i];
    }
}

vector<vector<string>> scheduling::getCritical_paths() const
{
    return critical_paths;
}


void scheduling::findCriticalPath(int tasknum, vector<string> &current_path)
{
    if (tasknum == d_task.size() - 1)
    {
        current_path.push_back(d_task[tasknum].name);
        critical_paths.push_back(current_path);
        current_path.pop_back();
        return;
    }

    if (d_task[tasknum].latest_start == d_task[tasknum].earliest_start)
        {
        current_path.push_back(d_task[tasknum].name);
        for (int i = 0; i < d_task[tasknum].successors.size(); i++)
        {
                findCriticalPath(d_task[tasknum].successors[i] - 1, current_path);
        }
        current_path.pop_back();
    }
}

void scheduling::calculateCriticalPath()
{
    vector<string> current_path;
    findCriticalPath(0,current_path);
}

