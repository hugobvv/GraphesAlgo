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
    vector<int> getFs();
    vector<int> getAps();
    vector<vector<int>>getTaskCostMatrix();

private:
    void createMainWindow();
        void click_DirectedGraphButton();
            void createWindow_DirectedGraph();
            void click_KeyboardEnterD();
                graph genGraph();
                void graphClear();
                void addAlgorithmButtons(QVBoxLayout *mainBox);
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
                    void DantzigAlgorithm();
                    void DikjstraAlgorithm();
                    void KruskalAlgorithm();

                    void click_ButtonAddMatrix();
                        vector<vector<QLineEdit*>> TaskCostEntries;
                        vector<vector<int>> TaskCostValues;
                        boolean TaskCostValuesEmpty = true;
                        void SaveTaskCostEntries();
                        void DeleteTaskCostEntries();
                void createWindow_FileEnterD();
                    string choosenFileName="";
                    void createWindow_ChooseAlgorithm();
                        bool Check_TaskCost();
            void GraphicEnterD();
            void createWindow_UndirectedGraph();
            void KeyboardEnterU();
            void FileEnterU();
            void GraphicEnterU();
            bool oriented;
        void NodesAmountValueChanged(int value);

        void afficheFs();
        void afficheAps();
};
#endif // MAINWINDOW_H
