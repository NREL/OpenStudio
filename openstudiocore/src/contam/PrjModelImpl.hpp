/**********************************************************************
*  Copyright (c) 2013-2014, Alliance for Sustainable Energy.
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
#ifndef PRJMODELIMPL_H
#define PRJMODELIMPL_H

#include "PrjDefines.hpp"
#include "PrjReader.hpp"
#include "PrjObjects.hpp"
#include "PrjAirflowElements.hpp"

#include <utilities/core/Path.hpp>
#include <utilities/data/TimeSeries.hpp>

#include "ContamAPI.hpp"

namespace openstudio {
namespace contam {

class SimFile;

namespace detail {

class IndexModelImpl
{
public:
  IndexModelImpl();
  explicit IndexModelImpl(openstudio::path path);
  explicit IndexModelImpl(std::string filename);
  explicit IndexModelImpl(Reader &input);
  bool read(openstudio::path path);
  bool read(std::string filename);
  bool read(Reader &input);
  std::string toString();

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
  bool setWind_a(const std::string &wind_a);/** Returns the cell scaling factor. */
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

  RunControl rc() const;
  void setRc(const RunControl rc);

  std::vector<int> contaminants() const;
  std::vector <Species> species() const;
  void setSpecies(const std::vector<Species> &species);
  void addSpecies(Species &species);

  std::vector <Level> levels() const;
  void setLevels(const std::vector<Level> &levels);
  void addLevel(Level &level);

  std::vector <DaySchedule> daySchedules() const;
  void setDaySchedules(const std::vector<DaySchedule> &daySchedules);

  std::vector <WeekSchedule> weekSchedules() const;
  void setWeekSchedules(const std::vector<WeekSchedule> &weekSchedules);

  std::vector <WindPressureProfile> windPressureProfiles() const;
  void setWindPressureProfiles(const std::vector<WindPressureProfile> &windPressureProfiles);

  std::vector<PlrLeak1> getPlrLeak1();

  std::vector<PlrLeak2> getPlrLeak2();

  std::vector<PlrTest1> getPlrTest1();

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

  std::vector<CvfDat> getCvfDat();

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

  std::vector <Ahs> ahs() const;
  void setAhs(const std::vector<Ahs> &ahs);
  void addAhs(Ahs &ahs);

  std::vector<Zone> zones() const;
  void setZones(const std::vector<Zone> &zones);
  void addZone(Zone &zone);

  std::vector<AirflowPath> airflowPaths() const;
  void setAirflowPaths(const std::vector<AirflowPath> &paths);
  void addAirflowPath(AirflowPath &path);

  bool valid() const;

  std::vector<std::vector<int> > zoneExteriorFlowPaths();
  std::vector<TimeSeries> zoneInfiltration(SimFile *sim);
  std::vector<TimeSeries> pathInfiltration(std::vector<int> pathNrs, SimFile *sim);


private:
  void setDefaults();
  void rebuildContaminants();
  void readZoneIc(Reader &input);
  std::string writeZoneIc(int start=0);
  template <class T> std::string writeSectionVector(std::vector<T> vector, std::string label=std::string(), int start=0);
  template <class T> std::string writeSectionVector(QVector<QSharedPointer<T> > vector, std::string label=std::string(), int start=0);
  template <class T> std::string writeArray(std::vector<T> vector, std::string label=std::string(), int start=0);

  bool m_valid;

  std::map<std::string,std::string> m_unsupported;

  std::string m_programName; // program name "ContamW" (I1)
  std::string m_programVersion; // program version "3.1" (I1)
  int m_echo; // (0/1) echo input files to the log file (I2)
  std::string m_desc; // project description (I1) {W}
  int m_skheight;  // total SketchPad height [cells] (I2) {W}
  int m_skwidth;  // total SketchPad width [cells] (I2) {W}
  int m_def_units;  // default units: 0 = SI, 1 = US (I2) {W}
  int m_def_flows;  // default flows: 0 = mass, 1 = volume (I2) {W}
  PRJFLOAT m_def_T;  // default temperature for zones [K] (R4) {W}
  int m_udefT;  // units for temperature (I2) {W}
  PRJFLOAT m_rel_N;  // angle to true north [degrees] (R4) {W}
  PRJFLOAT m_wind_H;  // elevation for reference wind speed [m] (R4) {W}
  int m_uwH;  // units for _wind_H (I2) {W}
  PRJFLOAT m_wind_Ao;  // local terrain constant (R4) {W}
  PRJFLOAT m_wind_a;  // velocity profile exponent (R4) {W}
  PRJFLOAT m_scale;  // cell scaling factor (R4)
  int m_uScale;  // units of scaling factor: 0 = m, 1 = ft (I2)
  int m_orgRow;  // row of origin (I2)
  int m_orgCol;  // column of origin (I2)
  int m_invYaxis;  // 0 = positive above origin, 1 = negative above origin (I2)
  int m_showGeom;  // 0/1 = show pseudo-geometry info in status bar (I2)
  WeatherData m_ssWeather;  // weather data for steady-state simulation (WeatherData)
  WeatherData m_wptWeather;  // weather data the wind pressure test (WeatherData)
  std::string m_WTHpath;  // full name of weather file (CS)
  std::string m_CTMpath;  // full name of contaminant file (CS)
  std::string m_CVFpath;  // full name of continuous values file (CS)
  std::string m_DVFpath;  // full name of discrete values file (CS)
  std::string m_WPCfile;  // full name of WPC file (CS)
  std::string m_EWCfile;  // full name of EWC data source file (CS) {W}
  std::string m_WPCdesc;  // WPC description (CS) {W}
  PRJFLOAT m_X0;  // X-value of ContamW origin in EWC coordinates [m] (R4) {W}
  PRJFLOAT m_Y0;  // Y-value of ContamW origin in EWC coordinates [m] (R4) {W}
  PRJFLOAT m_Z0;  // Z-value of ContamW origin in EWC coordinates [m] (R4) {W}
  PRJFLOAT m_angle;  // Rotation of ContamW relative to EWC coordinates (R4) {W}
  int m_u_XYZ;  // units of coordinates (I2) {W}
  PRJFLOAT m_epsPath;  // tolerance for matching path locations [-] (R4) {W}
  PRJFLOAT m_epsSpcs;  // tolerance for matching species [-] (R4) {W}
  std::string m_tShift;  // time shift of EWC data {W} [s] (hh:mm:ss) {W}
  std::string m_dStart;  // date WPC data starts (mm/dd) {W}
  std::string m_dEnd;  // date WPC data ends (mm/dd) {W}
  int m_useWPCwp;  // if true, use WPC file wind pressures (I2)
  int m_useWPCmf;  // if true, use WPC file mass fractions (I2)
  int m_wpctrig;  // 0/1 = trigger airflow calculation by WPC data (I2)
  PRJFLOAT m_latd;  // latitude (degrees: north +, south -) (R4)
  PRJFLOAT m_lgtd;  // longitude (degrees: east +, west -) (R4)
  PRJFLOAT m_Tznr;  // time zone (Greenwich = 0, Eastern = -5, etc.) (R4)
  PRJFLOAT m_altd;  // elevation above sea level [m] (R4)
  PRJFLOAT m_Tgrnd;  // ground temperature [K] (R4)
  int m_utg;  // units for ground temperatures (I2)
  int m_u_a;  // units for elevation (I2)

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
  std::vector<AirflowPath> m_paths;
};

template <class T> std::string IndexModelImpl::writeSectionVector(std::vector<T> vector, std::string label, int start)
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

template <class T> std::string IndexModelImpl::writeSectionVector(QVector<QSharedPointer<T> > vector,
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

template <class T> std::string IndexModelImpl::writeArray(std::vector<T> vector, std::string label, int start)
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

} // detail
} // contam
} // openstudio

#endif // PRJMODELPRIVATE_H
