#ifndef PRJWRITER_H
#define PRJWRITER_H

#include <QString>

namespace openstudio {
namespace contam {
namespace prj {

template <class T> QString writeSection(QList<T> list, QString label=QString())
{
    QString string;
    if(label.isNull())
        string += QString("%1\n").arg(list.size());
    else
        string += QString("%1 ! %2\n").arg(list.size()).arg(label);
    for(int i=0;i<list.size();i++)
        string += list[i].write() + "\n";
    string += "-999\n";
    return string;
}

template <class T> QString writeArray(QList<T> list, QString label=QString())
{
    QString string;
    if(label.isNull())
        string += QString("%1\n").arg(list.size());
    else
        string += QString("%1 ! %2\n").arg(list.size()).arg(label);
    for(int i=0;i<list.size();i++)
        string += QString(" %1").arg(list[i]);
    return string;
}

QString writeEmptySection(QString label=QString());

}
}
}

#endif // PRJWRITER_H
