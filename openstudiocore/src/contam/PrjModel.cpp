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
#include "PrjModel.hpp"
#include "PrjReader.hpp"
#include "SimFile.hpp"

#include "PrjModelImpl.hpp"

#include <QFile>

namespace openstudio {
namespace contam {

IndexModel::IndexModel()
{
  m_impl = boost::shared_ptr<detail::IndexModelImpl>(new detail::IndexModelImpl);
}

IndexModel::IndexModel(openstudio::path path)
{
  m_impl = boost::shared_ptr<detail::IndexModelImpl>(new detail::IndexModelImpl);
  m_impl->read(path);
}

IndexModel::IndexModel(std::string filename)
{
  m_impl = boost::shared_ptr<detail::IndexModelImpl>(new detail::IndexModelImpl);
  m_impl->read(filename);
}

IndexModel::IndexModel(Reader &input)
{
  m_impl = boost::shared_ptr<detail::IndexModelImpl>(new detail::IndexModelImpl);
  m_impl->read(input);
}

IndexModel::IndexModel(const IndexModel &other) : m_impl(other.m_impl)
{}

IndexModel::~IndexModel()
{}

IndexModel& IndexModel::operator=(const IndexModel &other)
{
  m_impl = other.m_impl;
  return *this;
}

bool IndexModel::operator==(const IndexModel &other) const
{
  return m_impl==other.m_impl;
}

bool IndexModel::operator!=(const IndexModel &other) const
{
  return m_impl!=other.m_impl;
}

bool IndexModel::read(openstudio::path path)
{
  return m_impl->read(openstudio::toString(path));
}

bool IndexModel::read(std::string filename)
{
  return m_impl->read(filename);
}

bool IndexModel::read(Reader &input)
{
  return m_impl->read(input);
}

std::string IndexModel::toString()
{
  return m_impl->toString();
}

std::string IndexModel::programName() const
{
  return m_impl->programName();
}

void IndexModel::setProgramName(const std::string &name)
{
  m_impl->setProgramName(name);
}

std::string IndexModel::version() const
{
  return m_impl->version();
}

void IndexModel::setVersion(const std::string &version)
{
  m_impl->setVersion(version);
}

int IndexModel::echo() const
{
  return m_impl->echo();
}

void IndexModel::setEcho(const int echo)
{
  m_impl->setEcho(echo);
}

std::string IndexModel::desc() const
{
  return m_impl->desc();
}

void IndexModel::setDesc(const std::string &prjdesc)
{
  m_impl->setDesc(prjdesc);
}

int IndexModel::skheight() const
{
  return m_impl->skheight();
}

void IndexModel::setSkheight(const int skheight)
{
  m_impl->setSkheight(skheight);
}

int IndexModel::skwidth() const
{
  return m_impl->skwidth();
}

void IndexModel::setSkwidth(const int skwidth)
{
  m_impl->setSkwidth(skwidth);
}

int IndexModel::def_units() const
{
  return m_impl->def_units();
}

void IndexModel::setDef_units(const int def_units)
{
  m_impl->setDef_units(def_units);
}

int IndexModel::def_flows() const
{
  return m_impl->def_flows();
}

void IndexModel::setDef_flows(const int def_flows)
{
  m_impl->setDef_flows(def_flows);
}

double IndexModel::def_T() const
{
  return m_impl->def_T();
}

bool IndexModel::setDef_T(const double def_T)
{
  return m_impl->setDef_T(def_T);
}

bool IndexModel::setDef_T(const std::string &def_T)
{
  return m_impl->setDef_T(def_T);
}

int IndexModel::udefT() const
{
  return m_impl->udefT();
}

void IndexModel::setUdefT(const int udefT)
{
  m_impl->setUdefT(udefT);
}

double IndexModel::rel_N() const
{
  return m_impl->rel_N();
}

bool IndexModel::setRel_N(const double rel_N)
{
  return m_impl->setRel_N(rel_N);
}

bool IndexModel::setRel_N(const std::string &rel_N)
{
  return m_impl->setRel_N(rel_N);
}

double IndexModel::wind_H() const
{
  return m_impl->wind_H();
}

bool IndexModel::setWind_H(const double wind_H)
{
  return m_impl->setWind_H(wind_H);
}

bool IndexModel::setWind_H(const std::string &wind_H)
{
  return m_impl->setWind_H(wind_H);
}

int IndexModel::uwH() const
{
  return m_impl->uwH();
}

void IndexModel::setUwH(const int uwH)
{
  m_impl->setUwH(uwH);
}

double IndexModel::wind_Ao() const
{
  return m_impl->wind_Ao();
}

bool IndexModel::setWind_Ao(const double wind_Ao)
{
  return m_impl->setWind_Ao(wind_Ao);
}

bool IndexModel::setWind_Ao(const std::string &wind_Ao)
{
  return m_impl->setWind_Ao(wind_Ao);
}

double IndexModel::wind_a() const
{
  return m_impl->wind_a();
}

bool IndexModel::setWind_a(const double wind_a)
{
  return m_impl->setWind_a(wind_a);
}

bool IndexModel::setWind_a(const std::string &wind_a)
{
  return m_impl->setWind_a(wind_a);
}

double IndexModel::scale() const
{
  return m_impl->scale();
}

bool IndexModel::setScale(const double scale)
{
  return m_impl->setScale(scale);
}

bool IndexModel::setScale(const std::string &scale)
{
  return m_impl->setScale(scale);
}

int IndexModel::uScale() const
{
  return m_impl->uScale();
}

void IndexModel::setUScale(const int uScale)
{
  m_impl->setUScale(uScale);
}

int IndexModel::orgRow() const
{
  return m_impl->orgRow();
}

void IndexModel::setOrgRow(const int orgRow)
{
  m_impl->setOrgRow(orgRow);
}

int IndexModel::orgCol() const
{
  return m_impl->orgCol();
}

void IndexModel::setOrgCol(const int orgCol)
{
  m_impl->setOrgCol(orgCol);
}

int IndexModel::invYaxis() const
{
  return m_impl->invYaxis();
}

void IndexModel::setInvYaxis(const int invYaxis)
{
  m_impl->setInvYaxis(invYaxis);
}

int IndexModel::showGeom() const
{
  return m_impl->showGeom();
}

void IndexModel::setShowGeom(const int showGeom)
{
  m_impl->setShowGeom(showGeom);
}

WeatherData IndexModel::ssWeather() const
{
  return m_impl->ssWeather();
}

void IndexModel::setSsWeather(const WeatherData &ssWeather)
{
  m_impl->setSsWeather(ssWeather);
}

WeatherData IndexModel::wptWeather() const
{
  return m_impl->wptWeather();
}

void IndexModel::setWptWeather(const WeatherData &wptWeather)
{
  m_impl->setWptWeather(wptWeather);
}

std::string IndexModel::WTHpath() const
{
  return m_impl->WTHpath();
}

void IndexModel::setWTHpath(const std::string &WTHpath)
{
  m_impl->setWTHpath(WTHpath);
}

std::string IndexModel::CTMpath() const
{
  return m_impl->CTMpath();
}

void IndexModel::setCTMpath(const std::string &CTMpath)
{
  m_impl->setCTMpath(CTMpath);
}

std::string IndexModel::CVFpath() const
{
  return m_impl->CVFpath();
}

void IndexModel::setCVFpath(const std::string &CVFpath)
{
  m_impl->setCVFpath(CVFpath);
}

std::string IndexModel::DVFpath() const
{
  return m_impl->DVFpath();
}

void IndexModel::setDVFpath(const std::string &DVFpath)
{
  m_impl->setDVFpath(DVFpath);
}

std::string IndexModel::WPCfile() const
{
  return m_impl->WPCfile();
}

void IndexModel::setWPCfile(const std::string &WPCfile)
{
  m_impl->setWPCfile(WPCfile);
}

std::string IndexModel::EWCfile() const
{
  return m_impl->EWCfile();
}

void IndexModel::setEWCfile(const std::string &EWCfile)
{
  m_impl->setEWCfile(EWCfile);
}

std::string IndexModel::WPCdesc() const
{
  return m_impl->WPCdesc();
}

void IndexModel::setWPCdesc(const std::string &WPCdesc)
{
  m_impl->setWPCdesc(WPCdesc);
}

double IndexModel::X0() const
{
  return m_impl->X0();
}

bool IndexModel::setX0(const double X0)
{
  return m_impl->setX0(X0);
}

bool IndexModel::setX0(const std::string &X0)
{
  return m_impl->setX0(X0);
}

double IndexModel::Y0() const
{
  return m_impl->Y0();
}

bool IndexModel::setY0(const double Y0)
{
  return m_impl->setY0(Y0);
}

bool IndexModel::setY0(const std::string &Y0)
{
  return m_impl->setY0(Y0);
}

double IndexModel::Z0() const
{
  return m_impl->Z0();
}

bool IndexModel::setZ0(const double Z0)
{
  return m_impl->setZ0(Z0);
}

bool IndexModel::setZ0(const std::string &Z0)
{
  return m_impl->setZ0(Z0);
}

double IndexModel::angle() const
{
  return m_impl->angle();
}

bool IndexModel::setAngle(const double angle)
{
  return m_impl->setAngle(angle);
}

bool IndexModel::setAngle(const std::string &angle)
{
  return m_impl->setAngle(angle);
}

int IndexModel::u_XYZ() const
{
  return m_impl->u_XYZ();
}

void IndexModel::setU_XYZ(const int u_XYZ)
{
  m_impl->setU_XYZ(u_XYZ);
}

double IndexModel::epsPath() const
{
  return m_impl->epsPath();
}

bool IndexModel::setEpsPath(const double epsPath)
{
  return m_impl->setEpsPath(epsPath);
}

bool IndexModel::setEpsPath(const std::string &epsPath)
{
  return m_impl->setEpsPath(epsPath);
}

double IndexModel::epsSpcs() const
{
  return m_impl->epsSpcs();
}

bool IndexModel::setEpsSpcs(const double epsSpcs)
{
  return m_impl->setEpsSpcs(epsSpcs);
}

bool IndexModel::setEpsSpcs(const std::string &epsSpcs)
{
  return m_impl->setEpsSpcs(epsSpcs);
}

std::string IndexModel::tShift() const
{
  return m_impl->tShift();
}

void IndexModel::setTShift(const std::string &tShift)
{
  m_impl->setTShift(tShift);
}

std::string IndexModel::dStart() const
{
  return m_impl->dStart();
}

void IndexModel::setDStart(const std::string &dStart)
{
  m_impl->setDStart(dStart);
}

std::string IndexModel::dEnd() const
{
  return m_impl->dEnd();
}

void IndexModel::setDEnd(const std::string &dEnd)
{
  m_impl->setDEnd(dEnd);
}

int IndexModel::useWPCwp() const
{
  return m_impl->useWPCwp();
}

void IndexModel::setUseWPCwp(const int useWPCwp)
{
  m_impl->setUseWPCwp(useWPCwp);
}

int IndexModel::useWPCmf() const
{
  return m_impl->useWPCmf();
}

void IndexModel::setUseWPCmf(const int useWPCmf)
{
  m_impl->setUseWPCmf(useWPCmf);
}

int IndexModel::wpctrig() const
{
  return m_impl->wpctrig();
}

void IndexModel::setWpctrig(const int wpctrig)
{
  m_impl->setWpctrig(wpctrig);
}

double IndexModel::latd() const
{
  return m_impl->latd();
}

bool IndexModel::setLatd(const double latd)
{
  return m_impl->setLatd(latd);
}

bool IndexModel::setLatd(const std::string &latd)
{
  return m_impl->setLatd(latd);
}

double IndexModel::lgtd() const
{
  return m_impl->lgtd();
}

bool IndexModel::setLgtd(const double lgtd)
{
  return m_impl->setLgtd(lgtd);
}

bool IndexModel::setLgtd(const std::string &lgtd)
{
  return m_impl->setLgtd(lgtd);
}

double IndexModel::Tznr() const
{
  return m_impl->Tznr();
}

bool IndexModel::setTznr(const double Tznr)
{
  return m_impl->setTznr(Tznr);
}

bool IndexModel::setTznr(const std::string &Tznr)
{
  return m_impl->setTznr(Tznr);
}

double IndexModel::altd() const
{
  return m_impl->altd();
}

bool IndexModel::setAltd(const double altd)
{
  return m_impl->setAltd(altd);
}

bool IndexModel::setAltd(const std::string &altd)
{
  return m_impl->setAltd(altd);
}

double IndexModel::Tgrnd() const
{
  return m_impl->Tgrnd();
}

bool IndexModel::setTgrnd(const double Tgrnd)
{
  return m_impl->setTgrnd(Tgrnd);
}

bool IndexModel::setTgrnd(const std::string &Tgrnd)
{
  return m_impl->setTgrnd(Tgrnd);
}

int IndexModel::utg() const
{
  return m_impl->utg();
}

void IndexModel::setUtg(const int utg)
{
  m_impl->setUtg(utg);
}

int IndexModel::u_a() const
{
  return m_impl->u_a();
}

void IndexModel::setU_a(const int u_a)
{
  m_impl->setU_a(u_a);
}

RunControl IndexModel::rc() const
{
  return m_impl->rc();
}
void IndexModel::setRc(const RunControl rc)
{
  m_impl->setRc(rc);
}

std::vector<int> IndexModel::contaminants() const
{
  return m_impl->contaminants();
}

std::vector <Species> IndexModel::species() const
{
  return m_impl->species();
}
void IndexModel::setSpecies(const std::vector<Species> species)
{
  m_impl->setSpecies(species);
}

std::vector <Level> IndexModel::levels() const
{
  return m_impl->levels();
}
void IndexModel::setLevels(const std::vector<Level> levels)
{
  m_impl->setLevels(levels);
}
void IndexModel::addLevel(Level level)
{
  m_impl->addLevel(level);
}

std::vector <DaySchedule> IndexModel::daySchedules() const
{
  return m_impl->daySchedules();
}
void IndexModel::setDaySchedules(const std::vector<DaySchedule> daySchedules)
{
  m_impl->setDaySchedules(daySchedules);
}

std::vector <WeekSchedule> IndexModel::weekSchedules() const
{
  return m_impl->weekSchedules();
}
void IndexModel::setWeekSchedules(const std::vector<WeekSchedule> weekSchedules)
{
  m_impl->setWeekSchedules(weekSchedules);
}

std::vector <WindPressureProfile> IndexModel::windPressureProfiles() const
{
  return m_impl->windPressureProfiles();
}
void IndexModel::setWindPressureProfiles(const std::vector<WindPressureProfile> windPressureProfiles)
{
  m_impl->setWindPressureProfiles(windPressureProfiles);
}

std::vector<PlrTest1> IndexModel::getPlrTest1() const
{
  return m_impl->getPlrTest1();
}

bool IndexModel::addAirflowElement(PlrTest1 element)
{
  return m_impl->addAirflowElement(element);
}

int IndexModel::airflowElementNrByName(std::string name) const
{
  return m_impl->airflowElementNrByName(name);
}

bool IndexModel::replaceAirflowElement(int nr, PlrTest1 element)
{
  return m_impl->replaceAirflowElement(nr,element);
}

std::vector<CvfDat> IndexModel::getCvfDat() const
{
  return m_impl->getCvfDat();
}

bool IndexModel::addControlNode(CvfDat element, bool sequence)
{
  return m_impl->addControlNode(element,sequence);
}

std::vector <Ahs> IndexModel::ahs() const
{
  return m_impl->ahs();
}
void IndexModel::setAhs(const std::vector<Ahs> ahs)
{
  m_impl->setAhs(ahs);
}
void IndexModel::addAhs(Ahs ahs)
{
  m_impl->addAhs(ahs);
}

std::vector<Zone> IndexModel::zones() const
{
  return m_impl->zones();
}
void IndexModel::setZones(const std::vector<Zone> zones)
{
  m_impl->setZones(zones);
}
void IndexModel::addZone(Zone zone)
{
  m_impl->addZone(zone);
}

std::vector<AirflowPath> IndexModel::airflowPaths() const
{
  return m_impl->airflowPaths();
}
void IndexModel::setAirflowPaths(const std::vector<AirflowPath> paths)
{
  m_impl->setAirflowPaths(paths);
}
void IndexModel::addAirflowPath(AirflowPath path)
{
  m_impl->addAirflowPath(path);
}

bool IndexModel::valid() const
{
  return m_impl->valid();
}

std::vector<std::vector<int> > IndexModel::zoneExteriorFlowPaths()
{
  return m_impl->zoneExteriorFlowPaths();
}

std::vector<TimeSeries> IndexModel::zoneInfiltration(SimFile *sim)
{
  return m_impl->zoneInfiltration(sim);
}

std::vector<TimeSeries> IndexModel::pathInfiltration(std::vector<int> pathNrs, SimFile *sim)
{
  return m_impl->pathInfiltration(pathNrs, sim);
}

} // contam
} // openstudio
