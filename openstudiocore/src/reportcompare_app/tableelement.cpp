#include "tableelement.h"

#include <QVector>
#include <QStringList>
#include <QtWebKit/QWebElement>
#include <QString>
#include <QMessageBox>
#include <QDebug>

#include "htmltutil.h"
#include "idoc.h"

TableElement::TableElement(IDoc* doc, QString &title, QWebElement elm
                           , bool row1IsLabel
                           , bool column1IsLabel)
    :ICompareElement(doc)
{
    this->column1IsLabel = column1IsLabel;
    this->row1IsLabel = row1IsLabel;
    this->title = title;
    _isUse = false;
    this->elm = elm;
    doTable(this->elm);
}

TableElement::~TableElement()
{

}

QString TableElement::toInsideTBODY(){
    QString tbody;
    for(size_t r=0;r<rowCount();r++){
        tbody += "<tr>\n";

        bool isFirstFloat = false;
        float firstVal;

        for(size_t c=0;c<colCount();c++){

            QString td;
            if ( c % 2== 0)
                td = "<td  align=\"center\" bgcolor=\"#F7F7F7\">";
            else
                td = "<td align=\"center\">";

            QStringList& sls = at(r,c);
            QStringList::iterator slsit = sls.begin();
            isFirstFloat = peakFloat((*slsit), firstVal);
            while (slsit!=sls.end()) {
                if(c==0&&column1IsLabel){
                    if(r==0&&row1IsLabel){
                        tbody += (td+"</td>\n");
                    }
                    else{
                        tbody += (td +(*slsit)+ "</td>\n");
                    }
                    break;
                }else{
                    float secondVal;
                    QString sstr = (*slsit);
                    if(peakFloat((*slsit), secondVal) && isFirstFloat){
                        if(firstVal>secondVal){
                            tbody += (td +doHtmlTextColor(sstr, "#008A2E")+ "</td>\n");
                        }
                        else if(firstVal<secondVal){
                            tbody += (td +doHtmlTextColor(sstr, Qt::red)+ "</td>\n");
                        }
                        else{
                            tbody += (td +sstr+ "</td>\n");
                        }
                    }
                    else{
                        tbody += (td +sstr+ "</td>\n");
                    }
                }
                slsit++;
            }
        }
        tbody += "</tr>\n";
    }
    return tbody;
}

void TableElement::Update()
{
    elm.setInnerXml(toInsideTBODY());
}

void TableElement::doTable(QWebElement elm){
    QWebElementCollection trs = elm.findAll("TR");
    QString tagRow = "TD";
    for(int r=0;r<trs.count();r++){
        qDebug() << "TagRow=" << tagRow;
        QWebElementCollection tds = trs[r].findAll(tagRow);
        qDebug() << "xx tds.count = " << tds.count();
        if(tds.count()==0){
            tagRow = "TH";
            tds = trs[r].findAll(tagRow);
            QWebElement elm = trs[r].firstChild();
            qDebug() << "tds.count = " << tds.count() << ", tr.count=" << trs.count() << ", elmname:" << elm.tagName();
        }
        if(r==0){
            if(datas.size()==0){
                resize(trs.count(), tds.count());
            }
        }

        for(int c=0;c<tds.count();c++){
            if(r==0&&row1IsLabel){
                pushValue(r,c, tds[c].toPlainText()+QString("<br>(%1)").arg(doc->projectName()));
            }
            else{
                pushValue(r,c, tds[c].toPlainText());
            }
        }
    }
}

void TableElement::resize(size_t row, size_t col) {
    _row = row;
    _col = col;
    datas.resize(row*col);
}

void TableElement::pushValue(size_t row, size_t col, QString value){
    int index = _col*row+col;
    if(index>=datas.length())
        return;
    else
        datas[index].append(value);
}

QStringList &TableElement::at(size_t row, size_t col){
    int index = _col*row+col;
    if(index >= datas.length()){
        return datas[0];
    }
    else
        return datas[_col*row+col];
}

QString TableElement::Id()
{
    return title;
}

bool TableElement::Compare(ICompareElement* comparer)
{
    TableElement* other = static_cast<TableElement*>(comparer);
    if(!other)
        return false;

    clearError();

    if(rowCount() != other->rowCount()
       ||colCount() != other->colCount()){

        errMsg = QString(   "The project '%1'\n"
                            "table id '%2' is mismatch.\n"
                            "Source row is %3 and compare row is %4.\n"
                            "Source column is %5 and compare column is %6.\n"
                              )
                            .arg(doc->projectName())
                            .arg(Id())
                            .arg(rowCount()).arg(other->rowCount())
                            .arg(colCount()).arg(other->colCount());
        return false;
    }

    for(size_t r=0;r<other->rowCount();r++){
        for(size_t c=0;c<other->colCount();c++){
            pushValue(r,c, other->at(r,c).at(0));
        }
    }
    return true;
}
