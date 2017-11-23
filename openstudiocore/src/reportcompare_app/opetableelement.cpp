#include <QStringList>
#include <QDebug>
#include <QtWebKitWidgets/QWebView>
#include <QtWebKitWidgets/QWebFrame>

#include "opetableelement.h"
#include "htmltutil.h"

OPETableElement::OPETableElement(IDoc *doc, QString &title, QWebElement tableElm)
    :ICompareElement(doc)
{
    this->title = title;
    _isUse = false;
    this->tableElm = tableElm;
    this->tableElm.removeClass("table-striped");
    this->thread = tableElm.findFirst("THEAD");
    this->tbody = tableElm.findFirst("TBODY");
    doTable();
}

OPETableElement::~OPETableElement()
{

}

void OPETableElement::printInsideData()
{
    qDebug() << ".....DEBUG DATA:"<< title << ".....";
    for(int irow=0;irow<_row;irow++){
        QString str;
        for(int icol=0;icol<_col;icol++){
            QStringList& ls = this->at(irow, icol);
            for(int istack=0;istack<ls.length();istack++){
                str+=QString("(%1)").arg(istack);
                str+=(ls.at(istack)+":");
            }
            str+="|||";
        }
        qDebug() << str << "\n";
    }
}

void OPETableElement::doTable()
{
    QWebElementCollection thRows = thread.findAll("TR");
    QWebElementCollection tbRows = tbody.findAll("TR");

    QWebElementCollection ths = thread.findAll("TH");

    _row = tbRows.count()+1;
    _col = ths.count();

    if(datas.size()==0){
        resize(_row, _col);
    }

    for(int icol=0;icol<_col;icol++){
        QWebElementCollection ths = thRows[0].findAll("TH");
        pushValue(0,icol, ths[icol].toPlainText());
    }

    for(int irow=1;irow<_row;irow++){
        QWebElementCollection tbs = tbRows[irow].findAll("TD");
        for(int icol=0;icol<_col;icol++){
            pushValue(irow,icol, tbs[icol].toPlainText());
        }
    }
}

QString OPETableElement::Id()
{
    return title;
}

bool OPETableElement::Compare(ICompareElement *comparer)
{
    clearError();
    OPETableElement* other = static_cast<OPETableElement*>(comparer);

    if(!other){
        errMsg+=QString("Compare project '%1' title name '%2' not match type.")
                .arg(doc->projectName())
                .arg(title);
        return false;
    }

    if(_col!=other->_col||_row!=other->_row){
        errMsg = QString(   "The project '%1'\n"
                            "table id '%2' is mismatch.\n"
                            "Source row is %3 and compare row is %4.\n"
                            "Source column is %5 and compare column is %6.\n"
                              )
                            .arg(doc->projectName())
                            .arg(Id())
                            .arg(_row).arg(other->_row)
                            .arg(_col).arg(other->_col);
        return false;
    }

    for(int icol=0;icol<_col;icol++){
        pushValue(0,icol, other->at(0, icol).at(0)+QString("<br>(%1)").arg(other->projectName()));
    }

    for(int irow=1;irow<_row;irow++){
        for(int icol=0;icol<_col;icol++){
            QString cp = doCompareNumberText(at(irow,icol).at(0)
                                             , other->at(irow,icol).at(0));
            pushValue(irow,icol, cp);
        }
    }
    return true;
}

void OPETableElement::Update()
{
    QString threadStr="<tr>";
    for(int icol=0;icol<_col;icol++){
        QStringList& ls = this->at(0, icol);
        for(int istack=0;istack<ls.length();istack++){
            if ( icol % 2== 0)
                threadStr+=QString("<th align=\"center\" bgcolor=\"#F7F7F7\">%1</th>").arg(ls.at(istack));
            else
                threadStr+=QString("<th align=\"center\">%1</th>").arg(ls.at(istack));
            if(icol==0)
                break;
        }
    }
    threadStr+="</tr>\n";
    thread.setInnerXml(threadStr);

    QString tbodyStr;
    for(int irow=1;irow<(_row-1);irow++){
        tbodyStr+="<tr>";
        for(int icol=0;icol<_col;icol++){
            QStringList& ls = this->at(irow, icol);
            QString srcText = ls.at(0);
            for(int istack=0;istack<ls.length();istack++){
                QString strO = doCompareNumberText(srcText, ls.at(istack));
                if ( icol % 2== 0)
                    tbodyStr+=QString("<td align=\"right\" bgcolor=\"#F7F7F7\">%1</td>").arg(strO);
                else
                    tbodyStr+=QString("<td align=\"right\">%1</td>").arg(strO);

                if(icol==0)
                    break;
            }
        }
        tbodyStr+="</tr>\n";
    }
    tbody.setInnerXml(tbodyStr);
}

void OPETableElement::resize(size_t row, size_t col)
{
    _row = row;
    _col = col;
    datas.resize(row*col);
}

void OPETableElement::pushValue(size_t row, size_t col, QString value)
{
    int index = _col*row+col;
    if(index>=datas.length())
        return;
    else
        datas[index].append(value);
}

QStringList &OPETableElement::at(size_t row, size_t col)
{
    int index = _col*row+col;
    if(index >= datas.length()){
        return datas[0];
    }
    else
        return datas[_col*row+col];
}
