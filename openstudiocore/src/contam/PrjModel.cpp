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
#include "PrjModel.hpp"
#include "PrjReader.hpp"
#include "SimFile.hpp"

#include "PrjModelImpl.hpp"

#include <QFile>

namespace openstudio {
namespace contam {

PrjModel::PrjModel()
{
  d = new PrjModelImpl;
}

PrjModel::PrjModel(openstudio::path path)
{
  d = new PrjModelImpl;
  d->read(path);
}

PrjModel::PrjModel(std::string filename)
{
  d = new PrjModelImpl;
  d->read(filename);
}

PrjModel::PrjModel(Reader &input)
{
  d = new PrjModelImpl;
  d->read(input);
}

PrjModel::PrjModel(const PrjModel &other) : d(other.d)
{}

PrjModel::~PrjModel()
{}

PrjModel& PrjModel::operator=(const PrjModel &other)
{
  d = other.d;
  return *this;
}

bool PrjModel::operator==(const PrjModel &other) const
{
  return d==other.d;
}

bool PrjModel::operator!=(const PrjModel &other) const
{
  return d!=other.d;
}

bool PrjModel::read(openstudio::path path)
{
  return d->read(openstudio::toString(path));
}

bool PrjModel::read(std::string filename)
{
  return d->read(filename);
}

bool PrjModel::read(Reader &input)
{
  return d->read(input);
}

std::string PrjModel::toString()
{
  return d->toString();
}

std::string PrjModel::programName() const
{
  return d->programName();
}

void PrjModel::setProgramName(const std::string &name)
{
  d->setProgramName(name);
}

std::string PrjModel::version() const
{
  return d->version();
}

void PrjModel::setVersion(const std::string &version)
{
  d->setVersion(version);
}

int PrjModel::echo() const
{
  return d->echo();
}

void PrjModel::setEcho(const int echo)
{
  d->setEcho(echo);
}

std::string PrjModel::desc() const
{
  return d->desc();
}

void PrjModel::setDesc(const std::string &prjdesc)
{
  d->setDesc(prjdesc);
}

int PrjModel::skheight() const
{
  return d->skheight();
}

void PrjModel::setSkheight(const int skheight)
{
  d->setSkheight(skheight);
}

int PrjModel::skwidth() const
{
  return d->skwidth();
}

void PrjModel::setSkwidth(const int skwidth)
{
  d->setSkwidth(skwidth);
}

int PrjModel::def_units() const
{
  return d->def_units();
}

void PrjModel::setDef_units(const int def_units)
{
  d->setDef_units(def_units);
}

int PrjModel::def_flows() const
{
  return d->def_flows();
}

void PrjModel::setDef_flows(const int def_flows)
{
  d->setDef_flows(def_flows);
}

double PrjModel::def_T() const
{
  return d->def_T();
}

bool PrjModel::setDef_T(const double def_T)
{
  return d->setDef_T(def_T);
}

bool PrjModel::setDef_T(const std::string &def_T)
{
  return d->setDef_T(def_T);
}

int PrjModel::udefT() const
{
  return d->udefT();
}

void PrjModel::setUdefT(const int udefT)
{
  d->setUdefT(udefT);
}

double PrjModel::rel_N() const
{
  return d->rel_N();
}

bool PrjModel::setRel_N(const double rel_N)
{
  return d->setRel_N(rel_N);
}

bool PrjModel::setRel_N(const std::string &rel_N)
{
  return d->setRel_N(rel_N);
}

double PrjModel::wind_H() const
{
  return d->wind_H();
}

bool PrjModel::setWind_H(const double wind_H)
{
  return d->setWind_H(wind_H);
}

bool PrjModel::setWind_H(const std::string &wind_H)
{
  return d->setWind_H(wind_H);
}

int PrjModel::uwH() const
{
  return d->uwH();
}

void PrjModel::setUwH(const int uwH)
{
  d->setUwH(uwH);
}

double PrjModel::wind_Ao() const
{
  return d->wind_Ao();
}

bool PrjModel::setWind_Ao(const double wind_Ao)
{
  return d->setWind_Ao(wind_Ao);
}

bool PrjModel::setWind_Ao(const std::string &wind_Ao)
{
  return d->setWind_Ao(wind_Ao);
}

double PrjModel::wind_a() const
{
  return d->wind_a();
}

bool PrjModel::setWind_a(const double wind_a)
{
  return d->setWind_a(wind_a);
}

bool PrjModel::setWind_a(const std::string &wind_a)
{
  return d->setWind_a(wind_a);
}

double PrjModel::scale() const
{
  return d->scale();
}

bool PrjModel::setScale(const double scale)
{
  return d->setScale(scale);
}

bool PrjModel::setScale(const std::string &scale)
{
  return d->setScale(scale);
}

int PrjModel::uScale() const
{
  return d->uScale();
}

void PrjModel::setUScale(const int uScale)
{
  d->setUScale(uScale);
}

int PrjModel::orgRow() const
{
  return d->orgRow();
}

void PrjModel::setOrgRow(const int orgRow)
{
  d->setOrgRow(orgRow);
}

int PrjModel::orgCol() const
{
  return d->orgCol();
}

void PrjModel::setOrgCol(const int orgCol)
{
  d->setOrgCol(orgCol);
}

int PrjModel::invYaxis() const
{
  return d->invYaxis();
}

void PrjModel::setInvYaxis(const int invYaxis)
{
  d->setInvYaxis(invYaxis);
}

int PrjModel::showGeom() const
{
  return d->showGeom();
}

void PrjModel::setShowGeom(const int showGeom)
{
  d->setShowGeom(showGeom);
}

WeatherData PrjModel::ssWeather() const
{
  return d->ssWeather();
}

void PrjModel::setSsWeather(const WeatherData &ssWeather)
{
  d->setSsWeather(ssWeather);
}

WeatherData PrjModel::wptWeather() const
{
  return d->wptWeather();
}

void PrjModel::setWptWeather(const WeatherData &wptWeather)
{
  d->setWptWeather(wptWeather);
}

std::string PrjModel::WTHpath() const
{
  return d->WTHpath();
}

void PrjModel::setWTHpath(const std::string &WTHpath)
{
  d->setWTHpath(WTHpath);
}

std::string PrjModel::CTMpath() const
{
  return d->CTMpath();
}

void PrjModel::setCTMpath(const std::string &CTMpath)
{
  d->setCTMpath(CTMpath);
}

std::string PrjModel::CVFpath() const
{
  return d->CVFpath();
}

void PrjModel::setCVFpath(const std::string &CVFpath)
{
  d->setCVFpath(CVFpath);
}

std::string PrjModel::DVFpath() const
{
  return d->DVFpath();
}

void PrjModel::setDVFpath(const std::string &DVFpath)
{
  d->setDVFpath(DVFpath);
}

std::string PrjModel::WPCfile() const
{
  return d->WPCfile();
}

void PrjModel::setWPCfile(const std::string &WPCfile)
{
  d->setWPCfile(WPCfile);
}

std::string PrjModel::EWCfile() const
{
  return d->EWCfile();
}

void PrjModel::setEWCfile(const std::string &EWCfile)
{
  d->setEWCfile(EWCfile);
}

std::string PrjModel::WPCdesc() const
{
  return d->WPCdesc();
}

void PrjModel::setWPCdesc(const std::string &WPCdesc)
{
  d->setWPCdesc(WPCdesc);
}

double PrjModel::X0() const
{
  return d->X0();
}

bool PrjModel::setX0(const double X0)
{
  return d->setX0(X0);
}

bool PrjModel::setX0(const std::string &X0)
{
  return d->setX0(X0);
}

double PrjModel::Y0() const
{
  return d->Y0();
}

bool PrjModel::setY0(const double Y0)
{
  return d->setY0(Y0);
}

bool PrjModel::setY0(const std::string &Y0)
{
  return d->setY0(Y0);
}

double PrjModel::Z0() const
{
  return d->Z0();
}

bool PrjModel::setZ0(const double Z0)
{
  return d->setZ0(Z0);
}

bool PrjModel::setZ0(const std::string &Z0)
{
  return d->setZ0(Z0);
}

double PrjModel::angle() const
{
  return d->angle();
}

bool PrjModel::setAngle(const double angle)
{
  return d->setAngle(angle);
}

bool PrjModel::setAngle(const std::string &angle)
{
  return d->setAngle(angle);
}

int PrjModel::u_XYZ() const
{
  return d->u_XYZ();
}

void PrjModel::setU_XYZ(const int u_XYZ)
{
  d->setU_XYZ(u_XYZ);
}

double PrjModel::epsPath() const
{
  return d->epsPath();
}

bool PrjModel::setEpsPath(const double epsPath)
{
  return d->setEpsPath(epsPath);
}

bool PrjModel::setEpsPath(const std::string &epsPath)
{
  return d->setEpsPath(epsPath);
}

double PrjModel::epsSpcs() const
{
  return d->epsSpcs();
}

bool PrjModel::setEpsSpcs(const double epsSpcs)
{
  return d->setEpsSpcs(epsSpcs);
}

bool PrjModel::setEpsSpcs(const std::string &epsSpcs)
{
  return d->setEpsSpcs(epsSpcs);
}

std::string PrjModel::tShift() const
{
  return d->tShift();
}

void PrjModel::setTShift(const std::string &tShift)
{
  d->setTShift(tShift);
}

std::string PrjModel::dStart() const
{
  return d->dStart();
}

void PrjModel::setDStart(const std::string &dStart)
{
  d->setDStart(dStart);
}

std::string PrjModel::dEnd() const
{
  return d->dEnd();
}

void PrjModel::setDEnd(const std::string &dEnd)
{
  d->setDEnd(dEnd);
}

int PrjModel::useWPCwp() const
{
  return d->useWPCwp();
}

void PrjModel::setUseWPCwp(const int useWPCwp)
{
  d->setUseWPCwp(useWPCwp);
}

int PrjModel::useWPCmf() const
{
  return d->useWPCmf();
}

void PrjModel::setUseWPCmf(const int useWPCmf)
{
  d->setUseWPCmf(useWPCmf);
}

int PrjModel::wpctrig() const
{
  return d->wpctrig();
}

void PrjModel::setWpctrig(const int wpctrig)
{
  d->setWpctrig(wpctrig);
}

double PrjModel::latd() const
{
    return d->latd();
}

bool PrjModel::setLatd(const double latd)
{
    return d->setLatd(latd);
}

bool PrjModel::setLatd(const std::string &latd)
{
    return d->setLatd(latd);
}

double PrjModel::lgtd() const
{
    return d->lgtd();
}

bool PrjModel::setLgtd(const double lgtd)
{
    return d->setLgtd(lgtd);
}

bool PrjModel::setLgtd(const std::string &lgtd)
{
    return d->setLgtd(lgtd);
}

double PrjModel::Tznr() const
{
    return d->Tznr();
}

bool PrjModel::setTznr(const double Tznr)
{
    return d->setTznr(Tznr);
}

bool PrjModel::setTznr(const std::string &Tznr)
{
    return d->setTznr(Tznr);
}

double PrjModel::altd() const
{
    return d->altd();
}

bool PrjModel::setAltd(const double altd)
{
    return d->setAltd(altd);
}

bool PrjModel::setAltd(const std::string &altd)
{
    return d->setAltd(altd);
}

double PrjModel::Tgrnd() const
{
    return d->Tgrnd();
}

bool PrjModel::setTgrnd(const double Tgrnd)
{
    return d->setTgrnd(Tgrnd);
}

bool PrjModel::setTgrnd(const std::string &Tgrnd)
{
    return d->setTgrnd(Tgrnd);
}

int PrjModel::utg() const
{
    return d->utg();
}

void PrjModel::setUtg(const int utg)
{
    d->setUtg(utg);
}

int PrjModel::u_a() const
{
    return d->u_a();
}

void PrjModel::setU_a(const int u_a)
{
    d->setU_a(u_a);
}

RunControl PrjModel::rc() const
{
  return d->rc();
}
void PrjModel::setRc(const RunControl rc)
{
  d->setRc(rc);
}

std::vector<int> PrjModel::contaminants() const
{
  return d->contaminants();
}

std::vector <Species> PrjModel::species() const
{
  return d->species();
}
void PrjModel::setSpecies(const std::vector<Species> species)
{
  d->setSpecies(species);
}

std::vector <Level> PrjModel::levels() const
{
  return d->levels();
}
void PrjModel::setLevels(const std::vector<Level> levels)
{
  d->setLevels(levels);
}
void PrjModel::addLevel(Level level)
{
  d->addLevel(level);
}

std::vector <DaySchedule> PrjModel::daySchedules() const
{
  return d->daySchedules();
}
void PrjModel::setDaySchedules(const std::vector<DaySchedule> daySchedules)
{
  d->setDaySchedules(daySchedules);
}

std::vector <WeekSchedule> PrjModel::weekSchedules() const
{
  return d->weekSchedules();
}
void PrjModel::setWeekSchedules(const std::vector<WeekSchedule> weekSchedules)
{
  d->setWeekSchedules(weekSchedules);
}

std::vector <WindPressureProfile> PrjModel::windPressureProfiles() const
{
  return d->windPressureProfiles();
}
void PrjModel::setWindPressureProfiles(const std::vector<WindPressureProfile> windPressureProfiles)
{
  d->setWindPressureProfiles(windPressureProfiles);
}

std::vector<PlrTest1> PrjModel::getPlrTest1() const
{
  return d->getPlrTest1();
}

bool PrjModel::addAirflowElement(PlrTest1 element)
{
  return d->addAirflowElement(element);
}

int PrjModel::airflowElementNrByName(std::string name) const
{
  return d->airflowElementNrByName(name);
}

bool PrjModel::replaceAirflowElement(int nr, PlrTest1 element)
{
  return d->replaceAirflowElement(nr,element);
}

std::vector<CvfDat> PrjModel::getCvfDat() const
{
  return d->getCvfDat();
}

bool PrjModel::addControlNode(CvfDat element, bool sequence)
{
  return d->addControlNode(element,sequence);
}

std::vector <Ahs> PrjModel::ahs() const
{
  return d->ahs();
}
void PrjModel::setAhs(const std::vector<Ahs> ahs)
{
  d->setAhs(ahs);
}
void PrjModel::addAhs(Ahs ahs)
{
  d->addAhs(ahs);
}

std::vector<Zone> PrjModel::zones() const
{
  return d->zones();
}
void PrjModel::setZones(const std::vector<Zone> zones)
{
  d->setZones(zones);
}
void PrjModel::addZone(Zone zone)
{
  d->addZone(zone);
}

std::vector<Path> PrjModel::paths() const
{
  return d->paths();
}
void PrjModel::setPaths(const std::vector<Path> paths)
{
  d->setPaths(paths);
}
void PrjModel::addPath(Path path)
{
  d->addPath(path);
}

bool PrjModel::valid() const
{
  return d->valid();
}

std::vector<std::vector<int> > PrjModel::zoneExteriorFlowPaths()
{
  return d->zoneExteriorFlowPaths();
}

std::vector<TimeSeries> PrjModel::zoneInfiltration(SimFile *sim)
{
  return d->zoneInfiltration(sim);
}

std::vector<TimeSeries> PrjModel::pathInfiltration(std::vector<int> pathNrs, SimFile *sim)
{
  return d->pathInfiltration(pathNrs, sim);
}

} // contam
} // openstudio
