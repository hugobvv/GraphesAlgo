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
            void click_KeyobardEnterD();
                graph genGraph();
                void createWindow_KeyobardEnterD(int NA); // NA = Nodes Amount = nombre de noeuds / sommets
                    QSpinBox* NodesAmountSpinBox;
                    int NodesAmountValue=0;
                    vector<QLineEdit*> SuccessorEntries; //attention rien à l'indice 0
                    vector<string> SuccessorEntriesValues; //attention rien à l'indice 0
                    void saveSuccessorEntries();
                    vector<int> fs;
                    vector <int> aps;
                    void click_ButtonRankAlgorithm();
                    void click_ButtonTarjanAlgorithm();
                    // les autres
                    void click_ButtonAddMatrix();
                        vector<vector<QLineEdit*>> TaskCostEntries;
                        vector<vector<int>> TaskCostValues;
                        boolean TaskCostValuesEmpty = true;
                        void SaveTaskCostEntries();
                        void DeleteTaskCostEntries();
            void click_FileEnterD();
                void createWindow_FileEnterD();
                    string choosenFileName;
                    void createWindow_ChooseAlgorithm();
                        void Check_TaskCost();
                            vector<vector<int>> TaskCost;
            void click_GraphicEnterD();
        void click_UndirectedGraphButton();
           void createWindow_UndirectedGraph();
           void click_KeyobardEnterU();
           void click_FileEnterU();
           void click_GraphicEnterU();
        void click_ExitButton();
        void NodesAmountValueChanged(int value);


        void afficheFs();
        void afficheAps();

private slots:
    void click_MenuTotalCancel();
    void click_MenuCancel();
};
#endif // MAINWINDOW_H
