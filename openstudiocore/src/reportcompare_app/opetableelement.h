#ifndef OPETABLEELEMENT_H
#define OPETABLEELEMENT_H

#include <QtWebKit/QWebElement>
#include <QVector>
#include "icompareelement.h"
#include "idoc.h"

////TODO:IMPLEMENT.
class OPETableElement : public ICompareElement {
public:
    OPETableElement(IDoc* doc, QString &title, QWebElement tableElm);
    ~OPETableElement();
    QWebElement getNext() { return tbody.nextSibling();}
protected:
    void doTable();
public:
    QString Id();
    bool Compare(ICompareElement *comparer);
    void Update();
    void printInsideData();
private:
    QString title;
    QWebElement tableElm;
    QWebElement thread;
    QWebElement tbody;
    QVector <QStringList> datas;

    int _row, _col;

    size_t rowCount(){return _row;}
    size_t colCount(){return _col;}
    void resize(size_t row, size_t col);
    void pushValue(size_t row, size_t col, QString value);
    QStringList &at(size_t row, size_t col);
};

#endif // OPETABLEELEMENT_H
