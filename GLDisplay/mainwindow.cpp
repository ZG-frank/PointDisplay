#include "mainwindow.h"

MainWindow::MainWindow(QWidget*parent) : QMainWindow(parent)
{
	// 工具栏
	QToolBar* toobar = this->addToolBar(QString::fromLocal8Bit("工具栏"));
	// Open选项
	QAction* QOpen = new QAction(QString::fromLocal8Bit("打开"), toobar);
	QOpen->setCheckable(false);
	QOpen->setChecked(false);
	toobar->addAction(QOpen);
	QOpen->setToolTip(QString::fromLocal8Bit("打开文件"));
	// Enlarge选项
	QAction* QEnlarge = new QAction(QString::fromLocal8Bit("放大"), toobar);
	QEnlarge->setCheckable(false);
	QEnlarge->setChecked(false);
	toobar->addAction(QEnlarge);
	QEnlarge->setToolTip(QString::fromLocal8Bit("点击放大"));
	// Narrow选项
	QAction* QNarrow = new QAction(QString::fromLocal8Bit("缩小"), toobar);
	QNarrow->setCheckable(false);
	QNarrow->setChecked(false);
	toobar->addAction(QNarrow);
	QNarrow->setToolTip(QString::fromLocal8Bit("点击缩小"));
	// Ortho选项
	QAction* QOrtho = new QAction(QString::fromLocal8Bit("正射投影"), toobar);
	QOrtho->setCheckable(true);
	QOrtho->setChecked(false);
	toobar->addAction(QOrtho);
	QOrtho->setToolTip(QString::fromLocal8Bit("切换到正射投影"));
	// Reset选项
	QAction* QReset = new QAction(QString::fromLocal8Bit("重置"), toobar);
	QReset->setCheckable(false);
	QReset->setChecked(false);
	toobar->addAction(QReset);
	QReset->setToolTip(QString::fromLocal8Bit("重置视图"));
	// Drawshell选项
	QAction* QDrawshell = new QAction(QString::fromLocal8Bit("外框"), toobar);
	QDrawshell->setCheckable(false);
	QDrawshell->setChecked(false);
	toobar->addAction(QDrawshell);
	QDrawshell->setToolTip(QString::fromLocal8Bit("显示/关闭外框"));
	// About选项
	QAction* QAbout = new QAction(QString::fromLocal8Bit("关于"), toobar);
	QAbout->setCheckable(false);
	QAbout->setChecked(false);
	toobar->addAction(QAbout);
	QAbout->setToolTip(QString::fromLocal8Bit("关于软件"));
	// 中心窗口
	m_GLWidget = new GLWidget();
	setCentralWidget(m_GLWidget);
	// 状态栏 
	statusBar();
	numOfPoint = new QLabel(this);
	numOfDisplayPoint = new QLabel(this);
	BoxLength = new QLabel(this);
	BoxWidth = new QLabel(this);
	BoxHeight = new QLabel(this);
	// 初始标签
	numOfPoint->setText(QString::fromLocal8Bit("总点数为:0  "));
	numOfDisplayPoint->setText(QString::fromLocal8Bit("当前显示点数为:0  "));
	BoxLength->setText(QString::fromLocal8Bit("外框长度为:0  "));
	BoxWidth->setText(QString::fromLocal8Bit("外框宽度为:0  "));
	BoxHeight->setText(QString::fromLocal8Bit("外框高度:0  "));
	// 永久显示
	statusBar()->addPermanentWidget(numOfPoint);
	statusBar()->addPermanentWidget(numOfDisplayPoint);
	statusBar()->addPermanentWidget(BoxLength);
	statusBar()->addPermanentWidget(BoxWidth);
	statusBar()->addPermanentWidget(BoxHeight);
	// 信号槽
	QObject::connect(QOpen, SIGNAL(triggered()), this, SLOT(OpenPntFils()));
	QObject::connect(QAbout, SIGNAL(triggered()), this, SLOT(Aboutsof()));

	QObject::connect(QEnlarge, SIGNAL(triggered()), this, SLOT(ClickedEnlargeBtn()));
	QObject::connect(this, SIGNAL(ClickedEnlarge()), m_GLWidget, SLOT(setEnlargeNum()));

	QObject::connect(QNarrow, SIGNAL(triggered()), this, SLOT(ClickedNarrowBtn()));
	QObject::connect(this, SIGNAL(ClickedNarrow()), m_GLWidget, SLOT(setNarrowNum()));

	QObject::connect(QOrtho, SIGNAL(triggered()), this, SLOT(ClickedOrthoBtn()));
	QObject::connect(this, SIGNAL(ClickedOrtho()), m_GLWidget, SLOT(setOrtho()));

	QObject::connect(QReset, SIGNAL(triggered()), this, SLOT(ClickedResetBtn()));
	QObject::connect(this, SIGNAL(ClickedReset()), m_GLWidget, SLOT(setResetNum()));

	QObject::connect(QDrawshell, SIGNAL(triggered()), this, SLOT(ClickeddrawshellBtn()));
	QObject::connect(this, SIGNAL(Clickeddrawshell()), m_GLWidget, SLOT(setDrawshellNum()));
}

void MainWindow::OpenPntFils()
{
	PntFilepath = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("Text Files(*.txt)"));
	if (!PntFilepath.isEmpty())
	{
		QFile file(PntFilepath);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QMessageBox::warning(this, QString::fromLocal8Bit("打开文件"), QString::fromLocal8Bit("不能打开文件\n%1").arg(PntFilepath));
			return;
		}
		double dTolX = 0;											// SumX
		double dTolY = 0;											// SumY
		double dTolZ = 0;											// SumZ
		double dMeanX = 0;											// MeanX
		double dMeanY = 0;											// MeanY
		double dMeanZ = 0;											// MeanZ
		QVector<Point3D> OriginPntsVec;								// 存储原始点
		// 读取数据
		QTextStream in(&file);
		while (!in.atEnd())
		{
			QString LinedataTemp = in.readLine();					// 读取一行数据（是string类型）
			QStringList list = LinedataTemp.split(QRegExp(" "));	// 把每一个块装进一个QStringList中
			bool bOK;
			Point3D PntsTemp;
			
			for (int i = 0; i < list.size(); i++)
			{
				QString strTemp = list[i];							// list里是字符
				double nval = strTemp.toDouble(&bOK);				// 转换成double

				if (bOK && i == 0)
				{
					PntsTemp.dX = nval;
					dTolX = dTolX + PntsTemp.dX;					// 计算TOTAL的X
				}
				else if (bOK && i == 1)
				{
					PntsTemp.dZ = nval; 
					dTolZ = dTolZ + PntsTemp.dZ;
				}
				else if (bOK && i == 2)
				{
					PntsTemp.dY = nval;
					dTolY = dTolY + PntsTemp.dY;
				}
				
				else if (!bOK || i<0 )
				{
					QMessageBox::warning(this, QString::fromLocal8Bit("警告"),
						QString::fromLocal8Bit("内容出错"), QMessageBox::Ok);
					return;
				}
			}		
			OriginPntsVec.push_back(PntsTemp);					// put in Vec
		}
		dMeanX = dTolX / OriginPntsVec.size();					// 计算MeanX
		dMeanY = dTolY / OriginPntsVec.size();					// 计算MeanY
		dMeanZ = dTolZ / OriginPntsVec.size();					// 计算MeanZ

		// 每个点减重心(使旋转中心与坐标中心重合)
		for (int i = 0; i < OriginPntsVec.size(); i++)
		{
			OriginPntsVec[i].dX = OriginPntsVec[i].dX - dMeanX;
			OriginPntsVec[i].dY = OriginPntsVec[i].dY - dMeanY;
			OriginPntsVec[i].dZ = OriginPntsVec[i].dZ - dMeanZ;
		}
		// 将数据传入到GLWidget
		m_GLWidget->loadData(OriginPntsVec);
		file.close();
		// 得到外框数据
		double dBoxLength = 0;
		double dBoxWidth  = 0;
		double dBoxHeight = 0;
		int	nCurrentPnt = 0;
		m_GLWidget->getBoxNum(dBoxLength, dBoxWidth, dBoxHeight);
		m_GLWidget->getCurrentPnt(nCurrentPnt);
		// 改变状态栏
		numOfPoint->setText(QString::fromLocal8Bit("总点数为:") + QString::number(OriginPntsVec.size())		+ "                    ");
		numOfDisplayPoint->setText(QString::fromLocal8Bit("当前显示点数为:") + QString::number(nCurrentPnt)	+ "                    ");
		BoxLength->setText(QString::fromLocal8Bit("外框长度为:") + QString::number(dBoxLength)				+ "                    ");
		BoxWidth->setText(QString::fromLocal8Bit("外框宽度为:") + QString::number(dBoxHeight)				+ "                    ");
		BoxHeight->setText(QString::fromLocal8Bit("外框高度为:") + QString::number(dBoxWidth)				+ "                    ");
	}
}

void MainWindow::ClickedEnlargeBtn()
{
	emit ClickedEnlarge();
}

void MainWindow::ClickedNarrowBtn()
{
	emit ClickedNarrow();
}

void MainWindow::ClickedOrthoBtn()
{
	emit ClickedOrtho();
}

void MainWindow::ClickedResetBtn()
{
	emit ClickedReset();
}

void MainWindow::ClickeddrawshellBtn()
{
	emit Clickeddrawshell();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	event->accept();
}

void MainWindow::Aboutsof()
{
	QMessageBox::information(this, QString::fromLocal8Bit("关于软件操作"), QString::fromLocal8Bit(
		"软件名称：3D显示\n"
		"左键拖动旋转\n"
		"右键点击返回初始大小和初始视角\n"
		"鼠标滚轮控制图形的放大缩小\n"
		"操作快捷键：\n"
		"Q：透射或正射投影\n"
		"W：显示或关闭外框\n"
		"↑：放大\n"
		"↓：缩小\n"));
}

MainWindow :: ~MainWindow()
{

}

