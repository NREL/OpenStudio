#include <QProgressDialog>
#include <QCoreApplication>
#include <QMessageBox>
#include <QDebug>

#include "becdoc.h"
#include "htmltutil.h"
#include "tableelement.h"

BECDoc::BECDoc(const QString &projectName, QWebView *webView)
    : IDoc(projectName, webView)
{
    QWebElement body = getBody(webView);
    QWebElement elmit = body.firstChild();
    QString title;
    bool isnext1 = nextTableWithTitle(title, elmit);
    while(isnext1){
        qDebug()  << "Create At title : " << title;
        QSharedPointer<TableElement> telm = QSharedPointer<TableElement>
                (new TableElement(this, title, elmit, true, false));
        elements.append(telm);
        elmit = elmit.nextSibling();
        isnext1 = nextTableWithTitle(title, elmit);
    }
}

BECDoc::~BECDoc()
{

}

void BECDoc::doCmp(const QString &projectName, QWebView *webView2)
{
    QString error;
    resetTables();

    QSharedPointer<BECDoc> other = QSharedPointer<BECDoc>(new BECDoc(projectName, webView2));
    QProgressDialog progress("Doing compare BEC.", "Abort", 0, (int)other->elementCount(), NULL);
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

