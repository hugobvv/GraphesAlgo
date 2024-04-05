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
private:
    // oui maman donne moi les gros attributs plein de lait


     void createMainWindow();
        void click_DirectedGraphButton();
            void createWindow_DirectedGraph();
            void click_KeyobardEnterD();
                void createWindow_KeyobardEnterD(int NA); // NA = Nodes Amount = nombre de noeuds / sommets
                    QSpinBox* NodesAmountSpinBox;
                    int NodesAmountValue=0;
                    vector<QLineEdit*> SuccessorEntries;
                    vector<int> fs;
                    vector <int> aps;
                    void click_ButtonRankAlgorithm();
                    void click_ButtonTarjanAlgorithm();
                    // les autres
                    void click_ButtonAddMatrix();
                        vector<vector<QLineEdit*>> TaskCostEntries;
                        vector<vector<int>> TaskCostValues;
                        void SaveTaskCostEntries();
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
