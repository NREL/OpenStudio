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
#include "PrjModelImpl.hpp"
#include "PrjReader.hpp"
#include "SimFile.hpp"
#include <QFile>

namespace openstudio {
namespace contam {
namespace detail {

void IndexModelImpl::setDefaults()
{
  m_programName = "ContamW";
  m_echo = 0;
  m_skheight = 0;
  m_skwidth = 0;
  m_def_units = 0;
  m_def_flows = 0;
  m_def_T = PRJFLOAT("0.0");
  m_udefT = 0;
  m_rel_N = PRJFLOAT("0.0");
  m_wind_H = PRJFLOAT("0.0");
  m_uwH = 0;
  m_wind_Ao = PRJFLOAT("0.0");
  m_wind_a = PRJFLOAT("0.0");
  m_scale = PRJFLOAT("0.0");
  m_uScale = 0;
  m_orgRow = 0;
  m_orgCol = 0;
  m_invYaxis = 0;
  m_showGeom = 0;
  m_X0 = PRJFLOAT("0.0");
  m_Y0 = PRJFLOAT("0.0");
  m_Z0 = PRJFLOAT("0.0");
  m_angle = PRJFLOAT("0.0");
  m_u_XYZ = 0;
  m_epsPath = PRJFLOAT("0.0");
  m_epsSpcs = PRJFLOAT("0.0");
  m_useWPCwp = 0;
  m_useWPCmf = 0;
  m_wpctrig = 0;
  m_latd = PRJFLOAT("0.0");
  m_lgtd = PRJFLOAT("0.0");
  m_Tznr = PRJFLOAT("0.0");
  m_altd = PRJFLOAT("0.0");
  m_Tgrnd = PRJFLOAT("0.0");
  m_utg = 0;
  m_u_a = 0;
}

IndexModelImpl::IndexModelImpl()
{
  setDefaults();
  m_valid=false;
}

IndexModelImpl::IndexModelImpl(openstudio::path path)
{
  setDefaults();
  read(path);
}

IndexModelImpl::IndexModelImpl(std::string filename)
{
  setDefaults();
  read(filename);
}

IndexModelImpl::IndexModelImpl(Reader &input)
{
  setDefaults();
  read(input);
}

bool IndexModelImpl::read(openstudio::path path)
{
  return read(openstudio::toString(path));
}

bool IndexModelImpl::read(std::string filename)
{
  setDefaults();
  QFile fp(QString().fromStdString(filename));

  m_valid = false;
  if (fp.open(QFile::ReadOnly))
  {
    QTextStream stream(&fp);
    Reader input(&stream);
    read(input);

  }
  return m_valid;
}

bool IndexModelImpl::read(Reader &input)
{
  m_valid = false;
  // Section 1: Project, Weather, Simulation, and Output Controls
  m_programName = input.readString();
  m_programVersion = input.readString();
  m_echo = input.read<int>();
  m_desc = input.readLine();
  setSkheight(input.read<int>());
  setSkwidth(input.read<int>());
  setDef_units(input.read<int>());
  setDef_flows(input.read<int>());
  setDef_T(input.readNumber<std::string>());
  setUdefT(input.read<int>());
  setRel_N(input.readNumber<std::string>());
  setWind_H(input.readNumber<std::string>());
  setUwH(input.read<int>());
  setWind_Ao(input.readNumber<std::string>());
  setWind_a(input.readNumber<std::string>());
  setScale(input.readNumber<std::string>());
  setUScale(input.read<int>());
  setOrgRow(input.read<int>());
  setOrgCol(input.read<int>());
  setInvYaxis(input.read<int>());
  setShowGeom(input.read<int>());
  m_ssWeather.read(input);
  m_wptWeather.read(input);
  m_WTHpath = input.readLine();
  m_CTMpath = input.readLine();
  m_CVFpath = input.readLine();
  m_DVFpath = input.readLine();
  m_WPCfile = input.readLine();
  m_EWCfile = input.readLine();
  m_WPCdesc = input.readLine();
  setX0(input.readNumber<std::string>());
  setY0(input.readNumber<std::string>());
  setZ0(input.readNumber<std::string>());
  setAngle(input.readNumber<std::string>());
  setU_XYZ(input.read<int>());
  setEpsPath(input.readNumber<std::string>());
  setEpsSpcs(input.readNumber<std::string>());
  setTShift(input.readString());
  setDStart(input.readString());
  setDEnd(input.readString());
  setUseWPCwp(input.read<int>());
  setUseWPCmf(input.read<int>());
  setWpctrig(input.read<int>());
  setLatd(input.readNumber<std::string>());
  setLgtd(input.readNumber<std::string>());
  setTznr(input.readNumber<std::string>());
  setAltd(input.readNumber<std::string>());
  setTgrnd(input.readNumber<std::string>());
  setUtg(input.read<int>());
  setU_a(input.read<int>());
  m_rc.read(input); // Read the run control section
  input.read999();
  // Section 2: Species and Contaminants
  m_contaminants = input.readIntVector(false);
  m_species = input.readSectionVector<Species>("species");
  // Section 3: Level and Icon Data
  m_levels = input.readSectionVector<Level>("level");
  // Section 4: Day Schedules
  m_daySchedules = input.readSectionVector<DaySchedule>("day schedule");
  // Section 5: Week Schedules
  m_weekSchedules = input.readSectionVector<WeekSchedule>("week schedule");
  // Section 6: Wind Pressure Profiles
  m_windPressureProfiles = input.readSectionVector<WindPressureProfile>("wind pressure profiles");
  // Section 7: Kinetic Reactions
  std::string kinr = input.readSection(); // Skip it
  m_unsupported["KineticReaction"] = kinr;
  // Section 8a: Filter Elements
  std::string flte = input.readSection(); // Skip it
  m_unsupported["FilterElement"] = flte;
  // Section 8b: Filters
  std::string filt = input.readSection(); // Skip it
  m_unsupported["Filter"] = filt;
  // Section 9: Source/Sink Elements
  std::string cse = input.readSection(); // Skip it
  m_unsupported["SourceSink"] = cse;
  // Section 10: Airflow Elements
  m_airflowElements = input.readElementVector<AirflowElement>("airflow element");
  // Section 11: Duct Elements
  std::string dfe = input.readSection(); // Skip it
  m_unsupported["DuctElement"] = dfe;
  // Section 12a: Control Super Elements
  std::string selmt = input.readSection(); // Skip it
  m_unsupported["ControlSuperElements"] = selmt;
  // Section 12b: Control Nodes
  //std::string ctrl = input.readSection(); // Skip it
  //m_unsupported["ControlNode"] = ctrl;
  m_controlNodes = input.readElementVector<ControlNode>("control node");
  // Section 13: Simple Air Handling System (AHS)
  m_ahs = input.readSectionVector<Ahs>("ahs");
  // Section 14: Zones
  m_zones = input.readSectionVector<Zone>("zone");
  // Section 15: Initial Zone Concentrations
  readZoneIc(input);
  // Section 16: Airflow Paths
  m_paths = input.readSectionVector<AirflowPath>("path");
  // Section 17: Duct Junctions
  std::string jct = input.readSection(); // Skip it
  m_unsupported["DuctJunction"] = jct;
  // Section 18: Initial Junction Concentrations
  std::string jctic = input.readSection(); // Skip it
  m_unsupported["JunctionIC"] = jctic;
  // Section 19: Duct Segments
  std::string dct = input.readSection(); // Skip it
  m_unsupported["DuctSegment"] = dct;
  // Section 20: Source/Sinks
  //m_sourceSinks = input.readSectionVector<SourceSink>(QString("source/sink"));
  std::string css = input.readSection(); // Skip it
  m_unsupported["SourceSink"] = css;
  // Section 21: Occupancy Schedules
  std::string osch = input.readSection(); // Skip it
  m_unsupported["OccupancySchedule"] = osch;
  // Section 22: Exposures
  std::string pexp = input.readSection(); // Skip it
  m_unsupported["Exposure"] = pexp;
  // Section 23: Annotations
  std::string note = input.readSection(); // Skip it
  m_unsupported["Annotation"] = note;
  input.readEnd();
  m_valid = true;
  return true;
}

std::string IndexModelImpl::toString()
{
  std::string output;
  if(!m_valid)
  {
    return output;
  }
  // Section 1: Project, Weather, Simulation, and Output Controls
  output += m_programName + ' ' + m_programVersion + ' ' + ANY_TO_STR(m_echo) + '\n';
  output += m_desc + '\n';
  output += ANY_TO_STR(m_skheight) + ' ' + ANY_TO_STR(m_skwidth) + ' ' + ANY_TO_STR(m_def_units) + ' '
    + ANY_TO_STR(m_def_flows) + ' ' + ANY_TO_STR(m_def_T) + ' ' + ANY_TO_STR(m_udefT) + ' '
    + ANY_TO_STR(m_rel_N) + ' ' + ANY_TO_STR(m_wind_H) + ' ' + ANY_TO_STR(m_uwH) + ' '
    + ANY_TO_STR(m_wind_Ao) + ' ' + ANY_TO_STR(m_wind_a) + '\n';
  output += ANY_TO_STR(m_scale) + ' ' + ANY_TO_STR(m_uScale) + ' ' + ANY_TO_STR(m_orgRow) + ' '
    + ANY_TO_STR(m_orgCol) + ' ' + ANY_TO_STR(m_invYaxis) + ' ' + ANY_TO_STR(m_showGeom) + '\n';
  output += m_ssWeather.write();
  output += m_wptWeather.write();
  output += m_WTHpath + '\n';
  output += m_CTMpath + '\n';
  output += m_CVFpath + '\n';
  output += m_DVFpath + '\n';
  output += m_WPCfile + '\n';
  output += m_EWCfile + '\n';
  output += m_WPCdesc + '\n';
  output += ANY_TO_STR(m_X0) + ' ' + ANY_TO_STR(m_Y0) + ' ' + ANY_TO_STR(m_Z0) + ' ' + ANY_TO_STR(m_angle)
    + ' ' + ANY_TO_STR(m_u_XYZ) + '\n';
  output += ANY_TO_STR(m_epsPath) + ' ' + ANY_TO_STR(m_epsSpcs) + ' ' + m_tShift + ' ' + m_dStart + ' '
    + m_dEnd + ' ' + ANY_TO_STR(m_useWPCwp) + ' ' + ANY_TO_STR(m_useWPCmf) + ' ' + ANY_TO_STR(m_wpctrig)
    + '\n';
  output += ANY_TO_STR(m_latd) + ' ' + ANY_TO_STR(m_lgtd) + ' ' + ANY_TO_STR(m_Tznr) + ' '
    + ANY_TO_STR(m_altd) + ' ' + ANY_TO_STR(m_Tgrnd) + ' ' + ANY_TO_STR(m_utg) + ' '
    + ANY_TO_STR(m_u_a) + '\n';
  output += m_rc.write();
  output += "-999\n";
  // Section 2: Species and Contaminants
  output += writeArray(m_contaminants,"contaminants:");
  output += writeSectionVector(m_species,"species:");
  // Section 3: Level and Icon Data
  output += writeSectionVector(m_levels,"levels:");
  // Section 4: Day Schedules
  output += writeSectionVector(m_daySchedules,"day-schedules:");
  // Section 5: Week Schedules
  output += writeSectionVector(m_weekSchedules,"week-schedules:");
  // Section 6: Wind Pressure Profiles
  output += writeSectionVector(m_windPressureProfiles,"wind pressure profiles:");
  // Section 7: Kinetic Reactions
  output += m_unsupported["KineticReaction"];
  // Section 8a: Filter Elements
  output += m_unsupported["FilterElement"];
  // Section 8b: Filters
  output += m_unsupported["Filter"];
  // Section 9: Source/Sink Elements
  output += m_unsupported["SourceSink"];
  // Section 10: Airflow Elements
  output += writeSectionVector(m_airflowElements,"flow elements:");
  // Section 11: Duct Elements
  output += m_unsupported["DuctElement"];
  // Section 12a: Control Super Elements
  output += m_unsupported["ControlSuperElements"];
  // Section 12b: Control Nodes
  //output += m_unsupported["ControlNode"];
  output += writeSectionVector(m_controlNodes,"control nodes:");
  // Section 13: Simple Air Handling System (AHS)
  output += writeSectionVector(m_ahs,"simple AHS:");
  // Section 14: Zones
  output += writeSectionVector(m_zones,"zones:");
  // Section 15: Initial Zone Concentrations
  output += writeZoneIc();
  // Section 16: Airflow Paths
  output += writeSectionVector(m_paths,"flow paths:");
  // Section 17: Duct Junctions
  output += m_unsupported["DuctJunction"];
  // Section 18: Initial Junction Concentrations
  output += m_unsupported["JunctionIC"];
  // Section 19: Duct Segments
  output += m_unsupported["DuctSegment"];
  // Section 20: Source/Sinks
  output += m_unsupported["SourceSink"];
  // Section 21: Occupancy Schedules
  output += m_unsupported["OccupancySchedule"];
  // Section 22: Exposures
  output += m_unsupported["Exposure"];
  // Section 23: Annotations
  output += m_unsupported["Annotation"];
  // End of the PRJ file
  output += "* end project file.";
  return output;
}

std::string IndexModelImpl::programName() const
{
  return m_programName;
}

void IndexModelImpl::setProgramName(const std::string &name)
{
  m_programName = name;
}

std::string IndexModelImpl::version() const
{
  return m_programVersion;
}

void IndexModelImpl::setVersion(const std::string &version)
{
  m_programVersion = version;
}

int IndexModelImpl::echo() const
{
  return m_echo;
}

void IndexModelImpl::setEcho(const int echo)
{
  m_echo = echo;
}

std::string IndexModelImpl::desc() const
{
  return m_desc;
}

void IndexModelImpl::setDesc(const std::string &prjdesc)
{
  m_desc = prjdesc;
}

int IndexModelImpl::skheight() const
{
  return m_skheight;
}

void IndexModelImpl::setSkheight(const int skheight)
{
  m_skheight = skheight;
}

int IndexModelImpl::skwidth() const
{
  return m_skwidth;
}

void IndexModelImpl::setSkwidth(const int skwidth)
{
  m_skwidth = skwidth;
}

int IndexModelImpl::def_units() const
{
  return m_def_units;
}

void IndexModelImpl::setDef_units(const int def_units)
{
  m_def_units = def_units;
}

int IndexModelImpl::def_flows() const
{
  return m_def_flows;
}

void IndexModelImpl::setDef_flows(const int def_flows)
{
  m_def_flows = def_flows;
}

double IndexModelImpl::def_T() const
{
  return m_def_T.toDouble();
}

bool IndexModelImpl::setDef_T(const double def_T)
{
  m_def_T = QString::number(def_T);
  return true;
}

bool IndexModelImpl::setDef_T(const std::string &def_T)
{
  bool ok;
  FLOAT_CHECK(def_T,&ok);
  if(ok)
  {
    m_def_T = STR_TO_FLOAT(def_T);
    return true;
  }
  return false;
}

int IndexModelImpl::udefT() const
{
  return m_udefT;
}

void IndexModelImpl::setUdefT(const int udefT)
{
  m_udefT = udefT;
}

double IndexModelImpl::rel_N() const
{
  return m_rel_N.toDouble();
}

bool IndexModelImpl::setRel_N(const double rel_N)
{
  m_rel_N = QString::number(rel_N);
  return true;
}

bool IndexModelImpl::setRel_N(const std::string &rel_N)
{
  bool ok;
  FLOAT_CHECK(rel_N,&ok);
  if(ok)
  {
    m_rel_N = STR_TO_FLOAT(rel_N);
    return true;
  }
  return false;
}

double IndexModelImpl::wind_H() const
{
  return m_wind_H.toDouble();
}

bool IndexModelImpl::setWind_H(const double wind_H)
{
  m_wind_H = QString::number(wind_H);
  return true;
}

bool IndexModelImpl::setWind_H(const std::string &wind_H)
{
  bool ok;
  FLOAT_CHECK(wind_H,&ok);
  if(ok)
  {
    m_wind_H = STR_TO_FLOAT(wind_H);
    return true;
  }
  return false;
}

int IndexModelImpl::uwH() const
{
  return m_uwH;
}

void IndexModelImpl::setUwH(const int uwH)
{
  m_uwH = uwH;
}

double IndexModelImpl::wind_Ao() const
{
  return m_wind_Ao.toDouble();
}

bool IndexModelImpl::setWind_Ao(const double wind_Ao)
{
  m_wind_Ao = QString::number(wind_Ao);
  return true;
}

bool IndexModelImpl::setWind_Ao(const std::string &wind_Ao)
{
  bool ok;
  FLOAT_CHECK(wind_Ao,&ok);
  if(ok)
  {
    m_wind_Ao = STR_TO_FLOAT(wind_Ao);
    return true;
  }
  return false;
}

double IndexModelImpl::wind_a() const
{
  return m_wind_a.toDouble();
}

bool IndexModelImpl::setWind_a(const double wind_a)
{
  m_wind_a = QString::number(wind_a);
  return true;
}

bool IndexModelImpl::setWind_a(const std::string &wind_a)
{
  bool ok;
  FLOAT_CHECK(wind_a,&ok);
  if(ok)
  {
    m_wind_a = STR_TO_FLOAT(wind_a);
    return true;
  }
  return false;
}

double IndexModelImpl::scale() const
{
  return m_scale.toDouble();
}

bool IndexModelImpl::setScale(const double scale)
{
  m_scale = QString::number(scale);
  return true;
}

bool IndexModelImpl::setScale(const std::string &scale)
{
  bool ok;
  FLOAT_CHECK(scale,&ok);
  if(ok)
  {
    m_scale = STR_TO_FLOAT(scale);
    return true;
  }
  return false;
}

int IndexModelImpl::uScale() const
{
  return m_uScale;
}

void IndexModelImpl::setUScale(const int uScale)
{
  m_uScale = uScale;
}

int IndexModelImpl::orgRow() const
{
  return m_orgRow;
}

void IndexModelImpl::setOrgRow(const int orgRow)
{
  m_orgRow = orgRow;
}

int IndexModelImpl::orgCol() const
{
  return m_orgCol;
}

void IndexModelImpl::setOrgCol(const int orgCol)
{
  m_orgCol = orgCol;
}

int IndexModelImpl::invYaxis() const
{
  return m_invYaxis;
}

void IndexModelImpl::setInvYaxis(const int invYaxis)
{
  m_invYaxis = invYaxis;
}

int IndexModelImpl::showGeom() const
{
  return m_showGeom;
}

void IndexModelImpl::setShowGeom(const int showGeom)
{
  m_showGeom = showGeom;
}

WeatherData IndexModelImpl::ssWeather() const
{
  return m_ssWeather;
}

void IndexModelImpl::setSsWeather(const WeatherData &ssWeather)
{
  m_ssWeather = ssWeather;
}

WeatherData IndexModelImpl::wptWeather() const
{
  return m_wptWeather;
}

void IndexModelImpl::setWptWeather(const WeatherData &wptWeather)
{
  m_wptWeather = wptWeather;
}

std::string IndexModelImpl::WTHpath() const
{
  return m_WTHpath;
}

void IndexModelImpl::setWTHpath(const std::string &WTHpath)
{
  m_WTHpath = WTHpath;
}

std::string IndexModelImpl::CTMpath() const
{
  return m_CTMpath;
}

void IndexModelImpl::setCTMpath(const std::string &CTMpath)
{
  m_CTMpath = CTMpath;
}

std::string IndexModelImpl::CVFpath() const
{
  return m_CVFpath;
}

void IndexModelImpl::setCVFpath(const std::string &CVFpath)
{
  m_CVFpath = CVFpath;
}

std::string IndexModelImpl::DVFpath() const
{
  return m_DVFpath;
}

void IndexModelImpl::setDVFpath(const std::string &DVFpath)
{
  m_DVFpath = DVFpath;
}

std::string IndexModelImpl::WPCfile() const
{
  return m_WPCfile;
}

void IndexModelImpl::setWPCfile(const std::string &WPCfile)
{
  m_WPCfile = WPCfile;
}

std::string IndexModelImpl::EWCfile() const
{
  return m_EWCfile;
}

void IndexModelImpl::setEWCfile(const std::string &EWCfile)
{
  m_EWCfile = EWCfile;
}

std::string IndexModelImpl::WPCdesc() const
{
  return m_WPCdesc;
}

void IndexModelImpl::setWPCdesc(const std::string &WPCdesc)
{
  m_WPCdesc = WPCdesc;
}

double IndexModelImpl::X0() const
{
  return m_X0.toDouble();
}

bool IndexModelImpl::setX0(const double X0)
{
  m_X0 = QString::number(X0);
  return true;
}

bool IndexModelImpl::setX0(const std::string &X0)
{
  bool ok;
  FLOAT_CHECK(X0,&ok);
  if(ok)
  {
    m_X0 = STR_TO_FLOAT(X0);
    return true;
  }
  return false;
}

double IndexModelImpl::Y0() const
{
  return m_Y0.toDouble();
}

bool IndexModelImpl::setY0(const double Y0)
{
  m_Y0 = QString::number(Y0);
  return true;
}

bool IndexModelImpl::setY0(const std::string &Y0)
{
  bool ok;
  FLOAT_CHECK(Y0,&ok);
  if(ok)
  {
    m_Y0 = STR_TO_FLOAT(Y0);
    return true;
  }
  return false;
}

double IndexModelImpl::Z0() const
{
  return m_Z0.toDouble();
}

bool IndexModelImpl::setZ0(const double Z0)
{
  m_Z0 = QString::number(Z0);
  return true;
}

bool IndexModelImpl::setZ0(const std::string &Z0)
{
  bool ok;
  FLOAT_CHECK(Z0,&ok);
  if(ok)
  {
    m_Z0 = STR_TO_FLOAT(Z0);
    return true;
  }
  return false;
}

double IndexModelImpl::angle() const
{
  return m_angle.toDouble();
}

bool IndexModelImpl::setAngle(const double angle)
{
  m_angle = QString::number(angle);
  return true;
}

bool IndexModelImpl::setAngle(const std::string &angle)
{
  bool ok;
  FLOAT_CHECK(angle,&ok);
  if(ok)
  {
    m_angle = STR_TO_FLOAT(angle);
    return true;
  }
  return false;
}

int IndexModelImpl::u_XYZ() const
{
  return m_u_XYZ;
}

void IndexModelImpl::setU_XYZ(const int u_XYZ)
{
  m_u_XYZ = u_XYZ;
}

double IndexModelImpl::epsPath() const
{
  return m_epsPath.toDouble();
}

bool IndexModelImpl::setEpsPath(const double epsPath)
{
  m_epsPath = QString::number(epsPath);
  return true;
}

bool IndexModelImpl::setEpsPath(const std::string &epsPath)
{
  bool ok;
  FLOAT_CHECK(epsPath,&ok);
  if(ok)
  {
    m_epsPath = STR_TO_FLOAT(epsPath);
    return true;
  }
  return false;
}

double IndexModelImpl::epsSpcs() const
{
  return m_epsSpcs.toDouble();
}

bool IndexModelImpl::setEpsSpcs(const double epsSpcs)
{
  m_epsSpcs = QString::number(epsSpcs);
  return true;
}

bool IndexModelImpl::setEpsSpcs(const std::string &epsSpcs)
{
  bool ok;
  FLOAT_CHECK(epsSpcs,&ok);
  if(ok)
  {
    m_epsSpcs = STR_TO_FLOAT(epsSpcs);
    return true;
  }
  return false;
}

std::string IndexModelImpl::tShift() const
{
  return m_tShift;
}

void IndexModelImpl::setTShift(const std::string &tShift)
{
  m_tShift = tShift;
}

std::string IndexModelImpl::dStart() const
{
  return m_dStart;
}

void IndexModelImpl::setDStart(const std::string &dStart)
{
  m_dStart = dStart;
}

std::string IndexModelImpl::dEnd() const
{
  return m_dEnd;
}

void IndexModelImpl::setDEnd(const std::string &dEnd)
{
  m_dEnd = dEnd;
}

int IndexModelImpl::useWPCwp() const
{
  return m_useWPCwp;
}

void IndexModelImpl::setUseWPCwp(const int useWPCwp)
{
  m_useWPCwp = useWPCwp;
}

int IndexModelImpl::useWPCmf() const
{
  return m_useWPCmf;
}

void IndexModelImpl::setUseWPCmf(const int useWPCmf)
{
  m_useWPCmf = useWPCmf;
}

int IndexModelImpl::wpctrig() const
{
  return m_wpctrig;
}

void IndexModelImpl::setWpctrig(const int wpctrig)
{
  m_wpctrig = wpctrig;
}

double IndexModelImpl::latd() const
{
  return m_latd.toDouble();
}

bool IndexModelImpl::setLatd(const double latd)
{
  m_latd = QString::number(latd);
  return true;
}

bool IndexModelImpl::setLatd(const std::string &latd)
{
  bool ok;
  FLOAT_CHECK(latd,&ok);
  if(ok)
  {
    m_latd = STR_TO_FLOAT(latd);
    return true;
  }
  return false;
}

double IndexModelImpl::lgtd() const
{
  return m_lgtd.toDouble();
}

bool IndexModelImpl::setLgtd(const double lgtd)
{
  m_lgtd = QString::number(lgtd);
  return true;
}

bool IndexModelImpl::setLgtd(const std::string &lgtd)
{
  bool ok;
  FLOAT_CHECK(lgtd,&ok);
  if(ok)
  {
    m_lgtd = STR_TO_FLOAT(lgtd);
    return true;
  }
  return false;
}

double IndexModelImpl::Tznr() const
{
  return m_Tznr.toDouble();
}

bool IndexModelImpl::setTznr(const double Tznr)
{
  m_Tznr = QString::number(Tznr);
  return true;
}

bool IndexModelImpl::setTznr(const std::string &Tznr)
{
  bool ok;
  FLOAT_CHECK(Tznr,&ok);
  if(ok)
  {
    m_Tznr = STR_TO_FLOAT(Tznr);
    return true;
  }
  return false;
}

double IndexModelImpl::altd() const
{
  return m_altd.toDouble();
}

bool IndexModelImpl::setAltd(const double altd)
{
  m_altd = QString::number(altd);
  return true;
}

bool IndexModelImpl::setAltd(const std::string &altd)
{
  bool ok;
  FLOAT_CHECK(altd,&ok);
  if(ok)
  {
    m_altd = STR_TO_FLOAT(altd);
    return true;
  }
  return false;
}

double IndexModelImpl::Tgrnd() const
{
  return m_Tgrnd.toDouble();
}

bool IndexModelImpl::setTgrnd(const double Tgrnd)
{
  m_Tgrnd = QString::number(Tgrnd);
  return true;
}

bool IndexModelImpl::setTgrnd(const std::string &Tgrnd)
{
  bool ok;
  FLOAT_CHECK(Tgrnd,&ok);
  if(ok)
  {
    m_Tgrnd = STR_TO_FLOAT(Tgrnd);
    return true;
  }
  return false;
}

int IndexModelImpl::utg() const
{
  return m_utg;
}

void IndexModelImpl::setUtg(const int utg)
{
  m_utg = utg;
}

int IndexModelImpl::u_a() const
{
  return m_u_a;
}

void IndexModelImpl::setU_a(const int u_a)
{
  m_u_a = u_a;
}

RunControl IndexModelImpl::rc() const
{
  return m_rc;
}
void IndexModelImpl::setRc(const RunControl rc)
{
  m_rc = rc;
}

std::vector<int> IndexModelImpl::contaminants() const
{
  return m_contaminants;
}

std::vector<Species> IndexModelImpl::species() const
{
  return m_species;
}

void IndexModelImpl::setSpecies(const std::vector<Species> &species)
{
  m_species = species;
  rebuildContaminants();
}

void IndexModelImpl::addSpecies(Species &species)
{
  species.setNr(m_species.size()+1);
  m_species.push_back(species);
}

std::vector<Level> IndexModelImpl::levels() const
{
  return m_levels;
}

void IndexModelImpl::setLevels(const std::vector<Level> &levels)
{
  m_levels = levels;
}

void IndexModelImpl::addLevel(Level &level)
{
  level.setNr(m_levels.size()+1);
  m_levels.push_back(level);
}

std::vector<DaySchedule> IndexModelImpl::daySchedules() const
{
  return m_daySchedules;
}

void IndexModelImpl::setDaySchedules(const std::vector<DaySchedule> &daySchedules)
{
  m_daySchedules = daySchedules;
}

std::vector<WeekSchedule> IndexModelImpl::weekSchedules() const
{
  return m_weekSchedules;
}
void IndexModelImpl::setWeekSchedules(const std::vector<WeekSchedule> &weekSchedules)
{
  m_weekSchedules = weekSchedules;
}

std::vector<WindPressureProfile> IndexModelImpl::windPressureProfiles() const
{
  return m_windPressureProfiles;
}
void IndexModelImpl::setWindPressureProfiles(const std::vector<WindPressureProfile> &windPressureProfiles)
{
  m_windPressureProfiles = windPressureProfiles;
}

std::vector<PlrLeak1> IndexModelImpl::getPlrLeak1()
{
  std::vector<PlrLeak1> afe;
  for(int i=0;i<m_airflowElements.size();i++)
  {
    if(m_airflowElements[i]->dataType() == "plr_leak1")
    {
      afe.push_back(*(m_airflowElements[i].dynamicCast<PlrLeak1>().data()));
    }
  }
  return afe;
}

std::vector<PlrLeak2> IndexModelImpl::getPlrLeak2()
{
  std::vector<PlrLeak2> afe;
  for(int i=0;i<m_airflowElements.size();i++)
  {
    if(m_airflowElements[i]->dataType() == "plr_leak2")
    {
      afe.push_back(*(m_airflowElements[i].dynamicCast<PlrLeak2>().data()));
    }
  }
  return afe;
}

std::vector<PlrTest1> IndexModelImpl::getPlrTest1()
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

std::vector<CvfDat> IndexModelImpl::getCvfDat()
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

std::vector<Ahs> IndexModelImpl::ahs() const
{
  return m_ahs;
}

void IndexModelImpl::setAhs(const std::vector<Ahs> &ahs)
{
  m_ahs = ahs;
}

void IndexModelImpl::addAhs(Ahs &ahs)
{
  ahs.setNr(m_ahs.size()+1);
  m_ahs.push_back(ahs);
}

std::vector<Zone> IndexModelImpl::zones() const
{
  return m_zones;
}

void IndexModelImpl::setZones(const std::vector<Zone> &zones)
{
  m_zones = zones;
}

void IndexModelImpl::addZone(Zone &zone)
{
  zone.setNr(m_zones.size()+1);
  m_zones.push_back(zone);
}

bool IndexModelImpl::valid() const
{
  return m_valid;
}

std::vector<AirflowPath> IndexModelImpl::airflowPaths() const
{
  return m_paths;
}

void IndexModelImpl::setAirflowPaths(const std::vector<AirflowPath> &paths)
{
  m_paths = paths;
}

void IndexModelImpl::addAirflowPath(AirflowPath &path)
{
  path.setNr(m_paths.size()+1);
  m_paths.push_back(path);
}

void IndexModelImpl::rebuildContaminants()
{
  m_contaminants.clear();
  for(unsigned i=1;i<=m_species.size();i++)
  {
    m_species[i].setNr(i);
    if(m_species[i].sflag())
    {
      m_contaminants.push_back(i);
    }
  }
}

void IndexModelImpl::readZoneIc(Reader &input)
{
  unsigned int nn = input.readUInt();
  if(nn != 0)
  {
    unsigned int nctm = m_contaminants.size();
    if(nn != nctm*m_zones.size())
    {
      QString mesg("Mismatch between number of zones, contaminants, and initial conditions");
      LOG_FREE_AND_THROW("openstudio.contam.ForwardTranslator",mesg.toStdString());
    }
    for(unsigned int i=0;i<m_zones.size();i++)
    {
      unsigned int nr = input.readUInt();
      if(nr != i+1)
      {
        QString mesg = QString("Mismatch between zone IC number and zone number at line %1 ")
          .arg(input.lineNumber());
        LOG_FREE_AND_THROW("openstudio.contam.ForwardTranslator",mesg.toStdString());
      }
      std::vector<std::string> ic;
      for(unsigned int j=0;j<nctm;j++)
      {
        ic.push_back(input.readNumber<std::string>());
      }
      m_zones[i].setIc(ic);
    }
  }
  input.read999("Failed to find zone IC section termination");
}

std::string IndexModelImpl::writeZoneIc(int start)
{
  int offset = 1;
  if(start != 0)
  {
    offset = 1-start;
  }
  int nctm = m_contaminants.size()*(m_zones.size()-start);
  std::string string = ANY_TO_STR(nctm) + " ! initial zone concentrations:\n";
  if(nctm)
  {
    for(unsigned i=start;i<m_zones.size();i++)
    {
      string += ANY_TO_STR(i+offset);
      for(unsigned j=0;j<m_contaminants.size();j++)
      {
        string += ' ' + ANY_TO_STR(m_zones[i].ic(j));
      }
      string += '\n';
    }
  }
  return string  + "-999\n";
}

int IndexModelImpl::airflowElementNrByName(std::string name) const
{
  for(int i=0;i<m_airflowElements.size();i++)
  {
    if(m_airflowElements[i]->name() == name)
    {
      return m_airflowElements[i]->nr();
    }
  }
  return 0;
}

std::vector<std::vector<int> > IndexModelImpl::zoneExteriorFlowPaths()
{
  std::vector<std::vector<int> > paths(m_zones.size());

  for(unsigned int i=0;i<m_paths.size();i++)
  {
    if(m_paths[i].pzn() == -1)
    {
      int nr = m_paths[i].pzm();
      if(nr > 0 && (unsigned int)nr<=m_zones.size())
      {
        paths[nr-1].push_back(-m_paths[i].nr()); // This flow path is negative for flow into zone
      }
    }
    else if(m_paths[i].pzm() == -1)
    {
      int nr = m_paths[i].pzn();
      if(nr > 0 && (unsigned int)nr<=m_zones.size())
      {
        paths[nr-1].push_back(m_paths[i].nr()); // This flow path is positive for flow into zone
      }
    }
  }
  return paths;
}

std::vector<TimeSeries> IndexModelImpl::zoneInfiltration(SimFile *sim)
{
  // This should probably include a lot more checks of things and is written in
  // somewhat strange way to avoid taking too much advantage of the specifics 
  // of the text form outputs.
  std::vector<TimeSeries> results;
  std::vector<std::vector<int> > paths = zoneExteriorFlowPaths();
  unsigned int ntimes = sim->dateTimes().size();
  for(unsigned int i=0; i<m_zones.size(); i++)
  {
    // This is lame, but I can't tell for sure if the values of a Vector are actually zero.
    Vector inf = createVector(std::vector<double>(ntimes,0));
    for(unsigned int j=0; j<paths[i].size(); j++)
    {
      if(paths[i][j] > 0) // Positive values are infiltration
      {
        boost::optional<openstudio::TimeSeries> optFlow = sim->pathFlow(paths[i][j]);
        if(optFlow)
        {
          Vector flow = optFlow.get().values();
          for(unsigned int k=0; k<ntimes; k++)
          {
            if(flow[k] > 0)
            {
              inf[k] += flow[k];
            }
          }
        }
        else
        {
          // Perhaps a warning? This shouldn't really happen unless someone has excluded a path from the 
          // results file for some reason - which is unlikely to be accidental. So there must be a good reason
          // for getting here, and for now we won't issue a warning.
        }
      }
      else // Negative values are infiltration
      {
        boost::optional<openstudio::TimeSeries> optFlow = sim->pathFlow(paths[i][j]);
        if(optFlow)
        {
          Vector flow = optFlow.get().values();
          for(unsigned int k=0; k<ntimes; k++)
          {
            if(flow[k] < 0)
            {
              inf[k] -= flow[k];
            }
          }
        }
        else
        {
          // See above comment
        }
      }
    }
    results.push_back(openstudio::TimeSeries(sim->dateTimes(),inf,"kg/s"));
  }
  return results;
}

std::vector<TimeSeries> IndexModelImpl::pathInfiltration(std::vector<int> pathNrs, SimFile *sim)
{
  // This should probably include a lot more checks of things and is written in
  // somewhat strange way to avoid taking too much advantage of the specifics 
  // of the text form outputs.
  std::vector<TimeSeries> results;
  std::vector<std::vector<int> > paths = zoneExteriorFlowPaths();
  unsigned int ntimes = 1;
  std::vector<DateTime> dateTimes = sim->dateTimes();
  if(sim->dateTimes().size()!=1)
  {
    ntimes = sim->dateTimes().size()-1;
    dateTimes = std::vector<DateTime>(dateTimes.begin() + 1,dateTimes.end());
  }
  for(unsigned int i=0; i<pathNrs.size(); i++)
  {
    Vector inf = createVector(std::vector<double>(ntimes,0.0));
    if(pathNrs[i]<=0 || (unsigned)pathNrs[i] > m_paths.size())
    {
      // Possibly should issue a warning here, the path number is out of range
    }
    else
    {
      contam::AirflowPath path = m_paths[pathNrs[i]-1];
      if(path.pzn() == -1)
      {
        // This flow path is negative for flow into zone
        boost::optional<openstudio::TimeSeries> optFlow = sim->pathFlow(path.nr());
        if(optFlow)
        {
          Vector flow = optFlow.get().values();
          for(unsigned int k=0; k<ntimes; k++)
          {
            if(flow[k] < 0)
            {
              inf[k] = -flow[k];
            }
          }
        }
        else
        {
          // Perhaps a warning? This shouldn't really happen unless someone has excluded a path from the 
          // results file for some reason - which is unlikely to be accidental. So there must be a good reason
          // for getting here, and for now we won't issue a warning.
        }
      }
      else if(path.pzm() == -1)
      {
        // This flow path is positive for flow into zone
        boost::optional<openstudio::TimeSeries> optFlow = sim->pathFlow(path.nr());
        if(optFlow)
        {
          Vector flow = optFlow.get().values();
          for(unsigned int k=0; k<ntimes; k++)
          {
            if(flow[k] > 0)
            {
              inf[k] = flow[k];
            }
          }
        }
        else
        {
          // Perhaps a warning? This shouldn't really happen unless someone has excluded a path from the 
          // results file for some reason - which is unlikely to be accidental. So there must be a good reason
          // for getting here, and for now we won't issue a warning.
        }
      }
      else
      {
        // Another situation that might need a warning, since the path is not connected to the ambient
      }
    }
    // Save the time series
    results.push_back(openstudio::TimeSeries(dateTimes,inf,"kg/s"));
  }
  return results;
}

} // detail
} // contam
} // openstudio
