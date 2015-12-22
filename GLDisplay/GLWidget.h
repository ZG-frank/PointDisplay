#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QWidget>
#include <ArcBall.h>
#include <QKeyEvent>
#include <GL/glu.h>
#include <QMessageBox>
#include <QApplication>
#include <qmath.h>
#include <QDebug>
#include <QWheelEvent>
#include <ft2build.h>
#include <freetype.h>
#include <ftglyph.h>
#include <FTGL/ftgl.h>
#include <FTGL/FTGLExtrdFont.h>
#include <ANN.h>
#include <ANNperf.h>
#include <ANNx.h>

struct Point3D
{
	double dX, dY, dZ;

	Point3D(double x, double y, double z)
	{
		dX = x;
		dY = y;
		dZ = z;
	}
	Point3D()
	{
		dX = dY = dZ = 0.0;
	}
	Point3D(const Point3D &pointtemp)
	{
		dX = pointtemp.dX;
		dY = pointtemp.dY;
		dZ = pointtemp.dZ;
	}
};

class GLWidget : public QGLWidget
{
	Q_OBJECT

public:
	GLWidget(QWidget *parent = 0);
	~GLWidget();
	void loadData(QVector<Point3D> PntsList);   
	void getBoxNum(double &BoxLength, double &BoxWidth, double &BoxHeight);
	void getCurrentPnt(int& PntNum);
	int m_Width;
	int m_Height;

protected:
	void resizeGL(int w, int h);
	void initializeGL();
	void paintGL();
	void drawshell();
	void keyPressEvent(QKeyEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
	void getMaxMinXYZ(double &maxX, double &minX, double &maxY, double &minY, double &maxZ, double &minZ);
	void FindNeighPnts(QVector<Point3D> PntsList);
public slots:
	void setEnlargeNum();
	void setNarrowNum();
	void setOrtho();
	void setResetNum();
	void setDrawshellNum();

private:
	bool				m_isdrawshell;			// �Ƿ�������
	bool				m_show_full_screen;		// �Ƿ�ȫ��
	bool				m_isPerspective;		// �Ƿ���͸��ͶӰ
	ArcBallT			m_arcBall;				// NEW: ArcBall Instance
	Point2fT			m_mousePt;				// ��굱ǰλ��
	bool				m_leftButtonClicked;	// �Ƿ������				
	QVector<Point3D>	m_ListPnts;				// ��¼��
	QVector<Point3D>	LBPntsVec;				// �洢�˲���ĵ�
	double				m_MoveIntoScreen = 0;	// ������Ļ����
	double				m_dZoomFactor;			// ����ϵ������ʼ��Ϊ1��
	double				m_MaxPntX = 0;			// MaxX
	double				m_MaxPntY = 0;			// MaxY
	double				m_MaxPntZ = 0;			// MaxZ
	double				m_MinPntX = 0;			// MinX
	double				m_MinPntY = 0;			// MinY
	double				m_MinPntZ = 0;			// MinZ
	double				m_BoxDiagLine = 0;		// ���Խ���
	double				m_dMouseNum = 0;		// ��������ĸ
	double				m_BoxLength = 0;		// ��򳤶�
	double				m_BoxWidth = 0;			// �����
	double				m_BoxHeight = 0;		// ���߶�
	FTGLExtrdFont		*m_freeTypeFont;		// ���Ƶ�����
	double				m_dScale = 0;			// �����С
	double				m_BoxMaxDis = 0;		// ��������
	double				m_PntR = 255;			// ��ɫR
	double				m_PntG = 255;			// ��ɫG
	double				m_PntB = 255;			// ��ɫB
	int					m_CurrentDisplayPnts;	// ��ǰ��ʾ�ĵ���

};

#endif // GLWIDGET_H