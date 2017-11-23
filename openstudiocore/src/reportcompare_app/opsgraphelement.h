#ifndef ELECTRICITYCONSUMPTIONGRAPHELEMENT_H
#define ELECTRICITYCONSUMPTIONGRAPHELEMENT_H

#include <QString>
#include <QtWebKit/QWebElement>
#include "icompareelement.h"

class OpsGraphElement : public ICompareElement
{
public:
    OpsGraphElement(IDoc* doc, const QString &title, QWebElement tableElm);
    ~OpsGraphElement();
    void setPosXY(float x, float y);
    // ICompareElement interface
public:
    QString Id();
    void setRect(const QSize &s);
    bool Compare(ICompareElement *other);
    void Update();
    float svgWidth();
private:
    QString title;
    QWebElement svg;
    QWebElementCollection ticks;
    QWebElementCollection gs;
    QWebElementCollection legends;
    QList<QList < QWebElement > > otherGS;
    QList<QList < QWebElement > > otherTick;
    OpsGraphElement* gother;
    bool isValid;
    void setWebElementPos(QWebElement elm, float x, float y);
    QSize baseW;
};

#endif // ELECTRICITYCONSUMPTIONGRAPHELEMENT_H
