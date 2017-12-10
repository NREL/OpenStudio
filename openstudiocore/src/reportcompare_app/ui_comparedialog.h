/********************************************************************************
** Form generated from reading UI file 'comparedialog.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COMPAREDIALOG_H
#define UI_COMPAREDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <dropfilebutton.h>
#include <QWebEngineView>

QT_BEGIN_NAMESPACE

class Ui_CompareDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    DropFileButton *btAddCmpPath;
    QLabel *lbInfo;
    QCheckBox *chkShowTree;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_2;
    QWidget *wLeft;
    QHBoxLayout *horizontalLayout_3;
    QWebEngineView *webView;
    QWidget *wCenter;
    QHBoxLayout *horizontalLayout_4;
    QWebEngineView *webViewDebug;
    QWidget *wRight;
    QHBoxLayout *horizontalLayout_5;
    QWebEngineView *webView2;

    void setupUi(QDialog *CompareDialog)
    {
        if (CompareDialog->objectName().isEmpty())
            CompareDialog->setObjectName(QStringLiteral("CompareDialog"));
        CompareDialog->resize(1009, 717);
        verticalLayout_2 = new QVBoxLayout(CompareDialog);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(3);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(3, 3, 3, 3);
        widget = new QWidget(CompareDialog);
        widget->setObjectName(QStringLiteral("widget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        btAddCmpPath = new DropFileButton(widget);
        btAddCmpPath->setObjectName(QStringLiteral("btAddCmpPath"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(btAddCmpPath->sizePolicy().hasHeightForWidth());
        btAddCmpPath->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(btAddCmpPath);

        lbInfo = new QLabel(widget);
        lbInfo->setObjectName(QStringLiteral("lbInfo"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(lbInfo->sizePolicy().hasHeightForWidth());
        lbInfo->setSizePolicy(sizePolicy2);
        lbInfo->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(lbInfo);

        chkShowTree = new QCheckBox(widget);
        chkShowTree->setObjectName(QStringLiteral("chkShowTree"));
        sizePolicy1.setHeightForWidth(chkShowTree->sizePolicy().hasHeightForWidth());
        chkShowTree->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(chkShowTree);


        verticalLayout->addWidget(widget);

        widget_2 = new QWidget(CompareDialog);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        horizontalLayout_2 = new QHBoxLayout(widget_2);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        wLeft = new QWidget(widget_2);
        wLeft->setObjectName(QStringLiteral("wLeft"));
        horizontalLayout_3 = new QHBoxLayout(wLeft);
        horizontalLayout_3->setSpacing(3);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        webView = new QWebEngineView(wLeft);
        webView->setObjectName(QStringLiteral("webView"));

        horizontalLayout_3->addWidget(webView);


        horizontalLayout_2->addWidget(wLeft);

        wCenter = new QWidget(widget_2);
        wCenter->setObjectName(QStringLiteral("wCenter"));
        horizontalLayout_4 = new QHBoxLayout(wCenter);
        horizontalLayout_4->setSpacing(3);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        webViewDebug = new QWebEngineView(wCenter);
        webViewDebug->setObjectName(QStringLiteral("webViewDebug"));

        horizontalLayout_4->addWidget(webViewDebug);


        horizontalLayout_2->addWidget(wCenter);

        wRight = new QWidget(widget_2);
        wRight->setObjectName(QStringLiteral("wRight"));
        wRight->setEnabled(true);
        horizontalLayout_5 = new QHBoxLayout(wRight);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        webView2 = new QWebEngineView(wRight);
        webView2->setObjectName(QStringLiteral("webView2"));

        horizontalLayout_5->addWidget(webView2);


        horizontalLayout_2->addWidget(wRight);


        verticalLayout->addWidget(widget_2);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(CompareDialog);

        QMetaObject::connectSlotsByName(CompareDialog);
    } // setupUi

    void retranslateUi(QDialog *CompareDialog)
    {
        CompareDialog->setWindowTitle(QApplication::translate("CompareDialog", "Result compare", 0));
        btAddCmpPath->setText(QApplication::translate("CompareDialog", "Add new compare path\n"
"or drop folder here.", 0));
        lbInfo->setText(QString());
        chkShowTree->setText(QApplication::translate("CompareDialog", "Show 3 View", 0));
    } // retranslateUi

};

namespace Ui {
    class CompareDialog: public Ui_CompareDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMPAREDIALOG_H
