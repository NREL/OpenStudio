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

#include "PrjModelImpl.hpp"
#include "PrjReader.hpp"
#include "SimFile.hpp"
#include <algorithm>

#include "../../utilities/core/StringHelpers.hpp"

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
  setDefaults();
  openstudio::filesystem::ifstream file(path);
  m_valid = false;
  if (file.is_open())
  {
    Reader input(file);
    read(input);
  }
  return m_valid;
}

bool IndexModelImpl::read(std::string filename)
{
  return read(openstudio::toPath(filename));
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
  std::vector<int> contaminants = input.readIntVector(false); // Might want to check that this matches the species data
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
  //m_sourceSinks = input.readSectionVector<SourceSink>(std::string("source/sink"));
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
  output += writeArray(contaminants(),"contaminants:");
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
  return boost::lexical_cast<double>(m_def_T);
}

bool IndexModelImpl::setDef_T(const double def_T)
{
  m_def_T = openstudio::number(def_T);
  return true;
}

bool IndexModelImpl::setDef_T(const std::string &def_T)
{
  return assign_if_valid<double>(def_T, m_def_T);
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
  return boost::lexical_cast<double>(m_rel_N);
}

bool IndexModelImpl::setRel_N(const double rel_N)
{
  m_rel_N = openstudio::number(rel_N);
  return true;
}

bool IndexModelImpl::setRel_N(const std::string &rel_N)
{
  return assign_if_valid<double>(rel_N, m_rel_N);
}

double IndexModelImpl::wind_H() const
{
  return boost::lexical_cast<double>(m_wind_H);
}

bool IndexModelImpl::setWind_H(const double wind_H)
{
  m_wind_H = openstudio::number(wind_H);
  return true;
}

bool IndexModelImpl::setWind_H(const std::string &wind_H)
{
  return assign_if_valid<double>(wind_H, m_wind_H);
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
  return boost::lexical_cast<double>(m_wind_Ao);
}

bool IndexModelImpl::setWind_Ao(const double wind_Ao)
{
  m_wind_Ao = openstudio::number(wind_Ao);
  return true;
}

bool IndexModelImpl::setWind_Ao(const std::string &wind_Ao)
{
  return assign_if_valid<double>(wind_Ao, m_wind_Ao);
}

double IndexModelImpl::wind_a() const
{
  return boost::lexical_cast<double>(m_wind_a);
}

bool IndexModelImpl::setWind_a(const double wind_a)
{
  m_wind_a = openstudio::number(wind_a);
  return true;
}

bool IndexModelImpl::setWind_a(const std::string &wind_a)
{
  return assign_if_valid<double>(wind_a, m_wind_a);
}

double IndexModelImpl::scale() const
{
  return boost::lexical_cast<double>(m_scale);
}

bool IndexModelImpl::setScale(const double scale)
{
  m_scale = openstudio::number(scale);
  return true;
}

bool IndexModelImpl::setScale(const std::string &scale)
{
  return assign_if_valid<double>(scale, m_scale);
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
  return boost::lexical_cast<double>(m_X0);
}

bool IndexModelImpl::setX0(const double X0)
{
  m_X0 = openstudio::number(X0);
  return true;
}

bool IndexModelImpl::setX0(const std::string &X0)
{
  return assign_if_valid<double>(X0, m_X0);
}

double IndexModelImpl::Y0() const
{
  return boost::lexical_cast<double>(m_Y0);
}

bool IndexModelImpl::setY0(const double Y0)
{
  m_Y0 = openstudio::number(Y0);
  return true;
}

bool IndexModelImpl::setY0(const std::string &Y0)
{
  return assign_if_valid<double>(Y0, m_Y0);
}

double IndexModelImpl::Z0() const
{
  return boost::lexical_cast<double>(m_Z0);
}

bool IndexModelImpl::setZ0(const double Z0)
{
  m_Z0 = openstudio::number(Z0);
  return true;
}

bool IndexModelImpl::setZ0(const std::string &Z0)
{
  return assign_if_valid<double>(Z0, m_Z0);
}

double IndexModelImpl::angle() const
{
  return boost::lexical_cast<double>(m_angle);
}

bool IndexModelImpl::setAngle(const double angle)
{
  m_angle = openstudio::number(angle);
  return true;
}

bool IndexModelImpl::setAngle(const std::string &angle)
{
  return assign_if_valid<double>(angle, m_angle);
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
  return boost::lexical_cast<double>(m_epsPath);
}

bool IndexModelImpl::setEpsPath(const double epsPath)
{
  m_epsPath = openstudio::number(epsPath);
  return true;
}

bool IndexModelImpl::setEpsPath(const std::string &epsPath)
{
  return assign_if_valid<double>(epsPath, m_epsPath);
}

double IndexModelImpl::epsSpcs() const
{
  return boost::lexical_cast<double>(m_epsSpcs);
}

bool IndexModelImpl::setEpsSpcs(const double epsSpcs)
{
  m_epsSpcs = openstudio::number(epsSpcs);
  return true;
}

bool IndexModelImpl::setEpsSpcs(const std::string &epsSpcs)
{
  return assign_if_valid<double>(epsSpcs, m_epsSpcs);
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
  return boost::lexical_cast<double>(m_latd);
}

bool IndexModelImpl::setLatd(const double latd)
{
  m_latd = openstudio::number(latd);
  return true;
}

bool IndexModelImpl::setLatd(const std::string &latd)
{
  return assign_if_valid<double>(latd, m_latd);
}

double IndexModelImpl::lgtd() const
{
  return boost::lexical_cast<double>(m_lgtd);
}

bool IndexModelImpl::setLgtd(const double lgtd)
{
  m_lgtd = openstudio::number(lgtd);
  return true;
}

bool IndexModelImpl::setLgtd(const std::string &lgtd)
{
  return assign_if_valid<double>(lgtd, m_lgtd);
}

double IndexModelImpl::Tznr() const
{
  return boost::lexical_cast<double>(m_Tznr);
}

bool IndexModelImpl::setTznr(const double Tznr)
{
  m_Tznr = openstudio::number(Tznr);
  return true;
}

bool IndexModelImpl::setTznr(const std::string &Tznr)
{
  return assign_if_valid<double>(Tznr, m_Tznr);
}

double IndexModelImpl::altd() const
{
  return boost::lexical_cast<double>(m_altd);
}

bool IndexModelImpl::setAltd(const double altd)
{
  m_altd = openstudio::number(altd);
  return true;
}

bool IndexModelImpl::setAltd(const std::string &altd)
{
  return assign_if_valid<double>(altd, m_altd);
}

double IndexModelImpl::Tgrnd() const
{
  return boost::lexical_cast<double>(m_Tgrnd);
}

bool IndexModelImpl::setTgrnd(const double Tgrnd)
{
  m_Tgrnd = openstudio::number(Tgrnd);
  return true;
}

bool IndexModelImpl::setTgrnd(const std::string &Tgrnd)
{
  return assign_if_valid<double>(Tgrnd, m_Tgrnd);
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
void IndexModelImpl::setRc(const RunControl &rc)
{
  m_rc = rc;
}

std::vector<int> IndexModelImpl::contaminants()
{
  std::vector<int> active;
  int nr = 1;
  for(Species &species : m_species) {
    species.setNr(nr);
    if(species.sflag()) {
      active.push_back(nr);
    }
    nr++;
  }
  return active;
}

std::vector<Species> IndexModelImpl::species() const
{
  return m_species;
}

void IndexModelImpl::setSpecies(const std::vector<Species> &species)
{
  m_species = species;
}

void IndexModelImpl::addSpecies(Species &species)
{
  species.setNr(m_species.size()+1);
  m_species.push_back(species);
}

bool IndexModelImpl::removeSpecies(const Species &species)
{
  unsigned originalSize = m_species.size();
  m_species.erase(std::remove_if(m_species.begin(), m_species.end(), [&](Species s){ return s==species; }), m_species.end());
  // There's probably a better way to do this
  if(m_species.size() != originalSize) {
    renumberVector(m_species);
    return true;
  }
  return false;
}

std::vector<Level> IndexModelImpl::levels() const
{
  return m_levels;
}

void IndexModelImpl::setLevels(const std::vector<Level> &levels)
{
  // This could use some validation, but we don't want to interfere too much in the loaded PRJ data
  m_levels = levels;
}

void IndexModelImpl::addLevel(Level &level)
{
  double refHt = 0;
  if(m_levels.size() > 0) {
    // Note that CONTAM stores heights in meters, so this next statement requires no conversion
    refHt = m_levels[m_levels.size()-1].refht() + m_levels[m_levels.size()-1].delht();
  }
  level.setNr(m_levels.size()+1);
  level.setRefht(refHt);
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

void IndexModelImpl::readZoneIc(Reader &input)
{
  unsigned int nn = input.readUInt();
  if(nn != 0)
  {
    unsigned int nctm = contaminants().size();
    if(nn != nctm*m_zones.size())
    {
      LOG_FREE_AND_THROW("openstudio.contam.ForwardTranslator", "Mismatch between number of zones, contaminants, and initial conditions");
    }
    for(unsigned int i=0;i<m_zones.size();i++)
    {
      unsigned int nr = input.readUInt();
      if(nr != i+1)
      {
        LOG_FREE_AND_THROW("openstudio.contam.ForwardTranslator", "Mismatch between zone IC number and zone number at line " << input.lineNumber());
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
  if(start != 0) {
    offset = 1-start;
  }
  int ncontaminants = contaminants().size();
  int nctm = ncontaminants*(m_zones.size()-start);
  std::string string = ANY_TO_STR(nctm) + " ! initial zone concentrations:\n";
  if(nctm) {
    for(unsigned i=start;i<m_zones.size();i++) {
      string += ANY_TO_STR(i+offset);
      for(unsigned j=0;j<(unsigned)ncontaminants;j++) {
        string += ' ' + ANY_TO_STR(m_zones[i].ic(j));
      }
      string += '\n';
    }
  }
  return string  + "-999\n";
}

int IndexModelImpl::airflowElementNrByName(std::string name) const
{
  for(unsigned i=0;i<m_airflowElements.size();i++) {
    if(m_airflowElements[i]->name() == name) {
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
