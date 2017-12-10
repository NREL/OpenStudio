#include <QApplication>
#include <QDebug>

#include "menuwindow.h"
#include "comparedialog.h"

#define ONLY_COMPARE_DIALOG_START

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug() << "argc=" <<argc;
#ifdef ONLY_COMPARE_DIALOG_START
    CompareDialog w;
    if(!w.SetParam(argc, argv))
        a.exit(0);
    else{
        w.show();
        return a.exec();
    }
#else
    MenuWindow w;
    w.show();
    return a.exec();
#endif
	return false;
}
