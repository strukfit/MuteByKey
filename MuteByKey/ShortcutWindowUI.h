#pragma once

#include <QDialog>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QPushButton>

class ShortcutWindowUI
{
public:
	~ShortcutWindowUI();

	void setupUI(QDialog* ShortcutWindowClass);

	QVBoxLayout* layout;
	QWidget* hotkeyWrapper;
	QVBoxLayout* hotkeyLayout;
	QLabel* hotkeyLabel;
	QHBoxLayout* saveCancelLayout;
	QWidget* saveCancelWrapper;
	QPushButton* saveButton;
	QPushButton* cancelButton;

	QHBoxLayout* hotkeyVisualizerLayout;
	QPushButton* ctrlButton;
	QPushButton* shiftButton;
	QPushButton* altButton;
	QPushButton* keyButton;
};

namespace Ui
{
	class ShortcutWindowClass : public ShortcutWindowUI {};
}