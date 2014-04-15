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

PrjModel::PrjModel()
{
  m_impl = boost::shared_ptr<detail::PrjModelImpl>(new detail::PrjModelImpl);
}

PrjModel::PrjModel(openstudio::path path)
{
  m_impl = boost::shared_ptr<detail::PrjModelImpl>(new detail::PrjModelImpl);
  m_impl->read(path);
}

PrjModel::PrjModel(std::string filename)
{
  m_impl = boost::shared_ptr<detail::PrjModelImpl>(new detail::PrjModelImpl);
  m_impl->read(filename);
}

PrjModel::PrjModel(Reader &input)
{
  m_impl = boost::shared_ptr<detail::PrjModelImpl>(new detail::PrjModelImpl);
  m_impl->read(input);
}

PrjModel::PrjModel(const PrjModel &other) : m_impl(other.m_impl)
{}

PrjModel::~PrjModel()
{}

PrjModel& PrjModel::operator=(const PrjModel &other)
{
  m_impl = other.m_impl;
  return *this;
}

bool PrjModel::operator==(const PrjModel &other) const
{
  return m_impl==other.m_impl;
}

bool PrjModel::operator!=(const PrjModel &other) const
{
  return m_impl!=other.m_impl;
}

bool PrjModel::read(openstudio::path path)
{
  return m_impl->read(openstudio::toString(path));
}

bool PrjModel::read(std::string filename)
{
  return m_impl->read(filename);
}

bool PrjModel::read(Reader &input)
{
  return m_impl->read(input);
}

std::string PrjModel::toString()
{
  return m_impl->toString();
}

std::string PrjModel::programName() const
{
  return m_impl->programName();
}

void PrjModel::setProgramName(const std::string &name)
{
  m_impl->setProgramName(name);
}

std::string PrjModel::version() const
{
  return m_impl->version();
}

void PrjModel::setVersion(const std::string &version)
{
  m_impl->setVersion(version);
}

int PrjModel::echo() const
{
  return m_impl->echo();
}

void PrjModel::setEcho(const int echo)
{
  m_impl->setEcho(echo);
}

std::string PrjModel::desc() const
{
  return m_impl->desc();
}

void PrjModel::setDesc(const std::string &prjdesc)
{
  m_impl->setDesc(prjdesc);
}

int PrjModel::skheight() const
{
  return m_impl->skheight();
}

void PrjModel::setSkheight(const int skheight)
{
  m_impl->setSkheight(skheight);
}

int PrjModel::skwidth() const
{
  return m_impl->skwidth();
}

void PrjModel::setSkwidth(const int skwidth)
{
  m_impl->setSkwidth(skwidth);
}

int PrjModel::def_units() const
{
  return m_impl->def_units();
}

void PrjModel::setDef_units(const int def_units)
{
  m_impl->setDef_units(def_units);
}

int PrjModel::def_flows() const
{
  return m_impl->def_flows();
}

void PrjModel::setDef_flows(const int def_flows)
{
  m_impl->setDef_flows(def_flows);
}

double PrjModel::def_T() const
{
  return m_impl->def_T();
}

bool PrjModel::setDef_T(const double def_T)
{
  return m_impl->setDef_T(def_T);
}

bool PrjModel::setDef_T(const std::string &def_T)
{
  return m_impl->setDef_T(def_T);
}

int PrjModel::udefT() const
{
  return m_impl->udefT();
}

void PrjModel::setUdefT(const int udefT)
{
  m_impl->setUdefT(udefT);
}

double PrjModel::rel_N() const
{
  return m_impl->rel_N();
}

bool PrjModel::setRel_N(const double rel_N)
{
  return m_impl->setRel_N(rel_N);
}

bool PrjModel::setRel_N(const std::string &rel_N)
{
  return m_impl->setRel_N(rel_N);
}

double PrjModel::wind_H() const
{
  return m_impl->wind_H();
}

bool PrjModel::setWind_H(const double wind_H)
{
  return m_impl->setWind_H(wind_H);
}

bool PrjModel::setWind_H(const std::string &wind_H)
{
  return m_impl->setWind_H(wind_H);
}

int PrjModel::uwH() const
{
  return m_impl->uwH();
}

void PrjModel::setUwH(const int uwH)
{
  m_impl->setUwH(uwH);
}

double PrjModel::wind_Ao() const
{
  return m_impl->wind_Ao();
}

bool PrjModel::setWind_Ao(const double wind_Ao)
{
  return m_impl->setWind_Ao(wind_Ao);
}

bool PrjModel::setWind_Ao(const std::string &wind_Ao)
{
  return m_impl->setWind_Ao(wind_Ao);
}

double PrjModel::wind_a() const
{
  return m_impl->wind_a();
}

bool PrjModel::setWind_a(const double wind_a)
{
  return m_impl->setWind_a(wind_a);
}

bool PrjModel::setWind_a(const std::string &wind_a)
{
  return m_impl->setWind_a(wind_a);
}

double PrjModel::scale() const
{
  return m_impl->scale();
}

bool PrjModel::setScale(const double scale)
{
  return m_impl->setScale(scale);
}

bool PrjModel::setScale(const std::string &scale)
{
  return m_impl->setScale(scale);
}

int PrjModel::uScale() const
{
  return m_impl->uScale();
}

void PrjModel::setUScale(const int uScale)
{
  m_impl->setUScale(uScale);
}

int PrjModel::orgRow() const
{
  return m_impl->orgRow();
}

void PrjModel::setOrgRow(const int orgRow)
{
  m_impl->setOrgRow(orgRow);
}

int PrjModel::orgCol() const
{
  return m_impl->orgCol();
}

void PrjModel::setOrgCol(const int orgCol)
{
  m_impl->setOrgCol(orgCol);
}

int PrjModel::invYaxis() const
{
  return m_impl->invYaxis();
}

void PrjModel::setInvYaxis(const int invYaxis)
{
  m_impl->setInvYaxis(invYaxis);
}

int PrjModel::showGeom() const
{
  return m_impl->showGeom();
}

void PrjModel::setShowGeom(const int showGeom)
{
  m_impl->setShowGeom(showGeom);
}

WeatherData PrjModel::ssWeather() const
{
  return m_impl->ssWeather();
}

void PrjModel::setSsWeather(const WeatherData &ssWeather)
{
  m_impl->setSsWeather(ssWeather);
}

WeatherData PrjModel::wptWeather() const
{
  return m_impl->wptWeather();
}

void PrjModel::setWptWeather(const WeatherData &wptWeather)
{
  m_impl->setWptWeather(wptWeather);
}

std::string PrjModel::WTHpath() const
{
  return m_impl->WTHpath();
}

void PrjModel::setWTHpath(const std::string &WTHpath)
{
  m_impl->setWTHpath(WTHpath);
}

std::string PrjModel::CTMpath() const
{
  return m_impl->CTMpath();
}

void PrjModel::setCTMpath(const std::string &CTMpath)
{
  m_impl->setCTMpath(CTMpath);
}

std::string PrjModel::CVFpath() const
{
  return m_impl->CVFpath();
}

void PrjModel::setCVFpath(const std::string &CVFpath)
{
  m_impl->setCVFpath(CVFpath);
}

std::string PrjModel::DVFpath() const
{
  return m_impl->DVFpath();
}

void PrjModel::setDVFpath(const std::string &DVFpath)
{
  m_impl->setDVFpath(DVFpath);
}

std::string PrjModel::WPCfile() const
{
  return m_impl->WPCfile();
}

void PrjModel::setWPCfile(const std::string &WPCfile)
{
  m_impl->setWPCfile(WPCfile);
}

std::string PrjModel::EWCfile() const
{
  return m_impl->EWCfile();
}

void PrjModel::setEWCfile(const std::string &EWCfile)
{
  m_impl->setEWCfile(EWCfile);
}

std::string PrjModel::WPCdesc() const
{
  return m_impl->WPCdesc();
}

void PrjModel::setWPCdesc(const std::string &WPCdesc)
{
  m_impl->setWPCdesc(WPCdesc);
}

double PrjModel::X0() const
{
  return m_impl->X0();
}

bool PrjModel::setX0(const double X0)
{
  return m_impl->setX0(X0);
}

bool PrjModel::setX0(const std::string &X0)
{
  return m_impl->setX0(X0);
}

double PrjModel::Y0() const
{
  return m_impl->Y0();
}

bool PrjModel::setY0(const double Y0)
{
  return m_impl->setY0(Y0);
}

bool PrjModel::setY0(const std::string &Y0)
{
  return m_impl->setY0(Y0);
}

double PrjModel::Z0() const
{
  return m_impl->Z0();
}

bool PrjModel::setZ0(const double Z0)
{
  return m_impl->setZ0(Z0);
}

bool PrjModel::setZ0(const std::string &Z0)
{
  return m_impl->setZ0(Z0);
}

double PrjModel::angle() const
{
  return m_impl->angle();
}

bool PrjModel::setAngle(const double angle)
{
  return m_impl->setAngle(angle);
}

bool PrjModel::setAngle(const std::string &angle)
{
  return m_impl->setAngle(angle);
}

int PrjModel::u_XYZ() const
{
  return m_impl->u_XYZ();
}

void PrjModel::setU_XYZ(const int u_XYZ)
{
  m_impl->setU_XYZ(u_XYZ);
}

double PrjModel::epsPath() const
{
  return m_impl->epsPath();
}

bool PrjModel::setEpsPath(const double epsPath)
{
  return m_impl->setEpsPath(epsPath);
}

bool PrjModel::setEpsPath(const std::string &epsPath)
{
  return m_impl->setEpsPath(epsPath);
}

double PrjModel::epsSpcs() const
{
  return m_impl->epsSpcs();
}

bool PrjModel::setEpsSpcs(const double epsSpcs)
{
  return m_impl->setEpsSpcs(epsSpcs);
}

bool PrjModel::setEpsSpcs(const std::string &epsSpcs)
{
  return m_impl->setEpsSpcs(epsSpcs);
}

std::string PrjModel::tShift() const
{
  return m_impl->tShift();
}

void PrjModel::setTShift(const std::string &tShift)
{
  m_impl->setTShift(tShift);
}

std::string PrjModel::dStart() const
{
  return m_impl->dStart();
}

void PrjModel::setDStart(const std::string &dStart)
{
  m_impl->setDStart(dStart);
}

std::string PrjModel::dEnd() const
{
  return m_impl->dEnd();
}

void PrjModel::setDEnd(const std::string &dEnd)
{
  m_impl->setDEnd(dEnd);
}

int PrjModel::useWPCwp() const
{
  return m_impl->useWPCwp();
}

void PrjModel::setUseWPCwp(const int useWPCwp)
{
  m_impl->setUseWPCwp(useWPCwp);
}

int PrjModel::useWPCmf() const
{
  return m_impl->useWPCmf();
}

void PrjModel::setUseWPCmf(const int useWPCmf)
{
  m_impl->setUseWPCmf(useWPCmf);
}

int PrjModel::wpctrig() const
{
  return m_impl->wpctrig();
}

void PrjModel::setWpctrig(const int wpctrig)
{
  m_impl->setWpctrig(wpctrig);
}

double PrjModel::latd() const
{
    return m_impl->latd();
}

bool PrjModel::setLatd(const double latd)
{
    return m_impl->setLatd(latd);
}

bool PrjModel::setLatd(const std::string &latd)
{
    return m_impl->setLatd(latd);
}

double PrjModel::lgtd() const
{
    return m_impl->lgtd();
}

bool PrjModel::setLgtd(const double lgtd)
{
    return m_impl->setLgtd(lgtd);
}

bool PrjModel::setLgtd(const std::string &lgtd)
{
    return m_impl->setLgtd(lgtd);
}

double PrjModel::Tznr() const
{
    return m_impl->Tznr();
}

bool PrjModel::setTznr(const double Tznr)
{
    return m_impl->setTznr(Tznr);
}

bool PrjModel::setTznr(const std::string &Tznr)
{
    return m_impl->setTznr(Tznr);
}

double PrjModel::altd() const
{
    return m_impl->altd();
}

bool PrjModel::setAltd(const double altd)
{
    return m_impl->setAltd(altd);
}

bool PrjModel::setAltd(const std::string &altd)
{
    return m_impl->setAltd(altd);
}

double PrjModel::Tgrnd() const
{
    return m_impl->Tgrnd();
}

bool PrjModel::setTgrnd(const double Tgrnd)
{
    return m_impl->setTgrnd(Tgrnd);
}

bool PrjModel::setTgrnd(const std::string &Tgrnd)
{
    return m_impl->setTgrnd(Tgrnd);
}

int PrjModel::utg() const
{
    return m_impl->utg();
}

void PrjModel::setUtg(const int utg)
{
    m_impl->setUtg(utg);
}

int PrjModel::u_a() const
{
    return m_impl->u_a();
}

void PrjModel::setU_a(const int u_a)
{
    m_impl->setU_a(u_a);
}

RunControl PrjModel::rc() const
{
  return m_impl->rc();
}
void PrjModel::setRc(const RunControl rc)
{
  m_impl->setRc(rc);
}

std::vector<int> PrjModel::contaminants() const
{
  return m_impl->contaminants();
}

std::vector <Species> PrjModel::species() const
{
  return m_impl->species();
}
void PrjModel::setSpecies(const std::vector<Species> species)
{
  m_impl->setSpecies(species);
}

std::vector <Level> PrjModel::levels() const
{
  return m_impl->levels();
}
void PrjModel::setLevels(const std::vector<Level> levels)
{
  m_impl->setLevels(levels);
}
void PrjModel::addLevel(Level level)
{
  m_impl->addLevel(level);
}

std::vector <DaySchedule> PrjModel::daySchedules() const
{
  return m_impl->daySchedules();
}
void PrjModel::setDaySchedules(const std::vector<DaySchedule> daySchedules)
{
  m_impl->setDaySchedules(daySchedules);
}

std::vector <WeekSchedule> PrjModel::weekSchedules() const
{
  return m_impl->weekSchedules();
}
void PrjModel::setWeekSchedules(const std::vector<WeekSchedule> weekSchedules)
{
  m_impl->setWeekSchedules(weekSchedules);
}

std::vector <WindPressureProfile> PrjModel::windPressureProfiles() const
{
  return m_impl->windPressureProfiles();
}
void PrjModel::setWindPressureProfiles(const std::vector<WindPressureProfile> windPressureProfiles)
{
  m_impl->setWindPressureProfiles(windPressureProfiles);
}

std::vector<PlrTest1> PrjModel::getPlrTest1() const
{
  return m_impl->getPlrTest1();
}

bool PrjModel::addAirflowElement(PlrTest1 element)
{
  return m_impl->addAirflowElement(element);
}

int PrjModel::airflowElementNrByName(std::string name) const
{
  return m_impl->airflowElementNrByName(name);
}

bool PrjModel::replaceAirflowElement(int nr, PlrTest1 element)
{
  return m_impl->replaceAirflowElement(nr,element);
}

std::vector<CvfDat> PrjModel::getCvfDat() const
{
  return m_impl->getCvfDat();
}

bool PrjModel::addControlNode(CvfDat element, bool sequence)
{
  return m_impl->addControlNode(element,sequence);
}

std::vector <Ahs> PrjModel::ahs() const
{
  return m_impl->ahs();
}
void PrjModel::setAhs(const std::vector<Ahs> ahs)
{
  m_impl->setAhs(ahs);
}
void PrjModel::addAhs(Ahs ahs)
{
  m_impl->addAhs(ahs);
}

std::vector<Zone> PrjModel::zones() const
{
  return m_impl->zones();
}
void PrjModel::setZones(const std::vector<Zone> zones)
{
  m_impl->setZones(zones);
}
void PrjModel::addZone(Zone zone)
{
  m_impl->addZone(zone);
}

std::vector<Path> PrjModel::paths() const
{
  return m_impl->paths();
}
void PrjModel::setPaths(const std::vector<Path> paths)
{
  m_impl->setPaths(paths);
}
void PrjModel::addPath(Path path)
{
  m_impl->addPath(path);
}

bool PrjModel::valid() const
{
  return m_impl->valid();
}

std::vector<std::vector<int> > PrjModel::zoneExteriorFlowPaths()
{
  return m_impl->zoneExteriorFlowPaths();
}

std::vector<TimeSeries> PrjModel::zoneInfiltration(SimFile *sim)
{
  return m_impl->zoneInfiltration(sim);
}

std::vector<TimeSeries> PrjModel::pathInfiltration(std::vector<int> pathNrs, SimFile *sim)
{
  return m_impl->pathInfiltration(pathNrs, sim);
}

} // contam
} // openstudio
