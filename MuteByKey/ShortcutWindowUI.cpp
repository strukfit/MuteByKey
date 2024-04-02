#include "ShortcutWindowUI.h"

ShortcutWindowUI::~ShortcutWindowUI()
{
}

void ShortcutWindowUI::setupUI(QDialog* ShortcutWindowClass)
{
	ShortcutWindowClass->setWindowTitle("Change shortcut");

	layout = new QVBoxLayout(ShortcutWindowClass);

	hotkeyWrapper = new QWidget(ShortcutWindowClass);
	hotkeyWrapper->setStyleSheet("background-color: white;");

	hotkeyLayout = new QVBoxLayout(hotkeyWrapper);

	hotkeyLabel = new QLabel("Press the keyboard shortcut.", hotkeyWrapper);

	hotkeyVisualizerLayout = new QHBoxLayout(hotkeyWrapper);

	ctrlButton = new QPushButton("CTRL", hotkeyWrapper);
	ctrlButton->setFixedSize(50, 25);
	ctrlButton->setStyleSheet("background-color: #0078D7; color: white; border: 0;");

	shiftButton = new QPushButton("SHIFT", hotkeyWrapper);
	shiftButton->setFixedSize(50, 25);
	shiftButton->setStyleSheet("background-color: #0078D7; color: white; border: 0;");

	altButton = new QPushButton("ALT", hotkeyWrapper);
	altButton->setFixedSize(50, 25);
	altButton->setStyleSheet("background-color: #0078D7; color: white; border: 0;");

	keyButton = new QPushButton("M", hotkeyWrapper);
	keyButton->setFixedSize(25, 25);
	keyButton->setStyleSheet("background-color: #0078D7; color: white; border: 0;");

	ctrlButton->hide();
	shiftButton->hide();
	altButton->hide();
	keyButton->hide();

	hotkeyVisualizerLayout->addWidget(ctrlButton);
	hotkeyVisualizerLayout->addWidget(shiftButton);
	hotkeyVisualizerLayout->addWidget(altButton);
	hotkeyVisualizerLayout->addWidget(keyButton);

	hotkeyLayout->addLayout(hotkeyVisualizerLayout);
	hotkeyLayout->addWidget(hotkeyLabel);

	saveCancelWrapper = new QWidget(ShortcutWindowClass);
	saveCancelWrapper->setStyleSheet("background-color: white;");

	saveCancelLayout = new QHBoxLayout(saveCancelWrapper);
	
	saveButton = new QPushButton("Save", saveCancelWrapper);
	saveButton->setFixedHeight(25);
	saveButton->setStyleSheet("QPushButton { background-color: #0078D7; border: 0; color: white; padding-left: 10px; padding-right: 10px} QPushButton:hover { background-color: #01355e; } QPushButton:pressed { background-color: #035799; }");

	cancelButton = new QPushButton("Cancel", saveCancelWrapper);
	cancelButton->setFixedHeight(25);
	cancelButton->setStyleSheet(" QPushButton { background-color: #cccccc; border: 0; color: white; } QPushButton:hover { background-color: #9c9c9c; } QPushButton:pressed { background-color: #bababa; }");

	saveCancelLayout->addWidget(saveButton);
	saveCancelLayout->addWidget(cancelButton);

	layout->addWidget(hotkeyWrapper);
	layout->addWidget(saveCancelWrapper);

	ShortcutWindowClass->setLayout(layout);
}
