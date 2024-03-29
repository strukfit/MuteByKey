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

    setHook();
}

MainWindow::~MainWindow()
{
	delete ui;
    delete audioManager;

    removeHook();
}

HHOOK MainWindow::keyboardHook = NULL;

KeyCombo MainWindow::userDefinedCombo(false, false, false, VK_F2);

LRESULT CALLBACK MainWindow::KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        if (wParam == WM_KEYDOWN) {
            KBDLLHOOKSTRUCT* pKeyboard = (KBDLLHOOKSTRUCT*)lParam;
            
            bool ctrlPressed = GetAsyncKeyState(VK_CONTROL) & 0x8000;
            bool shiftPressed = GetAsyncKeyState(VK_SHIFT) & 0x8000;
            bool altPressed = GetAsyncKeyState(VK_MENU) & 0x8000;
            
            if (ctrlPressed == userDefinedCombo.ctrlPressed &&
                shiftPressed == userDefinedCombo.shiftPressed &&
                altPressed == userDefinedCombo.altPressed &&
                pKeyboard->vkCode == userDefinedCombo.key) 
            {
                QMessageBox::information(nullptr, "", "Global hotkey pressed");
            }
        }
    }
    
    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

void MainWindow::setHook() {
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, MainWindow::KeyboardProc, NULL, 0);
}

void MainWindow::removeHook() {
    UnhookWindowsHookEx(keyboardHook);
}
