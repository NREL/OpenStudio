#ifndef PRJWRITER_H
#define PRJWRITER_H

#include <QList>
#include <QString>
#include <QSharedPointer>

#include "PrjDefs.hpp"

CONTAMNAMESPACESTART
namespace prj
{

template <class T> QString writeSection(QList<T*> list, QString label=QString(), int start=0)
{
    QString string;
    int number = list.size()-start;
    if(label.isNull())
        string += QString("%1\n").arg(number);
    else
        string += QString("%1 ! %2\n").arg(number).arg(label);
    for(int i=start;i<list.size();i++)
        string += list[i]->write() + "\n";
    string += "-999\n";
    return string;
}

template <class T> QString writeSection(QList<T> list, QString label=QString(), int start=0)
{
    QString string;
    int number = list.size()-start;
    if(label.isNull())
        string += QString("%1\n").arg(number);
    else
        string += QString("%1 ! %2\n").arg(number).arg(label);
    for(int i=start;i<list.size();i++)
        string += list[i].write() + "\n";
    string += "-999\n";
    return string;
}

template <class T, template <class T> class U> QString writeSection(U<T> list, QString label=QString(), int start=0)
{
    QString string;
    int number = list.size()-start;
    if(label.isNull())
        string += QString("%1\n").arg(number);
    else
        string += QString("%1 ! %2\n").arg(number).arg(label);
    for(int i=start;i<list.size();i++)
        string += list[i].write() + "\n";
    string += "-999\n";
    return string;
}

template <class T, template <class T> class U> QString writeSection(U<QSharedPointer<T> > list, QString label=QString(), int start=0)
{
    QString string;
    int number = list.size()-start;
    if(label.isNull())
        string += QString("%1\n").arg(number);
    else
        string += QString("%1 ! %2\n").arg(number).arg(label);
    for(int i=start;i<list.size();i++)
        string += list[i]->write() + "\n";
    string += "-999\n";
    return string;
}

template <class T, template <class T> class U> QString writeArray(U<T> list, QString label=QString(), int start=0)
{
    QString string;
    if(label.isNull())
        string += QString("%1\n").arg(list.size());
    else
        string += QString("%1 ! %2\n").arg(list.size()).arg(label);
    for(int i=start;i<list.size();i++)
        string += QString(" %1").arg(list[i]);
    return string;
}

QString writeEmptySection(QString label=QString());

}
CONTAMNAMESPACEEND

#endif // PRJWRITER_H
