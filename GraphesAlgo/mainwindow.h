#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QCloseEvent>
#include <QFile>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QLabel>
#include <QtCore>
#include <QString>
#include <vector>
#include <QLineEdit>
#include <QFormLayout>
#include <QDialog>
#include <QMessageBox>
#include <string>
#include <QtWidgets>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <QGridLayout>
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QScrollArea>
#include "graph.h"


using namespace std;
class QLineEdit;
class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void createMainWindow();
        void click_DirectedGraphButton();
            void createWindow_DirectedGraph();
            void click_KeyboardEnterD();
                graph genGraphD();
                graph genGraphU();
                void graphClear();
                void addAlgorithmButtons(QVBoxLayout *mainBox);
                void addExtraBox(QVBoxLayout *mainBox);
                void createWindow_KeyboardEnterD(int NA); // NA = Nodes Amount = nombre de noeuds / sommets
                    QSpinBox* NodesAmountSpinBox;
                    int NodesAmountValue=0;
                    vector<QLineEdit*> SuccessorEntries; //attention rien à l'indice 0
                    vector<string> SuccessorEntriesValues; //attention rien à l'indice 0
                    void saveSuccessorEntries();
                    vector<int> fs;
                    vector<int> aps;

                    void RankAlgorithm();
                    void TarjanAlgorithm();
                    void showTarjanGraph();
                    void DantzigAlgorithm();
                    void DikjstraAlgorithm();
                    void KruskalAlgorithm();
                    void PruferAlgorithm();
                    void SchedulingAlgorithm();
                        vector<int> TaskDuration;
                        vector<QLineEdit*> TaskDurationEntries;
                        void SaveTaskDurationEntries();
                        void click_ButtonDuration();
                    void AlgorithmsInformation();

                    void click_ButtonAddMatrix();
                        vector<vector<QLineEdit*>> TaskCostEntries;
                        vector<vector<int>> TaskCostValues;
                        bool TaskCostValuesEmpty = true;
                        bool EdgeExist(int i, int j) const;
                        void SaveTaskCostEntries();
                    void click_ButtonRename();
                        vector<QLineEdit*> infosEntries;
                        vector<string> infos;
                        void SaveRenameEntries();

                void createWindow_FileEnter();
                    string choosenFileName="";
                    void createWindow_ChooseAlgorithm();
                        bool Check_TaskCost();
            void createWindow_UndirectedGraph();
            void createWindow_KeyboardEnterU(int NC);
                vector<vector<int>> Connections;
                int EdgesNumber;
                int NodesAmountValueUnoriented() const;
            bool oriented;
            bool file;
        void NodesAmountValueChanged(int value);
        void saveGraphToFile();
        void showCurrentGraph();
        void showGraph(const graph &g, const QString &titre);
};
#endif // MAINWINDOW_H
