#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindowClass),
    selectedProcessId(NULL)
{
	ui->setupMainWindowUI(this);

    audioManager = new AudioManager();

	ui->model = audioManager->getProcessList();

	ui->processView->setModel(ui->model);

    QObject::connect(ui->processView->selectionModel(), &QItemSelectionModel::selectionChanged, [&](const QItemSelection& selected) {
        QModelIndexList selectedIndexes = selected.indexes();
        if (!selectedIndexes.isEmpty()) {
            QModelIndex selectedIndex = selectedIndexes.at(2); 
            QVariant processData = selectedIndex.data();
            if (processData.isValid()) {
                // Now you have access to the data associated with the selected row
                QString processId = processData.toString();
                selectedProcessId = processId.toInt();
            }
        }
    });

    QObject::connect(ui->upVolumeButton, &QPushButton::clicked, this, [&] {
        if (selectedProcessId)
            audioManager->setProcessVolume(static_cast<DWORD>(selectedProcessId), 1.0f);
    });

    QObject::connect(ui->downVolumeButton, &QPushButton::clicked, this, [&] {
        if (selectedProcessId)
            audioManager->setProcessVolume(static_cast<DWORD>(selectedProcessId), 0.1f);
    });
}

MainWindow::~MainWindow()
{
	delete ui;
    delete audioManager;
}
