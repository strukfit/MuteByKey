#include "MainWindow.h"

float MainWindow::volume;

KeyCombo MainWindow::userDefinedCombo(false, false, false, NULL);

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindowClass)
{
	ui->setupMainWindowUI(this);

    audioManager = new AudioManager();

	ui->model = audioManager->getProcessList();

	ui->processView->setModel(ui->model);

    settingsFile = QApplication::applicationDirPath() + "/settings.ini";

    loadSettings();

    QObject::connect(ui->processView->selectionModel(), &QItemSelectionModel::selectionChanged, [&](const QItemSelection& selected) {
        QModelIndexList selectedIndexes = selected.indexes();
        if (!selectedIndexes.isEmpty()) {
            QModelIndex selectedIndex = selectedIndexes.at(2); 
            QVariant processData = selectedIndex.data();
            if (processData.isValid()) {
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
    saveSettings();

	delete ui;
    delete audioManager;

    removeHook();
}

AudioManager* MainWindow::audioManager;

int MainWindow::selectedProcessId;

HHOOK MainWindow::keyboardHook = NULL;

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

void MainWindow::loadSettings()
{
    QSettings settings(settingsFile, QSettings::IniFormat);

    bool ctrlPressed = settings.value("userDefinedCombo/ctrlPressed", false).toBool();
    bool shiftPressed = settings.value("userDefinedCombo/shiftPressed", false).toBool();
    bool altPressed = settings.value("userDefinedCombo/altPressed", false).toBool();

    UINT key = settings.value("userDefinedCombo/key", 0).toInt();
    QString text = settings.value("changeHotkeyButton/text", "").toString();

    volume = settings.value("volume", 0.f).toFloat();
    ui->volumeSlider->setValue(volume * 100);
    ui->volumeSpinBox->setValue(volume * 100);

    userDefinedCombo = KeyCombo(ctrlPressed, shiftPressed, altPressed, key);

    ui->changeHotkeyButton->setText(text);
}

void MainWindow::saveSettings()
{   
    QSettings settings(settingsFile, QSettings::IniFormat);
    settings.setValue("volume", volume);
    settings.setValue("userDefinedCombo/ctrlPressed", userDefinedCombo.ctrlPressed);
    settings.setValue("userDefinedCombo/shiftPressed", userDefinedCombo.shiftPressed);
    settings.setValue("userDefinedCombo/altPressed", userDefinedCombo.altPressed);
    settings.setValue("userDefinedCombo/key", userDefinedCombo.key);
    settings.setValue("changeHotkeyButton/text", ui->changeHotkeyButton->text());
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