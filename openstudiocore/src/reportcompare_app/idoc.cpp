#include "idoc.h"
#include <QtWebKitWidgets/QWebFrame>
#include <QtWebKit/QWebElement>
#include <QtWebKitWidgets/QWebView>

IDoc::IDoc(const QString &projectName, QWebView *webView)
{
    this->_projectName = projectName;
    this->webView = webView;
}

IDoc::~IDoc()
{

}

void IDoc::clearCompare()
{

}

const QString &IDoc::projectName()
{
    return _projectName;
}

QWebElement IDoc::getBody(QWebView *webView)
{
    QWebFrame *frame = webView->page()->mainFrame();
    QWebElement doc = frame->documentElement();
    QWebElement body = doc.findFirst("body");
    return body;
}

size_t IDoc::elementCount()
{
    return elements.count();
}

QList<QSharedPointer<ICompareElement> > &IDoc::getElements()
{
    return elements;
}

void IDoc::setDisableCompare(bool isDisable)
{
    webView->blockSignals(!isDisable);
}

ICompareElement *IDoc::find(const QString &key)
{
    QList<QSharedPointer<ICompareElement> >::iterator it = elements.begin();
    while (it!=elements.end()) {
        if(!(*it)->isUse() && (*it)->Id() == key){
            return (*it).data();
        }
        it++;
    }
    return NULL;
}

void IDoc::resetTables()
{
    QList<QSharedPointer<ICompareElement> >::iterator it = elements.begin();
    while (it!=elements.end()) {
        (*it)->reset();
        it++;
    }
}

