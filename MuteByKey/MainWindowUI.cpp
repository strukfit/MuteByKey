#include "MainWindowUI.h"

MainWindowUI::~MainWindowUI()
{
    delete centralWidget;
    delete layout;
    delete processView;
    delete model;
    delete upVolumeButton;
    delete downVolumeButton;
}

void MainWindowUI::setupMainWindowUI(QMainWindow* MainWindowClass)
{
    centralWidget = new QWidget(MainWindowClass);

    layout = new QVBoxLayout(centralWidget);

    // Create a widget to display the list of processes
    processView = new QTreeView(centralWidget);
    
    processView->setWindowTitle(QObject::tr("Process list"));
    processView->setSelectionMode(QAbstractItemView::SingleSelection);
    processView->resize(600, 400);

    upVolumeButton = new QPushButton("100%", centralWidget);

    downVolumeButton = new QPushButton("10%", centralWidget);

    layout->addWidget(processView);

    layout->addWidget(upVolumeButton);

    layout->addWidget(downVolumeButton);

    MainWindowClass->setCentralWidget(centralWidget);

    MainWindowClass->setLayout(layout);

    MainWindowClass->resize(800, 600);
}
