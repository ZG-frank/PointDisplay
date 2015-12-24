#include "GLWidget.h"
#include <math.h>

// 变换矩阵
Matrix4fT   Transform = { 1.0f, 0.0f, 0.0f, 0.0f,			// NEW: Final Transform
						  0.0f, 1.0f, 0.0f, 0.0f,
						  0.0f, 0.0f, 1.0f, 0.0f,
						  0.0f, 0.0f, 0.0f, 1.0f };

Matrix3fT   LastRot =	{ 1.0f, 0.0f, 0.0f,					// NEW: Last Rotation
						  0.0f, 1.0f, 0.0f,
						  0.0f, 0.0f, 1.0f };

Matrix3fT   ThisRot =	{ 1.0f, 0.0f, 0.0f,					// NEW: This Rotation
						  0.0f, 1.0f, 0.0f,
					   	  0.0f, 0.0f, 1.0f };

ArcBallT    ArcBall(1946.0f, 1636.0f);				        // NEW: ArcBall Instance

#define _MAX2(a,b) (a>b? a:b)
#define _MAX3(a,b,c) (a>b?(a>c?a:c):(b>c?b:c))

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent), m_arcBall(1800.0f, 1600.0f)
{
	m_show_full_screen = false;
	m_leftButtonClicked = false;
	m_isdrawshell = true;
	m_isPerspective = true;
	m_freeTypeFont = NULL;
	m_CurrentDisplayPnts = 0;
	m_dZoomFactor = 1.0;
	showNormal();
}

GLWidget::~GLWidget()
{
	
}

void GLWidget::getMaxMinXYZ(double &maxX, double &minX, double &maxY, double &minY, double &maxZ, double &minZ)
{
	if (m_ListPnts.size() != 0)
	{
		maxX = minX = m_ListPnts[0].dX;
		maxY = minY = m_ListPnts[0].dY;
		maxZ = minZ = m_ListPnts[0].dZ;

		for (int i = 0; i < m_ListPnts.size(); i++)
		{
			if (m_ListPnts[i].dX > maxX)
				maxX = m_ListPnts[i].dX;
			if (m_ListPnts[i].dX < minX)
				minX = m_ListPnts[i].dX;
			if (m_ListPnts[i].dY > maxY)
				maxY = m_ListPnts[i].dY;
			if (m_ListPnts[i].dY < minY)
				minY = m_ListPnts[i].dY;
			if (m_ListPnts[i].dZ > maxZ)
				maxZ = m_ListPnts[i].dZ;
			if (m_ListPnts[i].dZ < minZ)
				minZ = m_ListPnts[i].dZ;
		}
	}
}

void GLWidget::drawshell()
{
	// 绘制外框注记
	glBegin(GL_LINES);
	glColor3f(255, 255, 255);
	glVertex3f(m_MaxPntX, m_MaxPntY, m_MaxPntZ);
	glVertex3f(m_MinPntX, m_MaxPntY, m_MaxPntZ);
	glVertex3f(m_MinPntX, m_MaxPntY, m_MinPntZ);
	glVertex3f(m_MaxPntX, m_MaxPntY, m_MinPntZ);
	glVertex3f(m_MinPntX, m_MaxPntY, m_MinPntZ);
	glVertex3f(m_MinPntX, m_MaxPntY, m_MaxPntZ);
	glVertex3f(m_MaxPntX, m_MaxPntY, m_MinPntZ);
	glVertex3f(m_MaxPntX, m_MaxPntY, m_MaxPntZ);
	glVertex3f(m_MaxPntX, m_MinPntY, m_MaxPntZ);
	glVertex3f(m_MinPntX, m_MinPntY, m_MaxPntZ);
	glVertex3f(m_MinPntX, m_MinPntY, m_MinPntZ);
	glVertex3f(m_MaxPntX, m_MinPntY, m_MinPntZ);
	glVertex3f(m_MinPntX, m_MinPntY, m_MinPntZ);
	glVertex3f(m_MinPntX, m_MinPntY, m_MaxPntZ);
	glVertex3f(m_MaxPntX, m_MinPntY, m_MinPntZ);
	glVertex3f(m_MaxPntX, m_MinPntY, m_MaxPntZ);
	glVertex3f(m_MinPntX, m_MaxPntY, m_MaxPntZ);
	glVertex3f(m_MinPntX, m_MinPntY, m_MaxPntZ);
	glVertex3f(m_MaxPntX, m_MaxPntY, m_MaxPntZ);
	glVertex3f(m_MaxPntX, m_MinPntY, m_MaxPntZ);
	glVertex3f(m_MaxPntX, m_MaxPntY, m_MinPntZ);
	glVertex3f(m_MaxPntX, m_MinPntY, m_MinPntZ);
	glVertex3f(m_MinPntX, m_MaxPntY, m_MinPntZ);
	glVertex3f(m_MinPntX, m_MinPntY, m_MinPntZ);
	glEnd();
}

void GLWidget::initializeGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);							// Black Background
	glClearDepth(1.0f);												// Depth Buffer Setup
	glDepthFunc(GL_LEQUAL);											// The Type Of Depth Testing (Less Or Equal)
	glEnable(GL_DEPTH_TEST);										// Enable Depth Testing
	glShadeModel(GL_FLAT);											// Select Flat Shading (Nice Definition Of Objects)
	glEnable(GL_LIGHTING);											// Enable Lighting
	glEnable(GL_COLOR_MATERIAL);									// Enable Color Material
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);				// 告诉系统对透视进行修正
	glLightModelf(GL_LIGHT_MODEL_AMBIENT,1);
	// 读入FreeType字体，路径确保正确
	QString fontPath = QApplication::applicationDirPath() + "/Test.ttf"; // Or "../x64/Debug/Test.ttf" Or "../x64/Release/Test.ttf"
	m_freeTypeFont = new FTGLExtrdFont(fontPath.toLatin1().data());
	m_freeTypeFont->FaceSize(1);
	m_freeTypeFont->Depth(0.05);
	m_freeTypeFont->CharMap(ft_encoding_unicode);
}

void GLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glLoadIdentity();
	// 设置投影方式
	if (!m_isPerspective)
	{
		// 旋转中心，投影中心
		double dCenterX = (m_MinPntX + m_MaxPntX) / 2;
		double dCenterY = (m_MinPntZ + m_MaxPntZ) / 2;
		glOrtho(-1.2 * m_BoxLength * ((double)m_Width / (double)m_Height) + dCenterX, 
				 1.2 * m_BoxLength * ((double)m_Width / (double)m_Height) + dCenterX, 
				-1.2 * m_BoxLength + dCenterY, 
				 1.2 * m_BoxLength + dCenterY, 
				 -10000.0f, 10000.0f);

		double dHalfW = (double)m_Width / 2;
		double dHalfH = (double)m_Height / 2;
		// 设置视点(正射投影通过视点控制远近)
		glViewport(dHalfW - dHalfW * m_dZoomFactor, dHalfH - dHalfH * m_dZoomFactor,
				   2 * dHalfW * m_dZoomFactor, 2 * dHalfH * m_dZoomFactor);
	}
	else
	{
		gluPerspective(45.0f, (GLfloat)m_Width / (GLfloat)m_Height, 0.00001f, 10000.0f);
		// 设置视点(透射投影通过glTranslatef控制远近)
		glViewport(0, 0, m_Width, m_Height);						// 0，0为左下角
	}
	glTranslatef(0, 0, m_MoveIntoScreen);							// Move Into The Screen

	// 开始绘制点
	glPushMatrix();													// NEW: Prepare Dynamic Transform
	glMultMatrixf(Transform.M);										// NEW: Apply Dynamic Transform
	if (LBPntsVec.size() != 0)
	{
		glShadeModel(GL_SMOOTH_POINT_SIZE_RANGE);
		glPointSize(3.0f);
		glBegin(GL_POINTS);
		for (int i = 0; i < LBPntsVec.size(); i++)
		{
			// 实际设色是OpenGL的Y轴
			double fRate = (LBPntsVec[i].dY - m_MinPntY) / (m_MaxPntY - m_MinPntY);
			// 实际去噪的还是垂直屏幕的方向，即OpenGL的Z轴
			double fZRate = (LBPntsVec[i].dZ - m_MinPntZ) / (m_MaxPntZ - m_MinPntZ);
#if 0
			// 线性方程设色
			if (fRate <= 0.5)
			{
				m_PntR = 0;
				m_PntG = 20 * fRate;
				m_PntB = -20 * fRate + 10;
			}
			else
			{
				m_PntR = fRate * 20 - 10;
				m_PntG = -20 * fRate + 20;
				m_PntB = 0;
			}
#endif
			// 一元二次方程赋色方案
			m_PntR = 10 * fRate * fRate;
			m_PntG = -40 * fRate * fRate + 40 * fRate;
			m_PntB = 10 * (fRate - 1) * (fRate - 1);
#if 0
			//Google Map设色方案
			if (fRate < 0)
			{
				Pntcolor.Rx = 1.0f;
				Pntcolor.Gy = 0.0f;
				Pntcolor.Bz = 0.0f;
			}
			else if (fRate <= 0.5f)
			{
				if (fRate <= 0.2)									// 蓝色->青色
				{
					Pntcolor.Rx = 0.0f;
					Pntcolor.Gy = fRate * 12.0;
					Pntcolor.Bz = 1.0f;
				}
				else if (fRate <= 0.60)								// 青色->绿色
				{
					Pntcolor.Rx = 0.0f;
					Pntcolor.Gy = 1.0f;
					Pntcolor.Bz = -6.0*fRate + 1.5;
				}
				else												// 绿色->黄色 
				{
					Pntcolor.Rx = 4.0*fRate - 1;
					Pntcolor.Gy = 1.0f;
					Pntcolor.Bz = 0.0f;
				}
			}
			else if (fRate <= 0.8f)
			{
				Pntcolor.Rx = 1.0f;
				Pntcolor.Gy = -2 * fRate + 2;
				Pntcolor.Bz = 0.0f;									// 黄色->红色
			}
			else
			{
				Pntcolor.Rx = 1.0f;
				Pntcolor.Gy = 0.0f;
				Pntcolor.Bz = 0.0f;
			}

			double m_PntR = Pntcolor.Rx * 127;
			double m_PntG = Pntcolor.Gy * 127;
			double m_PntB = Pntcolor.Bz * 127;
#endif
			 // 去噪
			if (fZRate >= 0.05 && fZRate <= 0.95)
			{
				// 绘制
				glColor3f(m_PntR, m_PntG, m_PntB);
				glVertex3d(LBPntsVec[i].dX, LBPntsVec[i].dY, LBPntsVec[i].dZ);
				 // 显示点加1
				++m_CurrentDisplayPnts;
			}
		}

		glEnd();
	}
	glPopMatrix();													 // NEW: Unapply Dynamic Transform
	glFlush();

	// 绘制外围框					
	glPushMatrix();															
	glMultMatrixf(Transform.M);												
	if (LBPntsVec.size() != 0)
	{
		// 字体比例
		m_dScale = m_BoxMaxDis / 35;
		if (m_isdrawshell)
		{
			// 绘制外框
			drawshell();
			// 绘制外框长度
			QString renderText = QString::number(m_BoxLength);
			glScalef(m_dScale, m_dScale, m_dScale);
			m_freeTypeFont->Render(renderText.toLatin1().data(), -1, 
				FTPoint(0, m_MaxPntY / m_dScale, m_MaxPntZ / m_dScale));
			// 绘制外框高度
			QString renderText1 = QString::number(m_BoxHeight);
			m_freeTypeFont->Render(renderText1.toLatin1().data(), -1, 
				FTPoint(m_MinPntX / m_dScale, m_MaxPntY / m_dScale, 0));
			// 绘制外框宽度
			QString renderText2 = QString::number(m_BoxWidth);
			m_freeTypeFont->Render(renderText2.toLatin1().data(), -1, 
				FTPoint(m_MinPntX / m_dScale, 0, m_MaxPntZ / m_dScale));
		}
	}
	glPopMatrix();														
	glFlush();
}

void GLWidget::resizeGL(int w, int h)
{
	glViewport(0, 0, (GLsizei)(w), (GLsizei)(h));							// Reset The Current Viewport
	glMatrixMode(GL_PROJECTION);											// Select The Projection Matrix
	glLoadIdentity();														// Reset The Projection Matrix
	//if (m_isPerspective)
	//{
	// 透视投影
		//gluPerspective(45.0f, (GLfloat)(w) / (GLfloat)(h), 0.00001f, 10000.0f);
	//}
	//else
	//{
	// 正射投影
		//glOrtho(-0.5 * m_BoxLength + m_MinPntX, 0.5 * m_BoxLength + m_MaxPntX, -0.5 * m_BoxWidth + m_MinPntY, 0.5 * m_BoxWidth + m_MaxPntY, 0.00001f, 10000.0f);
	//}
	glMatrixMode(GL_MODELVIEW);												// Select The Modelview Matrix
	glLoadIdentity();														// Reset The Modelview Matrix
	m_arcBall.setBounds((GLfloat)w, (GLfloat)h);							// *NEW* Update mouse bounds for arcball
	m_Width = w;
	m_Height = h;
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (m_leftButtonClicked)
	{
		m_mousePt.s.X = (GLfloat)(event->pos().x());						// 转换成GLfloat
		m_mousePt.s.Y = (GLfloat)(event->pos().y());
		Quat4fT ThisQuat;
		m_arcBall.drag(&m_mousePt, &ThisQuat);								// Update End Vector And Get Rotation As Quaternion
		Matrix3fSetRotationFromQuat4f(&ThisRot, &ThisQuat);					// Convert Quaternion Into Matrix3fT
		Matrix3fMulMatrix3f(&ThisRot, &LastRot);							// Accumulate Last Rotation Into This One
		Matrix4fSetRotationFromMatrix3f(&Transform, &ThisRot);				// Set Our Final Transform's Rotation From This One
		updateGL();
	}
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
	// 右键
	if (event->button() == Qt::RightButton)
	{
		Matrix3fSetIdentity(&LastRot);										// Reset Rotation
		Matrix3fSetIdentity(&ThisRot);										// Reset Rotation
		Matrix4fSetRotationFromMatrix3f(&Transform, &ThisRot);				// Reset Rotation
		m_MoveIntoScreen = -1.3 * m_BoxDiagLine;							// 返回最初视角
		m_dZoomFactor = 1;
		updateGL();
	}
	// 左键
	else if (event->button() == Qt::LeftButton)
	{
		m_mousePt.s.X = (GLfloat)(event->pos().x());
		m_mousePt.s.Y = (GLfloat)(event->pos().y());
		LastRot = ThisRot;													// Set Last Static Rotation To Last Dynamic One
		m_arcBall.click(&m_mousePt);										// Update Start Vector And Prepare For Dragging
		m_leftButtonClicked = true;
	}
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
	m_leftButtonClicked = false;
	updateGL();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
	if (!m_isPerspective)
	{
		m_dZoomFactor = m_dZoomFactor + (float)event->delta() / 2000;
	}
	else
	{
		m_MoveIntoScreen = m_MoveIntoScreen + ((float)event->delta() / (m_dMouseNum));	// Move Into The Screen 
	}
	updateGL();
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{
		// Q键
	case Qt::Key_Q:
		m_isPerspective = !m_isPerspective;
		break;
		// W键
	case Qt::Key_W:
		m_isdrawshell = !m_isdrawshell;
		updateGL();
		break;
		// Esc键
	case Qt::Key_Escape:
	{
		qApp->exit();
		break;
	}
		// ↑键
	case Qt::Key_Up:
		if (!m_isPerspective)
		{
			m_dZoomFactor = m_dZoomFactor + 0.1;
		}
		else
		{
			if (m_MoveIntoScreen < -1000)
			{
				m_MoveIntoScreen = m_MoveIntoScreen + 400;
			}
			else if (m_MoveIntoScreen < -100)
			{
				m_MoveIntoScreen = m_MoveIntoScreen + 40;
			}
			else if (m_MoveIntoScreen < -10)
			{
				m_MoveIntoScreen = m_MoveIntoScreen + 4;
			}
			else if (m_MoveIntoScreen < -1)
			{
				m_MoveIntoScreen = m_MoveIntoScreen + 0.4;
			}
			else if (m_MoveIntoScreen < -0.1)
			{
				m_MoveIntoScreen = m_MoveIntoScreen + 0.04;
			}
			else if (m_MoveIntoScreen < -0.01)
			{
				m_MoveIntoScreen = m_MoveIntoScreen + 0.004;
			}
			else if (m_MoveIntoScreen < -0.002)
			{
				m_MoveIntoScreen = m_MoveIntoScreen + 0.0008;
			}
		}
		updateGL();
		break;
		// ↓键
	case Qt::Key_Down:
		if (!m_isPerspective)
		{
			m_dZoomFactor = m_dZoomFactor - 0.1;
		}
		else
		{
			if (m_MoveIntoScreen < -1000)
			{
				m_MoveIntoScreen = m_MoveIntoScreen - 400;
			}
			else if (m_MoveIntoScreen < -100)
			{
				m_MoveIntoScreen = m_MoveIntoScreen - 40;
			}
			else if (m_MoveIntoScreen < -10)
			{
				m_MoveIntoScreen = m_MoveIntoScreen - 4;
			}
			else if (m_MoveIntoScreen < -1)
			{
				m_MoveIntoScreen = m_MoveIntoScreen - 0.4;
			}
			else if (m_MoveIntoScreen < -0.1)
			{
				m_MoveIntoScreen = m_MoveIntoScreen - 0.04;
			}
			else if (m_MoveIntoScreen < -0.01)
			{
				m_MoveIntoScreen = m_MoveIntoScreen - 0.004;
			}
			else if (m_MoveIntoScreen < -0.002)
			{
				m_MoveIntoScreen = m_MoveIntoScreen - 0.0008;
			}
		}
		updateGL();
		break;
	}
}

void GLWidget::loadData(QVector<Point3D> PntsList)
{
	m_ListPnts = PntsList;
	LBPntsVec = PntsList;
	// 每读取一次数据就要将当前显示的点重置为0
	m_CurrentDisplayPnts = 0;
	if (m_ListPnts.size() != 0)
	{
		getMaxMinXYZ(m_MaxPntX, m_MinPntX, m_MaxPntY, m_MinPntY, m_MaxPntZ, m_MinPntZ);	// 得到XYZ最值
	}

	m_BoxLength = fabs(m_MaxPntX - m_MinPntX);											// 计算外框长度
	m_BoxWidth = fabs(m_MaxPntY - m_MinPntY);											// 计算外框宽度
	m_BoxHeight = fabs(m_MaxPntZ - m_MinPntZ);											// 计算外框高度

	// 构建KdTree
	ANNpointArray pDataPnts = NULL;
	ANNkd_tree*	pKdTree = NULL;

	int nDim = 3;// 3维点
	//int nDim = 2;
	int nPntsCount = m_ListPnts.size();

	try
	{
		pDataPnts = annAllocPts(nPntsCount, nDim);				// 初始化data	

		for (int i = 0; i < nPntsCount; ++i)
		{
			pDataPnts[i][0] = m_ListPnts[i].dX;
			pDataPnts[i][1] = m_ListPnts[i].dY;
			pDataPnts[i][2] = m_ListPnts[i].dZ;
		}

		pKdTree = new ANNkd_tree(pDataPnts, nPntsCount, nDim);	// 构建查找树
	}
	catch (...)
	{
		printf("Failed to create the kd_tree!");
	}

	const long nK = 10;		// 最近points个数
	ANNidx nnIdx[nK];		// 最近points索引
	ANNdist dists[nK];		// 最近points距离(没有开方的)
	double eps = 1e-5;
	double dStep = 0;

	try
	{
		for (int i = 0; i < m_ListPnts.size(); ++i)
		{
			// 二维点
			//ANNcoord queryPt[2] = { m_ListPnts[i].dX, m_ListPnts[i].dY };
			// 三维点
			ANNcoord queryPt[3] = { m_ListPnts[i].dX, m_ListPnts[i].dY, m_ListPnts[i].dZ };
			// annkSearch
			pKdTree->annkPriSearch(queryPt, nK, nnIdx, dists, eps);

			double dWeightSum = 0;					
			double dSumZ = 0;						
			double dNewZ = 0;

			// 均值滤波器 Average Filter
			for (long j = 1; j < nK; ++j)
			{
				double dWeight = 1.0 / dists[j];
				dWeightSum = dWeightSum + dWeight;
				dSumZ = dSumZ + (m_ListPnts[nnIdx[j]].dZ * dWeight);
			}
			dNewZ = dSumZ / dWeightSum;	
			// New Z put in Vec
			LBPntsVec[i].dZ = dNewZ;
		}
	}
	catch (...)
	{
		printf("Failed to create the kd_tree!");
	}
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	delete pKdTree;
	
	annDeallocPts(pDataPnts);
	annClose();


	// 计算最大的外框边
	m_BoxMaxDis = _MAX3(m_BoxLength, m_BoxWidth,m_BoxHeight);
	
	// 计算外框对角线长度
	m_BoxDiagLine = sqrt(pow(m_BoxLength, 2) + pow(m_BoxWidth, 2) + pow(m_BoxHeight, 2));
	m_MoveIntoScreen = -1.3 * m_BoxDiagLine;											// 移入屏幕1.3倍对角线距离
	
	if (m_MoveIntoScreen < -1000)
	{
		m_dMouseNum = 1;
	}
	else if (m_MoveIntoScreen < -100)
	{
		m_dMouseNum = 12;
	}
	else if (m_MoveIntoScreen < -10)
	{
		m_dMouseNum = 120;
	}
	else if (m_MoveIntoScreen < -1)
	{
		m_dMouseNum = 1200;
	}
	else if (m_MoveIntoScreen < -0.1)
	{
		m_dMouseNum = 12000;
	}
	else if (m_MoveIntoScreen < -0.01)
	{
		m_dMouseNum = 120000;
	}
	updateGL();
}

void GLWidget::setEnlargeNum()
{
	if (!m_isPerspective)
	{
		m_dZoomFactor = m_dZoomFactor + 0.1;
	}
	else
	{
		if (m_MoveIntoScreen < -1000)
		{
			m_MoveIntoScreen = m_MoveIntoScreen + 400;
		}
		else if (m_MoveIntoScreen < -100)
		{
			m_MoveIntoScreen = m_MoveIntoScreen + 40;
		}
		else if (m_MoveIntoScreen < -10)
		{
			m_MoveIntoScreen = m_MoveIntoScreen + 4;
		}
		else if (m_MoveIntoScreen < -1)
		{
			m_MoveIntoScreen = m_MoveIntoScreen + 0.4;
		}
		else if (m_MoveIntoScreen < -0.1)
		{
			m_MoveIntoScreen = m_MoveIntoScreen + 0.04;
		}
		else if (m_MoveIntoScreen < -0.01)
		{
			m_MoveIntoScreen = m_MoveIntoScreen + 0.004;
		}
		else if (m_MoveIntoScreen < -0.001)
		{
			m_MoveIntoScreen = m_MoveIntoScreen + 0.0004;
		}
	}
	updateGL();
}

void GLWidget::setNarrowNum()
{
	if (!m_isPerspective)
	{
		m_dZoomFactor = m_dZoomFactor - 0.1;
	}
	else
	{
		if (m_MoveIntoScreen < -1000)
		{
			m_MoveIntoScreen = m_MoveIntoScreen - 400;
		}
		else if (m_MoveIntoScreen < -100)
		{
			m_MoveIntoScreen = m_MoveIntoScreen - 40;
		}
		else if (m_MoveIntoScreen < -10)
		{
			m_MoveIntoScreen = m_MoveIntoScreen - 4;
		}
		else if (m_MoveIntoScreen < -1)
		{
			m_MoveIntoScreen = m_MoveIntoScreen - 0.4;
		}
		else if (m_MoveIntoScreen < -0.1)
		{
			m_MoveIntoScreen = m_MoveIntoScreen - 0.04;
		}
		else if (m_MoveIntoScreen < -0.01)
		{
			m_MoveIntoScreen = m_MoveIntoScreen - 0.004;
		}
		else if (m_MoveIntoScreen < -0.001)
		{
			m_MoveIntoScreen = m_MoveIntoScreen - 0.0004;
		}
	}
	updateGL();
}

void GLWidget::setOrtho()
{
	m_isPerspective = !m_isPerspective;
	//m_dZoomFactor = 1;
	//m_MoveIntoScreen = -1.3 * m_BoxDiagLine;					// 返回最初视角
	updateGL();
}

void GLWidget::setResetNum()
{
	Matrix3fSetIdentity(&LastRot);								// Reset Rotation
	Matrix3fSetIdentity(&ThisRot);								// Reset Rotation
	Matrix4fSetRotationFromMatrix3f(&Transform, &ThisRot);		// Reset Rotation
	m_MoveIntoScreen = -1.3 * m_BoxDiagLine;					// 返回最初视角
	m_dZoomFactor = 1;
	updateGL();
}

void GLWidget::setDrawshellNum()
{
	m_isdrawshell = !m_isdrawshell;
	updateGL();
}

void GLWidget::getBoxNum(double &BoxLength, double &BoxWidth, double &BoxHeight)
{
	BoxLength = m_BoxLength;
	BoxWidth  = m_BoxWidth;
	BoxHeight = m_BoxHeight;
}

void GLWidget::getCurrentPnt(int& PntNum)
{
	PntNum = m_CurrentDisplayPnts;
}