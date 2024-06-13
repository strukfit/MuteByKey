#pragma once

#include <QSettings>
#include <QApplication>
#include <QTimer>

#include "MainWindowUI.h"
#include "AudioManager.h"
#include "ShortcutWindow.h"

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

public slots:
	void openShortcutWindow();
	void changeShortcut(bool ctrlPressed, bool shiftPressed, bool altPressed, UINT key, QString title);

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

	static LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
	void setHook();
	void removeHook();

private slots:
	void filterTreeView(const QString& text);

protected:
	void showEvent(QShowEvent* event) override;
	void hideEvent(QHideEvent* event) override;

private:
	void loadSettings();
	void saveSettings();
	void updateProcessView();

	void setprocessViewConnections();

	QString settingsFile;
	QTimer* updateTimer;

	Ui::MainWindowClass* ui;

	static AudioManager* audioManager;
	static int selectedProcessId;
	static HHOOK keyboardHook;
	static KeyCombo userDefinedCombo;
	static bool shortcutActive;
	static bool volumeDecreased;
	static float volume;
};