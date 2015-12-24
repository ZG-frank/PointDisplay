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
	void ClickedEnlarge();						// �ۺ���
	void ClickedNarrow();
	void ClickedReset();
	void Clickeddrawshell();
	void ClickedOrtho();

protected slots:
	void OpenPntFils();							// ����򿪰�ť
	void Aboutsof();							// ������ڰ�ť
	void closeEvent(QCloseEvent *);				// �ر��¼�
	void ClickedEnlargeBtn();					// ����Ŵ�ť
	void ClickedNarrowBtn();					// �����С��ť
	void ClickedOrthoBtn();						// �������ͶӰ��ť
	void ClickedResetBtn();						// ���������ͼ��ť
	void ClickeddrawshellBtn();					// ����Ƿ�������ť

private:
	QString PntFilepath;						// ���ļ�·��
	QLabel* numOfPoint;							// �ܵ���
	QLabel* numOfDisplayPoint;					// ��ʾ����
	QLabel* BoxLength;							// ���
	QLabel* BoxWidth;							// ����
	QLabel* BoxHeight;							// ����

};

#endif