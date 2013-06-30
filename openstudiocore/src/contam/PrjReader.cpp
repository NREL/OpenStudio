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
#include "PrjReader.hpp"
#include <iostream>
#include <stdlib.h>
#include "PrjData.hpp"

CONTAMNAMESPACESTART
namespace prj
{

Reader::Reader(QTextStream *stream):stream(stream),m_lineNumber(0),allocated(false)
{
}

Reader::Reader(QString string, int starting):m_lineNumber(starting),allocated(true)
{
    stream = new QTextStream(&string);
}

Reader::~Reader()
{
    if(allocated)
        delete stream;
    allocated = false;
}

float Reader::readFloat(DECFILELINE)
{
    bool ok;
    QString string = readString(ARGFILELINE);
    float value = string.toFloat(&ok);
    if(!ok)
        error(QString("Floating point (float) conversion error at line %1 for \"%2\"").arg(m_lineNumber).arg(string) ARGCFILELINE);
    return value;
}

double Reader::readDouble(DECFILELINE)
{
    bool ok;
    QString string = readString(ARGFILELINE);
    double value = string.toDouble(&ok);
    if(!ok)
        error(QString("Floating point (double) conversion error at line %1 for \"%2\"").arg(m_lineNumber).arg(string) ARGCFILELINE);
    return value;
}

QString Reader::readString(DECFILELINE)
{
    while(1)
    {
        while(this->entries.size() == 0)
        {
            QString input = stream->readLine();
            if(input.isNull())
                error(QString("Failed to read input at line %1").arg(m_lineNumber) ARGCFILELINE);
            m_lineNumber++;
            while(input[0]=='!')
            {
                input = stream->readLine();
                if(input.isNull())
                    error(QString("Failed to read input at line %1").arg(m_lineNumber) ARGCFILELINE);
                m_lineNumber++;
            }
            entries = input.split(" ",QString::SkipEmptyParts);
        }
        QString out = entries.takeFirst();
        if(out[0] == '!')
            entries.clear();
        else
            return out;
    }
}

int Reader::readInt(DECFILELINE)
{
    bool ok;
    QString string = readString(ARGFILELINE);
    int value = string.toInt(&ok);
    if(!ok)
        error(QString("Integer conversion error at line %1 for \"%2\"").arg(m_lineNumber).arg(string) ARGCFILELINE);
    return value;
}

QString Reader::readLine(DECFILELINE)
{
    /* Dump any other input */
    if(entries.size())
        entries.clear();
    QString input = stream->readLine();
    if(input.isNull())
        error(QString("Failed to read input at line %1").arg(m_lineNumber) ARGCFILELINE);
    m_lineNumber++;
    while(input[0]=='!')
    {
        input = stream->readLine();
        if(input.isNull())
            error(QString("Failed to read input at line %1").arg(m_lineNumber) ARGCFILELINE);
        m_lineNumber++;
    }
    return input;
}

void Reader::read999(DECFILELINE)
{
    QString input = readLine(ARGFILELINE);
    if(!input.startsWith(QString("-999")))
        error(QString("Failed to read -999 at line %1").arg(m_lineNumber) ARGCFILELINE);
}

void Reader::read999(QString mesg DECCFILELINE)
{
    QString input = readLine(ARGFILELINE);
    if(!input.startsWith(QString("-999")))
        error(mesg+(QString(" at line %1").arg(m_lineNumber)) ARGCFILELINE);
}

void Reader::readEnd(DECFILELINE)
{
    QString input = readLine(ARGFILELINE);
    if(!input.startsWith(QString("* end project file.")))
        error(QString("Failed to read file end at line %1").arg(m_lineNumber) ARGCFILELINE);
}

void Reader::skipSection(DECFILELINE)
{
    QString input;
    while(1)
    {
        input = readLine(ARGFILELINE);
        if(input.startsWith(QString("-999")))
            break;
    }
}

QString Reader::storeSection(DECFILELINE)
{
    QString section;
    while(1)
    {
        QString input = stream->readLine();
        if(input.isNull())
            error(QString("Failed to read input at line %1").arg(m_lineNumber) ARGCFILELINE);
        m_lineNumber++;
        section += input + '\n';
        if(input.startsWith(QString("-999")))
            break;
    }
    return section;
}

//QList<int> PrjReader::readIntArray(const char *file, int line, bool terminated)
//{
//    QList<int> list;
//    int n = readInt(file,line);
//    for(int i=0;i<n;i++)
//        list << readInt(file,line);
//    if(terminated)
//        read999(QString("Failed to find section termination"),file,line);
//    return list;
//}

QVector<int> Reader::readIntArray(DECFILELINEC bool terminated)
{
    int n = readInt(ARGFILELINE);
    QVector<int> list(n);
    for(int i=0;i<n;i++)
        list[i] = readInt(ARGFILELINE);
    if(terminated)
        read999(QString("Failed to find section termination") ARGCFILELINE);
    return list;
}

template <> int Reader::read<int>(DECFILELINE)
{
    return readInt(ARGFILELINE);
}

template <> double Reader::read<double>(DECFILELINE)
{
    return readDouble(ARGFILELINE);
}

template <> float Reader::read<float>(DECFILELINE)
{
    return readFloat(ARGFILELINE);
}

template <> QString Reader::read<QString>(DECFILELINE)
{
    return readString(ARGFILELINE);
}

template <> double Reader::readNumber<double>(DECFILELINE)
{
    return readInt(ARGFILELINE);
}

template <> float Reader::readNumber<float>(DECFILELINE)
{
    return readFloat(ARGFILELINE);
}

template <> QString Reader::readNumber<QString>(DECFILELINE)
{
    bool ok;
    QString string = readString(ARGFILELINE);
    string.toDouble(&ok);
    if(!ok)
        error(QString("Invalid number \"%2\" on line %1").arg(m_lineNumber).arg(string) ARGCFILELINE);
    return string;
}

void Reader::error(QString mesg DECCFILELINE)
{
	LOG(Error, mesg.toStdString());
//#ifdef NOFILELINE
//    std::cout << mesg.toStdString() << std::endl;
//#else
//    std::cout << mesg.toStdString() << '(' << file << ',' << line << ")\n";
//#endif
//    exit(EXIT_FAILURE);
}

}
CONTAMNAMESPACEEND
