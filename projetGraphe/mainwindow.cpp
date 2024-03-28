#include "mainwindow.h"

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

}

void MainWindow::click_GraphicEnterD()
{

}

void MainWindow::click_MenuTotalCancel()
{
    createMainWindow();
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

    setMinimumHeight(220+(NA/2.0)*53);
    setMaximumHeight(220+(NA/2.0)*53);
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

    auto AlgorithmsButtonBox = new QHBoxLayout;
    mainBox->addLayout(AlgorithmsButtonBox);
        auto ButtonRankAlgortihm = new QPushButton{tr("Algorithme du RANG")};
        ButtonRankAlgortihm->setMinimumHeight(40);
        AlgorithmsButtonBox->addWidget(ButtonRankAlgortihm);
        connect(ButtonRankAlgortihm, &QPushButton::clicked, this, &MainWindow::click_ButtonRankAlgorithm);

        auto ButtonTarjanAlgortihm = new QPushButton{tr("Algorithme de TARJAN")};
        ButtonTarjanAlgortihm->setMinimumHeight(40);
        AlgorithmsButtonBox->addWidget(ButtonTarjanAlgortihm);
        connect(ButtonTarjanAlgortihm, &QPushButton::clicked, this, &MainWindow::click_ButtonTarjanAlgorithm);

        auto tkt = new QPushButton{tr("vasy les autres après")};
        tkt->setMinimumHeight(40);
        AlgorithmsButtonBox->addWidget(tkt);

    auto LigneH2 = new QFrame{};
    LigneH2->setFrameStyle(QFrame::HLine | QFrame :: Sunken);
    mainBox->addWidget(LigneH2);

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
    NodesAmountValue = value;
}

void MainWindow::click_ButtonRankAlgorithm()
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
                tmp.push_back(number);
            }
            for(int n : tmp)
                fs.push_back(n);
        }
        fs.push_back(0);
    }
    fs[0] = fs.size()-1;

    aps.push_back(1);
    for(int i=1; i<static_cast<int>(fs.size()); i++) // remplir aps
    {
        if(fs[i] == 0 && i+1 < static_cast<int>(fs.size()))
            aps.push_back(i+1);
    }
    aps[0] = aps.size()-1;

    string s = "fs : ";
    for(int i=0; i<static_cast<int>(fs.size());i++)
        s += to_string(fs[i]) + ", ";
    s += "\n aps : ";
    for(int i=0; i<static_cast<int>(aps.size());i++)
        s += to_string(aps[i]) + ", ";

    QMessageBox{QMessageBox::Warning, "Fs et Aps",QString::fromStdString(s), QMessageBox::Ok}.exec();
}

void MainWindow::click_ButtonTarjanAlgorithm()
{
    click_ButtonRankAlgorithm(); // pour l'instant ça fait la meme chose
}


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

