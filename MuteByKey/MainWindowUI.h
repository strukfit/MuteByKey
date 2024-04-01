#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTreeView>
#include <QtGUI/QStandardItemModel>
#include <QtWidgets/QPushButton>
#include <QMainWindow>
#include <QBoxLayout>
#include <QMessageBox>
#include <QLabel>

class MainWindowUI
{
public:
	~MainWindowUI();

	void setupMainWindowUI(QMainWindow* MainWindowClass);

	QWidget* centralWidget;
	QVBoxLayout* layout;
	QTreeView* processView;
	QStandardItemModel* model;
	QPushButton* upVolumeButton;
	QPushButton* downVolumeButton;
	QHBoxLayout* hotkeyLayout;
	QLabel* hotkeyLabel; 
	QPushButton* changeHotkeyButton;
};

namespace Ui
{
	class MainWindowClass : public MainWindowUI {};
}