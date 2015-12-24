#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QWidget>
#include <QMainWindow>
#include <QAction>
#include <QToolBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QCloseEvent>
#include "GLWidget.h"
#include <QLabel>

#define READ_LINE_SIZE 256

class MainWindow : public QMainWindow
{
	Q_OBJECT;

public:
	MainWindow(QWidget* parent);
	~MainWindow();
	//work space
	GLWidget *m_GLWidget;

signals:
	void ClickedEnlarge();						// 槽函数
	void ClickedNarrow();
	void ClickedReset();
	void Clickeddrawshell();
	void ClickedOrtho();

protected slots:
	void OpenPntFils();							// 点击打开按钮
	void Aboutsof();							// 点击关于按钮
	void closeEvent(QCloseEvent *);				// 关闭事件
	void ClickedEnlargeBtn();					// 点击放大按钮
	void ClickedNarrowBtn();					// 点击缩小按钮
	void ClickedOrthoBtn();						// 点击正射投影按钮
	void ClickedResetBtn();						// 点击重置视图按钮
	void ClickeddrawshellBtn();					// 点击是否绘制外框按钮

private:
	QString PntFilepath;						// 点文件路径
	QLabel* numOfPoint;							// 总点数
	QLabel* numOfDisplayPoint;					// 显示点数
	QLabel* BoxLength;							// 外框长
	QLabel* BoxWidth;							// 外框宽
	QLabel* BoxHeight;							// 外框高

};

#endif