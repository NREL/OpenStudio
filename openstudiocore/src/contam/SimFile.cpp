/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
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
  openstudio::path nfrPath = path.replace_extension(openstudio::toPath("nfr").string());
  m_hasNfr = readNfr(openstudio::toQString(nfrPath));
}

bool SimFile::computeDateTimes(QVector<QString> day, QVector<QString> time)
{
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
    unsigned month=split[0].toInt(&ok);
    if(!ok || month > 12)
    {
      return false;
    }
    unsigned dayOfMonth=split[1].toInt(&ok);
    if(!ok)
    {
      return false;
    }
    m_dateTimes.push_back(DateTime(Date(monthOfYear(month),dayOfMonth),Time(time[i].toStdString())));
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
    m_dateTimes.clear();
    LOG(Error,"Failed to compute date and time objects from LFR input");
    return false;
  }
  return true;
}

void SimFile::clearNfr()
{
  m_T.clear();
  m_P.clear();
  m_D.clear();
}

bool SimFile::readNfr(QString fileName)
{
  clearNfr();
  QVector<QString> day;
  QVector<QString> time;
  QFile file(fileName);
  if(!file.open(QFile::ReadOnly))
  {
    LOG(Error,"Failed to open NFR file '" << fileName.toStdString() << "'");
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
  if(row.size() != ncols && row.size() != ncols+2)
  {
    LOG(Error,"NFR file has " << row.size() << " columns, not the expected " << ncols);
    return false;
  }
  // Read the data
  QString line;
  while(!(line=textStream.readLine()).isNull())
  {
    QStringList row = line.split('\t');
    if(row.size() != ncols && row.size() != ncols+2)
    {
      clearNfr();
      LOG(Error,"NFR data line has " << row.size() << " columns, not the expected " << ncols);
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
      clearNfr();
      LOG(Error,"Invalid node number '" << row[2].toStdString() << "'");
      return false;
    }
    if(!nrMap.contains(nr))
    {
      int sz = nrMap.size();
      nrMap[nr] = sz;
      m_T.resize(sz+1);
      m_P.resize(sz+1);
      m_D.resize(sz+1);
    }
    double T = row[3].toDouble(&convOk);
    if(!convOk)
    {
      clearNfr();
      LOG(Error,"Invalid temperature '" << row[3].toStdString() << "'");
      return false;
    }
    double P = row[4].toDouble(&convOk);
    if(!convOk)
    {
      clearNfr();
      LOG(Error,"Invalid pressure '" << row[4].toStdString() << "'");
      return false;
    }
    double D = row[5].toDouble(&convOk);
    if(!convOk)
    {
      if(nr==0)
      {
        D=0.0;
      }
      else
      {
        clearNfr();
        LOG(Error,"Invalid density '" << row[5].toStdString() << "'");
        return false;
      }
    }
    m_T[nrMap[nr]].push_back(T);
    m_P[nrMap[nr]].push_back(P);
    m_D[nrMap[nr]].push_back(D);

  }
  // Unwind the zone number map;
  QList<int> keys = nrMap.keys();
  m_nodeNr.resize(keys.size());
  for(int i=0;i<keys.size();i++)
  {
    m_nodeNr[nrMap[keys[i]]] = keys[i];
  }
  file.close();
  // Something should probably be done here to make sure that the times here match up with what we
  // already have. For now, if nothing is known about the dates, then try to compute it
  if(m_dateTimes.size() == 0)
  {
    if(!computeDateTimes(day,time))
    {
      clearLfr();
      m_dateTimes.clear();
      LOG(Error,"Failed to compute date and time objects from NFR input");
      return false;
    }
  }
  return true;
}

static openstudio::TimeSeries convertData(std::vector<openstudio::DateTime> inputDateTimes,
                                          std::vector<double> inputValues, std::string units)
{
  // Use a per-interval trapezoidal approximation to convert the CONTAM point data into E+ interval data
  std::vector<openstudio::DateTime> dateTimes;
  std::vector<double> values;
  if(inputDateTimes.size()==1) // Account for steady simulation results
  {
    return openstudio::TimeSeries(inputDateTimes,createVector(inputValues),units);
  }
  for(unsigned i=1;i<inputDateTimes.size();i++)
  {
    dateTimes.push_back(inputDateTimes[i]);
    values.push_back(0.5*(inputValues[i-1]+inputValues[i]));
  }
  return openstudio::TimeSeries(dateTimes,createVector(values),units);
}


boost::optional<openstudio::TimeSeries> SimFile::pathDeltaP(int nr) const
{
  int index = m_pathNr.indexOf(nr);
  if(index == -1)
  {
    return boost::optional<openstudio::TimeSeries>();
  }
  openstudio::TimeSeries series = convertData(m_dateTimes,m_dP[index],"Pa");
  return boost::optional<openstudio::TimeSeries>(series);
}

boost::optional<openstudio::TimeSeries> SimFile::pathFlow0(int nr) const
{
  int index = m_pathNr.indexOf(nr);
  if(index == -1)
  {
    return boost::optional<openstudio::TimeSeries>();
  }
  openstudio::TimeSeries series = convertData(m_dateTimes,m_F0[index],"kg/s");
  return boost::optional<openstudio::TimeSeries>(series);
}

boost::optional<openstudio::TimeSeries> SimFile::pathFlow1(int nr) const
{
  int index = m_pathNr.indexOf(nr);
  if(index == -1)
  {
    return boost::optional<openstudio::TimeSeries>();
  }
  openstudio::TimeSeries series = convertData(m_dateTimes,m_F1[index],"kg/s");
  return boost::optional<openstudio::TimeSeries>(series);
}

boost::optional<openstudio::TimeSeries> SimFile::pathFlow(int nr) const
{
  int index = m_pathNr.indexOf(nr);
  if(index == -1)
  {
    return boost::optional<openstudio::TimeSeries>();
  }
  std::vector<double> flow(m_dateTimes.size());
  for(unsigned i=0;i<m_dateTimes.size();i++)
  {
    flow[i] = m_F0[index][i] + m_F1[index][i];
  }
  // Need to confirm that the total flow is F0+F1, since it also could be F0-F1
  openstudio::TimeSeries series = convertData(m_dateTimes,flow,"kg/s");
  return boost::optional<openstudio::TimeSeries>(series);
}

boost::optional<openstudio::TimeSeries> SimFile::nodeTemperature(int nr) const
{
  int index = m_nodeNr.indexOf(nr);
  if(index == -1)
  {
    return boost::optional<openstudio::TimeSeries>();
  }
  openstudio::TimeSeries series = convertData(m_dateTimes,m_T[index],"K");
  return boost::optional<openstudio::TimeSeries>(series);
}

boost::optional<openstudio::TimeSeries> SimFile::nodePressure(int nr) const
{
  int index = m_nodeNr.indexOf(nr);
  if(index == -1)
  {
    return boost::optional<openstudio::TimeSeries>();
  }
  openstudio::TimeSeries series = convertData(m_dateTimes,m_P[index],"Pa");
  return boost::optional<openstudio::TimeSeries>(series);
}

boost::optional<openstudio::TimeSeries> SimFile::nodeDensity(int nr) const
{
  int index = m_nodeNr.indexOf(nr);
  if(index == -1)
  {
    return boost::optional<openstudio::TimeSeries>();
  }
  openstudio::TimeSeries series = convertData(m_dateTimes,m_D[index],"kg/m^3");
  return boost::optional<openstudio::TimeSeries>(series);
}

std::vector<openstudio::DateTime> SimFile::dateTimes() const
{
  if(m_dateTimes.size() == 1) {
    return std::vector<openstudio::DateTime>(m_dateTimes.begin(),m_dateTimes.end());
  } else {
    return std::vector<openstudio::DateTime>(m_dateTimes.begin()+1,m_dateTimes.end());
  }
}

/*

This code is a holdover from earlier versions that also read in contaminants. Hopefully, this text-based
version will not ever be needed. Also, the current OpenStudio implementation only supports airflow and
no contaminant transport.

bool Contaminants::read(QString fileName)
{
  bool ok=false;
  clear();
  QFile file(fileName);
  if(file.open(QFile::ReadOnly))
  {
    QTextStream textStream(&file);
    QString line;
    QString header = textStream.readLine();
    if(header.isNull())
      return false;
    QStringList headerList = header.split('\t');
    int ncols = headerList.size();
    for(int i=3;i<ncols;i++)
    {
      m_nr.push_back(headerList[i].toInt(&ok));
      if(!ok)return false;
    }
    // The number of zones is ncols - 3 (date,time,ctm) - 1 (ambient)
    m_mf.resize(ncols-3);
    while(!(line=textStream.readLine()).isNull())
    {
      QStringList row = line.split('\t');
      if(row.size() != ncols)
      {
        clear();
        return false;
      }
      m_day << row[0];
      m_time << row[1];
      for(int i=3;i<ncols;i++)
        m_mf[i-3] << row[i].toDouble();
    }
    file.close();
    if(!computeSeconds())
      m_s.clear();
    ok=true;
  }
  return ok;
}

QVector<double> Contaminants::integrate()
{
  QVector<double> value(m_mf.size());
  int n = m_mf[0].size();
  for(int i=1;i<m_mf.size();i++)
    n = qMin(n,m_mf[i].size());
  n = qMin(n,m_s.size());
  for(int i=0;i<m_mf.size();i++)
  {
    value[i]=0.0;
    for(int j=1;j<n;j++)
    {
      value[i] += 0.5*(m_mf[i][j-1]+m_mf[i][j])*(m_s[j]-m_s[j-1]);
    }
  }
  return value;
}
*/

} // contam
} // openstudio
