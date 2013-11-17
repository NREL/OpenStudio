/**********************************************************************
*  Copyright (c) 2013, Alliance for Sustainable Energy.
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
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
*  02110-1301  USA
**********************************************************************/
#include "PrjReader.hpp"
#include <iostream>
#include <stdlib.h>

#include <utilities/core/Logger.hpp>

namespace openstudio {
namespace contam {

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
  QString string = readQString(ARGFILELINE);
  float value = string.toFloat(&ok);
  if(!ok)
  {
    QString mesg=QString("Floating point (float) conversion error at line %1 for \"%2\"")
      .arg(m_lineNumber).arg(string);
#ifndef NOFILELINE
    mesg +=  QString(" (%1,%2)").arg(file).arg(line);
#endif
    LOG_AND_THROW(mesg.toStdString());
  }
  return value;
}

double Reader::readDouble(DECFILELINE)
{
  bool ok;
  QString string = readQString(ARGFILELINE);
  double value = string.toDouble(&ok);
  if(!ok)
  {
    QString mesg=QString("Floating point (double) conversion error at line %1 for \"%2\"")
      .arg(m_lineNumber).arg(string);
#ifndef NOFILELINE
    mesg +=  QString(" (%1,%2)").arg(file).arg(line);
#endif
    LOG_AND_THROW(mesg.toStdString());
  }
  return value;
}

QString Reader::readQString(DECFILELINE)
{
  while(1)
  {
    while(this->entries.size() == 0)
    {
      QString input = stream->readLine();
#ifdef DEBUG
      std::cout << "Line read: " << input.toStdString() << std::endl;
#endif
      if(input.isNull())
      {
        QString mesg=QString("Failed to read input at line %1").arg(m_lineNumber);
#ifndef NOFILELINE
        mesg +=  QString(" (%1,%2)").arg(file).arg(line);
#endif
        LOG_AND_THROW(mesg.toStdString());
      }
      m_lineNumber++;
      while(input[0]=='!')
      {
        input = stream->readLine();
#ifdef DEBUG
        std::cout << "Line read: " << input.toStdString() << std::endl;
#endif
        if(input.isNull())
        {
          QString mesg=QString("Failed to read input at line %1").arg(m_lineNumber);
#ifndef NOFILELINE
          mesg +=  QString(" (%1,%2)").arg(file).arg(line);
#endif
          LOG_AND_THROW(mesg.toStdString());
        }
        m_lineNumber++;
      }
      entries = input.split(" ",QString::SkipEmptyParts);
    }
    QString out = entries.takeFirst();
    if(out[0] == '!')
    {
      entries.clear();
    }
    else
    {
#ifdef DEBUG
      std::cout << "String return: " << out.toStdString() << std::endl;
#endif
      return out;
    }
  }
}

std::string Reader::readStdString(DECFILELINE)
{
  return readQString(ARGFILELINE).toStdString();
}

std::string Reader::readString(DECFILELINE)
{
  return readQString(ARGFILELINE).toStdString();
}

int Reader::readInt(DECFILELINE)
{
  bool ok;
  QString string = readQString(ARGFILELINE);
  int value = string.toInt(&ok);
  if(!ok)
  {
    QString mesg=QString("Integer conversion error at line %1 for \"%2\"").arg(m_lineNumber).arg(string);
#ifndef NOFILELINE
    mesg +=  QString(" (%1,%2)").arg(file).arg(line);
#endif
    LOG_AND_THROW(mesg.toStdString());
  }
  return value;
}

unsigned int Reader::readUInt(DECFILELINE)
{
  bool ok;
  QString string = readQString(ARGFILELINE);
  int value = string.toUInt(&ok);
  if(!ok)
  {
    QString mesg=QString("Unsigned integer conversion error at line %1 for \"%2\"").arg(m_lineNumber).arg(string);
#ifndef NOFILELINE
    mesg +=  QString(" (%1,%2)").arg(file).arg(line);
#endif
    LOG_AND_THROW(mesg.toStdString());
  }
  return value;
}

std::string Reader::readLine(DECFILELINE)
{
  return readLineQString(ARGFILELINE).toStdString();
}

QString Reader::readLineQString(DECFILELINE)
{
  /* Dump any other input */
  if(entries.size())
  {
    entries.clear();
  }
  QString input = stream->readLine();
#ifdef DEBUG
  std::cout << "Line read: " << input.toStdString() << std::endl;
#endif
  if(input.isNull())
  {
    QString mesg=QString("Failed to read input at line %1").arg(m_lineNumber);
#ifndef NOFILELINE
    mesg +=  QString(" (%1,%2)").arg(file).arg(line);
#endif
    LOG_AND_THROW(mesg.toStdString());
  }
  m_lineNumber++;
  while(input[0]=='!')
  {
    input = stream->readLine();
#ifdef DEBUG
    std::cout << "Line read: " << input.toStdString() << std::endl;
#endif
    if(input.isNull())
    {
      QString mesg=QString("Failed to read input at line %1").arg(m_lineNumber);
#ifndef NOFILELINE
      mesg +=  QString(" (%1,%2)").arg(file).arg(line);
#endif
      LOG_AND_THROW(mesg.toStdString());
    }
    m_lineNumber++;
  }
  return input;
}

void Reader::read999(DECFILELINE)
{
  QString input = readLineQString(ARGFILELINE);
  if(!input.startsWith(QString("-999")))
  {
    QString mesg=QString("Failed to read -999 at line %1").arg(m_lineNumber);
#ifndef NOFILELINE
    mesg +=  QString(" (%1,%2)").arg(file).arg(line);
#endif
    LOG_AND_THROW(mesg.toStdString());
  }
}

void Reader::read999(std::string mesg DECCFILELINE)
{
  QString input = readLineQString(ARGFILELINE);
  if(!input.startsWith(QString("-999")))
  {
    QString errmesg = QString().fromStdString(mesg) + QString(" at line %1").arg(m_lineNumber);
#ifndef NOFILELINE
    errmesg +=  QString(" (%1,%2)").arg(file).arg(line);
#endif
    LOG_AND_THROW(errmesg.toStdString());
  }
}

void Reader::readEnd(DECFILELINE)
{
  QString input = readLineQString(ARGFILELINE);
  if(!input.startsWith(QString("* end project file.")))
  {
    QString mesg = QString("Failed to read file end at line %1").arg(m_lineNumber);
#ifndef NOFILELINE
    mesg +=  QString(" (%1,%2)").arg(file).arg(line);
#endif
    LOG_AND_THROW(mesg.toStdString());
  }
}

void Reader::skipSection(DECFILELINE)
{
  readSection(ARGFILELINE);
}

std::string Reader::readSection(DECFILELINE)
{
  QString section;
  while(1)
  {
    QString input = stream->readLine();
    if(input.isNull())
    {
      QString mesg = QString("Failed to read input at line %1").arg(m_lineNumber);
#ifndef NOFILELINE
      mesg +=  QString(" (%1,%2)").arg(file).arg(line);
#endif
      LOG_AND_THROW(mesg.toStdString());
    }
    m_lineNumber++;
    section += input + '\n';
    if(input.startsWith(QString("-999")))
      break;
  }
  return section.toStdString();
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

std::vector<int> Reader::readIntVector(DECFILELINEC bool terminated)
{
  int n = readInt(ARGFILELINE);
  std::vector<int> vector;
  for(int i=0;i<n;i++)
  {
    vector.push_back(readInt(ARGFILELINE));
  }
  if(terminated)
  {
    read999("Failed to find section termination" ARGCFILELINE);
  }
  return vector;
}

//std::vector<int> Reader::readIntStdVector(DECFILELINEC bool terminated)
//{
//    int n = readInt(ARGFILELINE);
//    std::vector<int> list;
//    for(int i=0;i<n;i++)
//        list.push_back(readInt(ARGFILELINE));
//    if(terminated)
//        read999("Failed to find section termination" ARGCFILELINE);
//    return list;
//}

template <> int Reader::read<int>(DECFILELINE)
{
  return readInt(ARGFILELINE);
}

template <> unsigned int Reader::read<unsigned int>(DECFILELINE)
{
  return readUInt(ARGFILELINE);
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
  return readQString(ARGFILELINE);
}

template <> std::string Reader::read<std::string>(DECFILELINE)
{
  return readStdString(ARGFILELINE);
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
  QString string = readQString(ARGFILELINE);
  string.toDouble(&ok);
  if(!ok)
  {
    QString mesg = QString("Invalid number \"%2\" on line %1").arg(m_lineNumber).arg(string);
#ifndef NOFILELINE
    mesg +=  QString(" (%1,%2)").arg(file).arg(line);
#endif
    LOG_AND_THROW(mesg.toStdString());
  }
  return string;
}

template <> std::string Reader::readNumber<std::string>(DECFILELINE)
{
  bool ok;
  QString string = readQString(ARGFILELINE);
  string.toDouble(&ok);
  if(!ok)
  {
    QString mesg = QString("Invalid number \"%2\" on line %1").arg(m_lineNumber).arg(string);
#ifndef NOFILELINE
    mesg +=  QString(" (%1,%2)").arg(file).arg(line);
#endif
    LOG_AND_THROW(mesg.toStdString());
  }
  return string.toStdString();
}

} // contam
} // openstudio
