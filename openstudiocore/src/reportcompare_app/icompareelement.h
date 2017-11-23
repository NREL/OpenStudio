#ifndef ICOMPAREELEMENT_H
#define ICOMPAREELEMENT_H

#include <typeindex>
#include <QSharedPointer>

class IDoc;

class ICompareElement
{
public:
    ICompareElement(IDoc* doc);
    ~ICompareElement();
    virtual QString Id()=0;
    virtual bool Compare(ICompareElement* other)=0;
    virtual void Update()=0;
    bool isUse(){ return _isUse; }
    void setUse(bool used){ _isUse = used;}
    void reset(){_isUse = false;}
    QString getError();
    void clearError();
    const QString& projectName();
protected:
    IDoc* doc;
    bool _isUse;
    QString errMsg;
    void errorMsg(const QString& error);
    void errorMsgLn(const QString& error);
};

#endif // ICOMPAREELEMENT_H
