#include "PrjReader.hpp"
#include <iostream>
#include <stdlib.h>
#include "PrjData.hpp"
#include "Error.hpp"

PrjReader::PrjReader(QTextStream *stream):stream(stream),m_lineNumber(0),allocated(false)
{
}

PrjReader::PrjReader(QString string, int starting):m_lineNumber(starting),allocated(true)
{
    stream = new QTextStream(&string);
}

PrjReader::~PrjReader()
{
    if(allocated)
        delete stream;
    allocated = false;
}

float PrjReader::readFloat(DECFILELINE)
{
    bool ok;
    QString string = readString(ARGFILELINE);
    float value = string.toFloat(&ok);
    if(!ok)
        error(QString("Floating point (float) conversion error at line %1 for \"%2\"").arg(m_lineNumber).arg(string) ARGCFILELINE);
    return value;
}

double PrjReader::readDouble(DECFILELINE)
{
    bool ok;
    QString string = readString(ARGFILELINE);
    double value = string.toDouble(&ok);
    if(!ok)
        error(QString("Floating point (double) conversion error at line %1 for \"%2\"").arg(m_lineNumber).arg(string) ARGCFILELINE);
    return value;
}

QString PrjReader::readString(DECFILELINE)
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

int PrjReader::readInt(DECFILELINE)
{
    bool ok;
    QString string = readString(ARGFILELINE);
    int value = string.toInt(&ok);
    if(!ok)
        error(QString("Integer conversion error at line %1 for \"%2\"").arg(m_lineNumber).arg(string) ARGCFILELINE);
    return value;
}

QString PrjReader::readLine(DECFILELINE)
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

void PrjReader::read999(DECFILELINE)
{
    QString input = readLine(ARGFILELINE);
    if(!input.startsWith(QString("-999")))
        error(QString("Failed to read -999 at line %1").arg(m_lineNumber) ARGCFILELINE);
}

void PrjReader::read999(QString mesg DECCFILELINE)
{
    QString input = readLine(ARGFILELINE);
    if(!input.startsWith(QString("-999")))
        error(mesg+(QString(" at line %1").arg(m_lineNumber)) ARGCFILELINE);
}

void PrjReader::readEnd(DECFILELINE)
{
    QString input = readLine(ARGFILELINE);
    if(!input.startsWith(QString("* end project file.")))
        error(QString("Failed to read file end at line %1").arg(m_lineNumber) ARGCFILELINE);
}

void PrjReader::skipSection(DECFILELINE)
{
    QString input;
    while(1)
    {
        input = readLine(ARGFILELINE);
        if(input.startsWith(QString("-999")))
            break;
    }
}

QString PrjReader::storeSection(DECFILELINE)
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

QVector<int> PrjReader::readIntArray(DECFILELINEC bool terminated)
{
    int n = readInt(ARGFILELINE);
    QVector<int> list(n);
    for(int i=0;i<n;i++)
        list[i] = readInt(ARGFILELINE);
    if(terminated)
        read999(QString("Failed to find section termination") ARGCFILELINE);
    return list;
}

template <> int PrjReader::read<int>(DECFILELINE)
{
    return readInt(ARGFILELINE);
}

template <> double PrjReader::read<double>(DECFILELINE)
{
    return readDouble(ARGFILELINE);
}

template <> float PrjReader::read<float>(DECFILELINE)
{
    return readFloat(ARGFILELINE);
}

template <> QString PrjReader::read<QString>(DECFILELINE)
{
    return readString(ARGFILELINE);
}

template <> double PrjReader::readNumber<double>(DECFILELINE)
{
    return readInt(ARGFILELINE);
}

template <> float PrjReader::readNumber<float>(DECFILELINE)
{
    return readFloat(ARGFILELINE);
}

template <> QString PrjReader::readNumber<QString>(DECFILELINE)
{
    bool ok;
    QString string = readString(ARGFILELINE);
    string.toDouble(&ok);
    if(!ok)
        error(QString("Invalid number \"%2\" on line %1").arg(m_lineNumber).arg(string) ARGCFILELINE);
    return string;
}


