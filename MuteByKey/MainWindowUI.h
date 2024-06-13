#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTreeView>
#include <QtGUI/QStandardItemModel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QLineEdit>

class MainWindowUI
{
public:
	void setupMainWindowUI(QMainWindow* MainWindowClass);

	QWidget* centralWidget;
	QTreeView* processView;
	QPushButton* changeHotkeyButton;
	QSlider* volumeSlider;
	QSpinBox* volumeSpinBox;
	QPushButton* refreshButton;
	QPushButton* searchClearButton;
	QLineEdit* searchLineEdit;
	QLabel* processIcon;
	QLabel* processName;
};

namespace Ui
{
	class MainWindowClass : public MainWindowUI {};
}