#pragma once

#include <QtWidgets/QDialog>
#include <QKeyEvent>

#include "ShortcutWindowUI.h"

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
	void ctrlButtonHide();
	void shiftButtonHide();
	void altButtonHide();
	void keyButtonHide();

	Ui::ShortcutWindowClass* ui;

	bool ctrlPressed, shiftPressed, altPressed;

	unsigned int key;

	QString title;

	bool fixed;
};