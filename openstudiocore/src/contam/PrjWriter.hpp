/**********************************************************************
 *  Copyright (c) 2013, The Pennsylvania State University.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/
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
