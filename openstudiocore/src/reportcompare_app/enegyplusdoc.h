#ifndef ENEGYPLUSDOC_H
#define ENEGYPLUSDOC_H

#include <idoc.h>

class QWebElement;

//TODO:MAKE ENEGY PLUS DOC TO INTERFACE DOC
// ::nextTable || nextMergElement
// ::find return QSharedPointer<IMergElement>
class EnegyPlusDoc : public IDoc
{
public:
    EnegyPlusDoc(const QString& projectName, QWebView *webView);
    void doCmp(const QString& projectName, QWebView *webView2);
    ~EnegyPlusDoc();
};


#endif // ENEGYPLUSDOC_H
