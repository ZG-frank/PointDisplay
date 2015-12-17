#include "GLDisplay.h"
#include <QtWidgets/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow* mainwindow = new MainWindow(NULL);
	mainwindow->setWindowTitle(QString::fromLocal8Bit("3DÏÔÊ¾---"));
	mainwindow->showMaximized();
	mainwindow->show();
	return a.exec();
}
