#ifndef MENUWINDOW_H
#define MENUWINDOW_H

#include <QMainWindow>

namespace Ui {
class MenuWindow;
}

class MenuWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MenuWindow(QWidget *parent = 0);
    ~MenuWindow();

private slots:
    void on_btGenBEC_clicked();
    void on_btTest_clicked();
    void on_btCPBEC_clicked();
    void on_btCPEnergyPlus_clicked();
    void on_btCPOPenStudio_clicked();

private:
    Ui::MenuWindow *ui;
    void ShowCompare(const QString &file1, const QString &file2, const QString &type);
    QStringList args;
};

#endif // MENUWINDOW_H
