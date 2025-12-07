/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actiongbhjkn;
    QAction *actioncvb;
    QAction *actioncfgvhjb;
    QWidget *centralwidget;
    QHBoxLayout *centralLayout;
    QSplitter *mainSplitter;
    QFrame *explorerPanel;
    QVBoxLayout *explorerLayout;
    QLabel *explorerLabel;
    QTreeWidget *explorerTree;
    QSplitter *rightSplitter;
    QFrame *queryPanel;
    QVBoxLayout *queryLayout;
    QTabWidget *tabWidget;
    QWidget *widget;
    QWidget *tab_2;
    QTextEdit *textEdit;
    QFrame *frame;
    QTabWidget *bottomTabs;
    QWidget *resultsTab_2;
    QVBoxLayout *resultsLayout_2;
    QTableWidget *resultsTable_2;
    QWidget *errorsTab_2;
    QVBoxLayout *errorsLayout_2;
    QTextEdit *errorMessages_2;
    QMenuBar *menubar;
    QAction *actionRun;
    QMenu *menuRun;
    QMenu *menuEdit;
    QMenu *menuView;
    QMenu *menuhelp;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(793, 526);
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::WindowText, brush);
        QBrush brush1(QColor(219, 203, 167, 255));
        brush1.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Button, brush1);
        QBrush brush2(QColor(255, 255, 255, 255));
        brush2.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Light, brush2);
        QBrush brush3(QColor(245, 243, 237, 255));
        brush3.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Midlight, brush3);
        QBrush brush4(QColor(118, 116, 110, 255));
        brush4.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Dark, brush4);
        QBrush brush5(QColor(157, 154, 147, 255));
        brush5.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Mid, brush5);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Text, brush);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::BrightText, brush2);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::ButtonText, brush);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Base, brush1);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Window, brush1);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Shadow, brush);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::AlternateBase, brush3);
        QBrush brush6(QColor(255, 255, 220, 255));
        brush6.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::ToolTipBase, brush6);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::ToolTipText, brush);
        QBrush brush7(QColor(0, 0, 0, 127));
        brush7.setStyle(Qt::BrushStyle::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::PlaceholderText, brush7);
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Accent, brush2);
#endif
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::WindowText, brush);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Button, brush1);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Light, brush2);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Midlight, brush3);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Dark, brush4);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Mid, brush5);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Text, brush);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::BrightText, brush2);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::ButtonText, brush);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Base, brush1);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Window, brush1);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Shadow, brush);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::AlternateBase, brush3);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::ToolTipBase, brush6);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::PlaceholderText, brush7);
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Accent, brush2);
#endif
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::WindowText, brush4);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Button, brush1);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Light, brush2);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Midlight, brush3);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Dark, brush4);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Mid, brush5);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Text, brush4);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::BrightText, brush2);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::ButtonText, brush4);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Base, brush1);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Window, brush1);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Shadow, brush);
        QBrush brush8(QColor(236, 231, 220, 255));
        brush8.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::AlternateBase, brush8);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::ToolTipBase, brush6);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::ToolTipText, brush);
        QBrush brush9(QColor(118, 116, 110, 127));
        brush9.setStyle(Qt::BrushStyle::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::PlaceholderText, brush9);
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Accent, brush2);
#endif
        MainWindow->setPalette(palette);
        MainWindow->setStyleSheet(QString::fromUtf8("background-color: #dbcba7;"));
        actiongbhjkn = new QAction(MainWindow);
        actiongbhjkn->setObjectName("actiongbhjkn");
        actioncvb = new QAction(MainWindow);
        actioncvb->setObjectName("actioncvb");
        actioncfgvhjb = new QAction(MainWindow);
        actioncfgvhjb->setObjectName("actioncfgvhjb");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        centralwidget->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicy);
        QPalette palette1;
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::WindowText, brush);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Button, brush1);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Light, brush2);
        QBrush brush10(QColor(240, 251, 252, 255));
        brush10.setStyle(Qt::BrushStyle::SolidPattern);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Midlight, brush10);
        QBrush brush11(QColor(113, 124, 125, 255));
        brush11.setStyle(Qt::BrushStyle::SolidPattern);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Dark, brush11);
        QBrush brush12(QColor(150, 165, 167, 255));
        brush12.setStyle(Qt::BrushStyle::SolidPattern);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Mid, brush12);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Text, brush);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::BrightText, brush2);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::ButtonText, brush);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Base, brush1);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Window, brush1);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Shadow, brush);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::AlternateBase, brush10);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::ToolTipBase, brush6);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::PlaceholderText, brush7);
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Accent, brush2);
#endif
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::WindowText, brush);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Button, brush1);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Light, brush2);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Midlight, brush10);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Dark, brush11);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Mid, brush12);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Text, brush);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::BrightText, brush2);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::ButtonText, brush);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Base, brush1);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Window, brush1);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Shadow, brush);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::AlternateBase, brush10);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::ToolTipBase, brush6);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::PlaceholderText, brush7);
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Accent, brush2);
#endif
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::WindowText, brush11);
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Button, brush1);
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Light, brush2);
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Midlight, brush10);
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Dark, brush11);
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Mid, brush12);
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Text, brush11);
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::BrightText, brush2);
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::ButtonText, brush11);
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Base, brush1);
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Window, brush1);
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Shadow, brush);
        QBrush brush13(QColor(225, 247, 250, 255));
        brush13.setStyle(Qt::BrushStyle::SolidPattern);
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::AlternateBase, brush13);
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::ToolTipBase, brush6);
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::ToolTipText, brush);
        QBrush brush14(QColor(113, 124, 125, 127));
        brush14.setStyle(Qt::BrushStyle::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::PlaceholderText, brush14);
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
        palette1.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Accent, brush2);
#endif
        centralwidget->setPalette(palette1);
        QFont font;
        font.setKerning(true);
        centralwidget->setFont(font);
        centralLayout = new QHBoxLayout(centralwidget);
        centralLayout->setObjectName("centralLayout");
        mainSplitter = new QSplitter(centralwidget);
        mainSplitter->setObjectName("mainSplitter");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(mainSplitter->sizePolicy().hasHeightForWidth());
        mainSplitter->setSizePolicy(sizePolicy1);
        mainSplitter->setFrameShape(QFrame::Shape::NoFrame);
        mainSplitter->setFrameShadow(QFrame::Shadow::Plain);
        mainSplitter->setLineWidth(0);
        mainSplitter->setOrientation(Qt::Orientation::Horizontal);
        mainSplitter->setOpaqueResize(true);
        mainSplitter->setHandleWidth(6);
        explorerPanel = new QFrame(mainSplitter);
        explorerPanel->setObjectName("explorerPanel");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy2.setHorizontalStretch(1);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(explorerPanel->sizePolicy().hasHeightForWidth());
        explorerPanel->setSizePolicy(sizePolicy2);
        explorerPanel->setMinimumSize(QSize(100, 0));
        QPalette palette2;
        palette2.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::WindowText, brush);
        QBrush brush15(QColor(245, 245, 245, 255));
        brush15.setStyle(Qt::BrushStyle::SolidPattern);
        palette2.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Button, brush15);
        palette2.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Light, brush2);
        QBrush brush16(QColor(253, 249, 239, 255));
        brush16.setStyle(Qt::BrushStyle::SolidPattern);
        palette2.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Midlight, brush16);
        QBrush brush17(QColor(126, 122, 111, 255));
        brush17.setStyle(Qt::BrushStyle::SolidPattern);
        palette2.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Dark, brush17);
        QBrush brush18(QColor(168, 162, 149, 255));
        brush18.setStyle(Qt::BrushStyle::SolidPattern);
        palette2.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Mid, brush18);
        palette2.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Text, brush);
        palette2.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::BrightText, brush2);
        palette2.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::ButtonText, brush);
        palette2.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Base, brush15);
        palette2.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Window, brush15);
        palette2.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Shadow, brush);
        palette2.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::AlternateBase, brush16);
        palette2.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::ToolTipBase, brush6);
        palette2.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette2.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::PlaceholderText, brush7);
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
        palette2.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Accent, brush2);
#endif
        palette2.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::WindowText, brush);
        palette2.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Button, brush15);
        palette2.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Light, brush2);
        palette2.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Midlight, brush16);
        palette2.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Dark, brush17);
        palette2.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Mid, brush18);
        palette2.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Text, brush);
        palette2.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::BrightText, brush2);
        palette2.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::ButtonText, brush);
        palette2.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Base, brush15);
        palette2.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Window, brush15);
        palette2.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Shadow, brush);
        palette2.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::AlternateBase, brush16);
        palette2.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::ToolTipBase, brush6);
        palette2.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette2.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::PlaceholderText, brush7);
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
        palette2.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Accent, brush2);
#endif
        palette2.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::WindowText, brush17);
        palette2.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Button, brush15);
        palette2.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Light, brush2);
        palette2.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Midlight, brush16);
        palette2.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Dark, brush17);
        palette2.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Mid, brush18);
        palette2.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Text, brush17);
        palette2.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::BrightText, brush2);
        palette2.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::ButtonText, brush17);
        palette2.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Base, brush15);
        palette2.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Window, brush15);
        palette2.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Shadow, brush);
        QBrush brush19(QColor(252, 243, 223, 255));
        brush19.setStyle(Qt::BrushStyle::SolidPattern);
        palette2.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::AlternateBase, brush19);
        palette2.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::ToolTipBase, brush6);
        palette2.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::ToolTipText, brush);
        QBrush brush20(QColor(126, 122, 111, 127));
        brush20.setStyle(Qt::BrushStyle::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette2.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::PlaceholderText, brush20);
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
        palette2.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Accent, brush2);
#endif
        explorerPanel->setPalette(palette2);
        explorerPanel->setToolTipDuration(-2);
        explorerPanel->setAutoFillBackground(false);
        explorerPanel->setStyleSheet(QString::fromUtf8("background-color: #F5F5F5; border: 1px solid #CCCCCC; border-radius: 6px;"));
        explorerPanel->setFrameShape(QFrame::Shape::Box);
        explorerLayout = new QVBoxLayout(explorerPanel);
        explorerLayout->setObjectName("explorerLayout");
        explorerLayout->setSizeConstraint(QLayout::SizeConstraint::SetDefaultConstraint);
        explorerLabel = new QLabel(explorerPanel);
        explorerLabel->setObjectName("explorerLabel");
        sizePolicy.setHeightForWidth(explorerLabel->sizePolicy().hasHeightForWidth());
        explorerLabel->setSizePolicy(sizePolicy);
        QFont font1;
        explorerLabel->setFont(font1);
        explorerLabel->setStyleSheet(QString::fromUtf8("color: #000000; background-color: transparent; border:0px; "));

        explorerLayout->addWidget(explorerLabel);

        explorerTree = new QTreeWidget(explorerPanel);
        explorerTree->setObjectName("explorerTree");
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(explorerTree->sizePolicy().hasHeightForWidth());
        explorerTree->setSizePolicy(sizePolicy3);
        explorerTree->setMaximumSize(QSize(16777215, 16777215));
        explorerTree->setStyleSheet(QString::fromUtf8("background-color: #FFFFFF; alternate-background-color: #F0F0F0; gridline-color: #D0D0D0;"));

        explorerLayout->addWidget(explorerTree);

        mainSplitter->addWidget(explorerPanel);
        rightSplitter = new QSplitter(mainSplitter);
        rightSplitter->setObjectName("rightSplitter");
        QSizePolicy sizePolicy4(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy4.setHorizontalStretch(4);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(rightSplitter->sizePolicy().hasHeightForWidth());
        rightSplitter->setSizePolicy(sizePolicy4);
        rightSplitter->setOrientation(Qt::Orientation::Vertical);
        queryPanel = new QFrame(rightSplitter);
        queryPanel->setObjectName("queryPanel");
        QPalette palette3;
        palette3.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::WindowText, brush);
        palette3.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Button, brush2);
        palette3.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Light, brush2);
        QBrush brush21(QColor(251, 251, 246, 255));
        brush21.setStyle(Qt::BrushStyle::SolidPattern);
        palette3.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Midlight, brush21);
        QBrush brush22(QColor(124, 124, 118, 255));
        brush22.setStyle(Qt::BrushStyle::SolidPattern);
        palette3.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Dark, brush22);
        QBrush brush23(QColor(165, 165, 158, 255));
        brush23.setStyle(Qt::BrushStyle::SolidPattern);
        palette3.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Mid, brush23);
        palette3.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Text, brush);
        palette3.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::BrightText, brush2);
        palette3.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::ButtonText, brush);
        palette3.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Base, brush2);
        palette3.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Window, brush2);
        palette3.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Shadow, brush);
        palette3.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::AlternateBase, brush21);
        palette3.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::ToolTipBase, brush6);
        palette3.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette3.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::PlaceholderText, brush7);
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
        palette3.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Accent, brush2);
#endif
        palette3.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::WindowText, brush);
        palette3.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Button, brush2);
        palette3.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Light, brush2);
        palette3.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Midlight, brush21);
        palette3.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Dark, brush22);
        palette3.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Mid, brush23);
        palette3.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Text, brush);
        palette3.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::BrightText, brush2);
        palette3.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::ButtonText, brush);
        palette3.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Base, brush2);
        palette3.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Window, brush2);
        palette3.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Shadow, brush);
        palette3.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::AlternateBase, brush21);
        palette3.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::ToolTipBase, brush6);
        palette3.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette3.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::PlaceholderText, brush7);
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
        palette3.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Accent, brush2);
#endif
        palette3.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::WindowText, brush22);
        palette3.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Button, brush2);
        palette3.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Light, brush2);
        palette3.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Midlight, brush21);
        palette3.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Dark, brush22);
        palette3.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Mid, brush23);
        palette3.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Text, brush22);
        palette3.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::BrightText, brush2);
        palette3.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::ButtonText, brush22);
        palette3.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Base, brush2);
        palette3.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Window, brush2);
        palette3.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Shadow, brush);
        QBrush brush24(QColor(248, 247, 237, 255));
        brush24.setStyle(Qt::BrushStyle::SolidPattern);
        palette3.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::AlternateBase, brush24);
        palette3.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::ToolTipBase, brush6);
        palette3.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::ToolTipText, brush);
        QBrush brush25(QColor(124, 124, 118, 127));
        brush25.setStyle(Qt::BrushStyle::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette3.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::PlaceholderText, brush25);
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
        palette3.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Accent, brush2);
#endif
        queryPanel->setPalette(palette3);
        queryPanel->setStyleSheet(QString::fromUtf8("background-color: #FFFFFF; border: 1px solid #CCCCCC;"));
        queryPanel->setFrameShape(QFrame::Shape::StyledPanel);
        queryLayout = new QVBoxLayout(queryPanel);
        queryLayout->setObjectName("queryLayout");
        tabWidget = new QTabWidget(queryPanel);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setEnabled(true);
        QSizePolicy sizePolicy5(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(3);
        sizePolicy5.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy5);
        tabWidget->setMinimumSize(QSize(0, 300));
        tabWidget->setTabShape(QTabWidget::TabShape::Triangular);
        tabWidget->setTabsClosable(true);
        tabWidget->setMovable(true);
        widget = new QWidget();
        widget->setObjectName("widget");
        tabWidget->addTab(widget, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        textEdit = new QTextEdit(tab_2);
        textEdit->setObjectName("textEdit");
        textEdit->setGeometry(QRect(3, 3, 351, 191));
        QSizePolicy sizePolicy6(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy6.setHorizontalStretch(1);
        sizePolicy6.setVerticalStretch(1);
        sizePolicy6.setHeightForWidth(textEdit->sizePolicy().hasHeightForWidth());
        textEdit->setSizePolicy(sizePolicy6);
        textEdit->setAutoFillBackground(true);
        tabWidget->addTab(tab_2, QString());

        queryLayout->addWidget(tabWidget);

        rightSplitter->addWidget(queryPanel);
        frame = new QFrame(rightSplitter);
        frame->setObjectName("frame");
        QSizePolicy sizePolicy7(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy7.setHorizontalStretch(0);
        sizePolicy7.setVerticalStretch(2);
        sizePolicy7.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy7);
        frame->setMinimumSize(QSize(0, 100));
        frame->setAutoFillBackground(false);
        frame->setStyleSheet(QString::fromUtf8("background-color:#f0e6c5; border-raidus:15px;"));
        frame->setFrameShape(QFrame::Shape::NoFrame);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        bottomTabs = new QTabWidget(frame);
        bottomTabs->setObjectName("bottomTabs");
        bottomTabs->setGeometry(QRect(0, 0, 386, 131));
        QSizePolicy sizePolicy8(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy8.setHorizontalStretch(0);
        sizePolicy8.setVerticalStretch(0);
        sizePolicy8.setHeightForWidth(bottomTabs->sizePolicy().hasHeightForWidth());
        bottomTabs->setSizePolicy(sizePolicy8);
        bottomTabs->setMinimumSize(QSize(0, 100));
        bottomTabs->setStyleSheet(QString::fromUtf8("QTabWidget::pane { border: 1px solid #CCCCCC; border-radius: 6px; background-color: #FFF; }\n"
"QTabBar::tab { background: #f0e6c5; border: 1px solid #CCCCCC; border-bottom-color: #E0E0E0; padding: 5px; color: #000000; border-radius: 0 6 6 0  }\n"
"QTabBar::tab:selected { background: #FFFFFF; border-bottom-color: #FFFFFF; border-top: 2px solid #4A90E2; color: #000000; font-weight: bold; }"));
        bottomTabs->setTabPosition(QTabWidget::TabPosition::South);
        bottomTabs->setTabShape(QTabWidget::TabShape::Rounded);
        bottomTabs->setElideMode(Qt::TextElideMode::ElideMiddle);
        bottomTabs->setTabsClosable(false);
        resultsTab_2 = new QWidget();
        resultsTab_2->setObjectName("resultsTab_2");
        resultsLayout_2 = new QVBoxLayout(resultsTab_2);
        resultsLayout_2->setObjectName("resultsLayout_2");
        resultsTable_2 = new QTableWidget(resultsTab_2);
        resultsTable_2->setObjectName("resultsTable_2");
        QPalette palette4;
        QBrush brush26(QColor(240, 230, 197, 255));
        brush26.setStyle(Qt::BrushStyle::SolidPattern);
        palette4.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Button, brush26);
        palette4.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Base, brush26);
        palette4.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Window, brush26);
        palette4.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Button, brush26);
        palette4.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Base, brush26);
        palette4.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Window, brush26);
        palette4.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Button, brush26);
        palette4.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Base, brush26);
        palette4.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Window, brush26);
        resultsTable_2->setPalette(palette4);

        resultsLayout_2->addWidget(resultsTable_2);

        bottomTabs->addTab(resultsTab_2, QString());
        errorsTab_2 = new QWidget();
        errorsTab_2->setObjectName("errorsTab_2");
        errorsTab_2->setStyleSheet(QString::fromUtf8("background-color: #FFFFFF; border: none; QTabBar::tab:selected { font-weight: bold; };"));
        errorsLayout_2 = new QVBoxLayout(errorsTab_2);
        errorsLayout_2->setObjectName("errorsLayout_2");
        errorMessages_2 = new QTextEdit(errorsTab_2);
        errorMessages_2->setObjectName("errorMessages_2");
        errorMessages_2->setAutoFillBackground(false);
        errorMessages_2->setStyleSheet(QString::fromUtf8("\n"
"              QTextEdit {\n"
"               background: #fff8f8;\n"
"               border: 1px solid #e6b8b8;\n"
"               color: #a33;\n"
"               font-family: Consolas;\n"
"              }\n"
"             "));
        errorMessages_2->setFrameShape(QFrame::Shape::NoFrame);
        errorMessages_2->setLineWidth(0);
        errorMessages_2->setReadOnly(true);

        errorsLayout_2->addWidget(errorMessages_2);

        bottomTabs->addTab(errorsTab_2, QString());
        rightSplitter->addWidget(frame);
        mainSplitter->addWidget(rightSplitter);

        centralLayout->addWidget(mainSplitter);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 793, 33));
        actionRun = new QAction(menubar);
        actionRun->setObjectName("actionRun");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/run.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionRun->setIcon(icon);
        menuRun = new QMenu(menubar);
        menuRun->setObjectName("menuRun");
        menuEdit = new QMenu(menubar);
        menuEdit->setObjectName("menuEdit");
        menuView = new QMenu(menubar);
        menuView->setObjectName("menuView");
        menuhelp = new QMenu(menubar);
        menuhelp->setObjectName("menuhelp");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuRun->menuAction());
        menubar->addAction(menuEdit->menuAction());
        menubar->addAction(menuView->menuAction());
        menubar->addAction(menuhelp->menuAction());

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);
        bottomTabs->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "SQL Studio", nullptr));
        actiongbhjkn->setText(QCoreApplication::translate("MainWindow", "gbhjkn", nullptr));
        actioncvb->setText(QCoreApplication::translate("MainWindow", "cvb", nullptr));
        actioncfgvhjb->setText(QCoreApplication::translate("MainWindow", "cfgvhjb'", nullptr));
        explorerLabel->setText(QCoreApplication::translate("MainWindow", "Object Explorer", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = explorerTree->headerItem();
        ___qtreewidgetitem->setText(0, QCoreApplication::translate("MainWindow", "Servers", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(widget), QCoreApplication::translate("MainWindow", "File", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "Tab 2", nullptr));
        bottomTabs->setTabText(bottomTabs->indexOf(resultsTab_2), QCoreApplication::translate("MainWindow", "Results", nullptr));
        bottomTabs->setTabText(bottomTabs->indexOf(errorsTab_2), QCoreApplication::translate("MainWindow", "Messages", nullptr));
        actionRun->setText(QCoreApplication::translate("MainWindow", "Run", nullptr));
        menuRun->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        menuEdit->setTitle(QCoreApplication::translate("MainWindow", "Edit", nullptr));
        menuView->setTitle(QCoreApplication::translate("MainWindow", "View", nullptr));
        menuhelp->setTitle(QCoreApplication::translate("MainWindow", "help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
