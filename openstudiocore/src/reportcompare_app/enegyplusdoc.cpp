#include "enegyplusdoc.h"
#include <QCoreApplication>
#include <QDebug>
#include <QProgressDialog>
#include <QMessageBox>

#include "tableelement.h"
#include "htmltutil.h"

EnegyPlusDoc::EnegyPlusDoc(const QString &projectName, QWebView* webView)
    :IDoc(projectName, webView)
{
    QWebElement body = getBody(webView);
    QWebElement elmit = body.firstChild();
    QString title;
    bool isnext1 = nextTableWithTitle(title, elmit);
    while(isnext1){
        qDebug()  << "Create At title : " << title;
        QSharedPointer<TableElement> telm = QSharedPointer<TableElement>(new TableElement(this, title, elmit));
        elements.append(telm);
        elmit = elmit.nextSibling();
        isnext1 = nextTableWithTitle(title, elmit);
    }
}

EnegyPlusDoc::~EnegyPlusDoc()
{

}

void EnegyPlusDoc::doCmp(const QString &projectName, QWebView *webView2)
{
    QString error;
    resetTables();

    QSharedPointer<EnegyPlusDoc> other = QSharedPointer<EnegyPlusDoc>(new EnegyPlusDoc(projectName, webView2));
    QProgressDialog progress("Doing compare Enegy plus.", "Abort", 0, (int)other->elementCount(), NULL);
    progress.setWindowModality(Qt::WindowModal);
    progress.show();
    QCoreApplication::processEvents();

    int i=0;
    QList<QSharedPointer<ICompareElement> >::iterator eit = other->getElements().begin();
    while(eit!=other->getElements().end()){
        ICompareElement* ce = (*eit).data();
        qDebug()  << "Create At title : " << ce->Id();
        ICompareElement* tbelm = find(ce->Id());
        if(tbelm){
            if(tbelm->Compare(ce)){
                tbelm->Update();
                tbelm->setUse(true);
            }
            else{
                error += tbelm->getError()+"\n";
                tbelm->clearError();
            }
        }
        progress.setValue(i++);
        eit++;
        QCoreApplication::processEvents();
    }
    progress.setValue((int)other->elementCount());
    QCoreApplication::processEvents();
    if(!error.isEmpty()){
        QMessageBox msgBox;
        msgBox.setText(error);
        msgBox.exec();
    }
}
