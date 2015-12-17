#ifndef GLDISPLAY_H
#define GLDISPLAY_H

#include <QtWidgets/QMainWindow>
#include "ui_GLDisplay.h"

class GLDisplay : public QMainWindow
{
	Q_OBJECT

public:
	GLDisplay(QWidget *parent = 0);
	~GLDisplay();
	
private:
	Ui::GLDisplayClass ui;
};

#endif // GLDISPLAY_H
