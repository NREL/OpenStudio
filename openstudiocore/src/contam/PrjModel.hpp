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

#include <QExplicitlySharedDataPointer>

#include <utilities/core/Path.hpp>
#include <utilities/data/TimeSeries.hpp>

#include "ContamAPI.hpp"

namespace openstudio {
namespace contam {

class SimFile;
class PrjModelPrivate;

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
  PrjModel();
  explicit PrjModel(openstudio::path path);
  explicit PrjModel(std::string filename);
  explicit PrjModel(Reader &input);

  PrjModel(const PrjModel &other);
  ~PrjModel();
  PrjModel& operator=(const PrjModel &other);
  bool operator==(const PrjModel &other) const;
  bool operator!=(const PrjModel &other) const;

  bool read(openstudio::path path);
  bool read(std::string filename);
  bool read(Reader &input);
  std::string toString();

  std::vector<std::vector<int> > zoneExteriorFlowPaths();
  std::vector<TimeSeries> zoneInfiltration(SimFile *sim);
  std::vector<TimeSeries> pathInfiltration(std::vector<int> pathNrs, SimFile *sim);
  bool setSteadyWeather(double windSpeed, double windDirection);

  RunControl rc() const;
  void setRc(const RunControl rc);

  std::vector<int> contaminants() const;

  std::vector <Species> species() const;
  void setSpecies(const std::vector<Species> species);

  std::vector <Level> levels() const;
  void setLevels(const std::vector<Level> levels);
  void addLevel(Level level);

  std::vector <DaySchedule> daySchedules() const;
  void setDaySchedules(const std::vector<DaySchedule> daySchedules);

  std::vector <WeekSchedule> weekSchedules() const;
  void setWeekSchedules(const std::vector<WeekSchedule> weekSchedules);

  std::vector <WindPressureProfile> windPressureProfiles() const;
  void setWindPressureProfiles(const std::vector<WindPressureProfile> windPressureProfiles);

  std::vector<PlrTest1> getPlrTest1() const;

  bool addAirflowElement(PlrTest1 element);

  int airflowElementNrByName(std::string name) const;

  bool replaceAirflowElement(int nr, PlrTest1 element);

  std::vector<CvfDat> getCvfDat() const;

  bool addControlNode(CvfDat element, bool sequence=true);

  std::vector <Ahs> ahs() const;
  void setAhs(const std::vector<Ahs> ahs);
  void addAhs(Ahs ahs);

  std::vector<Zone> zones() const;
  void setZones(const std::vector<Zone> zones);
  void addZone(Zone zone);

  std::vector<Path> paths() const;
  void setPaths(const std::vector<Path> paths);
  void addPath(Path path);

  bool valid() const;

private:
  QExplicitlySharedDataPointer<PrjModelPrivate> d;
};

} // contam
} // openstudio

#endif // PRJMODEL_H
