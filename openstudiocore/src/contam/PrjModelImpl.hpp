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
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/
#ifndef PRJMODELPRIVATE_H
#define PRJMODELPRIVATE_H

#include "PrjDefines.hpp"
#include "PrjReader.hpp"
#include "PrjObjects.hpp"
#include "PrjAirflowElements.hpp"

#include <QSharedData>

#include <utilities/core/Path.hpp>
#include <utilities/data/TimeSeries.hpp>

#include "ContamAPI.hpp"

namespace openstudio {
namespace contam {

class SimFile;

class PrjModel_Impl : public QSharedData
{
public:
  PrjModel_Impl()
  {
    m_valid = false;
  }
  explicit PrjModel_Impl(openstudio::path path);
  explicit PrjModel_Impl(std::string filename);
  explicit PrjModel_Impl(Reader &input);
  bool read(openstudio::path path);
  bool read(std::string filename);
  bool read(Reader &input);
  std::string toString();

  std::vector<std::vector<int> > zoneExteriorFlowPaths();
  std::vector<TimeSeries> zoneInfiltration(SimFile *sim);
  std::vector<TimeSeries> pathInfiltration(std::vector<int> pathNrs, SimFile *sim);
  bool setSteadyWeather(double windSpeed, double windDirection);

  RunControl rc() const
  {
    return m_rc;
  }
  void setRc(const RunControl rc)
  {
    m_rc = rc;
  }

  std::vector<int> contaminants() const
  {
    return m_contaminants;
  }

  std::vector <Species> species() const
  {
    return m_species;
  }
  void setSpecies(const std::vector<Species> species)
  {
    m_species = species;
    rebuildContaminants();
  }

  std::vector <Level> levels() const
  {
    return m_levels;
  }
  void setLevels(const std::vector<Level> levels)
  {
    m_levels = levels;
  }
  void addLevel(Level level)
  {
    level.setNr(m_levels.size()+1);
    m_levels.push_back(level);
  }

  std::vector <DaySchedule> daySchedules() const
  {
    return m_daySchedules;
  }
  void setDaySchedules(const std::vector<DaySchedule> daySchedules)
  {
    m_daySchedules = daySchedules;
  }

  std::vector <WeekSchedule> weekSchedules() const
  {
    return m_weekSchedules;
  }
  void setWeekSchedules(const std::vector<WeekSchedule> weekSchedules)
  {
    m_weekSchedules = weekSchedules;
  }

  std::vector <WindPressureProfile> windPressureProfiles() const
  {
    return m_windPressureProfiles;
  }
  void setWindPressureProfiles(const std::vector<WindPressureProfile> windPressureProfiles)
  {
    m_windPressureProfiles = windPressureProfiles;
  }

  std::vector<PlrTest1> getPlrTest1() const
  {
    std::vector<PlrTest1> afe;
    for(int i=0;i<m_airflowElements.size();i++)
    {
      if(m_airflowElements[i]->dataType() == "plr_test1")
      {
        afe.push_back(*(m_airflowElements[i].dynamicCast<PlrTest1>().data()));
      }
    }
    return afe;
  }

  template <class T> bool addAirflowElement(T element)
  {
    T *copy = new T;
    *copy = element;
    AirflowElement *pointer = dynamic_cast<AirflowElement*>(copy);
    if(pointer)
    {
      copy->setNr(m_airflowElements.size()+1);
      m_airflowElements.push_back(QSharedPointer<AirflowElement>(pointer));
      return true;
    }
    return false;
  }

  int airflowElementNrByName(std::string name) const;

  template <class T> bool replaceAirflowElement(int nr, T element)
  {
    if(nr>0 && nr<=m_airflowElements.size())
    {
      T *copy = new T;
      *copy = element;
      AirflowElement *pointer = dynamic_cast<AirflowElement*>(copy);
      if(pointer)
      {
        copy->setNr(nr);
        m_airflowElements.replace(nr-1,QSharedPointer<AirflowElement>(pointer));
        return true;
      }
    }
    return false;
  }

  std::vector<CvfDat> getCvfDat()
  {
    std::vector<CvfDat> ctrl;
    for(int i=0;i<m_controlNodes.size();i++)
    {
      QSharedPointer<CvfDat> cast = m_controlNodes[i].dynamicCast<CvfDat>();
      if(!cast.isNull())
      {
        ctrl.push_back(*(cast.data()));
      }
    }
    return ctrl;
  }

  template <class T> bool addControlNode(T element, bool sequence=true)
  {
    T *copy = new T;
    *copy = element;
    ControlNode *pointer = dynamic_cast<ControlNode*>(copy);
    if(pointer)
    {
      copy->setNr(m_controlNodes.size()+1);
      if(sequence)
      {
        copy->setSeqnr(copy->nr());
      }
      m_controlNodes.push_back(QSharedPointer<ControlNode>(pointer));
      return true;
    }
    return false;
  }

  std::vector <Ahs> ahs() const
  {
    return m_ahs;
  }
  void setAhs(const std::vector<Ahs> ahs)
  {
    m_ahs = ahs;
  }
  void addAhs(Ahs ahs)
  {
    ahs.setNr(m_ahs.size()+1);
    m_ahs.push_back(ahs);
  }

  std::vector<Zone> zones() const
  {
    return m_zones;
  }
  void setZones(const std::vector<Zone> zones)
  {
    m_zones = zones;
  }
  void addZone(Zone zone)
  {
    zone.setNr(m_zones.size()+1);
    m_zones.push_back(zone);
  }

  std::vector<Path> paths() const
  {
    return m_paths;
  }
  void setPaths(const std::vector<Path> paths)
  {
    m_paths = paths;
  }
  void addPath(Path path)
  {
    path.setNr(m_paths.size()+1);
    m_paths.push_back(path);
  }

  bool valid() const
  {
    return m_valid;
  }

private:
  void rebuildContaminants();
  void readZoneIc(Reader &input);
  std::string writeZoneIc(int start=0);
  template <class T> std::string writeSectionVector(std::vector<T> vector, std::string label=std::string(), int start=0);
  template <class T> std::string writeSectionVector(QVector<QSharedPointer<T> > vector, std::string label=std::string(), int start=0);
  template <class T> std::string writeArray(std::vector<T> vector, std::string label=std::string(), int start=0);

  bool m_valid;

  std::map<std::string,std::string> m_unsupported;

  RunControl m_rc;
  std::vector<int> m_contaminants;
  std::vector<Species> m_species;
  std::vector<Level> m_levels;
  std::vector<DaySchedule> m_daySchedules;
  std::vector<WeekSchedule> m_weekSchedules;
  std::vector<WindPressureProfile> m_windPressureProfiles;
  QVector<QSharedPointer<AirflowElement> > m_airflowElements;
  QVector<QSharedPointer<ControlNode> > m_controlNodes;
  std::vector<Ahs> m_ahs;
  std::vector<Zone> m_zones;
  std::vector<Path> m_paths;
};

template <class T> std::string PrjModel_Impl::writeSectionVector(std::vector<T> vector, std::string label, int start)
{
  std::string string;
  int number = vector.size()-start;
  if(label.empty())
  {
    string += openstudio::toString(number) + '\n';
  }
  else
  {
    string += openstudio::toString(number) + " ! " + label + '\n';
  }
  for(unsigned int i=start;i<vector.size();i++)
  {
    string += vector[i].write();
  }
  string += "-999\n";
  return string;
}

template <class T> std::string PrjModel_Impl::writeSectionVector(QVector<QSharedPointer<T> > vector,
  std::string label, int start)
{
  std::string string;
  int number = vector.size()-start;
  if(label.empty())
  {
    string += openstudio::toString(number) + '\n';
  }
  else
  {
    string += openstudio::toString(number) + " ! " + label + '\n';
  }
  for(int i=start;i<vector.size();i++)
  {
    string += vector[i]->write();
  }
  string += "-999\n";
  return string;
}

template <class T> std::string PrjModel_Impl::writeArray(std::vector<T> vector, std::string label, int start)
{
  std::string string;
  int number = vector.size()-start;
  if(label.empty())
  {
    string += openstudio::toString(number) + '\n';
  }
  else
  {
    string += openstudio::toString(number) + " ! " + label + '\n';
  }
  for(unsigned int i=start;i<vector.size();i++)
  {
    string += ' ' + openstudio::toString(vector[i]);
  }
  return string +'\n';
}

} // contam
} // openstudio

#endif // PRJMODELPRIVATE_H
