/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
 *  Copyright (c) 2013, The Pennsylvania State University.
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

#include <contam/ReverseTranslator.hpp>

#include <model/Model.hpp>
#include <model/ModelObject.hpp>
#include <model/ModelObject_Impl.hpp>
#include <model/Facility.hpp>
#include <model/Facility_Impl.hpp>
#include <model/Building.hpp>
#include <model/Building_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/Space.hpp>
#include <model/Space_Impl.hpp>
#include <model/Surface.hpp>
#include <model/Surface_Impl.hpp>
#include <model/SubSurface.hpp>
#include <model/SubSurface_Impl.hpp>
#include <model/ShadingSurface.hpp>
#include <model/ShadingSurface_Impl.hpp>
#include <model/ShadingSurfaceGroup.hpp>
#include <model/ShadingSurfaceGroup_Impl.hpp>
#include <model/SizingZone.hpp>
#include <model/BuildingStory.hpp>
#include <model/BuildingStory_Impl.hpp>
#include <model/AirLoopHVAC.hpp>
#include <model/AirLoopHVAC_Impl.hpp>
#include <model/HVACTemplates.hpp>

#include <utilities/core/Assert.hpp>
#include <utilities/units/UnitFactory.hpp>
#include <utilities/units/QuantityConverter.hpp>
#include <utilities/plot/ProgressBar.hpp>

#include <QFile>
#include <QSharedPointer>
#include <QThread>

namespace openstudio {
namespace contam {

ReverseTranslator::ReverseTranslator()
{
  m_progressBar = 0;
  m_logSink.setLogLevel(Warn);
  m_logSink.setChannelRegex(boost::regex("openstudio\\.contam\\.ReverseTranslator"));
  m_logSink.setThreadId(QThread::currentThread());
}

ReverseTranslator::~ReverseTranslator()
{
}

static Point3d transform(prj::Icon *icon, int originX, int originY, double z, double scale, bool invertY)
{
  if(invertY)
    return Point3d((icon->col-originX)*scale, (originY-icon->row)*scale, z);
  else
    return Point3d((icon->col-originX)*scale, (icon->row-originY)*scale, z);
}

boost::optional<openstudio::model::Model> ReverseTranslator::translate(const prj::Data &data, ProgressBar *progressBar)
{
  m_logSink.setThreadId(QThread::currentThread());
  m_logSink.resetStringStream();

  if(!data.valid)
  {
    LOG(Error, "Invalid PRJ data.");
    return false;
  }
  openstudio::model::Model model;
  // Translate levels
  m_levelMap.clear();
  translateLevels(model, data);
  // Translate zones
  m_zoneMap.clear();
  if(!translateZones(model, data))
  {
    LOG(Error, "Airflow zone translation failed.");
    return false;
  }
  // Translate simple air handling systems
  m_ahsMap.clear();
  m_connections.clear();
  if(!translateSimpleAHS(model, data))
  {
    LOG(Error, "Simple AHS translation failed.");
    return false;
  }
  return boost::optional<model::Model>(model);
}

void ReverseTranslator::translateLevels(openstudio::model::Model &model, const prj::Data &data)
{
  if(!data.valid)return;
  if(m_progressBar)
  {
    m_progressBar->setWindowTitle(toString("Translating levels"));
    m_progressBar->setMinimum(0);
    m_progressBar->setMaximum(data.zones.size()+data.levels.size()); 
    m_progressBar->setValue(0);
  }
  // Construct zones on a level-by-level basis
  // Count up the zones on each level - only make stories for levels with zones
  QVector<int> counts(data.levels.size(),0);
  for(int i=0;i<data.zones.size();i++)
  {
    if(data.zones[i].pl>0)
    {
      counts[data.zones[i].pl-1] += 1;
    }
    if(m_progressBar)
    {
      m_progressBar->setValue(m_progressBar->value() + 1);
    }
  }
  // Now create the levels
  for(int k=0;k<data.levels.size();k++)
  {
    if(counts[k])
    {
      model::BuildingStory story(model);
      story.setName(data.levels[k].name.toStdString());
      story.setNominalZCoordinate(data.levels[k].refht.toDouble());
      story.setNominalFloortoFloorHeight(data.levels[k].delht.toDouble());
      m_levelMap[k+1] = story.handle();
    }
    if(m_progressBar)
    {
      m_progressBar->setValue(m_progressBar->value() + 1);
    }
  }
}

bool ReverseTranslator::translateZones(openstudio::model::Model &model, const prj::Data &data)
{
  if(!data.valid)
  {
    return false;
  }
  if(m_progressBar)
  {
    m_progressBar->setWindowTitle(toString("Translating zones"));
    m_progressBar->setMinimum(0);
    m_progressBar->setMaximum(data.zones.size()); 
    m_progressBar->setValue(0);
  }
  for(int i=0;i<data.geometry.size();i++)
  {
    if(!data.geometry[i].zone->system())
    {
      if(data.geometry[i].loops.size() != 1)
      {
        // Warn about skipping zones with non-simple floorplan
        LOG(Warn, "Non-simple zone'" << data.geometry[i].zone->name.toStdString() << "' skipped.");
      }
      else
      {
        std::vector<Point3d> points;
        double z = data.levels[data.geometry[i].zone->pl-1].refht.toDouble();
        double h = data.levels[data.geometry[i].zone->pl-1].delht.toDouble();
        if(data.rc.invYaxis==1)
        {
          for(int j=0;j<data.geometry[i].loops[0]->walls.size();j++)
          {
            points.insert(points.begin(),transform(data.geometry[i].loops[0]->walls[j]->start,
              data.rc.orgCol,data.rc.orgRow,z,data.rc.scale.toDouble(),(data.rc.invYaxis==1)));
          }
        }
        else
        {
          for(int j=0;j<data.geometry[i].loops[0]->walls.size();j++)
          {
            points.push_back(transform(data.geometry[i].loops[0]->walls[j]->start,data.rc.orgCol,data.rc.orgRow,z,
              data.rc.scale.toDouble(),(data.rc.invYaxis==1)));
          }
        }
        boost::optional<model::Space> space = model::Space::fromFloorPrint(points, h, model);
        if(!space)
        {
          // Failure!
          LOG(Error, "Unable to create space for airflow zone '" << data.geometry[i].zone->name.toStdString() << "'.");
          return false;
        }
        // Create  thermal zone and set building story
        model::ThermalZone zone(model);
        model::SizingZone sizingZone(model, zone);
        zone.setName(data.geometry[i].zone->name.toStdString());
        m_zoneMap[data.geometry[i].zone->nr] = zone.handle();
        //zone.setThermostatSetpointDualSetpoint(*thermostat);
        space->setName((data.geometry[i].zone->name+" Space").toStdString());
        space->setThermalZone(zone);
        boost::optional<model::BuildingStory> story = model.getModelObject<model::BuildingStory>(m_levelMap[data.geometry[i].zone->pl]);
        if(!story)
        {
          // Failure!
          LOG(Error, "Unable to find building story for airflow zone '" << data.geometry[i].zone->name.toStdString() << "'.");
          return false;
        }
        space->setBuildingStory(*story);
      }
    }
    if(m_progressBar)
    {
      m_progressBar->setValue(m_progressBar->value() + 1);
    }
  }
  return true;
}

bool ReverseTranslator::translateSimpleAHS(openstudio::model::Model &model, const prj::Data &data)
{
  if(!data.valid)
  {
    return false;
  }
  if(data.ahs.size()==0)
  {
    return true;
  }
  if(m_progressBar)
  {
    m_progressBar->setWindowTitle(toString("Translating HVAC"));
    m_progressBar->setMinimum(0);
    m_progressBar->setMaximum(data.levels.size()+data.ahs.size()); 
    m_progressBar->setValue(0);
  }
  QVector<int> counts(data.ahs.size(),0);
  // Use icons to figure out what connects to what and count connections to each AHS
  for(int k=0;k<data.levels.size();k++)
  {
    for(int j=0;j<data.levels[k].icons.size();j++)
    {
      if(data.levels[k].icons[j].icon == BOX_S)
      {
        int path = data.levels[k].icons[j].nr;
        int fromZone = data.paths[path-1].pzn;
        int toZone = data.paths[path-1].pzm;
        if(!m_zoneMap.contains(toZone))
        {
          // Error! The connected zone isn't in the zone map
          LOG(Error, "Unexpected airflow zone number '" << toZone << "'.");
          return false;
        }
        int ahsIndex = -1;
        for(int i=0;i<data.ahs.size();i++)
        {
          if(data.ahs[i].zone_s == fromZone)
          {
            ahsIndex=i;
            break;
          }
        }
        if(ahsIndex == -1)
        {
          // Error! Failed to find the AHS
          LOG(Error, "Failed to fine simple AHS supply volume '" << fromZone << "'.");
          return false;
        }
        counts[ahsIndex] += 1;
        if(m_connections.contains(toZone))
        {
          // Warn that the previous connection is being overwritten
          LOG(Error, "Multiple AHS connections to airflow zone number '" << toZone << "', overwriting previous connection.");
        }
        m_connections[toZone] = ahsIndex;
      }
    }
    if(m_progressBar)
    {
      m_progressBar->setValue(m_progressBar->value() + 1);
    }
  }
  // Make an airloop for each AHS that is actually used
  for(int k=0;k<data.ahs.size();k++)
  {
    if(counts[k])
    {
      model::Loop loop = addSystemType3(model);
      model::AirLoopHVAC airLoop = loop.cast<model::AirLoopHVAC>();
      m_ahsMap[k] = airLoop.handle();
    }
    if(m_progressBar)
    {
      m_progressBar->setValue(m_progressBar->value() + 1);
    }
  }
  // Now connect up the zones to the air loop HVAC objects
  if(m_progressBar)
  {
    m_progressBar->setWindowTitle(toString("Connecting HVAC"));
    m_progressBar->setMinimum(0);
    m_progressBar->setMaximum(data.zones.size()); 
    m_progressBar->setValue(0);
  }
  for(int i=1;i<data.zones.size();i++) // Skip the fake ambient zone
  {
    if(m_connections.contains(data.zones[i].nr))
    {
      boost::optional<model::AirLoopHVAC> airLoop = model.getModelObject<model::AirLoopHVAC>(m_ahsMap[m_connections[data.zones[i].nr]]);
      boost::optional<model::ThermalZone> thermalZone = model.getModelObject<model::ThermalZone>(m_zoneMap[data.zones[i].nr]);
      if(!airLoop || !thermalZone)
      {
        // Error!
        LOG(Error, "Unable to find airloop or thermal zone for airflow zone '" << data.zones[i].name.toStdString() << "'.");
        return false;
      }
      airLoop->addBranchForZone(thermalZone.get());
    }
    if(m_progressBar)
    {
      m_progressBar->setValue(m_progressBar->value() + 1);
    }
  }
  return true;
}

std::vector<LogMessage> ReverseTranslator::warnings() const
{
  std::vector<LogMessage> result;

  BOOST_FOREACH(LogMessage logMessage, m_logSink.logMessages())
  {
    if (logMessage.logLevel() == Warn)
    {
      result.push_back(logMessage);
    }
  }

  return result;
}

std::vector<LogMessage> ReverseTranslator::errors() const
{
  std::vector<LogMessage> result;

  BOOST_FOREACH(LogMessage logMessage, m_logSink.logMessages())
  {
    if (logMessage.logLevel() > Warn)
    {
      result.push_back(logMessage);
    }
  }

  return result;
}

} // contam
} // openstudio
