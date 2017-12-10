#include "menuwindow.h"
#include "ui_menuwindow.h"
#include "comparedialog.h"

#include <QMessageBox>
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>

MenuWindow::MenuWindow(QWidget *parent)
    :QMainWindow(parent)
    , ui(new Ui::MenuWindow)
{
    ui->setupUi(this);
}

void MenuWindow::ShowCompare(const QString& file1, const QString &file2, const QString& type)
{
    if(!file1.isEmpty() && !file2.isEmpty() && !type.isEmpty()){
        CompareDialog* dlg = new CompareDialog();
        dlg->SetParam(file1, file2, type);
        dlg->show();
    }
}

MenuWindow::~MenuWindow()
{
    delete ui;
}

void MenuWindow::on_btGenBEC_clicked()
{

}

void MenuWindow::on_btTest_clicked()
{

}

void MenuWindow::on_btCPBEC_clicked()
{
    ShowCompare("C:/cmpres/demo", "C:/Users/Gorn/Desktop/DEMO-OS/demox/run", "b");
}

void MenuWindow::on_btCPEnergyPlus_clicked()
{
    ShowCompare("C:/cmpres/demo", "C:/Users/Gorn/Desktop/DEMO-OS/demox/run", "e");
}

void MenuWindow::on_btCPOPenStudio_clicked()
{
    ShowCompare("C:/cmpres/demo", "C:/Users/Gorn/Desktop/DEMO-OS/demox/run", "o");
}
