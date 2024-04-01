#pragma once

#include <QDialog>
#include <QWidget>
#include <QLabel>
#include <QtWidgets/QMessageBox>
#include <QKeyEvent>

class ShortcutWindow : public QDialog
{
	Q_OBJECT

public:
	ShortcutWindow(QWidget* parent = nullptr);
	~ShortcutWindow();

signals:
	void changeShortcut(bool ctrlPressed, bool shiftPressed, bool altPressed, unsigned int key, QString title);

protected:
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;

private:
	QLabel* hotkeyLabel;

	bool ctrlPressed, shiftPressed, altPressed;
};