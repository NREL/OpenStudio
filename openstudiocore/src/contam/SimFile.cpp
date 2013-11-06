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
#include "SimFile.hpp"

#include <QFile>
#include <QStringList>

namespace openstudio {
namespace contam {

SimFile::SimFile(openstudio::path path)
{
  m_hasLfr = false;
  m_hasNfr = false;
  m_hasNcr = false;
  // For now, we need to cheat and assume that the .lfr etc. actually exist
  // This means that simread has to have been run for this to work
  openstudio::path lfrPath = path.replace_extension(openstudio::toPath("lfr").string());
  m_hasLfr = readLfr(openstudio::toQString(lfrPath));
}

bool SimFile::computeDateTimes(QVector<QString> day, QVector<QString> time)
{
  unsigned month,dayOfMonth;
  bool ok;
  QStringList split;
  int n = qMin(day.size(),time.size());
  for(int i=0;i<n;i++)
  {
    split = day[i].split('/');
    if(split.size() != 2)
    {
      return false;
    }
    month=split[0].toInt(&ok);
    if(!ok || month > 12)
    {
      return false;
    }
    dayOfMonth=split[1].toInt(&ok);
    if(!ok)
    {
      return false;
    }
    m_dateTimes.push_back(DateTime(Date(monthOfYear(month),dayOfMonth,2013),Time(time[i].toStdString())));
  }
  return true;
}

void SimFile::clearLfr()
{
  m_dP.clear();
  m_F0.clear();
  m_F1.clear();
}

bool SimFile::readLfr(QString fileName)
{
  clearLfr();
  QVector<QString> day;
  QVector<QString> time;
  QFile file(fileName);
  if(!file.open(QFile::ReadOnly))
  {
    LOG(Error,"Failed to open LFR file '" << fileName.toStdString() << "'");
    return false;
  }
  QTextStream textStream(&file);
  QMap<int,int> nrMap;
  // Read the header
  QString header = textStream.readLine();
  if(header.isNull())
  {
    LOG(Error,"No data in LFR file '" << fileName.toStdString() << "'");
    return false;
  }
  QStringList row = header.split('\t');
  int ncols = 6;
  if(row.size() != ncols)
  {
    LOG(Error,"LFR file has " << row.size() << " columns, not the expected " << ncols);
    return false;
  }
  // Read the data
  QString line;
  while(!(line=textStream.readLine()).isNull())
  {
    QStringList row = line.split('\t');
    if(row.size() != ncols)
    {
      clearLfr();
      LOG(Error,"LFR data line has " << row.size() << " columns, not the expected " << ncols);
      return false;
    }
    if(time.size())
    {
      if(time[time.size()-1] != row[1])
      {
        day << row[0];
        time << row[1];
      }
    }
    else
    {
      day << row[0];
      time << row[1];
    }
    bool convOk;
    int nr = row[2].toInt(&convOk);
    if(!convOk)
    {
      clearLfr();
      LOG(Error,"Invalid link number '" << row[2].toStdString() << "'");
      return false;
    }
    if(!nrMap.contains(nr))
    {
      int sz = nrMap.size();
      nrMap[nr] = sz;
      m_dP.resize(sz+1);
      m_F0.resize(sz+1);
      m_F1.resize(sz+1);
    }
    double dP = row[3].toDouble(&convOk);
    if(!convOk)
    {
      clearLfr();
      LOG(Error,"Invalid pressure difference '" << row[3].toStdString() << "'");
      return false;
    }
    double F0 = row[4].toDouble(&convOk);
    if(!convOk)
    {
      clearLfr();
      LOG(Error,"Invalid flow 0 '" << row[4].toStdString() << "'");
      return false;
    }
    double F1 = row[5].toDouble(&convOk);
    if(!convOk)
    {
      clearLfr();
      LOG(Error,"Invalid flow 1 '" << row[5].toStdString() << "'");
      return false;
    }
    m_dP[nrMap[nr]].push_back(dP);
    m_F0[nrMap[nr]].push_back(F0);
    m_F1[nrMap[nr]].push_back(F1);

  }
  // Unwind the zone number map;
  QList<int> keys = nrMap.keys();
  m_pathNr.resize(keys.size());
  for(int i=0;i<keys.size();i++)
  {
    m_pathNr[nrMap[keys[i]]] = keys[i];
  }
  file.close();
  // Compute the required date/time objects - this needs to be moved elsewhere if the NCR and NFR are also read
  if(!computeDateTimes(day,time))
  {
    clearLfr();
    LOG(Error,"Failed to compute date and time objects");
    return false;
  }
  return true;
}

openstudio::TimeSeries SimFile::deltaP(int nr) const
{
  return openstudio::TimeSeries(m_dateTimes,createVector(m_dP[nr]),"Pa");
}

openstudio::TimeSeries SimFile::flow0(int nr) const
{
  return openstudio::TimeSeries(m_dateTimes,createVector(m_F0[nr]),"kg/s");
}

openstudio::TimeSeries SimFile::flow1(int nr) const
{
  return openstudio::TimeSeries(m_dateTimes,createVector(m_F1[nr]),"kg/s");
}

} // contam
} // openstudio
