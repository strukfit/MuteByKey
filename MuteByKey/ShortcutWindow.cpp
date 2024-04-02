#include "ShortcutWindow.h"

ShortcutWindow::ShortcutWindow(QWidget* parent):
	QDialog(parent),
	ui(new Ui::ShortcutWindowClass),
	ctrlPressed(false),
	shiftPressed(false),
	altPressed(false),
	fixed(false)
{
	ui->setupUI(this);

	setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

	QObject::connect(ui->saveButton, &QPushButton::clicked, [&] {
		emit changeShortcut(ctrlPressed, shiftPressed, altPressed, key, title);
		close();
	});

	QObject::connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::close);
}

ShortcutWindow::~ShortcutWindow()
{
}

void ShortcutWindow::keyPressEvent(QKeyEvent* event)
{	
	if (!ui->hotkeyLabel->isHidden()) ui->hotkeyLabel->hide();

	int key = event->key();

	switch (key)
	{
	case Qt::Key_Control:
		if (fixed)
		{
			fixed = false;
			shiftButtonHide();
			altButtonHide();
		}
			
		ctrlPressed = true;
		ui->ctrlButton->show();

		keyButtonHide();

		break;
	case Qt::Key_Shift:
		if (fixed)
		{
			fixed = false;
			ctrlButtonHide();
			altButtonHide();
		}

		shiftPressed = true;
		ui->shiftButton->show();

		keyButtonHide();

		break;
	case Qt::Key_Alt:
		if (fixed)
		{
			fixed = false;
			ctrlButtonHide();
			shiftButtonHide();
		}

		altPressed = true;
		ui->altButton->show();

		keyButtonHide();

		break;
	default:
		if (fixed)
		{
			fixed = false;
			ctrlButtonHide();
			shiftButtonHide();
			altButtonHide();
		}

		QString title;
		if (ctrlPressed)
			title += "CTRL + ";
		if (shiftPressed)
			title += "SHIFT + ";
		if (altPressed)
			title += "ALT + ";
		title += QKeySequence(key).toString();
		this->key = event->nativeVirtualKey();
		this->title = title;

		ui->keyButton->setText(QKeySequence(key).toString());
		ui->keyButton->show();

		fixed = true;
	}

	QWidget::keyPressEvent(event);
}

void ShortcutWindow::keyReleaseEvent(QKeyEvent* event)
{
	if (!fixed)
	{
		int key = event->key();

		switch (key)
		{
		case Qt::Key_Control:
			ctrlButtonHide();
			break;
		case Qt::Key_Shift:
			shiftButtonHide();
			break;
		case Qt::Key_Alt:
			altButtonHide();
			break;
		}
	}

	QWidget::keyReleaseEvent(event);
}

void ShortcutWindow::ctrlButtonHide()
{
	ctrlPressed = false;
	ui->ctrlButton->hide();
}

void ShortcutWindow::shiftButtonHide()
{
	shiftPressed = false;
	ui->shiftButton->hide();
}

void ShortcutWindow::altButtonHide()
{
	altPressed = false;
	ui->altButton->hide();
}

void ShortcutWindow::keyButtonHide()
{
	key = NULL;
	ui->keyButton->hide();
}
