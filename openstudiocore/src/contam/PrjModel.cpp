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
  d = new PrjModel_Impl;
}

PrjModel::PrjModel(openstudio::path path)
{
  d = new PrjModel_Impl;
  d->read(path);
}

PrjModel::PrjModel(std::string filename)
{
  d = new PrjModel_Impl;
  d->read(filename);
}

PrjModel::PrjModel(Reader &input)
{
  d = new PrjModel_Impl;
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

bool PrjModel::setSteadyWeather(double windSpeed, double windDirection)
{
  return d->setSteadyWeather(windSpeed, windDirection);
}

} // contam
} // openstudio
