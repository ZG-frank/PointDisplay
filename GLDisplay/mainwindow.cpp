#include "mainwindow.h"

MainWindow::MainWindow(QWidget*parent) : QMainWindow(parent)
{
	// ������
	QToolBar* toobar = this->addToolBar(QString::fromLocal8Bit("������"));
	// Openѡ��
	QAction* QOpen = new QAction(QString::fromLocal8Bit("��"), toobar);
	QOpen->setCheckable(false);
	QOpen->setChecked(false);
	toobar->addAction(QOpen);
	QOpen->setToolTip(QString::fromLocal8Bit("���ļ�"));
	// Enlargeѡ��
	QAction* QEnlarge = new QAction(QString::fromLocal8Bit("�Ŵ�"), toobar);
	QEnlarge->setCheckable(false);
	QEnlarge->setChecked(false);
	toobar->addAction(QEnlarge);
	QEnlarge->setToolTip(QString::fromLocal8Bit("����Ŵ�"));
	// Narrowѡ��
	QAction* QNarrow = new QAction(QString::fromLocal8Bit("��С"), toobar);
	QNarrow->setCheckable(false);
	QNarrow->setChecked(false);
	toobar->addAction(QNarrow);
	QNarrow->setToolTip(QString::fromLocal8Bit("�����С"));
	// Orthoѡ��
	QAction* QOrtho = new QAction(QString::fromLocal8Bit("����ͶӰ"), toobar);
	QOrtho->setCheckable(true);
	QOrtho->setChecked(false);
	toobar->addAction(QOrtho);
	QOrtho->setToolTip(QString::fromLocal8Bit("�л�������ͶӰ"));
	// Resetѡ��
	QAction* QReset = new QAction(QString::fromLocal8Bit("����"), toobar);
	QReset->setCheckable(false);
	QReset->setChecked(false);
	toobar->addAction(QReset);
	QReset->setToolTip(QString::fromLocal8Bit("������ͼ"));
	// Drawshellѡ��
	QAction* QDrawshell = new QAction(QString::fromLocal8Bit("���"), toobar);
	QDrawshell->setCheckable(false);
	QDrawshell->setChecked(false);
	toobar->addAction(QDrawshell);
	QDrawshell->setToolTip(QString::fromLocal8Bit("��ʾ/�ر����"));
	// Aboutѡ��
	QAction* QAbout = new QAction(QString::fromLocal8Bit("����"), toobar);
	QAbout->setCheckable(false);
	QAbout->setChecked(false);
	toobar->addAction(QAbout);
	QAbout->setToolTip(QString::fromLocal8Bit("�������"));
	// ���Ĵ���
	m_GLWidget = new GLWidget();
	setCentralWidget(m_GLWidget);
	// ״̬�� 
	statusBar();
	numOfPoint = new QLabel(this);
	numOfDisplayPoint = new QLabel(this);
	BoxLength = new QLabel(this);
	BoxWidth = new QLabel(this);
	BoxHeight = new QLabel(this);
	// ��ʼ��ǩ
	numOfPoint->setText(QString::fromLocal8Bit("�ܵ���Ϊ:0  "));
	numOfDisplayPoint->setText(QString::fromLocal8Bit("��ǰ��ʾ����Ϊ:0  "));
	BoxLength->setText(QString::fromLocal8Bit("��򳤶�Ϊ:0  "));
	BoxWidth->setText(QString::fromLocal8Bit("�����Ϊ:0  "));
	BoxHeight->setText(QString::fromLocal8Bit("���߶�:0  "));
	// ������ʾ
	statusBar()->addPermanentWidget(numOfPoint);
	statusBar()->addPermanentWidget(numOfDisplayPoint);
	statusBar()->addPermanentWidget(BoxLength);
	statusBar()->addPermanentWidget(BoxWidth);
	statusBar()->addPermanentWidget(BoxHeight);
	// �źŲ�
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
			QMessageBox::warning(this, QString::fromLocal8Bit("���ļ�"), QString::fromLocal8Bit("���ܴ��ļ�\n%1").arg(PntFilepath));
			return;
		}
		double dTolX = 0;											// SumX
		double dTolY = 0;											// SumY
		double dTolZ = 0;											// SumZ
		double dMeanX = 0;											// MeanX
		double dMeanY = 0;											// MeanY
		double dMeanZ = 0;											// MeanZ
		QVector<Point3D> OriginPntsVec;								// �洢ԭʼ��
		// ��ȡ����
		QTextStream in(&file);
		while (!in.atEnd())
		{
			QString LinedataTemp = in.readLine();					// ��ȡһ�����ݣ���string���ͣ�
			QStringList list = LinedataTemp.split(QRegExp(" "));	// ��ÿһ����װ��һ��QStringList��
			bool bOK;
			Point3D PntsTemp;
			
			for (int i = 0; i < list.size(); i++)
			{
				QString strTemp = list[i];							// list�����ַ�
				double nval = strTemp.toDouble(&bOK);				// ת����double

				if (bOK && i == 0)
				{
					PntsTemp.dX = nval;
					dTolX = dTolX + PntsTemp.dX;					// ����TOTAL��X
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
					QMessageBox::warning(this, QString::fromLocal8Bit("����"),
						QString::fromLocal8Bit("���ݳ���"), QMessageBox::Ok);
					return;
				}
			}		
			OriginPntsVec.push_back(PntsTemp);					// put in Vec
		}
		dMeanX = dTolX / OriginPntsVec.size();					// ����MeanX
		dMeanY = dTolY / OriginPntsVec.size();					// ����MeanY
		dMeanZ = dTolZ / OriginPntsVec.size();					// ����MeanZ

		// ÿ���������(ʹ��ת���������������غ�)
		for (int i = 0; i < OriginPntsVec.size(); i++)
		{
			OriginPntsVec[i].dX = OriginPntsVec[i].dX - dMeanX;
			OriginPntsVec[i].dY = OriginPntsVec[i].dY - dMeanY;
			OriginPntsVec[i].dZ = OriginPntsVec[i].dZ - dMeanZ;
		}
		// �����ݴ��뵽GLWidget
		m_GLWidget->loadData(OriginPntsVec);
		file.close();
		// �õ��������
		double dBoxLength = 0;
		double dBoxWidth  = 0;
		double dBoxHeight = 0;
		int	nCurrentPnt = 0;
		m_GLWidget->getBoxNum(dBoxLength, dBoxWidth, dBoxHeight);
		m_GLWidget->getCurrentPnt(nCurrentPnt);
		// �ı�״̬��
		numOfPoint->setText(QString::fromLocal8Bit("�ܵ���Ϊ:") + QString::number(OriginPntsVec.size())		+ "                    ");
		numOfDisplayPoint->setText(QString::fromLocal8Bit("��ǰ��ʾ����Ϊ:") + QString::number(nCurrentPnt)	+ "                    ");
		BoxLength->setText(QString::fromLocal8Bit("��򳤶�Ϊ:") + QString::number(dBoxLength)				+ "                    ");
		BoxWidth->setText(QString::fromLocal8Bit("�����Ϊ:") + QString::number(dBoxHeight)				+ "                    ");
		BoxHeight->setText(QString::fromLocal8Bit("���߶�Ϊ:") + QString::number(dBoxWidth)				+ "                    ");
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
	QMessageBox::information(this, QString::fromLocal8Bit("�����������"), QString::fromLocal8Bit(
		"������ƣ�3D��ʾ\n"
		"����϶���ת\n"
		"�Ҽ�������س�ʼ��С�ͳ�ʼ�ӽ�\n"
		"�����ֿ���ͼ�εķŴ���С\n"
		"������ݼ���\n"
		"Q��͸�������ͶӰ\n"
		"W����ʾ��ر����\n"
		"�����Ŵ�\n"
		"������С\n"));
}

MainWindow :: ~MainWindow()
{

}

