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
	bool				m_isdrawshell;			// 是否绘制外框
	bool				m_show_full_screen;		// 是否全屏
	bool				m_isPerspective;		// 是否是透视投影
	ArcBallT			m_arcBall;				// NEW: ArcBall Instance
	Point2fT			m_mousePt;				// 鼠标当前位置
	bool				m_leftButtonClicked;	// 是否点击左键				
	QVector<Point3D>	m_ListPnts;				// 记录点
	QVector<Point3D>	LBPntsVec;				// 存储滤波后的点
	double				m_MoveIntoScreen = 0;	// 移入屏幕距离
	double				m_dZoomFactor;			// 缩放系数，初始化为1。
	double				m_MaxPntX = 0;			// MaxX
	double				m_MaxPntY = 0;			// MaxY
	double				m_MaxPntZ = 0;			// MaxZ
	double				m_MinPntX = 0;			// MinX
	double				m_MinPntY = 0;			// MinY
	double				m_MinPntZ = 0;			// MinZ
	double				m_BoxDiagLine = 0;		// 外框对角线
	double				m_dMouseNum = 0;		// 鼠标滚动分母
	double				m_BoxLength = 0;		// 外框长度
	double				m_BoxWidth = 0;			// 外框宽度
	double				m_BoxHeight = 0;		// 外框高度
	FTGLExtrdFont		*m_freeTypeFont;		// 绘制的字体
	double				m_dScale = 0;			// 字体大小
	double				m_BoxMaxDis = 0;		// 外框的最大边
	double				m_PntR = 255;			// 颜色R
	double				m_PntG = 255;			// 颜色G
	double				m_PntB = 255;			// 颜色B
	int					m_CurrentDisplayPnts;	// 当前显示的点数

};

#endif // GLWIDGET_H