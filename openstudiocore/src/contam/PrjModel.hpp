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
class PrjModel_Impl;

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
  /** @name Constructors and Destructors */
  //@{

  /** Creates a new, empty PRJ model. */
  PrjModel();
  /** Creates a new PRJ model from input file at specified path. */
  explicit PrjModel(openstudio::path path);
  /** Creates a new PRJ model from named input file. */
  explicit PrjModel(std::string filename);
  /** Creates a new PRJ model from an input object. */
  explicit PrjModel(Reader &input);
  /** Creates a new PRJ model from another PRJ model. */
  PrjModel(const PrjModel &other);
  /** Destroy the model. */
  ~PrjModel();

  //@}
  /** @name Getters and Setters */
  //@{

  /** Returns the model run control object. */
  RunControl rc() const;
  /** Sets the model run control object. */
  void setRc(const RunControl rc);

  /** Returns a vector of CONTAM indices of all the active species in the model. */
  std::vector<int> contaminants() const;

  /** Returns a vector of the species in the model. */
  std::vector <Species> species() const;
  /** Sets the model species vector. */
  void setSpecies(const std::vector<Species> species);

  /** Returns a vector of the levels in the model. */
  std::vector <Level> levels() const;
  /** Sets the model level vector. */
  void setLevels(const std::vector<Level> levels);
  /** Add a level to the model. */
  void addLevel(Level level);

  /** Returns a vector of the day schedules in the model. */
  std::vector <DaySchedule> daySchedules() const;
  /** Sets the model day schedule vector. */
  void setDaySchedules(const std::vector<DaySchedule> daySchedules);

  /** Returns a vector of the week schedules in the model. */
  std::vector <WeekSchedule> weekSchedules() const;
  /** Sets the model week schedule vector. */
  void setWeekSchedules(const std::vector<WeekSchedule> weekSchedules);

  /** Returns a vector of the wind pressure profiles in the model. */
  std::vector <WindPressureProfile> windPressureProfiles() const;
  /** Sets the model wind pressure profiles vector. */
  void setWindPressureProfiles(const std::vector<WindPressureProfile> windPressureProfiles);

  /** Returns a vector of all PlrTest1 airflow elements in the model. */
  std::vector<PlrTest1> getPlrTest1() const;
  /** Add a PlrTest1 airflow element to the model. */
  bool addAirflowElement(PlrTest1 element);
  /** Return the element number of the named airflow element */
  int airflowElementNrByName(std::string name) const;
  /** Replace an airflow element with a PlrTest1 airflow element */
  bool replaceAirflowElement(int nr, PlrTest1 element);

  /** Returns a vector of all CvfDat control node in the model. */
  std::vector<CvfDat> getCvfDat() const;
  /** Add an CvfDat airflow element to the model. */
  bool addControlNode(CvfDat element, bool sequence=true);

  /** Returns a vector of all simple air handling systems in the model. */
  std::vector <Ahs> ahs() const;
  /** Sets the model simple air handling systems vector. */
  void setAhs(const std::vector<Ahs> ahs);
  /** Add a simple air handling system to the model. */
  void addAhs(Ahs ahs);

  /** Returns a vector of all airflow zones in the model. */
  std::vector<Zone> zones() const;
  /** Sets the model airflow zones vector. */
  void setZones(const std::vector<Zone> zones);
  /** Add an airflow zone to the model. */
  void addZone(Zone zone);

  /** Returns a vector of all airflow paths in the model. */
  std::vector<Path> paths() const;
  /** Sets the model airflow paths vector. */
  void setPaths(const std::vector<Path> paths);
  /** Add an airflow path to the model. */
  void addPath(Path path);

  /** Returns true if the model is a legitimate PRJ model. */
  bool valid() const;

  //@}
  /** @name Operators */
  //@{

  PrjModel& operator=(const PrjModel &other);
  bool operator==(const PrjModel &other) const;
  bool operator!=(const PrjModel &other) const;

  //@}
  /** @name Input and Output */
  //@{

  /** Creates a new PRJ model from input file at specified path. */
  bool read(openstudio::path path);
  /** Creates a new PRJ model from named input file. */
  bool read(std::string filename);
  /** Creates a new PRJ model from an input object. */
  bool read(Reader &input);

  /** Write the model in PRJ format to a string. */
  std::string toString();

  /** Return a vector of vectors of the CONTAM indices of all plot paths to the exterior of each zone. */
  std::vector<std::vector<int> > zoneExteriorFlowPaths();
  /** Compute the infiltration on a per zone basis from simulation results. */
  std::vector<TimeSeries> zoneInfiltration(SimFile *sim);
  /** Compute the infiltration on a per path basis from simulation results. */
  std::vector<TimeSeries> pathInfiltration(std::vector<int> pathNrs, SimFile *sim);
  /** Set the steady wind speed and direction. */
  bool setSteadyWeather(double windSpeed, double windDirection);
  //@}

private:
  QExplicitlySharedDataPointer<PrjModel_Impl> d;
};

} // contam
} // openstudio

#endif // PRJMODEL_H
