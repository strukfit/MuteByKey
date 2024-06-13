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
	QVBoxLayout* layout;
	QTreeView* processView;
	QHBoxLayout* hotkeyLayout;
	QPushButton* changeHotkeyButton;
	QSlider* volumeSlider;
	QSpinBox* volumeSpinBox;
	QHBoxLayout* volumeSliderLayout;
	QWidget* searchWrapper;
	QWidget* hotkeyWrapper;
	QHBoxLayout* searchLayout;
	QPushButton* refreshButton;
	QPushButton* searchClearButton;
	QLineEdit* searchLineEdit;
};

namespace Ui
{
	class MainWindowClass : public MainWindowUI {};
}