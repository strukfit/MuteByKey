#include "MainWindow.h"

float MainWindow::volume;

KeyCombo MainWindow::userDefinedCombo(false, false, false, NULL);

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindowClass)
{
	ui->setupMainWindowUI(this);

    audioManager = new AudioManager();

	ui->processView->setModel(audioManager->getProcessList());
    ui->processView->setHeaderHidden(true);
    ui->processView->setColumnHidden(2, true);
    ui->processView->setColumnWidth(0, 50);

    settingsFile = QApplication::applicationDirPath() + "/settings.ini";

    loadSettings();

    setprocessViewConnections();

    QObject::connect(ui->searchLineEdit, &QLineEdit::textChanged, this, [&](const QString& text) {
        text.isEmpty() ? ui->searchClearButton->hide() : ui->searchClearButton->show();

        filterTreeView(text);
    });

    QObject::connect(ui->changeHotkeyButton, &QPushButton::clicked, this, &MainWindow::openShortcutWindow);

    QObject::connect(ui->volumeSpinBox, &QSpinBox::valueChanged, ui->volumeSlider, &QSlider::setValue);

    QObject::connect(ui->volumeSlider, &QSlider::valueChanged, this, [&](int value) {
        ui->volumeSpinBox->setValue(value);
        
        volume = value / 100.f;
    });

    QObject::connect(ui->refreshButton, &QPushButton::clicked, this, [&] {
        ui->searchLineEdit->clear();
        updateProcessView();
        setprocessViewConnections();
    });

    QObject::connect(ui->searchClearButton, &QPushButton::clicked, this, [&] {
        ui->searchLineEdit->clear();
        ui->searchClearButton->hide();
    });

    updateTimer = new QTimer(this);
    QObject::connect(updateTimer, &QTimer::timeout, [&] {
        updateProcessView();
        setprocessViewConnections();
        if(!ui->searchLineEdit->text().isEmpty())
            filterTreeView(ui->searchLineEdit->text());
    });
    updateTimer->setInterval(1000);

    setHook();
}

MainWindow::~MainWindow()
{
    saveSettings();

    audioManager->setProcessVolume(static_cast<DWORD>(selectedProcessId), 1.0f);

	delete ui;
    delete audioManager;

    removeHook();
}

void MainWindow::setprocessViewConnections()
{
    QObject::connect(ui->processView->selectionModel(), &QItemSelectionModel::selectionChanged, [&](const QItemSelection& selected) {
        QModelIndexList selectedIndexes = selected.indexes();
        if (!selectedIndexes.isEmpty()) {
            // Getting process id from selected row
            QModelIndex selectedIndex = selectedIndexes.at(2);
            QVariant processData = selectedIndex.data();
            if (processData.isValid()) {
                int processId = processData.toInt();

                if (processId == 0)
                    return;

                selectedProcessId = processId;
            }
            
            // Getting process icon from selected row
            selectedIndex = selectedIndexes.at(0);
            processData = selectedIndex.data(Qt::DecorationRole);
            if (processData.isValid())
                ui->processIcon->setPixmap(processData.value<QIcon>().pixmap(20, 20));
            
            // Getting process name from selected row
            selectedIndex = selectedIndexes.at(1);
            processData = selectedIndex.data();
            if (processData.isValid())
            {
                QString processPath = processData.toString();
                int lastIndex = processPath.lastIndexOf('\\');
                QString processName = processPath.mid(lastIndex + 1);
                QString formattedProcessName = processName.left(1).toUpper() + processName.mid(1);
                ui->processName->setText(formattedProcessName);
            }
        }
    });
}

void MainWindow::filterTreeView(const QString& text)
{
    QStandardItemModel* standardModel = qobject_cast<QStandardItemModel*>(ui->processView->model());
    if (!standardModel)
        return;

    for (int row = 0; row < standardModel->rowCount(); ++row) {
        QModelIndex index = standardModel->index(row, 1);
        bool match = index.data().toString().contains(text, Qt::CaseInsensitive);
        ui->processView->setRowHidden(row, index.parent(), !match);
    }
}

AudioManager* MainWindow::audioManager;
int MainWindow::selectedProcessId;
HHOOK MainWindow::keyboardHook = NULL;
bool MainWindow::shortcutActive = true;
bool MainWindow::volumeDecreased = false;

LRESULT CALLBACK MainWindow::KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) 
{
    if (!shortcutActive)
        return NULL;

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
                volumeDecreased ? audioManager->setProcessVolume(static_cast<DWORD>(selectedProcessId), 1.0f) : audioManager->setProcessVolume(static_cast<DWORD>(selectedProcessId), volume);

                volumeDecreased = !volumeDecreased;
            }
        }
    }

    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

void MainWindow::setHook() 
{
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, MainWindow::KeyboardProc, NULL, 0);
}

void MainWindow::removeHook() 
{
    UnhookWindowsHookEx(keyboardHook);
}

void MainWindow::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    updateTimer->start();
}

void MainWindow::hideEvent(QHideEvent* event)
{
    QWidget::hideEvent(event);
    updateTimer->stop();
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

void MainWindow::updateProcessView()
{ 
    auto oldModel = qobject_cast<QStandardItemModel*>(ui->processView->model());
    if (oldModel)
        delete oldModel;

    ui->processView->setModel(audioManager->getProcessList());

    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui->processView->model());
    if (!model)
        return;

    for (int row = 0; row < model->rowCount(); ++row)
    {
        QModelIndex removedIndex = ui->processView->model()->index(row, 2);
        QVariant processData = removedIndex.data();
        if (processData.isValid()) {
            int processId = processData.toInt();
            if (processId == selectedProcessId)
            {
                return;
            }
        }
    }

    ui->processIcon->clear();
    ui->processName->clear();
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