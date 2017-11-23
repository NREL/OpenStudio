#include "htmltutil.h"

bool peakFloat(const QString &str, float &val){
    bool isFloat;
    val = str.toFloat(&isFloat);
    return isFloat;
}

QString doHtmlTextColor(const QString &str, const QColor &color){
    return QString("<font color=\"%1\">%2</font>")
            .arg(color.name()).arg(str);
}

bool nextTableWithTitle(QString &title, QWebElement &table)
{
    QString lastTitle;
    bool isnext=false;
    while (!table.isNull()) {
        if(table.tagName() == "B"||
                table.tagName() == "H1"||
                table.tagName() == "H2"||
                table.tagName() == "H3"||
                table.tagName() == "H4"||
                table.tagName() == "H5"||
                table.tagName() == "H6")
        {
            lastTitle = table.toInnerXml();
        }
        else if(table.tagName() == "TABLE"){
            isnext = true;
            break;
        }
        table = table.nextSibling();
    }
    title = lastTitle;
    return isnext;
}

QString doCompareNumberText(const QString &src, const QString &compare)
{
    bool isSrcNumber, isCompareNumber;
    float srcNumber, cpNumber;
    srcNumber = src.toFloat(&isSrcNumber);
    cpNumber = compare.toFloat(&isCompareNumber);
    if(isSrcNumber&&isCompareNumber){
        if(srcNumber>cpNumber){
            return doHtmlTextColor(compare, Qt::green);
        }
        else if(srcNumber<cpNumber){
            return doHtmlTextColor(compare, Qt::red);
        }
    }
    return compare;
}
