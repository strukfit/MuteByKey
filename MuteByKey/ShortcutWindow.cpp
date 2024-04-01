#include "ShortcutWindow.h"

ShortcutWindow::ShortcutWindow(QWidget* parent):
	QDialog(parent),
	ctrlPressed(false),
	shiftPressed(false),
	altPressed(false)
{
	setWindowTitle("Change shortcut");
	hotkeyLabel = new QLabel("Press the keyboard shortcut.", this);
	hotkeyLabel->show();
}

ShortcutWindow::~ShortcutWindow()
{
	delete hotkeyLabel;
}

void ShortcutWindow::keyPressEvent(QKeyEvent* event)
{	
	int key = event->key();

	switch (key)
	{
	case Qt::Key_Control:
		ctrlPressed = true;
		break;
	case Qt::Key_Shift:
		shiftPressed = true;
		break;
	case Qt::Key_Alt:
		altPressed = true;
		break;
	default:
		QString title;
		if (ctrlPressed)
			title += "CTRL + ";
		if (shiftPressed)
			title += "SHIFT + ";
		if (altPressed)
			title += "ALT + ";
		title += QKeySequence(key).toString();
		emit changeShortcut(ctrlPressed, shiftPressed, altPressed, event->nativeVirtualKey(), title);
		close();
	}

	QWidget::keyPressEvent(event);
}

void ShortcutWindow::keyReleaseEvent(QKeyEvent* event)
{
	int key = event->key();

	switch (key)
	{
	case Qt::Key_Control:
		ctrlPressed = false;
		break;
	case Qt::Key_Shift:
		shiftPressed = false;
		break;
	case Qt::Key_Alt:
		altPressed = false;
		break;
	}

	QWidget::keyReleaseEvent(event);
}
