#include "MainWindowUI.h"

MainWindowUI::~MainWindowUI()
{
    delete centralWidget;
    delete layout;
    delete processView;
    delete model;
    delete volumeSlider;
}

void MainWindowUI::setupMainWindowUI(QMainWindow* MainWindowClass)
{
    centralWidget = new QWidget(MainWindowClass);

    layout = new QVBoxLayout(centralWidget);

    processView = new QTreeView(centralWidget);
    processView->setWindowTitle(QObject::tr("Process list"));
    processView->setSelectionMode(QAbstractItemView::SingleSelection);
    processView->resize(600, 400);

    hotkeyLabel = new QLabel("Hotkey", centralWidget);

    volumeSliderLayout = new QHBoxLayout(centralWidget);

    volumeSlider = new QSlider(Qt::Horizontal, centralWidget);
    volumeSlider->setRange(0, 100);
    volumeSlider->setTickPosition(QSlider::TicksBelow);

    volumeSpinBox = new QSpinBox(centralWidget);
    volumeSpinBox->setRange(0, 100);
    volumeSpinBox->setAlignment(Qt::AlignCenter);

    volumeSliderLayout->addWidget(volumeSlider);
    volumeSliderLayout->addWidget(volumeSpinBox);

    changeHotkeyButton = new QPushButton(centralWidget);

    hotkeyLayout = new QHBoxLayout(centralWidget);
    hotkeyLayout->addWidget(hotkeyLabel);
    hotkeyLayout->addWidget(changeHotkeyButton);

    layout->addWidget(processView);
    layout->addLayout(volumeSliderLayout);
    layout->addLayout(hotkeyLayout);

    MainWindowClass->setCentralWidget(centralWidget);

    MainWindowClass->setLayout(layout);

    MainWindowClass->resize(800, 600);
}
