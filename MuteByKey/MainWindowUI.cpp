#include "MainWindowUI.h"

void MainWindowUI::setupMainWindowUI(QMainWindow* MainWindowClass)
{
    centralWidget = new QWidget(MainWindowClass);

    layout = new QVBoxLayout(centralWidget);
    //layout->setContentsMargins(30, 30, 30, 30);

    searchWrapper = new QWidget(centralWidget);
    searchWrapper->setStyleSheet("background-color: white;");

    searchLayout = new QHBoxLayout(searchWrapper);
    searchLayout->setSpacing(5);

    searchLineEdit = new QLineEdit(searchWrapper);
    searchLineEdit->setStyleSheet("padding-left: 5px");
    searchLineEdit->setFixedHeight(25);
    searchLineEdit->setPlaceholderText("Search");

    refreshButton = new QPushButton(searchWrapper);
    refreshButton->setFixedSize(QSize(25, 25));
    refreshButton->setIcon(QIcon("refresh.svg"));
    refreshButton->setIconSize(QSize(15, 15));
    refreshButton->setStyleSheet(" QPushButton { background-color: #0078D7; border: 0; } QPushButton:hover { background-color: #01355e; } QPushButton:pressed { background-color: #035799; }");

    searchClearButton = new QPushButton(searchWrapper);
    searchClearButton->setFixedSize(QSize(25, 25));
    searchClearButton->setIcon(QIcon("x.svg"));
    searchClearButton->setIconSize(QSize(19, 19));
    searchClearButton->setStyleSheet(" QPushButton { background-color: #cccccc; border: 0; } QPushButton:hover { background-color: #9c9c9c; } QPushButton:pressed { background-color: #bababa; }");
    searchClearButton->hide();

    searchLayout->addWidget(searchLineEdit);
    searchLayout->addWidget(searchClearButton);
    searchLayout->addWidget(refreshButton);

    processView = new QTreeView(centralWidget);
    processView->setWindowTitle(QObject::tr("Process list"));
    processView->setSelectionMode(QAbstractItemView::SingleSelection);
    processView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    processView->setStyleSheet("background-color: white; border: 0; padding-top: 20px; padding-bottom: 20px; padding-right: 10px;");
    processView->resize(300, 200);

    QWidget* volumeWrapper = new QWidget(centralWidget);
    volumeWrapper->setStyleSheet("background-color: white;");

    volumeSliderLayout = new QHBoxLayout(volumeWrapper);

    volumeSlider = new QSlider(Qt::Horizontal, volumeWrapper);
    volumeSlider->setRange(0, 100);
    volumeSlider->setTickPosition(QSlider::TicksBelow);

    volumeSpinBox = new QSpinBox(volumeWrapper);
    volumeSpinBox->setRange(0, 100);
    volumeSpinBox->setAlignment(Qt::AlignCenter);

    volumeSliderLayout->addWidget(volumeSlider);
    volumeSliderLayout->addWidget(volumeSpinBox);

    hotkeyWrapper = new QWidget(centralWidget);
    hotkeyWrapper->setStyleSheet("background-color: white;");

    hotkeyLayout = new QHBoxLayout(hotkeyWrapper);
    hotkeyLayout->setAlignment(Qt::AlignCenter);
    
    changeHotkeyButton = new QPushButton("Hotkey", hotkeyWrapper);
    changeHotkeyButton->setFixedHeight(25);
    changeHotkeyButton->setMaximumWidth(150);
    changeHotkeyButton->setStyleSheet("QPushButton { background-color: #0078D7; border: 0; color: white; padding-left: 10px; padding-right: 10px} QPushButton:hover { background-color: #01355e; } QPushButton:pressed { background-color: #035799; }");

    hotkeyLayout->addWidget(changeHotkeyButton);

    layout->addWidget(searchWrapper);
    layout->addWidget(processView);
    layout->addWidget(volumeWrapper);
    layout->addWidget(hotkeyWrapper);

    MainWindowClass->setCentralWidget(centralWidget);

    MainWindowClass->setLayout(layout);

    MainWindowClass->resize(450, 300);
    MainWindowClass->setMinimumSize(QSize(450, 300));
}
