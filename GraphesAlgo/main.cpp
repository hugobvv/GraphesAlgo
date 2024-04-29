#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include "graph.h"
#include "algorithms.h"


void testRang()
{
    vector<int> aps = {9, 1, 4, 6, 9, 10, 11, 13, 16, 20};
    vector<int> fs = {19, 2, 5, 0, 6, 0, 2, 4, 0, 8, 0, 0, 0, 3, 6, 0, 5, 9, 0, 5};
    vector<string> info = {};
    graph g = graph(fs,aps,info);
    int *rang;

    algorithms algo = algorithms(g);

    algo.rang(rang);
    for (int i=0; i<=aps[0]; i++)
        qInfo()<<"rang["<<i<<"] = "<<rang[i];
}


void testTarjan()
{
    vector<int> aps = {10, 1, 3, 6, 8, 11, 13, 18, 20, 22, 24};
    vector<int> fs = {19, 3, 0, 1, 7, 0, 2, 0, 1, 5, 0, 10,
    0, 3, 4, 8, 9, 0, 8, 0, 3, 0, 4, 0, 9};
    vector<string> info = {};
    graph g = graph(fs,aps,info);
    int *cfc;
    int *prem;

    algorithms algo = algorithms(g);

    /* Affichage CFC et prem */
    algo.fortConnexe(prem, cfc);
    for (int i=0; i<=aps[0]; i++)
        qInfo() <<"cfc["<<i<<"] = "<<cfc[i];

    for (int i=0; i<=prem[0]; i++)
        qInfo() <<"prem["<<i<<"] = "<<prem[i];

    /* Affichage bases réduite */
    graph g1 = algo.graph_reduit(prem, cfc);
    for (int i=0; i<=g1.getFs(0); i++)
        qInfo() <<"fs["<<i<<"] = "<<g1.getFs(i);
}

void testPrufer()
{
    vector<int> aps = {8, 1, 5, 10, 13, 15, 17, 19, 21};
    vector<int> fs = {21,4,8,2,0,1,5,3,6,0,2,7,0,1,0,2,0,2,0,3,0,1};
    vector<string> info = {};

    graph g = graph(fs,aps,info);

    vector<int> pruferCode = g.prufer();

    for(int i = 0; i < pruferCode.size(); i++)
        qInfo()<<pruferCode[i] << " ";
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    testPrufer();
    w.show();
    return a.exec();
}
