#ifndef TABLEELEMENT_H
#define TABLEELEMENT_H

#include "icompareelement.h"
#include <QList>
#include <QVector>
#include <QtWebKit/QWebElement>
#include <QStringList>

class QWebElement;

class TableElement : public ICompareElement
{
public:
    TableElement(IDoc *doc, QString& title, QWebElement elm
                 , bool row1IsLabel=true
                 , bool column1IsLabel=true);
    ~TableElement();
    QString toInsideTBODY();
    QString getHeader(){return title;}
    void setColum1IsLabel(bool isLabel);
    void setRow1IsLabel(bool isLabel);

// ICompareElement interface
public:
    QString Id();
    bool Compare(ICompareElement *comparer);
    void Update();

protected:
    QString title;
    QWebElement elm;
    bool row1IsLabel;
    bool column1IsLabel;
    void doTable(QWebElement elm);

    QVector <QStringList> datas;
    int _row, _col;

    size_t rowCount(){return _row;}
    size_t colCount(){return _col;}
    void resize(size_t row, size_t col);
    void pushValue(size_t row, size_t col, QString value);
    QStringList& at(size_t row, size_t col);
};

#endif // TABLEELEMENT_H
