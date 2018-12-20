/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "SimFile.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>

namespace openstudio {
namespace contam {

int indexOf(const std::vector<int>& list, int target) {
  for (size_t i = 0; i < list.size(); ++i){
    if (list[i] == target) {
      return i;
    }
  }
  return -1;
}

SimFile::SimFile(openstudio::path path)
{
  m_hasLfr = false;
  m_hasNfr = false;
  m_hasNcr = false;
  // For now, we need to cheat and assume that the .lfr etc. actually exist
  // This means that simread has to have been run for this to work
  openstudio::path lfrPath = path.replace_extension(openstudio::toPath("lfr").string());
  m_hasLfr = readLfr(openstudio::toString(lfrPath));
  openstudio::path nfrPath = path.replace_extension(openstudio::toPath("nfr").string());
  m_hasNfr = readNfr(openstudio::toString(nfrPath));
}

bool SimFile::computeDateTimes(const std::vector<std::string>& day, const std::vector<std::string>& time)
{
  int n = qMin(day.size(),time.size());
  for(int i=0;i<n;i++)
  {
    std::vector<std::string> split;
    boost::split(split, day[i], boost::is_any_of("/"));
    if(split.size() != 2)
    {
      return false;
    }

    unsigned month;
    unsigned dayOfMonth;
    try {
      month = std::stoul(split[0]);
      dayOfMonth = std::stoul(split[1]);
      // DLM: what about month == 0?
      if (month > 12)
      {
        return false;
      }

      m_dateTimes.push_back(DateTime(Date(monthOfYear(month), dayOfMonth), Time(time[i])));
    }catch (const std::exception&){
      return false;
    }
  }
  return true;
}

void SimFile::clearLfr()
{
  m_dP.clear();
  m_F0.clear();
  m_F1.clear();
}

bool SimFile::readLfr(const std::string& fileName)
{
  clearLfr();
  std::vector<std::string> day;
  std::vector<std::string> time;
  openstudio::filesystem::ifstream file(openstudio::toPath(fileName));
  if(!file.is_open())
  {
    LOG(Error,"Failed to open LFR file '" << fileName << "'");
    return false;
  }
  std::map<int,int> nrMap;
  // Read the header
  std::string headerstr;
  std::getline(file, headerstr);
  std::string header = headerstr;
  if(header.empty())
  {
    LOG(Error,"No data in LFR file '" << fileName << "'");
    return false;
  }
  std::vector<std::string> row;
  boost::split(row, header, boost::is_any_of("\t"));
  int ncols = 6;
  if(row.size() != ncols)
  {
    LOG(Error,"LFR file has " << row.size() << " columns, not the expected " << ncols);
    return false;
  }
  // Read the data
  while(file.good())
  {
    std::string linestr;
    std::getline(file, linestr);
    std::string line = linestr;
    std::vector<std::string> row;
    boost::split(row, line, boost::is_any_of("\t"));
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
        day.push_back(row[0]);
        time.push_back(row[1]);
      }
    }
    else
    {
      day.push_back(row[0]);
      time.push_back(row[1]);
    }

    int nr = 0;
    try {
      nr = std::stoi(row[2]);
    }catch(const std::exception&){
      clearLfr();
      LOG(Error,"Invalid link number '" << row[2] << "'");
      return false;
    }
    if(nrMap.find(nr) == nrMap.end())
    {
      int sz = nrMap.size();
      nrMap[nr] = sz;
      m_dP.resize(sz+1);
      m_F0.resize(sz+1);
      m_F1.resize(sz+1);
    }
    double dP = 0;
    try {
      dP = std::stod(row[3]);
    }catch(const std::exception&){
      clearLfr();
      LOG(Error,"Invalid pressure difference '" << row[3] << "'");
      return false;
    }

    double F0 = 0;
    try {
      F0 = std::stod(row[4]);
    } catch (const std::exception&) {
      clearLfr();
      LOG(Error,"Invalid flow 0 '" << row[4] << "'");
      return false;
    }

    double F1 = 0;
    try {
      F1 = std::stod(row[5]);
    } catch (const std::exception&) {
      clearLfr();
      LOG(Error,"Invalid flow 1 '" << row[5] << "'");
      return false;
    }

    m_dP[nrMap[nr]].push_back(dP);
    m_F0[nrMap[nr]].push_back(F0);
    m_F1[nrMap[nr]].push_back(F1);

  }
  // Unwind the zone number map;
  std::vector<int> keys;
  for (const auto& pair : nrMap) {
    keys.push_back(pair.first);
  }
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

bool SimFile::readNfr(const std::string& fileName)
{
  clearNfr();
  std::vector<std::string> day;
  std::vector<std::string> time;
  openstudio::filesystem::ifstream file(openstudio::toPath(fileName));
  if(file.is_open())
  {
    LOG(Error,"Failed to open NFR file '" << fileName << "'");
    return false;
  }

  std::map<int,int> nrMap;
  // Read the header
  std::string headerstr;
  std::getline(file, headerstr);
  std::string header = headerstr;
  if(header.empty())
  {
    LOG(Error,"No data in LFR file '" << fileName << "'");
    return false;
  }
  std::vector<std::string> row;
  boost::split(row, header, boost::is_any_of("\t"));
  int ncols = 6;
  if(row.size() != ncols && row.size() != ncols+2)
  {
    LOG(Error,"NFR file has " << row.size() << " columns, not the expected " << ncols);
    return false;
  }
  // Read the data
  while(file.good())
  {
    std::string linestr;
    std::getline(file, linestr);
    std::string line = linestr;

    std::vector<std::string> row;
    boost::split(row, line, boost::is_any_of("\t"));
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
        day.push_back(row[0]);
        time.push_back(row[1]);
      }
    }
    else
    {
      day.push_back(row[0]);
      time.push_back(row[1]);
    }

    int nr = 0;
    try {
      nr = std::stoi(row[2]);
    } catch(const std::exception&){
      clearNfr();
      LOG(Error,"Invalid node number '" << row[2] << "'");
      return false;
    }
    if(nrMap.find(nr) == nrMap.end())
    {
      int sz = nrMap.size();
      nrMap[nr] = sz;
      m_T.resize(sz+1);
      m_P.resize(sz+1);
      m_D.resize(sz+1);
    }
    double T = 0;
    try {
      T = std::stod(row[3]);
    }catch(const std::exception&){
      clearNfr();
      LOG(Error,"Invalid temperature '" << row[3] << "'");
      return false;
    }

    double P = 0;
    try {
      P = std::stod(row[4]);
    } catch (const std::exception&) {
      clearNfr();
      LOG(Error,"Invalid pressure '" << row[4] << "'");
      return false;
    }

    double D = 0;
    try {
      D = std::stod(row[5]);
    } catch (const std::exception&) {
      if(nr==0)
      {
        D=0.0;
      }
      else
      {
        clearNfr();
        LOG(Error,"Invalid density '" << row[5] << "'");
        return false;
      }
    }
    m_T[nrMap[nr]].push_back(T);
    m_P[nrMap[nr]].push_back(P);
    m_D[nrMap[nr]].push_back(D);

  }
  // Unwind the zone number map;
  std::vector<int> keys;
  for (const auto& pair : nrMap) {
    keys.push_back(pair.first);
  }
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
  int index = indexOf(m_pathNr, nr);
  if(index == -1)
  {
    return boost::optional<openstudio::TimeSeries>();
  }
  openstudio::TimeSeries series = convertData(m_dateTimes,m_dP[index],"Pa");
  return boost::optional<openstudio::TimeSeries>(series);
}

boost::optional<openstudio::TimeSeries> SimFile::pathFlow0(int nr) const
{
  int index = indexOf(m_pathNr, nr);
  if(index == -1)
  {
    return boost::optional<openstudio::TimeSeries>();
  }
  openstudio::TimeSeries series = convertData(m_dateTimes,m_F0[index],"kg/s");
  return boost::optional<openstudio::TimeSeries>(series);
}

boost::optional<openstudio::TimeSeries> SimFile::pathFlow1(int nr) const
{
  int index = indexOf(m_pathNr, nr);
  if(index == -1)
  {
    return boost::optional<openstudio::TimeSeries>();
  }
  openstudio::TimeSeries series = convertData(m_dateTimes,m_F1[index],"kg/s");
  return boost::optional<openstudio::TimeSeries>(series);
}

boost::optional<openstudio::TimeSeries> SimFile::pathFlow(int nr) const
{
  int index = indexOf(m_pathNr, nr);
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
  int index = indexOf(m_nodeNr, nr);
  if(index == -1)
  {
    return boost::optional<openstudio::TimeSeries>();
  }
  openstudio::TimeSeries series = convertData(m_dateTimes,m_T[index],"K");
  return boost::optional<openstudio::TimeSeries>(series);
}

boost::optional<openstudio::TimeSeries> SimFile::nodePressure(int nr) const
{
  int index = indexOf(m_nodeNr, nr);
  if(index == -1)
  {
    return boost::optional<openstudio::TimeSeries>();
  }
  openstudio::TimeSeries series = convertData(m_dateTimes,m_P[index],"Pa");
  return boost::optional<openstudio::TimeSeries>(series);
}

boost::optional<openstudio::TimeSeries> SimFile::nodeDensity(int nr) const
{
  int index = indexOf(m_nodeNr, nr);
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

bool Contaminants::read(std::string fileName)
{
  bool ok=false;
  clear();
  QFile file(fileName);
  if(file.open(QFile::ReadOnly))
  {
    QTextStream textStream(&file);
    std::string line;
    std::string header = textStream.readLine();
    if(header.isNull())
      return false;
    std::stringList headerList = header.split('\t');
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
      std::stringList row = line.split('\t');
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
