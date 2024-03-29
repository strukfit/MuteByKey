#pragma once

#include "MainWindowUI.h"
#include "AudioManager.h"
//#include <QxtGlobalShortcut>
#include <QShortcut>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

private:
	Ui::MainWindowClass* ui;

	AudioManager* audioManager;

	int selectedProcessId;
};