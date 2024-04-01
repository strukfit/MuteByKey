#include "MainWindow.h"

float MainWindow::volume;

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindowClass)
{
	ui->setupMainWindowUI(this);

    audioManager = new AudioManager();

	ui->model = audioManager->getProcessList();

	ui->processView->setModel(ui->model);

    volume = ui->volumeSlider->value()/100.f;

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

    QObject::connect(ui->changeHotkeyButton, &QPushButton::clicked, this, &MainWindow::openShortcutWindow);

    QObject::connect(ui->volumeSpinBox, &QSpinBox::valueChanged, ui->volumeSlider, &QSlider::setValue);

    QObject::connect(ui->volumeSlider, &QSlider::valueChanged, this, [&](int value) {
        ui->volumeSpinBox->setValue(value);
        
        volume = value / 100.f;
    });

    setHook();
}

MainWindow::~MainWindow()
{
	delete ui;
    delete audioManager;

    removeHook();
}

AudioManager* MainWindow::audioManager;

int MainWindow::selectedProcessId;

HHOOK MainWindow::keyboardHook = NULL;

KeyCombo MainWindow::userDefinedCombo(false, false, false, NULL);

bool MainWindow::shortcutActive = true;

bool MainWindow::volumeDecreased = false;

LRESULT CALLBACK MainWindow::KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) 
{
    if (shortcutActive)
    {
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
                    //QMessageBox::information(nullptr, "", "Global hotkey pressed");
                    volumeDecreased ? audioManager->setProcessVolume(static_cast<DWORD>(selectedProcessId), 1.0f) : audioManager->setProcessVolume(static_cast<DWORD>(selectedProcessId), volume);

                    volumeDecreased = !volumeDecreased;
                }
            }
        }

        return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
    }
}

void MainWindow::setHook() 
{
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, MainWindow::KeyboardProc, NULL, 0);
}

void MainWindow::removeHook() 
{
    UnhookWindowsHookEx(keyboardHook);
}

void MainWindow::changeShortcut(bool ctrlPressed, bool shiftPressed, bool altPressed, UINT key, QString title)
{
    userDefinedCombo = KeyCombo(ctrlPressed, shiftPressed, altPressed, key);

    ui->changeHotkeyButton->setText(title);
}

void MainWindow::openShortcutWindow()
{
    ShortcutWindow shortcutWindow(this);

    QObject::connect(&shortcutWindow, &ShortcutWindow::changeShortcut, this, &MainWindow::changeShortcut);

    shortcutActive = false;

    shortcutWindow.exec();

    shortcutActive = true;
}