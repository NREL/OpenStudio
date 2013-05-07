#include "PrjReader.hpp"
//#include <iostream>
#include <stdlib.h>

namespace openstudio {
namespace contam {
namespace prj {

Reader::Reader(QTextStream *stream):stream(stream),m_lineNumber(0),allocated(false)
{
    //this->stream = stream;
    //this->lineNumber = 0;
}

Reader::Reader(QString string, int starting):m_lineNumber(starting),allocated(true)
{
    stream = new QTextStream(&string);
}

Reader::~Reader()
{
    if(allocated)
        delete stream;
}

float Reader::readFloat()
{
    bool ok;
    QString string = readString();
    float value = string.toFloat(&ok);
    if(!ok)
        LOG(Error, "Floating point (float) conversion error at line " << m_lineNumber << " for '" << string.toStdString() << "'");
    return value;
}

double Reader::readDouble()
{
    bool ok;
    QString string = readString();
    double value = string.toDouble(&ok);
    if(!ok)
        LOG(Error, "Floating point (double) conversion error at line " << m_lineNumber << " for '" << string.toStdString() << "'");
    return value;
}

QString Reader::readString()
{
    while(1)
    {
        while(this->entries.size() == 0)
        {
            QString input = stream->readLine();
            if(input.isNull())
                LOG(Error,"Failed to read input at line " << m_lineNumber);
            m_lineNumber++;
            while(input[0]=='!')
            {
                input = stream->readLine();
                if(input.isNull())
                    LOG(Error,"Failed to read input at line " << m_lineNumber);
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

int Reader::readInt()
{
    bool ok;
    QString string = readString();
    int value = string.toInt(&ok);
    if(!ok)
        LOG(Error, "Integer conversion error at line " << m_lineNumber << " for '" << string.toStdString() << "'");
    return value;
}

QString Reader::readLine()
{
    /* Dump any other input */
    if(entries.size())
        entries.clear();
    QString input = stream->readLine();
    if(input.isNull())
        LOG(Error,"Failed to read input at line " << m_lineNumber);
    m_lineNumber++;
    while(input[0]=='!')
    {
        input = stream->readLine();
        if(input.isNull())
            LOG(Error,"Failed to read input at line " << m_lineNumber);
        m_lineNumber++;
    }
    return input;
}

void Reader::read999()
{
    QString input = readLine();
    if(!input.startsWith(QString("-999")))
        LOG(Error,"Failed to read -999 at line " << m_lineNumber);
}

void Reader::readEnd()
{
    QString input = readLine();
    if(!input.startsWith(QString("* end project file.")))
        LOG(Error, "Failed to read file end at line " << m_lineNumber);
}

void Reader::skipSection()
{
    QString input;
    while(1)
    {
        input = readLine();
        if(input.startsWith(QString("-999")))
            break;
    }
}

QString Reader::storeSection()
{
    QString section;
    while(1)
    {
        QString input = stream->readLine();
        if(input.isNull())
            LOG(Error,"Failed to read input at line " << m_lineNumber);
        m_lineNumber++;
        section += input + '\n';
        if(input.startsWith(QString("-999")))
            break;
    }
    return section;
}

QList<int> Reader::readIntArray(bool terminated)
{
    QList<int> list;
    int n = readInt();
    for(int i=0;i<n;i++)
        list << readInt();
    if(terminated)
        read999();
    return list;
}

template <> double Reader::readNumber<double>()
{
    return readInt();
}

template <> float Reader::readNumber<float>()
{
    return readFloat();
}

template <> QString Reader::readNumber<QString>()
{
    bool ok;
    QString string = readString();
    string.toDouble(&ok);
    if(!ok)
        LOG(Error, "Invalid number '" << string.toStdString() << "' on line " << m_lineNumber);
    return string;
}

}
}
}
