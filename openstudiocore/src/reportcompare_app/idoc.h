#ifndef IDOC_H
#define IDOC_H

#include <QSharedPointer>
#include <QList>
#include <QtWebKit/QWebElement>
#include "icompareelement.h"

class QWebView;

class IDoc{
public:
    IDoc(const QString& projectName, QWebView *webView);
    ~IDoc();
    virtual void doCmp(const QString& projectName, QWebView *webView2)=0;
    void clearCompare();
    const QString &projectName();
    QWebElement getBody(QWebView *webView);
    size_t elementCount();
    QList<QSharedPointer<ICompareElement> > &getElements();
    QWebView* getView(){return webView;}
    void setDisableCompare(bool isDisable);
protected:
    QList<QSharedPointer<ICompareElement>> elements;
    QWebView* webView;
    QString _projectName;
    //TODO:USE HASH TABLE FIND.
    ICompareElement *find(const QString &key);
    void resetTables();
};

#endif // IDOC_H
