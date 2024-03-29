#pragma once

#include "MainWindowUI.h"
#include "AudioManager.h"

struct KeyCombo {
	bool ctrlPressed;
	bool shiftPressed;
	bool altPressed;
	UINT key;

	KeyCombo(bool ctrl, bool shift, bool alt, UINT k) : ctrlPressed(ctrl), shiftPressed(shift), altPressed(alt), key(k) {}
};

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

	static LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
	void setHook();
	void removeHook();

private:
	Ui::MainWindowClass* ui;

	AudioManager* audioManager;

	int selectedProcessId;

	static HHOOK keyboardHook;

	static KeyCombo userDefinedCombo;
};