#include "mainwindow.h"
#include "graph.h"
#include "algorithms.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createMainWindow();
}

MainWindow::~MainWindow()
{
}

void MainWindow::createMainWindow()
{
    resize(400,200);
    setMinimumSize(400,200);
    setMaximumSize(400,200);
    setWindowTitle(tr("Création de Graphe"));
    menuBar()->clear();

    //setWindowIcon(QIcon("../Ressources/icon.png"));  //<--pour mettre un icon mais azy jpense pas le temps

    auto MainWidget= new QWidget;
   setCentralWidget(MainWidget);

    auto mainBox = new QVBoxLayout;
    MainWidget->setLayout(mainBox);

    auto DirectedGraphButton = new QPushButton{tr("Créer un graphe ORIENTÉ")};
    DirectedGraphButton->setMinimumHeight(60);
    mainBox->addWidget(DirectedGraphButton);
    connect(DirectedGraphButton, &QPushButton::clicked, this, &MainWindow::click_DirectedGraphButton);

    auto UndirectedGraphButton = new QPushButton{tr("Créer un graphe NON-ORIENTÉ")};
    UndirectedGraphButton->setMinimumHeight(60);
    mainBox->addWidget(UndirectedGraphButton);
    connect(UndirectedGraphButton, &QPushButton::clicked, this, &MainWindow::click_UndirectedGraphButton);

    auto ExitButton = new QPushButton{tr("QUITTER")};
    ExitButton->setMinimumHeight(40);
    mainBox->addWidget(ExitButton);
    connect(ExitButton, &QPushButton::clicked, this, &MainWindow::click_ExitButton);
}

void MainWindow::click_DirectedGraphButton()
{
    createWindow_DirectedGraph();
}

void MainWindow::click_UndirectedGraphButton()
{
    createWindow_UndirectedGraph();
}

void MainWindow::click_ExitButton()
{
    close();
}


//----------------- FIN FENETRE PRINCIPALE----------------------//




//----------------- DEBUT FENETRE GRAPHE ORIENTE----------------------//

void MainWindow::createWindow_DirectedGraph()
{

    resize(400,220);
    setMinimumSize(400,220);
    setMaximumSize(400,220);
    setWindowTitle(tr("Création d'un Graphe Orienté"));

    //NETTOYAGE SINON ANCIENS BOUTONS REVIENNENT
    menuBar()->clear();
    auto MainWidget= new QWidget;
    setCentralWidget(MainWidget);

    //CREATION DE BOX ET BOUTONS
    auto mainBox = new QVBoxLayout;
    MainWidget->setLayout(mainBox);

    auto KeyobardEnter = new QPushButton{tr("Saisir le graphe AU CLAVIER")};
    KeyobardEnter->setMinimumHeight(50);
    mainBox->addWidget(KeyobardEnter);
    connect(KeyobardEnter, &QPushButton::clicked, this, &MainWindow::click_KeyobardEnterD);

    auto FileEnter = new QPushButton{tr("Saisir le graphe avec FICHIER")};
    FileEnter->setMinimumHeight(50);
    mainBox->addWidget(FileEnter);
    connect(FileEnter, &QPushButton::clicked, this, &MainWindow::click_FileEnterD);

    auto GraphicEnter = new QPushButton{tr("Saisir le graphe GRAPHIQUEMENT")};
    GraphicEnter->setMinimumHeight(50);
    mainBox->addWidget(GraphicEnter);
    connect(GraphicEnter, &QPushButton::clicked, this, &MainWindow::click_GraphicEnterD);

    //MENU
    auto MenuTotalCancel = menuBar()->addAction("Retour au menu principal");
    connect(MenuTotalCancel,&QAction::triggered,this,&MainWindow::click_MenuTotalCancel);
}

void MainWindow::click_KeyobardEnterD()
{
    createWindow_KeyobardEnterD(0);
}

void MainWindow::click_FileEnterD()
{
    createWindow_FileEnterD();
}

void MainWindow::click_GraphicEnterD()
{

}

void MainWindow::click_MenuTotalCancel()
{
    createMainWindow();
}

graph MainWindow::genGraph()
{
    /* Génère le graphe courant */

    if (fs.size() == 0) // si le graphe n'a pas été encore généré
    {
        fs.clear();
        fs.push_back(0); // initialiser la premiere case à 0 (nbr de cases du tableau fs)
        aps.clear();
        aps.push_back(0);

        for(int i=1; i<static_cast<int>(SuccessorEntries.size());i++)
        {
            string V = SuccessorEntries[i]->text().toStdString();


            if(!(V.length() == 0))
            {
                vector<int> tmp;
                stringstream ss(V);
                while (ss.good())
                {
                    int number;
                    char delimiter;
                    ss >> number >> delimiter;

                    if(number > static_cast<int>(SuccessorEntries.size())-1 || number < 0) // saisie securisée, j'ai pas mis que le delimiteur doit etre obligatoirement "," mais az ptetre à faire
                    {
                        QMessageBox{QMessageBox::Warning, "Erreur de saisie","Le sommet " + QString::fromStdString(to_string(number)) + " n'existe pas." ,QMessageBox::Ok}.exec();
                        return graph({},{},{});
                    }
                    if(number == 0 || !(isdigit(V[V.length() - 1])))
                    {
                        QMessageBox{QMessageBox::Warning, "Erreur de saisie","Un sommet 0 a été saisi ou la saisie a mal été effectuée, vérifiez vos valeurs.\nSi un sommet n'a pas de successeurs, laissez la case vide.", QMessageBox::Ok}.exec();
                        return graph({},{},{});
                    }

                    tmp.push_back(number);
                }
                for(int n : tmp)
                    fs.push_back(n);
            }
            fs.push_back(0);
        }
        fs[0] = fs.size()-1;

        aps.push_back(1);
        for(int i=1; i<static_cast<int>(fs.size()); i++) // remplir aps à partir de fs
        {
            if(fs[i] == 0 && i+1 < static_cast<int>(fs.size()))
                aps.push_back(i+1);
        }
        aps[0] = aps.size()-1;
    }
    vector<string> info = {};
    return graph(fs,aps,info,true);
}

//----------------- FIN FENETRE GRAPHE ORIENTE----------------------//




//----------------- DEBUT FENETRE GRAPHE NON-ORIENTE----------------------//

void MainWindow::createWindow_UndirectedGraph()
{
    resize(400,220);
    setMinimumSize(400,220);
    setMaximumSize(400,220);
    setWindowTitle(tr("Création d'un Graphe Non-Orienté"));

    //NETTOYAGE SINON ANCIENS BOUTONS REVIENNENT
    menuBar()->clear();
    auto MainWidget= new QWidget;
    setCentralWidget(MainWidget);

    //CREATION DE BOX ET BOUTONS
    auto mainBox = new QVBoxLayout;
    MainWidget->setLayout(mainBox);

    auto KeyobardEnter = new QPushButton{tr("Saisir le graphe AU CLAVIER")};
    KeyobardEnter->setMinimumHeight(50);
    mainBox->addWidget(KeyobardEnter);
    connect(KeyobardEnter, &QPushButton::clicked, this, &MainWindow::click_KeyobardEnterU);

    auto FileEnter = new QPushButton{tr("Saisir le graphe avec FICHIER")};
    FileEnter->setMinimumHeight(50);
    mainBox->addWidget(FileEnter);
    connect(FileEnter, &QPushButton::clicked, this, &MainWindow::click_FileEnterU);

    auto GraphicEnter = new QPushButton{tr("Saisir le graphe GRAPHIQUEMENT")};
    GraphicEnter->setMinimumHeight(50);
    mainBox->addWidget(GraphicEnter);
    connect(GraphicEnter, &QPushButton::clicked, this, &MainWindow::click_GraphicEnterU);

    //MENU
    auto MenuTotalCancel = menuBar()->addAction("Retour au menu principal");
    connect(MenuTotalCancel,&QAction::triggered,this,&MainWindow::click_MenuTotalCancel);
}

void MainWindow::click_KeyobardEnterU()
{

}

void MainWindow::click_FileEnterU()
{

}

void MainWindow::click_GraphicEnterU()
{

}

//----------------- FIN FENETRE GRAPHE NON-ORIENTE----------------------//


//----------------- DEBUT FENETRE SAISIE CLAVIER GRAPHE ORIENTE----------------------//

void MainWindow::createWindow_KeyobardEnterD(int NA)
{
    SuccessorEntries.resize(NA+1);
    for(int i=1; i<static_cast<int>(SuccessorEntries.size());i++)
        SuccessorEntries[i]= new QLineEdit{""};

    if(SuccessorEntriesValues.size()!=0)
        for(int i=1; i<static_cast<int>(SuccessorEntriesValues.size()); i++)
                SuccessorEntries[i]->setText(QString::fromStdString(SuccessorEntriesValues[i]));

    setMinimumSize(600,350+(NA/2.0)*53);
    setMaximumSize(600,350+(NA/2.0)*53);
    setWindowTitle(tr("Saisie au clavier d'un graphe orienté"));

    //NETTOYAGE SINON ANCIENS BOUTONS REVIENNENT
    menuBar()->clear();
    auto MainWidget= new QWidget;
    setCentralWidget(MainWidget);

    //CREATION DE BOX ET BOUTONS
    auto mainBox = new QVBoxLayout;
    MainWidget->setLayout(mainBox);


    auto NodesAmountChoiceBox = new QHBoxLayout;
    mainBox->addLayout(NodesAmountChoiceBox);
        auto NodesAmountLabel = new QLabel{tr("                 NOMBRE DE SOMMETS DU GRAPHE : ")};
        NodesAmountChoiceBox->addWidget(NodesAmountLabel, 0, Qt::AlignCenter);
        NodesAmountLabel->setMinimumHeight(50);
        auto NodesAmountSpinBox = new QSpinBox{};
        NodesAmountSpinBox->setValue(NA);
        NodesAmountSpinBox->setMinimum(0);
        NodesAmountSpinBox->setMaximum(20);
        NodesAmountSpinBox->setMinimumWidth(50);
        NodesAmountSpinBox->setMinimumHeight(30);
        NodesAmountChoiceBox->addWidget(NodesAmountSpinBox, 0, Qt::AlignCenter);

        connect(NodesAmountSpinBox, &QSpinBox::valueChanged, this, &MainWindow::NodesAmountValueChanged);
        connect(NodesAmountSpinBox, &QSpinBox::valueChanged, this, [this]{ MainWindow::createWindow_KeyobardEnterD(NodesAmountValue);});

    auto LigneH = new QFrame{};
    LigneH->setFrameStyle(QFrame::HLine | QFrame :: Sunken);
    mainBox->addWidget(LigneH);

    auto AlgorithmsButtonBox = new QVBoxLayout;
    mainBox->addLayout(AlgorithmsButtonBox);


        auto AlgorithmsButtonLayer1 = new QHBoxLayout;
        AlgorithmsButtonBox->addLayout(AlgorithmsButtonLayer1);

            auto ButtonRankAlgortihm = new QPushButton{tr("Algorithme du RANG")};
            ButtonRankAlgortihm->setMinimumHeight(40);
            AlgorithmsButtonLayer1->addWidget(ButtonRankAlgortihm);
            connect(ButtonRankAlgortihm, &QPushButton::clicked, this, &MainWindow::click_ButtonRankAlgorithm);

            auto ButtonTarjanAlgortihm = new QPushButton{tr("Algorithme de TARJAN")};
            ButtonTarjanAlgortihm->setMinimumHeight(40);
            AlgorithmsButtonLayer1->addWidget(ButtonTarjanAlgortihm);
            connect(ButtonTarjanAlgortihm, &QPushButton::clicked, this, &MainWindow::click_ButtonTarjanAlgorithm);


        auto AlgorithmsButtonLayer2 = new QHBoxLayout;
        AlgorithmsButtonBox->addLayout(AlgorithmsButtonLayer2);

            auto ButtonSchedulingAlgortihm = new QPushButton{tr("Algorithme de l'ORDONNANCEMENT")};
            ButtonSchedulingAlgortihm->setMinimumHeight(40);
            AlgorithmsButtonLayer2->addWidget(ButtonSchedulingAlgortihm);

            auto ButtonDantzigAlgortihm = new QPushButton{tr("Algorithme de DANTZIG")};
            ButtonDantzigAlgortihm->setMinimumHeight(40);
            AlgorithmsButtonLayer2->addWidget(ButtonDantzigAlgortihm);

            auto ButtonDijkstraAlgortihm = new QPushButton{tr("Algorithme de DIJKSTRA")};
            ButtonDijkstraAlgortihm->setMinimumHeight(40);
            AlgorithmsButtonLayer2->addWidget(ButtonDijkstraAlgortihm);


        auto AlgorithmsButtonLayer3 = new QHBoxLayout;
        AlgorithmsButtonBox->addLayout(AlgorithmsButtonLayer3);

            auto ButtonKruskalAlgortihm = new QPushButton{tr("Algorithme de KRUSKAL")};
            ButtonKruskalAlgortihm->setMinimumHeight(40);
            AlgorithmsButtonLayer3->addWidget(ButtonKruskalAlgortihm);

            auto ButtonPruferAlgortihm = new QPushButton{tr("Algorithme de PRÜFER")};
            ButtonPruferAlgortihm->setMinimumHeight(40);
            AlgorithmsButtonLayer3->addWidget(ButtonPruferAlgortihm);

    auto LigneH2 = new QFrame{};
    LigneH2->setFrameStyle(QFrame::HLine | QFrame :: Sunken);
    mainBox->addWidget(LigneH2);

    auto ButtonAddMatrix = new QPushButton{tr("Ajouter une matrice de coûts")};
    ButtonAddMatrix->setMinimumHeight(30);
    QFont font;
    ButtonAddMatrix->setFont(font);
    ButtonAddMatrix->setStyleSheet("color: Green");
    mainBox->addWidget(ButtonAddMatrix);
    connect(ButtonAddMatrix, &QPushButton::clicked, this, &MainWindow::saveSuccessorEntries);
    connect(ButtonAddMatrix, &QPushButton::clicked, this, &MainWindow::click_ButtonAddMatrix);

    auto LigneH4 = new QFrame{};
    LigneH4->setFrameStyle(QFrame::HLine | QFrame :: Sunken);
    mainBox->addWidget(LigneH4);

    auto HelpLabel = new QLabel{tr("Saisissez le nombre de successeurs de chaque sommet respectif : \n [Exemple] : 4,8,9,14")};
    mainBox->addWidget(HelpLabel, 0, Qt::AlignCenter);
    HelpLabel->setMinimumHeight(50);

    auto LigneH3 = new QFrame{};
    LigneH3->setFrameStyle(QFrame::HLine | QFrame :: Sunken);
    mainBox->addWidget(LigneH3);

    auto FormBox = new QFormLayout;
    mainBox->addLayout(FormBox);
    for(int i=1; i<=NA; i++)
    {
        auto NodesAmountLabel = new QLabel{"SOMMET N° " + QString::number(i)};
        FormBox->addRow(NodesAmountLabel, SuccessorEntries[i]);
    }

    //MENU
    auto MenuCancel = menuBar()->addAction("Retour");
    connect(MenuCancel,&QAction::triggered,this,&MainWindow::click_MenuCancel);
}

void MainWindow::click_MenuCancel()
{
    createWindow_DirectedGraph();
}

void MainWindow::NodesAmountValueChanged(int value)
{
    saveSuccessorEntries();
    NodesAmountValue = value;
}

void MainWindow::click_ButtonRankAlgorithm()
{
    /* Algorithme du rang */

    graph g = genGraph();
    int *rang;
    algorithms algo = algorithms(g);
    algo.rang(rang);

    string s = "";
    for (int i=1; i<=aps[0]; i++)
        s+= "rang(" + to_string(i) + ") = " + to_string(rang[i]) + "\n";

    QMessageBox{QMessageBox::Information, "Algorithme du rang",QString::fromStdString(s), QMessageBox::Ok}.exec();
}

void MainWindow::click_ButtonTarjanAlgorithm()
{
    /* Algorithme de Tarjan */

    graph g = genGraph();
    int *cfc;
    int *prem;
    algorithms algo = algorithms(g);
    algo.fortConnexe(prem, cfc);

    string s = "";
    for (int i=1; i<=aps[0]; i++)
        s+= "CFC(" + to_string(i) + ") = " + to_string(cfc[i]) + "\n";
    s+= "\n";
    for (int i=1; i<=prem[0]; i++)
        s+= "Prem(" + to_string(i) + ") = " + to_string(prem[i]) + "\n";

    //graph g1 = algo.graph_reduit(prem, cfc);
    //afficher graph g1

    QMessageBox{QMessageBox::Information, "Algorithme de Tarjan",QString::fromStdString(s), QMessageBox::Ok}.exec();
}

void MainWindow::click_ButtonAddMatrix()
{
    if(NodesAmountValue > 0)
    {
        setWindowTitle("Saisie de la matrice des coûts");
        setMinimumSize(80+NodesAmountValue*50,NodesAmountValue*50);
        setMaximumSize(80+NodesAmountValue*50,NodesAmountValue*50);

        menuBar()->clear();
        auto MainWidget= new QWidget;
        setCentralWidget(MainWidget);

        auto mainBox = new QHBoxLayout;
        MainWidget->setLayout(mainBox);

        auto matrixBox = new QGridLayout;
        mainBox->addLayout(matrixBox);

        for (int i = 1; i <= NodesAmountValue; ++i)
        {
            auto tmpLabel= new QLabel{QString::number(i)};
            tmpLabel->setStyleSheet("color: blue;");
            matrixBox->addWidget(tmpLabel, 0, i, Qt::AlignCenter);
        }
        for (int i = 1; i <= NodesAmountValue; ++i)
        {
            auto tmpLabel= new QLabel{QString::number(i)};
            tmpLabel->setStyleSheet("color: blue;");
            matrixBox->addWidget(tmpLabel, i, 0, Qt::AlignCenter);
        }

        TaskCostEntries.resize(NodesAmountValue+1);

        for (int i = 1; i <= NodesAmountValue; ++i)
        {
            TaskCostEntries[i].resize(NodesAmountValue+1);

            for (int j = 1; j <= NodesAmountValue; ++j)
            {
                QLineEdit *lineEdit = new QLineEdit;
                TaskCostEntries[i][j] = lineEdit;
                lineEdit->setFixedSize(35, 35);
                lineEdit->setAlignment(Qt::AlignCenter);
                matrixBox->addWidget(lineEdit, i, j);
            }
        }

        auto LigneH1 = new QFrame{};
        LigneH1->setFrameStyle(QFrame::VLine | QFrame :: Sunken);
        mainBox->addWidget(LigneH1);

        auto ButtonsBox = new QVBoxLayout;
        mainBox->addLayout(ButtonsBox);

            auto ButtonSave = new QPushButton{tr("Enregistrer")};
            ButtonsBox->addWidget(ButtonSave);
            ButtonSave->setMinimumHeight(NodesAmountValue*50/2 - 8);
            connect(ButtonSave, &QPushButton::clicked, this,&MainWindow::SaveTaskCostEntries);

            auto ButtonCancel = new QPushButton{tr("Annuler")};
            ButtonsBox->addWidget(ButtonCancel);
            ButtonCancel->setMinimumHeight(NodesAmountValue*50/2 - 8);
            connect(ButtonCancel, &QPushButton::clicked, this, [this]{ MainWindow::createWindow_KeyobardEnterD(NodesAmountValue);});

            if(!TaskCostValuesEmpty)
            {
                auto ButtonDelete = new QPushButton{tr("Supprimer")};
                ButtonsBox->addWidget(ButtonDelete);
                ButtonSave->setMinimumHeight(NodesAmountValue*50/3 - 8);
                ButtonCancel->setMinimumHeight(NodesAmountValue*50/3 - 8);
                ButtonDelete->setMinimumHeight(NodesAmountValue*50/3 - 8);
                connect(ButtonDelete, &QPushButton::clicked, this,&MainWindow::DeleteTaskCostEntries);
            }
    }
    else
    {
        QMessageBox{QMessageBox::Warning, "Nombre de sommets vide","Votre nombre de sommets est vide.", QMessageBox::Ok}.exec();
        return;
    }
}

void MainWindow::SaveTaskCostEntries()
{
    TaskCostValues.resize(NodesAmountValue+1);
    for (int i = 1; i <= NodesAmountValue; ++i)
    {
        TaskCostValues[i].resize(NodesAmountValue+1);
        for (int j = 1; j <= NodesAmountValue; ++j)
        {
            stringstream ss(TaskCostEntries[i][j]->text().toStdString());
            int number;
            if(ss >> number)
                TaskCostValues[i][j] = TaskCostEntries[i][j]->text().toInt();
            else
                TaskCostValues[i][j] = 9999;
        }
    }
    QMessageBox{QMessageBox::Information,"Matrice des coûts","La matrice des coûts a été enregistrée avec succès.", QMessageBox::Ok}.exec();
    createWindow_KeyobardEnterD(NodesAmountValue);
    TaskCostValuesEmpty = false;
}

void MainWindow::DeleteTaskCostEntries()
{
    for(int i=0; i<TaskCostEntries.size(); i++)
        TaskCostEntries[i].clear();
    TaskCostEntries.clear();
    TaskCostValuesEmpty = true;
    QMessageBox{QMessageBox::Information,"Matrice des coûts","La matrice des coûts a été supprimée.", QMessageBox::Ok}.exec();
    click_ButtonAddMatrix();
}

void MainWindow::saveSuccessorEntries()
{
    SuccessorEntriesValues.clear();
    SuccessorEntriesValues.push_back("#");
    for(int i=1; i<SuccessorEntries.size(); i++)
    {
        SuccessorEntriesValues.push_back(SuccessorEntries[i]->text().toStdString());
    }
}



//----------------- FIN FENETRE SAISIE CLAVIER GRAPHE ORIENTE----------------------//


//----------------- DEBUT FENETRE SAISIE FICHIER GRAPHE ORIENTE----------------------//

void MainWindow::createWindow_FileEnterD()
{
    QFileDialog F(nullptr, "Choisir un fichier");
    F.setNameFilter("Fichiers texte (*.txt)");


    if (F.exec() == QDialog::Accepted)
    {
        QFile f(F.selectedFiles().first());
        if(!f.exists())
        {
            QMessageBox{QMessageBox::Warning, "Fichier inconnu","Le fichier n'existe pas.", QMessageBox::Ok}.exec();
            return;
        }
        if(!f.fileName().endsWith(".txt"))
        {
            QMessageBox{QMessageBox::Warning, "Fichier incompatible","Veuillez choisir un fichier .txt", QMessageBox::Ok}.exec();
            return;
        }
        string str1, str2;
        ifstream inputFile(f.fileName().toStdString());

        if (inputFile.is_open())
        {
            getline(inputFile, str1);
            getline(inputFile, str2);
        }
        replace(str1.begin(), str1.end(), ' ', ',');
        replace(str2.begin(), str2.end(), ' ', ',');

        stringstream ss1(str1);
        aps.clear();

        int number;
        char separateur;
        while(!ss1.eof())
        {
            if(!(ss1 >> number))
            {
                aps.clear();
                QMessageBox{QMessageBox::Warning, "Fichier corrompu","Votre fichier n'est pas adapté à la création d'un graphe.", QMessageBox::Ok}.exec();
                return;
            }
            ss1 >> separateur;
            aps.push_back(number);
        }

        stringstream ss2(str2);
        fs.clear();

        while(!ss2.eof())
        {
            if(!(ss2 >> number))
            {
                fs.clear();
                QMessageBox{QMessageBox::Warning, "Fichier corrompu bite","Votre fichier n'est pas adapté à la création d'un graphe.", QMessageBox::Ok}.exec();
                return;
            }
            ss2 >> separateur;
            fs.push_back(number);
        }
            choosenFileName = f.fileName().toStdString();
            createWindow_ChooseAlgorithm();
    }
}

void MainWindow::createWindow_ChooseAlgorithm()
{
    resize(650,300);
    setMinimumSize(650,300);
    setMaximumSize(650,300);
    setWindowTitle(tr("Choix de l'algorithme"));

    //NETTOYAGE SINON ANCIENS BOUTONS REVIENNENT
    menuBar()->clear();
    auto MainWidget= new QWidget;
    setCentralWidget(MainWidget);

    auto mainBox = new QVBoxLayout;
    MainWidget->setLayout(mainBox);

    auto choosenFileLabel= new QLabel{"Fichier séléctionné : "};
    choosenFileLabel->setFixedHeight(10);
    mainBox->addWidget(choosenFileLabel, 0, Qt::AlignCenter);

    auto choosenFileLabelLink= new QLabel{QString::fromStdString(choosenFileName)};
    choosenFileLabelLink->setStyleSheet("color: blue;");
    QFont font;
    font.setItalic(true);
    choosenFileLabelLink->setFont(font);
    choosenFileLabelLink->setFixedHeight(20);
    mainBox->addWidget(choosenFileLabelLink, 0, Qt::AlignCenter);

    auto LigneH1 = new QFrame{};
    LigneH1->setFrameStyle(QFrame::HLine | QFrame :: Sunken);
    mainBox->addWidget(LigneH1);

    auto infoLabel= new QLabel{tr("Choisissez l'algorithme à utiliser")};
    mainBox->addWidget(infoLabel, 0, Qt::AlignCenter);

    //MENU
    auto MenuCancel = menuBar()->addAction("Retour");
    connect(MenuCancel,&QAction::triggered,this,&MainWindow::click_MenuCancel);

    auto AlgorithmsButtonBox = new QVBoxLayout;
    mainBox->addLayout(AlgorithmsButtonBox);


        auto AlgorithmsButtonLayer1 = new QHBoxLayout;
        AlgorithmsButtonBox->addLayout(AlgorithmsButtonLayer1);

            auto ButtonRankAlgortihm = new QPushButton{tr("Algorithme du RANG")};
            ButtonRankAlgortihm->setMinimumHeight(40);
            AlgorithmsButtonLayer1->addWidget(ButtonRankAlgortihm);
            connect(ButtonRankAlgortihm, &QPushButton::clicked, this, &MainWindow::click_ButtonRankAlgorithm);

            auto ButtonTarjanAlgortihm = new QPushButton{tr("Algorithme de TARJAN")};
            ButtonTarjanAlgortihm->setMinimumHeight(40);
            AlgorithmsButtonLayer1->addWidget(ButtonTarjanAlgortihm);
            connect(ButtonTarjanAlgortihm, &QPushButton::clicked, this, &MainWindow::click_ButtonTarjanAlgorithm);


        auto AlgorithmsButtonLayer2 = new QHBoxLayout;
        AlgorithmsButtonBox->addLayout(AlgorithmsButtonLayer2);

            auto ButtonSchedulingAlgortihm = new QPushButton{tr("Algorithme de l'ORDONNANCEMENT")};
            ButtonSchedulingAlgortihm->setMinimumHeight(40);
            AlgorithmsButtonLayer2->addWidget(ButtonSchedulingAlgortihm);

            auto ButtonDantzigAlgortihm = new QPushButton{tr("Algorithme de DANTZIG")};
            ButtonDantzigAlgortihm->setMinimumHeight(40);
            AlgorithmsButtonLayer2->addWidget(ButtonDantzigAlgortihm);
            connect(ButtonDantzigAlgortihm, &QPushButton::clicked, this, &MainWindow::Check_TaskCost);

            auto ButtonDijkstraAlgortihm = new QPushButton{tr("Algorithme de DIJKSTRA")};
            ButtonDijkstraAlgortihm->setMinimumHeight(40);
            AlgorithmsButtonLayer2->addWidget(ButtonDijkstraAlgortihm);
            connect(ButtonDijkstraAlgortihm, &QPushButton::clicked, this, &MainWindow::Check_TaskCost);


        auto AlgorithmsButtonLayer3 = new QHBoxLayout;
        AlgorithmsButtonBox->addLayout(AlgorithmsButtonLayer3);

            auto ButtonKruskalAlgortihm = new QPushButton{tr("Algorithme de KRUSKAL")};
            ButtonKruskalAlgortihm->setMinimumHeight(40);
            AlgorithmsButtonLayer3->addWidget(ButtonKruskalAlgortihm);

            auto ButtonPruferAlgortihm = new QPushButton{tr("Algorithme de PRÜFER")};
            ButtonPruferAlgortihm->setMinimumHeight(40);
            AlgorithmsButtonLayer3->addWidget(ButtonPruferAlgortihm);



        //rang, tarjan, ordonneancembnt, dantzig, dikstra, kruskhal, prufer
}

void MainWindow::Check_TaskCost()
{
    ifstream inputFile(choosenFileName);
    string str3;
    if (inputFile.is_open())
        for(int i=0; i<2; i++)
            getline(inputFile, str3);

    vector<vector<int>> TaskCost(fs[0],vector<int>(fs[0], 0));

    for(int i=0; i<fs[0]; i++)
    {
        replace(str3.begin(), str3.end(), ' ', ',');
        getline(inputFile, str3);


        stringstream ssTMP(str3);
        vector<int> T;
        int number=0;
        char separateur;
        while(!ssTMP.eof())
        {
            if(!(ssTMP >> number))
            {
                QMessageBox{QMessageBox::Warning, "Fichier incompatible","La matrice des coûts du fichier séléctionné n'est pas correctement écrite ou n'existe pas.", QMessageBox::Ok}.exec();
                TaskCost.clear();
                return;
            }
            T.push_back(number);
            ssTMP >> separateur;
        }

        if(static_cast<int>(T.size()) != fs[0])
        {
            QMessageBox{QMessageBox::Warning, "Fichier incompatible","La matrice des coûts du fichier séléctionné n'est pas adaptée au graphe du même fichier ou n'existe pas", QMessageBox::Ok}.exec();
            TaskCost.clear();
            return;
        }

        if(inputFile.eof() && i != fs[0]-1)
        {
            QMessageBox{QMessageBox::Warning, "Fichier incompatible","La matrice des coûts du fichier séléctionné n'est pas complète.", QMessageBox::Ok}.exec();
            TaskCost.clear();
            return;
        }

        TaskCost[i] = T;
    }

    for(int i=0; i< TaskCost.size(); i++)
        qInfo() << TaskCost[i];
}



//----------------- FIN FENETRE SAISIE FICHIER GRAPHE ORIENTE----------------------//










void MainWindow::afficheFs()
{
    for(int i=0; i<static_cast<int>(fs.size());i++)
    {
        qInfo() << fs[i];
    }
}

void MainWindow::afficheAps()
{
    for(int i=0; i<static_cast<int>(aps.size());i++)
    {
        qInfo() << aps[i];
    }
}

vector<int> MainWindow::getFs()
{
    return fs;
}

vector<int> MainWindow::getAps()
{
    return aps;
}

vector<vector<int>> MainWindow::getTaskCostMatrix()
{
    return TaskCostValues;
}
