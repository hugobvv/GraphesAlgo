#include "mainwindow.h"
#include "graph.h"
#include "algorithms.h"
#include "graphWithCosts.h"
#include "scheduling.h"


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
    connect(DirectedGraphButton, &QPushButton::clicked, this, &MainWindow::createWindow_DirectedGraph);

    auto UndirectedGraphButton = new QPushButton{tr("Créer un graphe NON-ORIENTÉ")};
    UndirectedGraphButton->setMinimumHeight(60);
    mainBox->addWidget(UndirectedGraphButton);
    connect(UndirectedGraphButton, &QPushButton::clicked, this, &MainWindow::createWindow_UndirectedGraph);

    auto ExitButton = new QPushButton{tr("QUITTER")};
    ExitButton->setMinimumHeight(40);
    mainBox->addWidget(ExitButton);
    connect(ExitButton, &QPushButton::clicked, this, &MainWindow::close);
}

//----------------- FIN FENETRE PRINCIPALE----------------------//




//----------------- DEBUT FENETRE GRAPHE ORIENTE----------------------//

void MainWindow::createWindow_DirectedGraph()
{
    oriented = true;
    resize(400,220);
    setMinimumSize(400,220);
    setMaximumSize(400,220);
    setWindowTitle(tr("Création d'un Graphe Orienté"));

    //NETTOYAGE SINON ANCIENS BOUTONS REVIENNENT
    menuBar()->clear();
    auto MainWidget= new QWidget;
    setCentralWidget(MainWidget);
    graphClear();

    //CREATION DE BOX ET BOUTONS
    auto mainBox = new QVBoxLayout;
    MainWidget->setLayout(mainBox);

    auto KeyboardEnter = new QPushButton{tr("Saisir le graphe AU CLAVIER")};
    KeyboardEnter->setMinimumHeight(80);
    mainBox->addWidget(KeyboardEnter);
    connect(KeyboardEnter, &QPushButton::clicked, this, &MainWindow::click_KeyboardEnterD);

    auto FileEnter = new QPushButton{tr("Saisir le graphe avec FICHIER")};
    FileEnter->setMinimumHeight(80);
    mainBox->addWidget(FileEnter);
    connect(FileEnter, &QPushButton::clicked, this, &MainWindow::createWindow_FileEnter);

    //MENU
    auto MenuTotalCancel = menuBar()->addAction("Retour au menu principal");
    connect(MenuTotalCancel,&QAction::triggered,this,&MainWindow::createMainWindow);
}

void MainWindow::click_KeyboardEnterD()
{
    createWindow_KeyboardEnterD(0);
}

graph MainWindow::genGraphD()
{
    /* Génère le graphe courant */
    infos.push_back("null");
    for(int i=0; i<SuccessorEntries.size()-1; i++)
    {
        infos.push_back(to_string(i+1));
    }

    if (file) //si le graphe a été généré par fichier
        return graph(fs,aps,infos);



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
                    return graph();
                }
                if(number == 0 || !(isdigit(V[V.length() - 1])))
                {
                    QMessageBox{QMessageBox::Warning, "Erreur de saisie","Un sommet 0 a été saisi ou la saisie a mal été effectuée, vérifiez vos valeurs.\nSi un sommet n'a pas de successeurs, laissez la case vide.", QMessageBox::Ok}.exec();
                    return graph();
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
        if(fs[i] == 0 && i+1 < static_cast<int>(fs.size()))
            aps.push_back(i+1);
    aps[0] = aps.size()-1;

    saveSuccessorEntries();
    return graph(fs,aps,infos);
}

graph MainWindow::genGraphU()
{
    /* Génère le graphe courant (non-orienté)*/

    if (choosenFileName != "") //si le graphe a été généré par fichier
        return graph(fs,aps,infos);

    fs.clear();
    fs.push_back(0); // initialiser la premiere case à 0 (nbr de cases du tableau fs)
    aps.clear();
    aps.push_back(0);

    Connections.clear();
    Connections.resize(2);

    for(int i=1; i<static_cast<int>(SuccessorEntries.size());i++)
    {
        string V = SuccessorEntries[i]->text().toStdString();
        replace(V.begin(), V.end(), ' ', '-');

        if(!(V.length() == 0))
        {
            stringstream ss(V);
            while (ss.good())
            {

                int number1{999}, number2{999};
                char delimiter;
                ss >> number1 >> delimiter >> number2;

                if(number1 == 999 || number2 == 999)
                {
                    QMessageBox{QMessageBox::Warning, "Erreur de saisie","Erreur de syntaxe dans la saisie", QMessageBox::Ok}.exec();
                    return graph();
                }
                if(number1 > static_cast<int>(SuccessorEntries.size())+1 || number1 < 0)
                {
                    QMessageBox{QMessageBox::Warning, "Erreur de saisie","Le sommet " + QString::fromStdString(to_string(number1)) + " n'existe pas." ,QMessageBox::Ok}.exec();
                    return graph();
                }
                if(number2 > static_cast<int>(SuccessorEntries.size())+1 || number2 < 0)
                {
                    QMessageBox{QMessageBox::Warning, "Erreur de saisie","Le sommet " + QString::fromStdString(to_string(number2)) + " n'existe pas." ,QMessageBox::Ok}.exec();
                    return graph();
                }
                if(number1 == 0 || number2 == 0 || !(isdigit(V[V.length() - 1])))
                {
                    QMessageBox{QMessageBox::Warning, "Erreur de saisie","Un sommet 0 a été saisi ou la saisie a mal été effectuée, vérifiez vos valeurs.", QMessageBox::Ok}.exec();
                    return graph();
                }
                Connections[0].push_back(number1);
                Connections[1].push_back(number2);
            }
        }
    }

    /* Remplir fs et aps */
    int max = NodesAmountValueUnoriented(); //trouver le nb de sommets
    for (int s=1; s<=max; ++s) //pour chaque sommet
    {
        aps.push_back(fs.size());
        for (int i=0; i<Connections[0].size(); ++i)
            if(Connections[0][i] == s)
                fs.push_back(Connections[1][i]); //ajouter successeurs

        for (int i=0; i<Connections[1].size(); ++i)
            if(Connections[1][i] == s)
                fs.push_back(Connections[0][i]);
        fs.push_back(0);
    }
    aps[0] = aps.size()-1;
    fs[0] = fs.size()-1;
    NodesAmountValue = max;
    return graph(fs,aps,infos);
}

void MainWindow::graphClear()
{
    fs.clear();
    aps.clear();
    infos.clear();
    choosenFileName = "";
    for(int i=0; i<TaskCostEntries.size(); i++)
        TaskCostEntries[i].clear();
    for(int i=0; i<TaskCostValues.size(); i++)
        TaskCostValues[i].clear();
    TaskCostEntries.clear();
    TaskCostValues.clear();
    TaskCostValuesEmpty = true;
}

//----------------- FIN FENETRE GRAPHE ORIENTE----------------------//


//----------------- DEBUT FENETRE GRAPHE NON-ORIENTE----------------------//

void MainWindow::createWindow_UndirectedGraph()
{
    oriented = false;
    resize(400,220);
    setMinimumSize(400,220);
    setMaximumSize(400,220);
    setWindowTitle(tr("Création d'un Graphe Non-Orienté"));

    //NETTOYAGE SINON ANCIENS BOUTONS REVIENNENT
    menuBar()->clear();
    auto MainWidget= new QWidget;
    setCentralWidget(MainWidget);
    graphClear();

    //CREATION DE BOX ET BOUTONS
    auto mainBox = new QVBoxLayout;
    MainWidget->setLayout(mainBox);

    auto KeyboardEnter = new QPushButton{tr("Saisir le graphe AU CLAVIER")};
    KeyboardEnter->setMinimumHeight(80);
    mainBox->addWidget(KeyboardEnter);
    connect(KeyboardEnter, &QPushButton::clicked, this, &MainWindow::createWindow_KeyboardEnterU);

    auto FileEnter = new QPushButton{tr("Saisir le graphe avec FICHIER")};
    FileEnter->setMinimumHeight(80);
    mainBox->addWidget(FileEnter);
    connect(FileEnter, &QPushButton::clicked, this, &MainWindow::createWindow_FileEnter);

    //MENU
    auto MenuTotalCancel = menuBar()->addAction("Retour au menu principal");
    connect(MenuTotalCancel,&QAction::triggered,this,&MainWindow::createMainWindow);
}

void MainWindow::createWindow_KeyboardEnterU(int NC)
{
    file = false;
    choosenFileName = "";
    SuccessorEntries.resize(NC+1);
    for(int i=1; i<static_cast<int>(SuccessorEntries.size());i++)
        SuccessorEntries[i]= new QLineEdit{""};

    SuccessorEntriesValues.resize(NC+1);
    for(int i=1; i<static_cast<int>(SuccessorEntriesValues.size()); i++)
        SuccessorEntries[i]->setText(QString::fromStdString(SuccessorEntriesValues[i]));

    setMinimumSize(650,380+(NC/2.0)*53);
    setMaximumSize(650,380+(NC/2.0)*53);
    setWindowTitle(tr("Saisie au clavier d'un graphe non-orienté"));

        //NETTOYAGE SINON ANCIENS BOUTONS REVIENNENT
            menuBar()->clear();
            auto MainWidget= new QWidget;
            setCentralWidget(MainWidget);

            //CREATION DE BOX ET BOUTONS
            auto mainBox = new QVBoxLayout;
            MainWidget->setLayout(mainBox);

            auto NodesAmountChoiceBox = new QHBoxLayout;
                mainBox->addLayout(NodesAmountChoiceBox);
                    auto NodesAmountLabel = new QLabel{tr("                 NOMBRE D'ARÊTES DU GRAPHE : ")};
                    NodesAmountChoiceBox->addWidget(NodesAmountLabel, 0, Qt::AlignCenter);
                    NodesAmountLabel->setMinimumHeight(50);
                    auto NodesAmountSpinBox = new QSpinBox{};
                    NodesAmountSpinBox->setValue(NC);
                    NodesAmountSpinBox->setMinimum(0);
                    NodesAmountSpinBox->setMaximum(20);
                    NodesAmountSpinBox->setMinimumWidth(50);
                    NodesAmountSpinBox->setMinimumHeight(30);
                    NodesAmountChoiceBox->addWidget(NodesAmountSpinBox, 0, Qt::AlignCenter);

                    connect(NodesAmountSpinBox, &QSpinBox::valueChanged, this, &MainWindow::NodesAmountValueChanged);
                    connect(NodesAmountSpinBox, &QSpinBox::valueChanged, this, [this]{ MainWindow::createWindow_KeyboardEnterU(EdgesNumber);});

            addAlgorithmButtons(mainBox);
            addExtraBox(mainBox);

            auto HelpLabel = new QLabel{tr("Saisissez les arêtes du graphe : \n [Exemple] : 1 - 6    OU    2 - 5")};
                mainBox->addWidget(HelpLabel, 0, Qt::AlignCenter);
                HelpLabel->setMinimumHeight(50);

            auto LigneH3 = new QFrame{};
                LigneH3->setFrameStyle(QFrame::HLine | QFrame :: Sunken);
                mainBox->addWidget(LigneH3);

            auto FormBox = new QFormLayout;
                mainBox->addLayout(FormBox);
                for(int i=1; i<=NC; i++)
                {
                    auto NodesAmountLabel = new QLabel{"ARÊTE N° " + QString::number(i)};
                    FormBox->addRow(NodesAmountLabel, SuccessorEntries[i]);
                }

            //MENU
            auto MenuCancel = menuBar()->addAction("Retour");
            auto MenuSave = menuBar()->addAction("Enregistrer");
            connect(MenuCancel,&QAction::triggered,this,&MainWindow::createWindow_UndirectedGraph);
            connect(MenuSave,&QAction::triggered,this,&MainWindow::saveGraphToFile);
}

int MainWindow::NodesAmountValueUnoriented() const
{
    /* Retourne le max des sommets des arêtes pour trouver le nb de sommets */
    int max = -1;
    for (int i=0; i<Connections.size(); i++)
        for (int j=0; j<Connections[i].size(); j++)
            if (Connections[i][j]>max)
                max = Connections[i][j];
    return max;
}

//----------------- FIN FENETRE GRAPHE NON-ORIENTE----------------------//


//----------------- DEBUT FENETRE SAISIE CLAVIER GRAPHE ORIENTE----------------------//

void MainWindow::addAlgorithmButtons(QVBoxLayout *mainBox)
{
    /* Créer les boutons pour choisir les algorithmes */

    auto LigneH = new QFrame{};
    LigneH->setFrameStyle(QFrame::HLine | QFrame :: Sunken);
    mainBox->addWidget(LigneH);

    auto AlgorithmsButtonBox = new QVBoxLayout;
    mainBox->addLayout(AlgorithmsButtonBox);

        auto AlgorithmsButtonLayer1 = new QHBoxLayout;
        AlgorithmsButtonBox->addLayout(AlgorithmsButtonLayer1);

            auto ButtonRankAlgorithm = new QPushButton{tr("Algorithme du RANG")};
            ButtonRankAlgorithm->setMinimumHeight(40);
            AlgorithmsButtonLayer1->addWidget(ButtonRankAlgorithm);
            connect(ButtonRankAlgorithm, &QPushButton::clicked, this, &MainWindow::RankAlgorithm);

            auto ButtonTarjanAlgorithm = new QPushButton{tr("Algorithme de TARJAN")};
            ButtonTarjanAlgorithm->setMinimumHeight(40);
            AlgorithmsButtonLayer1->addWidget(ButtonTarjanAlgorithm);
            connect(ButtonTarjanAlgorithm, &QPushButton::clicked, this, &MainWindow::TarjanAlgorithm);

            auto ButtonGraphTarjan = new QPushButton{tr("Graphe réduit selon TARJAN")};
            ButtonGraphTarjan->setMinimumHeight(40);
            AlgorithmsButtonLayer1->addWidget(ButtonGraphTarjan);
            connect(ButtonGraphTarjan, &QPushButton::clicked, this, &MainWindow::showTarjanGraph);

        auto AlgorithmsButtonLayer2 = new QHBoxLayout;
        AlgorithmsButtonBox->addLayout(AlgorithmsButtonLayer2);

            auto ButtonSchedulingAlgorithm = new QPushButton{tr("Algorithme de l'ORDONNANCEMENT")};
            ButtonSchedulingAlgorithm->setMinimumHeight(40);
            AlgorithmsButtonLayer2->addWidget(ButtonSchedulingAlgorithm);
            connect(ButtonSchedulingAlgorithm, &QPushButton::clicked, this, &MainWindow::SchedulingAlgorithm);

            auto ButtonDantzigAlgorithm = new QPushButton{tr("Algorithme de DANTZIG")};
            ButtonDantzigAlgorithm->setMinimumHeight(40);
            AlgorithmsButtonLayer2->addWidget(ButtonDantzigAlgorithm);
            connect(ButtonDantzigAlgorithm, &QPushButton::clicked, this, &MainWindow::DantzigAlgorithm);

            auto ButtonDijkstraAlgorithm = new QPushButton{tr("Algorithme de DIJKSTRA")};
            ButtonDijkstraAlgorithm->setMinimumHeight(40);
            AlgorithmsButtonLayer2->addWidget(ButtonDijkstraAlgorithm);
            connect(ButtonDijkstraAlgorithm, &QPushButton::clicked, this, &MainWindow::DikjstraAlgorithm);

        auto AlgorithmsButtonLayer3 = new QHBoxLayout;
        AlgorithmsButtonBox->addLayout(AlgorithmsButtonLayer3);

            auto ButtonKruskalAlgorithm = new QPushButton{tr("Algorithme de KRUSKAL")};
            ButtonKruskalAlgorithm->setMinimumHeight(40);
            AlgorithmsButtonLayer3->addWidget(ButtonKruskalAlgorithm);
            connect(ButtonKruskalAlgorithm, &QPushButton::clicked, this, &MainWindow::KruskalAlgorithm);

            auto ButtonPruferAlgorithm = new QPushButton{tr("Algorithme de PRÜFER")};
            ButtonPruferAlgorithm->setMinimumHeight(40);
            AlgorithmsButtonLayer3->addWidget(ButtonPruferAlgorithm);
            connect(ButtonPruferAlgorithm, &QPushButton::clicked, this, &MainWindow::PruferAlgorithm);

            auto ButtonInformations = new QPushButton{tr("INFORMATIONS")};
            ButtonInformations->setMinimumHeight(40);
            AlgorithmsButtonLayer3->addWidget(ButtonInformations);
            connect(ButtonInformations, &QPushButton::clicked, this, &MainWindow::AlgorithmsInformation);

    auto LigneH1 = new QFrame{};
    LigneH1->setFrameStyle(QFrame::HLine | QFrame :: Sunken);
    mainBox->addWidget(LigneH);

    auto showGraphBox = new QHBoxLayout;
    mainBox->addLayout(showGraphBox);

        auto ButtonShowGraph = new QPushButton{tr("AFFICHER LE GRAPHE")};
        ButtonShowGraph->setMinimumHeight(30);
        QFont font;
        ButtonShowGraph->setFont(font);
        ButtonShowGraph->setStyleSheet("color: Blue");
        showGraphBox->addWidget(ButtonShowGraph);
        connect(ButtonShowGraph, &QPushButton::clicked, this, &MainWindow::showCurrentGraph);
}

void MainWindow::addExtraBox(QVBoxLayout *mainBox)
{
    auto LigneH1 = new QFrame{};
    LigneH1->setFrameStyle(QFrame::HLine | QFrame :: Sunken);
    mainBox->addWidget(LigneH1);

    auto box = new QHBoxLayout{};
    mainBox->addLayout(box);

    auto ButtonAddMatrix = new QPushButton{tr("Ajouter une matrice de coûts")};
    ButtonAddMatrix->setMinimumHeight(40);
    QFont font;
    ButtonAddMatrix->setFont(font);
    ButtonAddMatrix->setStyleSheet("color: Green");
    box->addWidget(ButtonAddMatrix);
    connect(ButtonAddMatrix, &QPushButton::clicked, this, &MainWindow::saveSuccessorEntries);
    connect(ButtonAddMatrix, &QPushButton::clicked, this, &MainWindow::click_ButtonAddMatrix);

    auto ButtonAddDurations = new QPushButton{tr("Ajouter une durée aux tâches")};
    ButtonAddDurations->setMinimumHeight(40);
    ButtonAddDurations->setFont(font);
    ButtonAddDurations->setStyleSheet("color: Green");
    box->addWidget(ButtonAddDurations);
    connect(ButtonAddDurations, &QPushButton::clicked, this, &MainWindow::saveSuccessorEntries);
    connect(ButtonAddDurations, &QPushButton::clicked, this, &MainWindow::click_ButtonDuration);

    auto ButtonRename = new QPushButton{tr("Renommer les sommets")};
    ButtonRename->setMinimumHeight(40);
    ButtonRename->setFont(font);
    ButtonRename->setStyleSheet("color: Green");
    box->addWidget(ButtonRename);
    connect(ButtonRename, &QPushButton::clicked, this, &MainWindow::saveSuccessorEntries);
    connect(ButtonRename, &QPushButton::clicked, this, &MainWindow::click_ButtonRename);

    auto LigneH2 = new QFrame{};
    LigneH2->setFrameStyle(QFrame::HLine | QFrame :: Sunken);
    mainBox->addWidget(LigneH2);
}

void MainWindow::createWindow_KeyboardEnterD(int NA)
{
    file = false;
    choosenFileName = "";
    SuccessorEntries.resize(NA+1);
    for(int i=1; i<static_cast<int>(SuccessorEntries.size());i++)
        SuccessorEntries[i]= new QLineEdit{""};

    SuccessorEntriesValues.resize(NA+1);
    for(int i=1; i<static_cast<int>(SuccessorEntriesValues.size()); i++)
        SuccessorEntries[i]->setText(QString::fromStdString(SuccessorEntriesValues[i]));

    setMinimumSize(650,380+(NA/2.0)*53);
    setMaximumSize(650,380+(NA/2.0)*53);
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
        connect(NodesAmountSpinBox, &QSpinBox::valueChanged, this, [this]{ MainWindow::createWindow_KeyboardEnterD(NodesAmountValue);});

    addAlgorithmButtons(mainBox);
    addExtraBox(mainBox);

    auto HelpLabel = new QLabel{tr("Saisissez les successeurs de chaque sommet respectif : \n [Exemple] : 4,8,9,14")};
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
    auto MenuSave = menuBar()->addAction("Enregistrer");
    connect(MenuCancel,&QAction::triggered,this,&MainWindow::createWindow_DirectedGraph);
    connect(MenuSave,&QAction::triggered,this,&MainWindow::saveGraphToFile);
}

void MainWindow::NodesAmountValueChanged(int value)
{
    saveSuccessorEntries();
    if (oriented)
        NodesAmountValue = value;
    else
        EdgesNumber = value;
}

void MainWindow::click_ButtonAddMatrix()
{
    oriented ? genGraphD() : genGraphU();

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
                if (!EdgeExist(i,j)) //si l'arête n'existe pas
                {
                    lineEdit->setText("∞");
                    lineEdit->setReadOnly(true);
                }
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
            if (oriented)
                connect(ButtonCancel, &QPushButton::clicked, this, [this]{ MainWindow::createWindow_KeyboardEnterD(NodesAmountValue);});
            else
                connect(ButtonCancel, &QPushButton::clicked, this, [this]{ MainWindow::createWindow_KeyboardEnterU(EdgesNumber);});
    }
    else
    {
        QMessageBox{QMessageBox::Warning, "Nombre de sommets vide","Votre nombre de sommets est vide.", QMessageBox::Ok}.exec();
        return;
    }
}

bool MainWindow::EdgeExist(int i, int j) const
{
    /* Vérifie si l'arête existe */
    int t;
    for (int k=aps[i]; (t=fs[k])!=0; ++k)
        if (t == j)
            return true;
    return false;
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
    TaskCostValuesEmpty = false;

    oriented ? createWindow_KeyboardEnterD(NodesAmountValue) : createWindow_KeyboardEnterU(EdgesNumber);
}

void MainWindow::click_ButtonRename()
{
    oriented ? genGraphD() : genGraphU();

    if(NodesAmountValue > 0)
    {
        setWindowTitle("Renommer les sommets");
        setMinimumSize(400,NodesAmountValue*30);
        setMaximumSize(400,NodesAmountValue*30);

        menuBar()->clear();
        auto MainWidget= new QWidget;
        setCentralWidget(MainWidget);

        auto mainBox = new QHBoxLayout;
        MainWidget->setLayout(mainBox);

        auto FormBox = new QFormLayout;
        mainBox->addLayout(FormBox);

        infosEntries.resize(NodesAmountValue+1);
        for(int i=1; i<=NodesAmountValue; i++)
        {
            auto NodesAmountLabel = new QLabel{"SOMMET N° " + QString::number(i)};
            QLineEdit *lineEdit = new QLineEdit;
            infosEntries[i] = lineEdit;
            FormBox->addRow(NodesAmountLabel, lineEdit);
        }

        auto LigneH1 = new QFrame{};
        LigneH1->setFrameStyle(QFrame::VLine | QFrame :: Sunken);
        mainBox->addWidget(LigneH1);

        auto ButtonsBox = new QVBoxLayout;
        mainBox->addLayout(ButtonsBox);

            auto ButtonSave = new QPushButton{tr("Enregistrer")};
            ButtonsBox->addWidget(ButtonSave);
            ButtonSave->setMinimumHeight((NodesAmountValue*30)/2 - 8);
            connect(ButtonSave, &QPushButton::clicked, this,&MainWindow::SaveRenameEntries);

            auto ButtonCancel = new QPushButton{tr("Annuler")};
            ButtonsBox->addWidget(ButtonCancel);
            ButtonCancel->setMinimumHeight((NodesAmountValue*30)/2 - 8);
            if (oriented)
                connect(ButtonCancel, &QPushButton::clicked, this, [this]{ MainWindow::createWindow_KeyboardEnterD(NodesAmountValue);});
            else
                connect(ButtonCancel, &QPushButton::clicked, this, [this]{ MainWindow::createWindow_KeyboardEnterU(EdgesNumber);});
    }
    else
        QMessageBox{QMessageBox::Warning, "Nombre de sommets vide","Votre nombre de sommets est vide.", QMessageBox::Ok}.exec();
}

void MainWindow::SaveRenameEntries()
{
    infos.resize(NodesAmountValue+1);
    for (int i = 1; i <= NodesAmountValue; ++i)
    {
        stringstream ss(infosEntries[i]->text().toStdString());
        string name = ss.str();
        if (name!="")
            infos[i] = name;
        else
            infos[i] = to_string(i);
    }
    QMessageBox{QMessageBox::Information,"Renommage des sommets","Le nom des sommets a été enregistré avec succès.", QMessageBox::Ok}.exec();
    oriented ? createWindow_KeyboardEnterD(NodesAmountValue) : createWindow_KeyboardEnterU(EdgesNumber);
}

void MainWindow::saveSuccessorEntries()
{
    SuccessorEntriesValues.clear();
    SuccessorEntriesValues.push_back("#");
    for(int i=1; i<SuccessorEntries.size(); i++)
        SuccessorEntriesValues.push_back(SuccessorEntries[i]->text().toStdString());
}

void MainWindow::RankAlgorithm()
{
    graph g = oriented? genGraphD() : genGraphU();

    if (g.getFsSize()<=1) //le graphe n'a pas été saisi
        return;

    int *rang;
    algorithms algo = algorithms(g);
    algo.rang(rang);

    string s = "";
    if(infos.size()>0) //si les sommets sont renommés
        for (int i=1; i<=aps[0]; i++)
            s+= "rang(" + g.getInfo(i) + ") = " + to_string(rang[i]) + "\n";
    else
        for (int i=1; i<=aps[0]; i++)
            s+= "rang(" + to_string(i) + ") = " + to_string(rang[i]) + "\n";
    QMessageBox{QMessageBox::Information, "Algorithme du rang",QString::fromStdString(s), QMessageBox::Ok}.exec();
}

void MainWindow::TarjanAlgorithm()
{
    graph g = oriented? genGraphD() : genGraphU();

    if (g.getFsSize()<=1) //le graphe n'a pas été saisi
        return;

    int *cfc;
    int *prem;
    algorithms algo = algorithms(g);
    algo.fortConnexe(prem, cfc);

    string s = "";
    if(infos.size()>0) //si les sommets sont renommés
    {
        for (int i=1; i<=aps[0]; i++)
            s+= "CFC(" + g.getInfo(i) + ") = " + to_string(cfc[i]) + "\n";
        s+= "\n";
        for (int i=1; i<=prem[0]; i++)
            s+= "Prem(" + g.getInfo(i) + ") = " + to_string(prem[i]) + "\n";
    }
    else
    {
        for (int i=1; i<=aps[0]; i++)
            s+= "CFC(" + to_string(i) + ") = " + to_string(cfc[i]) + "\n";
        s+= "\n";
        for (int i=1; i<=prem[0]; i++)
            s+= "Prem(" + to_string(i) + ") = " + to_string(prem[i]) + "\n";
    }
    QMessageBox{QMessageBox::Information, "Algorithme de Tarjan",QString::fromStdString(s), QMessageBox::Ok}.exec();
}

void MainWindow::showTarjanGraph()
{
    graph g = oriented? genGraphD() : genGraphU();

    if (g.getFsSize()<=1) //le graphe n'a pas été saisi
        return;

    int *cfc;
    int *prem;
    algorithms algo = algorithms(g);
    algo.fortConnexe(prem, cfc);
    graph g1 = algo.graph_reduit(prem, cfc);
    showGraph(g1, "Afficher le graphe réduit selon Tarjan");
}

void MainWindow::DantzigAlgorithm()
{
    if (!Check_TaskCost()) //si matrice des coûts absente ou mal saisie
        return;

    graph g = oriented? genGraphD() : genGraphU();

    if (g.getFsSize()<=1) //le graphe n'a pas été saisi
        return;

    graphWithCosts gc = graphWithCosts{g,TaskCostValues};
    string dantzig = gc.Dantzig();

    if (dantzig != "") //circuit absorbant
        QMessageBox{QMessageBox::Warning, "Algorithme de Dantzig",QString::fromStdString(dantzig), QMessageBox::Ok}.exec();

    else
    {
        QString message = "Matrice des coûts après application de l'algorithme de Dantzig :\n\n       ";
        for (int i=1; i<= g.getAps(0); i++)
            message += "(" + QString::number(i) + ")" + "  ";
        message += "\n      ";
        for (int i=1; i<= g.getAps(0); i++)
            message += "----";
        for (int i=1; i<= g.getAps(0); i++)
        {
            message += "\n("+ QString::number(i) + ") | ";
            for (int j=1; j<= g.getAps(0); j++)
                if (gc.getCout(i,j) != 9999) // il y a un coût
                    message += " " + QString::number(gc.getCout(i,j)) + "   ";
                else
                    message += "  ∅  ";
        }
        QMessageBox{QMessageBox::Information, "Algorithme de Dantzig",message, QMessageBox::Ok}.exec();
    }
}

void MainWindow::DikjstraAlgorithm()
{
    if (!Check_TaskCost()) //si matrice des coûts absente ou mal saisie
        return;

    graph g = oriented? genGraphD() : genGraphU();

    if (g.getFsSize()<=1) //le graphe n'a pas été saisi
        return;

    graphWithCosts gc = graphWithCosts{g,TaskCostValues};

    if (!gc.positiveCosts()) //si des coûts sont négatifs
    {
        QMessageBox{QMessageBox::Warning, "Algorithme de Dikjstra","Les coûts doivent être positifs pour appliquer l'algorithme de Dikjstra", QMessageBox::Ok}.exec();
        return;
    }

    int **dist;
    int **pred;
    gc.Dikjstra(dist,pred);

    QString message = "Matrice des distances après application de l'algorithme de Dikjstra :\n\n      ";

    for (int i=1; i<= g.getAps(0); i++)
        message += "(" + QString::number(i) + ")" + "  ";
    message += "\n      ";
    for (int i=1; i<= g.getAps(0); i++)
        message += "----";

    for(int i=1; i<= g.getAps(0); i++)
    {
        message += "\n("+ QString::number(i) + ") | ";
        for (int j=1; j<= g.getAps(0); j++)
            if (dist[i][j] != 9999)
                message += " " + QString::number(dist[i][j]) + "   ";
            else
                message += " ∞  ";

    }

    message += "\n\nMatrice des prédecesseurs :\n\n      ";

    for (int i=1; i<= g.getAps(0); i++)
        message += "(" + QString::number(i) + ")" + "  ";
    message += "\n      ";
    for (int i=1; i<= g.getAps(0); i++)
        message += "----";
    for(int i=1; i<= g.getAps(0); i++)
    {
        message += "\n("+ QString::number(i) + ") | ";
        for (int j=1; j<= g.getAps(0); j++)
            message += QString::number(pred[i][j]) + "   ";
    }

    QMessageBox{QMessageBox::Information, "Algorithme de Dikjstra",message, QMessageBox::Ok}.exec();
}

void MainWindow::KruskalAlgorithm()
{
    if (!Check_TaskCost()) //si matrice des coûts absente ou mal saisie
        return;

    if (oriented)
    {
        QMessageBox{QMessageBox::Warning, "Algorithme de Kruskal","Le graphe ne doit pas être orienté", QMessageBox::Ok}.exec();
        return;
    }

    graph g = genGraphU();
    if (g.getFsSize()<=1) //le graphe n'a pas été saisi
        return;

    graphWithCosts gc = graphWithCosts{g,TaskCostValues};
    gc.Kruskal();

    QString message = "Graphe recouvrant minimal après application de l'algorithme de Kruskal :\n\n";
    if (infos.size()>0)
        for (int i=0; i<gc.getNbAreteKruskal(); ++i)
        {
            arete a = gc.getArete(i);
            message+= "Arête n°" + QString::number(i+1) + ": (" + QString::fromStdString(g.getInfo(a.s)) + "," + QString::fromStdString(g.getInfo(a.t));
            message+= ") -> poids : " + QString::number(a.p) + "\n";
        }
    else
        for (int i=0; i<gc.getNbAreteKruskal(); ++i)
        {
            arete a = gc.getArete(i);
            message+= "Arête n°" + QString::number(i+1) + ": (" + QString::number(a.s) + "," + QString::number(a.t);
            message+= ") -> poids : " + QString::number(a.p) + "\n";
        }
    QMessageBox{QMessageBox::Information, "Algorithme de Kruskal",message, QMessageBox::Ok}.exec();
}

void MainWindow::PruferAlgorithm()
{
    if (oriented)
    {
        QMessageBox{QMessageBox::Warning, "Algorithme de Kruskal","Le graphe ne doit pas être orienté", QMessageBox::Ok}.exec();
        return;
    }

    graph g = genGraphU();
    if (g.getFsSize()<=1) //le graphe n'a pas été saisi
        return;
    vector<int> Prufer = g.prufer();
    if (Prufer.size() == 0)
    {
        QMessageBox{QMessageBox::Warning, "Algorithme de Prufer","Le graphe n'est pas un arbre", QMessageBox::Ok}.exec();
        return;
    }

    QString message = "Codage de Prüfer :\n\n[";
    if (infos.size()>0)
    {
        for (int i=0; i<Prufer.size()-1; ++i)
            message+= QString::fromStdString(g.getInfo(Prufer[i])) + ",";
        message+= QString::fromStdString(g.getInfo(Prufer[Prufer.size()-1])) + "]";
    }
    else
    {
        for (int i=0; i<Prufer.size()-1; ++i)
            message+= QString::number(Prufer[i]) + ",";
        message+= QString::number(Prufer[Prufer.size()-1]) + "]";
    }

    QMessageBox{QMessageBox::Information, "Algorithme de Prüfer",message, QMessageBox::Ok}.exec();
}

void MainWindow::SchedulingAlgorithm()
{
    if (!oriented)
    {
        QMessageBox{QMessageBox::Warning, "Algorithme de l'ordonnancement","Le graphe doit être orienté", QMessageBox::Ok}.exec();
        return;
    }
    graph g = genGraphD();
    if (g.getFsSize()<=1) //le graphe n'a pas été saisi
        return;

    scheduling s = scheduling(g);
    if(TaskDuration.size() == 0)
    {
        QMessageBox{QMessageBox::Warning, "Erreur","Les durées des tâches n'ont pas été entrées.", QMessageBox::Ok}.exec();
        return;
    }
    s.enterduration(TaskDuration);
    s.calculateEarliestStart();
    s.calculateLatestStart();
    s.calculateCriticalPath();

    string msg{""};
    vector<vector<string>> cp = s.getCritical_paths();
    for(int i=0; i<cp.size(); i++)
    {
        for(int j=0; j<cp[i].size()-1; j++)
        {
            msg += cp[i][j] + " -> ";
        }
        msg += cp[i][cp[i].size()-1] + "\n";
    }

    QMessageBox{QMessageBox::Warning, "Algorithme de l'ordonnancement","Voici les chemins critiques : \n"
                 + QString::fromStdString(msg),QMessageBox::Ok}.exec();
}

void MainWindow::click_ButtonDuration()
{
    if(NodesAmountValue > 0)
    {
        setWindowTitle("Ajouter une durée aux tâches");
        setMinimumSize(400,NodesAmountValue*30);
        setMaximumSize(400,NodesAmountValue*30);

        menuBar()->clear();
        auto MainWidget= new QWidget;
        setCentralWidget(MainWidget);

        auto mainBox = new QHBoxLayout;
        MainWidget->setLayout(mainBox);

        auto FormBox = new QFormLayout;
        mainBox->addLayout(FormBox);

        TaskDurationEntries.resize(NodesAmountValue+1);
        for(int i=1; i<=NodesAmountValue; i++)
        {
            auto NodesAmountLabel = new QLabel{"Durée de la tâche N° " + QString::number(i)};
            QLineEdit *lineEdit = new QLineEdit;
            TaskDurationEntries[i] = lineEdit;
            FormBox->addRow(NodesAmountLabel, lineEdit);
        }

        auto LigneH1 = new QFrame{};
        LigneH1->setFrameStyle(QFrame::VLine | QFrame :: Sunken);
        mainBox->addWidget(LigneH1);

        auto ButtonsBox = new QVBoxLayout;
        mainBox->addLayout(ButtonsBox);

            auto ButtonSave = new QPushButton{tr("Enregistrer")};
            ButtonsBox->addWidget(ButtonSave);
            ButtonSave->setMinimumHeight((NodesAmountValue*30)/2 - 8);
            connect(ButtonSave, &QPushButton::clicked, this,&MainWindow::SaveTaskDurationEntries);
            connect(ButtonSave, &QPushButton::clicked, this, [this]{ MainWindow::createWindow_KeyboardEnterD(NodesAmountValue);});

            auto ButtonCancel = new QPushButton{tr("Annuler")};
            ButtonsBox->addWidget(ButtonCancel);
            ButtonCancel->setMinimumHeight((NodesAmountValue*30)/2 - 8);
            connect(ButtonCancel, &QPushButton::clicked, this,&MainWindow::createWindow_KeyboardEnterD);


    }
    else
        QMessageBox{QMessageBox::Warning, "Nombre de sommets vide","Votre nombre de sommets est vide.", QMessageBox::Ok}.exec();
}

void MainWindow::SaveTaskDurationEntries()
{
    TaskDuration.clear();
    for (int i = 1; i <= NodesAmountValue; ++i)
    {
        string V = TaskDurationEntries[i]->text().toStdString() + " ";
        if(!(V.length() == 0))
        {
            stringstream ss(V);
                int number = -99999;
                ss >> number;
                if(number <= 0)
                {
                    QMessageBox{QMessageBox::Warning, "Erreur de saisie","La durée d'une tâche doit etre un entier différent de 0.", QMessageBox::Ok}.exec();
                    TaskDuration.clear();
                    return;
                }
                TaskDuration.push_back(number);
        }
    }
}


void MainWindow::AlgorithmsInformation()
{
    graph b = genGraphD();
    string message = "Prüfer :\n"
    "Le principe du codage de Prüfer est de représenter un arbre par une séquence de nombres de longueur n-2 où n est le nombre sommets et chaque nombre représente"
    " un sommet dans l'arbre. Cet algorithme élimine les feuilles par ordre croissant en rentrant dans la séquence de nombres le sommet lie à la feuille éliminée , il nécessite donc un arbre non oriente pour être exécuté."
    "\n\nDijkstra :\nL'algorithme de Dijkstra permet de trouver le chemin le plus court entre le sommet de départ et tous les sommets d'un graphe orienté."
    "L'algorithme de Dijkstra parcours les sommets en fonction de leur distance par rapport au sommet de départ, pour ensuite choisir le sommet le plus proche. "
    "Le processus est réitéré en remplaçant le chemin précédent si on trouve un chemin plus court entre 2 sommets. Pour pouvoir appliquer cet algorithme il faut que les coûts entre les sommets soient positifs ou nuls."
    "\n\nProblème d'ordonnancement :\nLe problème d'ordonnancement permet de trouver le ou les chemins critiques d'un graphe orienté où chaque sommet représente une tâche d'un même projet. Le cout"
    "entre les sommets représente les durées. Un chemin critique est un chemin dans le graphe qui va de la tâche de départ du projet jusqu'à la tâche d'arrivée et "
    "où chaque tâche ne peut pas être retardée. Pour trouver ce chemin l'algorithme calcule la date au plus tôt et au plus tard de chaque tâche."
    "\n\nKruskal :\nL'algorithme de Kruskal permet de trouver l'arbre de recouvrement minimal dans un graphe non orienté. Pour commencer l'algorithme trie les arêtes par poids croissant et "
    "cherche l'arête avec le poids le plus petit pour l'ajouter dans le graphe. Si l'arête ne crée pas de boucle ell est ajoutée, sinon on passe à l'arête suivante jusqu'à avoir parcouru toutes les arêtes du graphe."
    "\n\nTarjan :\nL'algorithme de tarjan permet de trouver les composantes fortement connexes d'un graphe orienté. Il commence par parcourir le graphe à partir d'un sommet de "
    "départ en attribuant à chaque sommet une numérotation jusqu'à ce qu'il ne puisse plus avancer, puis calcule le ro de chaque sommet. Si le ro est égal à la numérotation, tous les sommets de celui de départ au sommets courants deviennent une composante, ensuite il réitère le processus pour tous les successeurs de "
    "chaque sommet en veillant à ne pas aller dans un successeur déjà visité jusqu'à ce qu'il ait visité tous les sommets. L'algorithme de Tarjan permet aussi d'afficher le graphe réduit, qui est l'unique base d'un graphe orienté sans circuit formé des sommets qui n'ont pas de prédecesseurs."
    "\n\nDantzig :\nL'algorithme de Dantzig permet de trouver les chemins les plus courts dans un graphe orienté entre un sommet de départ et tous les autres sommets."
    "L'algorithme parcoure la matrice de coût : à chaque nouveau sommet, il ajoute une ligne et colonne a son parcours et en parallèle met à jour les distances les plus courtes."
    "\n\nRang :\nL'algorithme du rang permet de trouver le rang d'un sommet d'un graphe orienté (la longueur maximale d'un chemin arrivant en ce sommet). Pour cela il faut trouver la longueur maximale d'un chemin arrivant à ce sommet. "
    "Le maximum des rangs des sommets du graphe est appelé rang du graphe, si le graphe possède un circuit le rang maximal est égal a l'infini.";

    QDialog dialog;
    dialog.setMinimumSize(800,500);
    dialog.setMaximumSize(800,500);
    dialog.setWindowTitle("Information");

    QScrollArea *scrollArea = new QScrollArea(&dialog);
    scrollArea->setWidgetResizable(true);

    QTextEdit *textEdit = new QTextEdit(&dialog);
    textEdit->setReadOnly(true);
    textEdit->setText(QString::fromStdString(message));

    scrollArea->setWidget(textEdit);
    QPushButton *okButton = new QPushButton("OK", &dialog);
    QObject::connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    layout->addWidget(scrollArea);
    layout->addWidget(okButton);

    dialog.exec();
}

void MainWindow::showCurrentGraph()
{
    graph g = oriented ? genGraphD() : genGraphU();
    if (g.getFsSize()==0)
        return;
    showGraph(g, "Afficher le graphe courant");
}


//----------------- FIN FENETRE SAISIE CLAVIER GRAPHE ORIENTE----------------------//


//----------------- DEBUT FENETRE SAISIE FICHIER GRAPHE ORIENTE----------------------//

void MainWindow::createWindow_FileEnter()
{
    file = true;
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
                QMessageBox{QMessageBox::Warning, "Fichier corrompu","Votre fichier n'est pas adapté à la création d'un graphe.", QMessageBox::Ok}.exec();
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
    if(oriented)
        connect(MenuCancel,&QAction::triggered,this,&MainWindow::createWindow_DirectedGraph);
    else
        connect(MenuCancel,&QAction::triggered,this,&MainWindow::createWindow_UndirectedGraph);

    addAlgorithmButtons(mainBox);
}

bool MainWindow::Check_TaskCost()
{
    /* Vérifie si la matrice des coûts a été correctement remplie */

    if (!TaskCostValuesEmpty) //si elle est déjà remplie
        return true;

    if (choosenFileName == "") //si pas de fichier choisi
    {
        QMessageBox{QMessageBox::Warning, "Matrice des coûts absente", "Veuillez rentrer une matrice de coûts", QMessageBox::Ok}.exec();
        return false;
    }

    ifstream inputFile(choosenFileName);
    string str3;
    if (inputFile.is_open())
        for(int i=0; i<2; i++)
            getline(inputFile, str3);

    TaskCostValues.resize(aps[0]+1, vector<int>(aps[0]+1, 0));
    for (int i=0; i<aps[0]; i++)
        TaskCostValues[0].push_back(0);

    for(int i=1; i<=aps[0]; i++)
    {
        replace(str3.begin(), str3.end(), ' ', ',');
        getline(inputFile, str3);

        stringstream ssTMP(str3);
        vector<int> T;
        T.push_back(0);
        int number=0;
        char separateur;
        while(!ssTMP.eof())
        {
            if(!(ssTMP >> number))
            {
                QMessageBox{QMessageBox::Warning, "Fichier incompatible","La matrice des coûts du fichier séléctionné n'est pas correctement écrite ou n'existe pas.", QMessageBox::Ok}.exec();
                TaskCostValues.clear();
                return false;
            }
            T.push_back(number);
            ssTMP >> separateur;
        }

        if(static_cast<int>(T.size()) != aps[0]+1)
        {
            QMessageBox{QMessageBox::Warning, "Fichier incompatible","La matrice des coûts du fichier séléctionné n'est pas adaptée au graphe du même fichier ou n'existe pas", QMessageBox::Ok}.exec();
            TaskCostValues.clear();
            return false;
        }

        if(inputFile.eof() && i != aps[0])
        {
            QMessageBox{QMessageBox::Warning, "Fichier incompatible","La matrice des coûts du fichier séléctionné n'est pas complète.", QMessageBox::Ok}.exec();
            TaskCostValues.clear();
            return false;
        }

        TaskCostValues[i] = T;
    }
    TaskCostValuesEmpty = false;
    return true;
}

//----------------- FIN FENETRE SAISIE FICHIER GRAPHE ORIENTE----------------------//

void MainWindow::saveGraphToFile()
{
    graph g = oriented ? genGraphD() : genGraphU();

    if (g.getFsSize()<=1) //le graphe n'a pas été saisi
        return;

    QString filename = QFileDialog::getSaveFileName(nullptr, "Enregistrer le graphe", "", "Fichiers texte (*.txt)");

    if (filename.isEmpty())
        return;

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Erreur : Impossible d'ouvrir le fichier" << filename;
        return;
    }

    QTextStream out(&file);

    int i;
    for (i = 0; i < g.getAps(0); ++i)
    {
        out << g.getAps(i) << ",";
    }
    out << g.getAps(i) << Qt::endl;


    for (i = 0; i < g.getFsSize() - 1; ++i)
    {
        out << g.getFs(i) << ",";
    }
    out << g.getFs(i) << Qt::endl;

    if (!TaskCostValuesEmpty)
    {
        for (int j = 1; j < TaskCostValues.size(); ++j)
        {
            for (i = 1; i < TaskCostValues[j].size()-1; ++i)
            {
                out << TaskCostValues[j][i] << ",";
            }
            out << TaskCostValues[j][i] << Qt::endl;
        }
    }

    file.close();
}

void MainWindow::showGraph(const graph &g, const QString &titre)
{
    /* Affiche le graphe graphiquement */

    int nbSommet = g.getAps(0);
    menuBar()->clear();

    // Définir les coordonnées des sommets (sur 3 lignes)
    vector<QPointF> nodeCoordinates(nbSommet+1);
    int div = nbSommet/3;
    double horizontalSpacing = 800.0 / (nbSommet+1);
    for (int i=1; i<= div; ++i)
        nodeCoordinates[i] = QPointF(i * horizontalSpacing, 50.0);
    for (int i=div+1; i<= 2*div; ++i)
        nodeCoordinates[i] = QPointF((i-div) * horizontalSpacing + 20, 175.0);
    for (int i=2*div+1; i<= nbSommet; ++i)
        nodeCoordinates[i] = QPointF((i-2*div) * horizontalSpacing - 40, 350.0);

    // Créer la scène principale
    QGraphicsScene *mainScene = new QGraphicsScene();
    mainScene->setSceneRect(0, 0, 400, 400);

    // Dessiner les sommets
    QFont font("Arial", 12);
    for (int i = 1; i < nodeCoordinates.size(); ++i)
    {
        QPointF coord = nodeCoordinates[i];
        // Dessiner le cercle
        mainScene->addEllipse(coord.x() - 20, coord.y() - 20, 40, 40, QPen(Qt::black), QBrush(Qt::black));
        // Écrire le numéro à l'intérieur du cercle
        QGraphicsTextItem *text;
        if (!g.info())
            text = mainScene->addText(QString::number(i), font);
        else
            text = mainScene->addText(QString::fromStdString(g.getInfo(i)), font);
        text->setDefaultTextColor(Qt::red);
        text->setPos(coord.x()-10, coord.y()-10);
        text->setZValue(1); // Mettre le texte au-dessus du cercle
    }

    int s=1;
    if (oriented) // Dessiner les flèches
        for (int i=1; i<g.getFs(0); ++i)
        {
            if(g.getFs(i)!=0)
            {
                // Dessiner l'arête
                QPointF startPoint = nodeCoordinates[s];
                QPointF endPoint = nodeCoordinates[g.getFs(i)];
                mainScene->addLine(startPoint.x(), startPoint.y(), endPoint.x(), endPoint.y(), QPen(Qt::black));

                // Dessiner la flèche
                qreal arrowLength = 20.0;
                qreal angle = std::atan2(endPoint.y() - startPoint.y(), endPoint.x() - startPoint.x());
                QPointF arrowEnd = endPoint - QPointF(arrowLength * std::cos(angle), arrowLength * std::sin(angle));
                QGraphicsPolygonItem *arrowItem = new QGraphicsPolygonItem(QPolygonF() << QPointF(0, 0) << QPointF(-10, 4) << QPointF(-10, -4));
                arrowItem->setTransform(QTransform().translate(arrowEnd.x(), arrowEnd.y()).rotateRadians(angle));
                mainScene->addItem(arrowItem);
            }
            else
                s++;
        }
    else  // Dessiner les arêtes
    {
        s = 1;
        for (int i=1; i<g.getFs(0); ++i)
            if(g.getFs(i)!=0)
            {
                QPointF startPoint = nodeCoordinates[s];
                QPointF endPoint = nodeCoordinates[g.getFs(i)];
                mainScene->addLine(startPoint.x(), startPoint.y(), endPoint.x(), endPoint.y(), QPen(Qt::black));
            }
            else
                s++;
    }

    auto ButtonBack = new QPushButton{tr("Retour")};
    ButtonBack->setMinimumHeight(50);
    ButtonBack->setMaximumHeight(50);
    if(file)
        connect(ButtonBack,&QPushButton::clicked,this,&MainWindow::createWindow_ChooseAlgorithm);
    else if(oriented)
        connect(ButtonBack, &QPushButton::clicked, this, [this]{ MainWindow::createWindow_KeyboardEnterD(NodesAmountValue);});
    else
        connect(ButtonBack, &QPushButton::clicked, this, [this]{ MainWindow::createWindow_KeyboardEnterU(EdgesNumber);});

    // Afficher la scène principale dans une vue
    QGraphicsView *mainView = new QGraphicsView(mainScene, this);
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(mainView);
    layout->addWidget(ButtonBack);

    // Définir le layout comme layout principal de la fenêtre principale
    QWidget *centralWidget = new QWidget();
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
    setWindowTitle(titre);
    setFixedSize(800,500);
}


