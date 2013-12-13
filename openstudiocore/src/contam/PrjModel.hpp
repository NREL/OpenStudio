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
#ifndef PRJMODEL_H
#define PRJMODEL_H

#include "PrjDefines.hpp"
#include "PrjReader.hpp"
#include "PrjPublic.hpp"

#include <QSharedData>
#include <QExplicitlySharedDataPointer>

#include <utilities/core/Path.hpp>
#include <utilities/data/TimeSeries.hpp>

#include "ContamAPI.hpp"

namespace openstudio {
namespace contam {

class SimFile;

namespace detail {

class PrjModelPrivate : public QSharedData
{
public:
  PrjModelPrivate()
  {
    valid = false;
  }

  bool valid;

  std::map<std::string,std::string> unsupported;

  RunControl rc;
  std::vector<int> contaminants;
  std::vector<Species> species;
  std::vector<Level> levels;
  std::vector<DaySchedule> daySchedules;
  std::vector<WeekSchedule> weekSchedules;
  std::vector<WindPressureProfile> windPressureProfiles;
  QVector<QSharedPointer<AirflowElement> > airflowElements;
  QVector<QSharedPointer<ControlNode> > controlNodes;
  std::vector<Ahs> ahs;
  std::vector<Zone> zones;
  std::vector<Path> paths;
};

} // detail

/** PrjModel is primarily a container for CONTAM airflow model data.
 *
 *  PrjModel contains CONTAM airflow model elements and has several methods 
 *  to produce the PRJ file. The PRJ file is a positional text file and is
 *  the primary way in which data is provided to the ContamX solver. The 
 *  format is documented here: 
 *
 *  www.bfrl.nist.gov/IAQanalysis/CONTAM/manual/Content/html/PRJ/PRJ_PRJ_Sections.htm
 *
 *  Note that the representation in this object is not very sophisticated. In
 *  particular, links between many items are based upon array indices, so
 *  modifications to the model should be made with care.
 *
 */
class CONTAM_API PrjModel
{
public:
  PrjModel()
  {
    d = new detail::PrjModelPrivate();
  }
  explicit PrjModel(openstudio::path path);
  explicit PrjModel(std::string filename);
  explicit PrjModel(Reader &input);
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
    return d->rc;
  }
  void setRc(const RunControl rc)
  {
    d->rc = rc;
  }

  std::vector<int> contaminants() const
  {
    return d->contaminants;
  }

  std::vector <Species> species() const
  {
    return d->species;
  }
  void setSpecies(const std::vector<Species> species)
  {
    d->species = species;
    rebuildContaminants();
  }

  std::vector <Level> levels() const
  {
    return d->levels;
  }
  void setLevels(const std::vector<Level> levels)
  {
    d->levels = levels;
  }
  void addLevel(Level level)
  {
    level.setNr(d->levels.size()+1);
    d->levels.push_back(level);
  }

  std::vector <DaySchedule> daySchedules() const
  {
    return d->daySchedules;
  }
  void setDaySchedules(const std::vector<DaySchedule> daySchedules)
  {
    d->daySchedules = daySchedules;
  }

  std::vector <WeekSchedule> weekSchedules() const
  {
    return d->weekSchedules;
  }
  void setWeekSchedules(const std::vector<WeekSchedule> weekSchedules)
  {
    d->weekSchedules = weekSchedules;
  }

  std::vector <WindPressureProfile> windPressureProfiles() const
  {
    return d->windPressureProfiles;
  }
  void setWindPressureProfiles(const std::vector<WindPressureProfile> windPressureProfiles)
  {
    d->windPressureProfiles = windPressureProfiles;
  }

  std::vector<PlrTest1> getPlrTest1() const
  {
    std::vector<PlrTest1> afe;
    for(int i=0;i<d->airflowElements.size();i++)
    {
      if(d->airflowElements[i]->dataType() == "plr_test1")
      {
        afe.push_back(*(d->airflowElements[i].dynamicCast<PlrTest1>().data()));
      }
    }
    return afe;
  }

  template <class T> void addAirflowElement(T element)
  {
    T *copy = new T;
    *copy = element;
    AirflowElement *pointer = dynamic_cast<AirflowElement*>(copy);
    if(pointer)
    {
      copy->setNr(d->airflowElements.size()+1);
      d->airflowElements.push_back(QSharedPointer<AirflowElement>(pointer));
    }
  }

  int airflowElementNrByName(std::string name) const;

  template <class T> bool replaceAirflowElement(int nr, T element)
  {
    if(nr>0 && nr<=d->airflowElements.size())
    {
      T *copy = new T;
      *copy = element;
      AirflowElement *pointer = dynamic_cast<AirflowElement*>(copy);
      if(pointer)
      {
        copy->setNr(nr);
        d->airflowElements.replace(nr-1,QSharedPointer<AirflowElement>(pointer));
        return true;
      }
    }
    return false;
  }

  std::vector<CvfDat> getCvfDat()
  {
    std::vector<CvfDat> ctrl;
    for(int i=0;i<d->controlNodes.size();i++)
    {
      QSharedPointer<CvfDat> cast = d->controlNodes[i].dynamicCast<CvfDat>();
      if(!cast.isNull())
      {
        ctrl.push_back(*(cast.data()));
      }
    }
    return ctrl;
  }

  template <class T> void addControlNode(T element, bool sequence=true)
  {
    T *copy = new T;
    *copy = element;
    ControlNode *pointer = dynamic_cast<ControlNode*>(copy);
    if(pointer)
    {
      copy->setNr(d->controlNodes.size()+1);
      if(sequence)
      {
        copy->setSeqnr(copy->nr());
      }
      d->controlNodes.push_back(QSharedPointer<ControlNode>(pointer));
    }
  }

  std::vector <Ahs> ahs() const
  {
    return d->ahs;
  }
  void setAhs(const std::vector<Ahs> ahs)
  {
    d->ahs = ahs;
  }
  void addAhs(Ahs ahs)
  {
    ahs.setNr(d->ahs.size()+1);
    d->ahs.push_back(ahs);
  }

  std::vector<Zone> zones() const
  {
    return d->zones;
  }
  void setZones(const std::vector<Zone> zones)
  {
    d->zones = zones;
  }
  void addZone(Zone zone)
  {
    zone.setNr(d->zones.size()+1);
    d->zones.push_back(zone);
  }

  std::vector<Path> paths() const
  {
    return d->paths;
  }
  void setPaths(const std::vector<Path> paths)
  {
    d->paths = paths;
  }
  void addPath(Path path)
  {
    path.setNr(d->paths.size()+1);
    d->paths.push_back(path);
  }

  bool valid() const
  {
    return d->valid;
  }

private:
  void rebuildContaminants();
  void readZoneIc(Reader &input);
  std::string writeZoneIc(int start=0);
  template <class T> std::string writeSectionVector(std::vector<T> vector, std::string label=std::string(), int start=0);
  template <class T> std::string writeSectionVector(QVector<QSharedPointer<T> > vector, std::string label=std::string(), int start=0);
  template <class T> std::string writeArray(std::vector<T> vector, std::string label=std::string(), int start=0);

  QExplicitlySharedDataPointer<detail::PrjModelPrivate> d;
};

template <class T> std::string PrjModel::writeSectionVector(std::vector<T> vector, std::string label, int start)
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

template <class T> std::string PrjModel::writeSectionVector(QVector<QSharedPointer<T> > vector,
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

template <class T> std::string PrjModel::writeArray(std::vector<T> vector, std::string label, int start)
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

#endif // PRJMODEL_H
