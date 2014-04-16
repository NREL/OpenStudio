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
#ifndef PRJMODEL_H
#define PRJMODEL_H

#include "PrjDefines.hpp"
#include "PrjReader.hpp"
#include "PrjObjects.hpp"
#include "PrjAirflowElements.hpp"

#include <boost/shared_ptr.hpp>

#include <utilities/core/Path.hpp>
#include <utilities/data/TimeSeries.hpp>

#include "ContamAPI.hpp"

namespace openstudio {
namespace contam {

class SimFile;
namespace detail {
  class IndexModelImpl;
}

/** IndexModel is primarily a container for CONTAM airflow model data.
*
*  IndexModel contains CONTAM airflow model elements and has several methods 
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
class CONTAM_API IndexModel
{
public:
  /** @name Constructors and Destructors */
  //@{

  /** Creates a new, empty PRJ model. */
  IndexModel();
  /** Creates a new PRJ model from input file at specified path. */
  explicit IndexModel(openstudio::path path);
  /** Creates a new PRJ model from named input file. */
  explicit IndexModel(std::string filename);
  /** Creates a new PRJ model from an input object. */
  explicit IndexModel(Reader &input);
  /** Creates a new PRJ model from another PRJ model. */
  IndexModel(const IndexModel &other);
  /** Destroy the model. */
  ~IndexModel();

  //@}
  /** @name Operators */
  //@{

  /** Copy operator */
  IndexModel& operator=(const IndexModel &other);
  /** Equality operator */
  bool operator==(const IndexModel &other) const;
  /** Inequality operator */
  bool operator!=(const IndexModel &other) const;

  //@}
  /** @name Getters and Setters */
  //@{

  /** Returns the program name, should be "ContamW". */
  std::string programName() const;
  /** Sets the program name. */
  void setProgramName(const std::string &name);
  /** Returns the CONTAM version. */
  std::string version() const;
  /** Sets the CONTAM version. */
  void setVersion(const std::string &version);
  /** Returns the input echo flag: 0 - no echo, 1 - echo input files to the log file. */
  int echo() const;
  /** Sets the input echo flag: 0 - no echo, 1 - echo input files to the log file. */
  void setEcho(const int echo);
  /** Returns the project description. */
  std::string desc() const;
  /** Sets the project description. */
  void setDesc(const std::string &prjdesc);
  /** Returns the SketchPad height [cells]. */
  int skheight() const;
  /** Sets the SketchPad height [cells]. */
  void setSkheight(const int skheight);
  /** Returns the SketchPad width [cells]. */
  int skwidth() const;
  /** Sets the SketchPad width [cells]. */
  void setSkwidth(const int skwidth);
  /** Returns the default display units: 0 = SI, 1 = US. */
  int def_units() const;
  /** Sets the default display units: 0 = SI, 1 = US. */
  void setDef_units(const int def_units);
  /** Returns the default flows: 0 = mass, 1 = volume. */
  int def_flows() const;
  /** Sets the default flows: 0 = mass, 1 = volume. */
  void setDef_flows(const int def_flows);
  /** Returns the default temperature for zones [K]. */
  double def_T() const;
  /** Sets the default temperature for zones [K]. */
  bool setDef_T(const double def_T);
  /** Sets the default temperature for zones [K]. */
  bool setDef_T(const std::string &def_T);
  /** Returns the display units for temperature. */
  int udefT() const;
  /** Sets the display units for temperature. */
  void setUdefT(const int udefT);
  /** Returns the angle to true north [degrees]. */
  double rel_N() const;
  /** Sets the angle to true north [degrees]. */
  bool setRel_N(const double rel_N);
  /** Sets the angle to true north [degrees]. */
  bool setRel_N(const std::string &rel_N);
  /** Returns the elevation for reference wind speed [m]. */
  double wind_H() const;
  /** Sets the elevation for reference wind speed [m]. */
  bool setWind_H(const double wind_H);
  /** Sets the elevation for reference wind speed [m]. */
  bool setWind_H(const std::string &wind_H);
  /** Returns the display units for elevation for reference wind speed. */
  int uwH() const;
  /** Sets the display units for elevation for reference wind speed. */
  void setUwH(const int uwH);
  /** Returns the local terrain constant for wind. */
  double wind_Ao() const;
  /** Sets the local terrain constant for wind. */
  bool setWind_Ao(const double wind_Ao);
  /** Sets the local terrain constant for wind. */
  bool setWind_Ao(const std::string &wind_Ao);
  /** Returns the velocity profile exponent for wind. */
  double wind_a() const;
  /** Sets the velocity profile exponent for wind. */
  bool setWind_a(const double wind_a);
  /** Sets the velocity profile exponent for wind. */
  bool setWind_a(const std::string &wind_a);
  /** Returns the cell scaling factor. */
  double scale() const;
  /** Sets the cell scaling factor. */
  bool setScale(const double scale);
  /** Sets the cell scaling factor. */
  bool setScale(const std::string &scale);
  /** Returns the units of scaling factor: 0 = m, 1 = ft. */
  int uScale() const;
  /** Sets the units of scaling factor: 0 = m, 1 = ft. */
  void setUScale(const int uScale);
  /** Returns the row of the origin. */
  int orgRow() const;
  /** Sets the row of the origin. */
  void setOrgRow(const int orgRow);
  /** Returns the column of the origin. */
  int orgCol() const;
  /** Sets the column of the origin. */
  void setOrgCol(const int orgCol);
  /** Returns the y-axis inversion flag: 0 = positive above origin, 1 = negative above origin. */
  int invYaxis() const;
  /** Sets the y-axis inversion flag: 0 = positive above origin, 1 = negative above origin. */
  void setInvYaxis(const int invYaxis);
  /** Returns the pseudo-geometry display flag: 0 = don't show, 1 = show in status bar. */
  int showGeom() const;
  /** Sets the pseudo-geometry display flag: 0 = don't show, 1 = show in status bar. */
  void setShowGeom(const int showGeom);
  /** Returns the weather data for steady-state simulation. */
  WeatherData ssWeather() const;
  /** Sets the weather data for steady-state simulation. */
  void setSsWeather(const WeatherData &ssWeather);
  /** Returns the weather data the wind pressure test. */
  WeatherData wptWeather() const;
  /** Sets the weather data the wind pressure test. */
  void setWptWeather(const WeatherData &wptWeather);
  /** Returns the full name of weather file. */
  std::string WTHpath() const;
  /** Sets the full name of weather file. */
  void setWTHpath(const std::string &WTHpath);
  /** Returns the full name of contaminant file. */
  std::string CTMpath() const;
  /** Sets the full name of contaminant file. */
  void setCTMpath(const std::string &CTMpath);
  /** Returns the full name of continuous values file. */
  std::string CVFpath() const;
  /** Sets the full name of continuous values file. */
  void setCVFpath(const std::string &CVFpath);
  /** Returns the full name of discrete values file. */
  std::string DVFpath() const;
  /** Sets the full name of discrete values file. */
  void setDVFpath(const std::string &DVFpath);
  /** Returns the full name of WPC file. */
  std::string WPCfile() const;
  /** Sets the full name of WPC file. */
  void setWPCfile(const std::string &WPCfile);
  /** Returns the full name of EWC data source file. */
  std::string EWCfile() const;
  /** Sets the full name of EWC data source file. */
  void setEWCfile(const std::string &EWCfile);
  /** Returns the WPC description. */
  std::string WPCdesc() const;
  /** Sets the WPC description. */
  void setWPCdesc(const std::string &WPCdesc);
  /** Returns the X-value of ContamW origin in EWC coordinates [m]. */
  double X0() const;
  /** Sets the X-value of ContamW origin in EWC coordinates [m]. */
  bool setX0(const double X0);
  /** Sets the X-value of ContamW origin in EWC coordinates [m]. */
  bool setX0(const std::string &X0);
  /** Returns the Y-value of ContamW origin in EWC coordinates [m]. */
  double Y0() const;
  /** Sets the Y-value of ContamW origin in EWC coordinates [m]. */
  bool setY0(const double Y0);
  /** Sets the Y-value of ContamW origin in EWC coordinates [m]. */
  bool setY0(const std::string &Y0);
  /** Returns the Z-value of ContamW origin in EWC coordinates [m]. */
  double Z0() const;
  /** Sets the Z-value of ContamW origin in EWC coordinates [m]. */
  bool setZ0(const double Z0);
  /** Sets the Z-value of ContamW origin in EWC coordinates [m]. */
  bool setZ0(const std::string &Z0);
  /** Returns the rotation of ContamW relative to EWC coordinates. */
  double angle() const;
  /** Sets the rotation of ContamW relative to EWC coordinates. */
  bool setAngle(const double angle);
  /** Sets the rotation of ContamW relative to EWC coordinates. */
  bool setAngle(const std::string &angle);
  /** Returns the units of coordinates. */
  int u_XYZ() const;
  /** Sets the units of coordinates. */
  void setU_XYZ(const int u_XYZ);
  /** Returns the tolerance for matching path locations. */
  double epsPath() const;
  /** Sets the tolerance for matching path locations. */
  bool setEpsPath(const double epsPath);
  /** Sets the tolerance for matching path locations. */
  bool setEpsPath(const std::string &epsPath);
  /** Returns the tolerance for matching species. */
  double epsSpcs() const;
  /** Sets the tolerance for matching species. */
  bool setEpsSpcs(const double epsSpcs);
  /** Sets the tolerance for matching species. */
  bool setEpsSpcs(const std::string &epsSpcs);
  /** Returns the time shift of EWC data [hh:mm:ss]. */
  std::string tShift() const;
  /** Sets the time shift of EWC data [hh:mm:ss]. */
  void setTShift(const std::string &tShift);
  /** Returns the date WPC data starts [mm/dd]. */
  std::string dStart() const;
  /** Sets the date WPC data starts [mm/dd]. */
  void setDStart(const std::string &dStart);
  /** Returns the date WPC data ends [mm/dd]. */
  std::string dEnd() const;
  /** Sets the date WPC data ends [mm/dd]. */
  void setDEnd(const std::string &dEnd);
  /** Returns the WPC file wind pressure flag: 0 = don't use, 1 = use. */
  int useWPCwp() const;
  /** Sets the WPC file wind pressure flag: 0 = don't use, 1 = use. */
  void setUseWPCwp(const int useWPCwp);
  /** Returns the WPC file mass fraction flag: 0 = don't use, 1 = use. */
  int useWPCmf() const;
  /** Sets the WPC file mass fraction flag: 0 = don't use, 1 = use. */
  void setUseWPCmf(const int useWPCmf);
  /** Returns the WPC trigger flag: 0 = don't trigger, 1 = trigger airflow calculation by WPC data. */
  int wpctrig() const;
  /** Setss the WPC trigger flag: 0 = don't trigger, 1 = trigger airflow calculation by WPC data. */
  void setWpctrig(const int wpctrig);
  /** Returns the latitude (degrees: north +, south -). */
  double latd() const;
  /** Sets the latitude (degrees: north +, south -). */
  bool setLatd(const double latd);
  /** Sets the latitude (degrees: north +, south -). */
  bool setLatd(const std::string &latd);
  /** Returns the longitude (degrees: east +, west -). */
  double lgtd() const;
  /** Sets the longitude (degrees: east +, west -). */
  bool setLgtd(const double lgtd);
  /** Sets the longitude (degrees: east +, west -). */
  bool setLgtd(const std::string &lgtd);
  /** Returns the time zone (Greenwich = 0, Eastern = -5, etc.). */
  double Tznr() const;
  /** Sets the time zone (Greenwich = 0, Eastern = -5, etc.). */
  bool setTznr(const double Tznr);
  /** Sets the time zone (Greenwich = 0, Eastern = -5, etc.). */
  bool setTznr(const std::string &Tznr);
  /** Returns the elevation above sea level [m]. */
  double altd() const;
  /** Sets the elevation above sea level [m]. */
  bool setAltd(const double altd);
  /** Sets the elevation above sea level [m]. */
  bool setAltd(const std::string &altd);
  /** Returns the ground temperature [K]. */
  double Tgrnd() const;
  /** Sets the ground temperature [K]. */
  bool setTgrnd(const double Tgrnd);
  /** Sets the ground temperature [K]. */
  bool setTgrnd(const std::string &Tgrnd);
  /** Returns the display units for ground temperatures. */
  int utg() const;
  /** Sets the display units for ground temperatures. */
  void setUtg(const int utg);
  /** Returns the display units for elevation. */
  int u_a() const;
  /** Sets the display units for elevation. */
  void setU_a(const int u_a);

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
  std::vector<AirflowPath> airflowPaths() const;
  /** Sets the model airflow paths vector. */
  void setAirflowPaths(const std::vector<AirflowPath> paths);
  /** Add an airflow path to the model. */
  void addAirflowPath(AirflowPath path);

  /** Returns false if the model is not a legitimate PRJ model. */
  bool valid() const;

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

  //@}
  /** @name Other Functions */
  //@{

  /** Return a vector of vectors of the CONTAM indices of all plot paths to the exterior of each zone. */
  std::vector<std::vector<int> > zoneExteriorFlowPaths();
  /** Compute the infiltration on a per zone basis from simulation results. */
  std::vector<TimeSeries> zoneInfiltration(SimFile *sim);
  /** Compute the infiltration on a per path basis from simulation results. */
  std::vector<TimeSeries> pathInfiltration(std::vector<int> pathNrs, SimFile *sim);
  //@}

private:
  boost::shared_ptr<detail::IndexModelImpl> m_impl;
};

} // contam
} // openstudio

#endif // PRJMODEL_H
