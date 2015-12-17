/********************************************************************************
** Form generated from reading UI file 'GLDisplay.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GLDISPLAY_H
#define UI_GLDISPLAY_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GLDisplayClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *GLDisplayClass)
    {
        if (GLDisplayClass->objectName().isEmpty())
            GLDisplayClass->setObjectName(QStringLiteral("GLDisplayClass"));
        GLDisplayClass->resize(600, 400);
        menuBar = new QMenuBar(GLDisplayClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        GLDisplayClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(GLDisplayClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        GLDisplayClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(GLDisplayClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        GLDisplayClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(GLDisplayClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        GLDisplayClass->setStatusBar(statusBar);

        retranslateUi(GLDisplayClass);

        QMetaObject::connectSlotsByName(GLDisplayClass);
    } // setupUi

    void retranslateUi(QMainWindow *GLDisplayClass)
    {
        GLDisplayClass->setWindowTitle(QApplication::translate("GLDisplayClass", "GLDisplay", 0));
    } // retranslateUi

};

namespace Ui {
    class GLDisplayClass: public Ui_GLDisplayClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GLDISPLAY_H
