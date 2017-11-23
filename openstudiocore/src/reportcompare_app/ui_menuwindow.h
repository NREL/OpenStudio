/********************************************************************************
** Form generated from reading UI file 'menuwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MENUWINDOW_H
#define UI_MENUWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MenuWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *btCPEnergyPlus;
    QPushButton *btCPOPenStudio;
    QPushButton *btCPBEC;
    QPushButton *btGenBEC;
    QPushButton *btTest;
    QPushButton *btAddPV;

    void setupUi(QMainWindow *MenuWindow)
    {
        if (MenuWindow->objectName().isEmpty())
            MenuWindow->setObjectName(QStringLiteral("MenuWindow"));
        MenuWindow->resize(417, 182);
        centralWidget = new QWidget(MenuWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        btCPEnergyPlus = new QPushButton(centralWidget);
        btCPEnergyPlus->setObjectName(QStringLiteral("btCPEnergyPlus"));

        verticalLayout->addWidget(btCPEnergyPlus);

        btCPOPenStudio = new QPushButton(centralWidget);
        btCPOPenStudio->setObjectName(QStringLiteral("btCPOPenStudio"));

        verticalLayout->addWidget(btCPOPenStudio);

        btCPBEC = new QPushButton(centralWidget);
        btCPBEC->setObjectName(QStringLiteral("btCPBEC"));

        verticalLayout->addWidget(btCPBEC);

        btGenBEC = new QPushButton(centralWidget);
        btGenBEC->setObjectName(QStringLiteral("btGenBEC"));

        verticalLayout->addWidget(btGenBEC);

        btTest = new QPushButton(centralWidget);
        btTest->setObjectName(QStringLiteral("btTest"));

        verticalLayout->addWidget(btTest);

        btAddPV = new QPushButton(centralWidget);
        btAddPV->setObjectName(QStringLiteral("btAddPV"));

        verticalLayout->addWidget(btAddPV);

        MenuWindow->setCentralWidget(centralWidget);

        retranslateUi(MenuWindow);

        QMetaObject::connectSlotsByName(MenuWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MenuWindow)
    {
        MenuWindow->setWindowTitle(QApplication::translate("MenuWindow", "MenuWindow", 0));
        btCPEnergyPlus->setText(QApplication::translate("MenuWindow", "Compare Energy Plus", 0));
        btCPOPenStudio->setText(QApplication::translate("MenuWindow", "Compare Open Studio", 0));
        btCPBEC->setText(QApplication::translate("MenuWindow", "Compare BEC", 0));
        btGenBEC->setText(QApplication::translate("MenuWindow", "Gen BEC Report", 0));
        btTest->setText(QApplication::translate("MenuWindow", "Test", 0));
        btAddPV->setText(QApplication::translate("MenuWindow", "Add PV", 0));
    } // retranslateUi

};

namespace Ui {
    class MenuWindow: public Ui_MenuWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MENUWINDOW_H
