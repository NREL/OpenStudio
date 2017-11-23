#ifndef BECDOC_H
#define BECDOC_H

#include "enegyplusdoc.h"

class BECDoc : public IDoc
{
public:
     BECDoc(const QString& projectName, QWebView *webView);
    ~BECDoc();

     // IDoc interface
public:
     void doCmp(const QString &projectName, QWebView *webView2);
};

#endif // BECDOC_H
