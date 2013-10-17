/**********************************************************************
 *  Copyright (c) 2008-2010, Alliance for Sustainable Energy.
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

#include <contam/ForwardTranslator.hpp>
#include <contam/WindPressure.hpp>

#include <model/Model.hpp>
#include <model/Building.hpp>
#include <model/Building_Impl.hpp>
#include <model/BuildingStory.hpp>
#include <model/BuildingStory_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/Space.hpp>
#include <model/Space_Impl.hpp>
#include <model/Surface.hpp>
#include <model/Surface_Impl.hpp>
#include <model/AirLoopHVAC.hpp>
#include <model/AirLoopHVAC_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/PortList.hpp>
#include <model/WeatherFile.hpp>

#include <utilities/sql/SqlFile.hpp>
#include <utilities/core/Logger.hpp>
#include <utilities/geometry/Geometry.hpp>
#include <utilities/plot/ProgressBar.hpp>
#include <utilities/filetypes/EpwFile.hpp>

#include <boost/foreach.hpp>
#include <boost/math/constants/constants.hpp>


#include <QFile>
#include <QTextStream>
#include <QList>
#include <QMap>
#include <QThread>

namespace openstudio {
namespace contam {

CvFile::CvFile()
{
  m_start = Date(MonthOfYear::Jan,1);
  m_end = Date(MonthOfYear::Dec,31);
}

void CvFile::addTimeSeries(std::string name, TimeSeries series)
{
  m_names.push_back(name);
  m_series.push_back(series);
}

static std::string convertDateTime(const DateTime &datetime)
{
  return QString().sprintf("%02d/%02d\t%02d:%02d:%02d", month(datetime.date().monthOfYear()), datetime.date().dayOfMonth(),
    datetime.time().hours(),datetime.time().minutes(),datetime.time().seconds()).toStdString();
}

bool CvFile::write(openstudio::path filepath)
{
  QFile file(toQString(filepath));
  if(file.open(QFile::WriteOnly))
  {
    QTextStream textStream(&file);
    textStream << "ContinuousValuesFile ContamW 2.1\n";
    textStream << "CVF file from E+ results\n";
    textStream << month(m_start.monthOfYear()) << '/' << m_start.dayOfMonth() << '\t'
               <<  month(m_end.monthOfYear())  << '/' <<  m_end.dayOfMonth()  << '\n';
    textStream << m_names.size() << '\n';
    for(unsigned int i=0;i<m_names.size();i++)
    {
      textStream << m_names[i] << '\n';
    }
    Time delta(0,1,0,0); // Hard code hourly data for now
    DateTime current(m_start);
    DateTime last = current;
    // Another hard code for 8760 hours, really not sure what to do with 1/1 00:00:00
    textStream << convertDateTime(current);
    for(unsigned int i=0;i<m_names.size();i++)
    {
      double value = m_series[i].value(current+delta); // This works now...
      if(m_series[i].units() == "C")
      {
        value += 273.15;
      }
      textStream << '\t' << value;
    }
    textStream << '\n';
    // And the big hardcoded loop
    for(int j=0;j<8760;j++)
    {
      current += delta;
      // Mess with the time a little bit to put 24:00:00 in for 00:00:00
      if(current.time().hours() == 0 && current.time().minutes() == 0 && current.time().seconds() == 0)
      {
        textStream << QString().sprintf("%02d/%02d\t24:00:00",month(last.date().monthOfYear()),
          last.date().dayOfMonth()).toStdString();
      }
      else
      {
        textStream << convertDateTime(current);
      }
      last = current;
      // Write out the data. May need more conversion here in the future.
      for(unsigned int i=0;i<m_names.size();i++)
      {
        double value = m_series[i].value(current);
        if(m_series[i].units() == "C")
        {
          value += 273.15;
        }
        textStream << '\t' << value;
      }
      textStream << '\n';
    }
    file.close();
    return true;
  }
  return false;
}

ForwardTranslator::ForwardTranslator()
{
  m_ready=false;
  m_ratioOverride=false;
  m_returnSupplyRatio=0.9;
  m_logSink.setLogLevel(Warn);
  m_logSink.setChannelRegex(boost::regex("openstudio\\.contam\\.ForwardTranslator"));
  m_logSink.setThreadId(QThread::currentThread());
  m_progressBar = 0;
  read(":/templates/template.prj");
  if(valid())
  {
    // The template is a legal PRJ file, so it has one level. Not for long.
    setLevels(std::vector<prj::Level>());
  }
  setAirtightnessLevel("Average");
}

ForwardTranslator::ForwardTranslator(std::string leakageDescriptor)
{
  m_ready=false;
  m_ratioOverride=false;
  m_returnSupplyRatio=0.9;
  m_logSink.setLogLevel(Warn);
  m_logSink.setChannelRegex(boost::regex("openstudio\\.contam\\.ForwardTranslator"));
  m_logSink.setThreadId(QThread::currentThread());
  m_progressBar = 0;
  read(":/templates/template.prj");
  if(valid())
  {
    // The template is a legal PRJ file, so it has one level. Not for long.
    setLevels(std::vector<prj::Level>());
  }
  setAirtightnessLevel(leakageDescriptor);
}

ForwardTranslator::ForwardTranslator(double flow,double n,double deltaP)
{
  m_ready=false;
  m_ratioOverride=false;
  m_returnSupplyRatio=0.9;
  m_logSink.setLogLevel(Warn);
  m_logSink.setChannelRegex(boost::regex("openstudio\\.contam\\.ForwardTranslator"));
  m_logSink.setThreadId(QThread::currentThread());
  m_progressBar = 0;
  read(":/templates/template.prj");
  if(valid())
  {
    // The template is a legal PRJ file, so it has one level. Not for long.
    setLevels(std::vector<prj::Level>());
  }
  setExteriorFlowRate(flow,n,deltaP);
}

int ForwardTranslator::tableLookup(QMap<std::string,int> map, std::string str, const char *name)
{
  int nr = map.value(str,0);
  if(!nr)
  {
    LOG(Warn, "Unable to look up '" << str << "' in " << name);
  }
  return nr;
}

int ForwardTranslator::tableLookup(QMap<Handle,int> map, Handle handle, const char *name)
{
  int nr = map.value(handle,0);
  if(!nr)
  {
    LOG(Warn, "Unable to look up '" << handle << "' in " << name);
  }
  return nr;
}

int ForwardTranslator::tableLookup(std::map<Handle,int> map, Handle handle, const char *name)
{
  std::map<Handle,int>::const_iterator iter = map.find(handle);
  int nr = 0;
  if(iter == map.end())
  {
    LOG(Warn, "Unable to look up '" << handle << "' in " << name);
  }
  else
  {
    nr = iter->second;
  }
  return nr;
}

std::string ForwardTranslator::reverseLookup(QMap<std::string,int> map, int nr, const char *name)
{
  if(nr > 0)
  {
    QList<std::string> keys = map.keys(nr);
    if(keys.size()>0)
    {
      if(keys.size()>1)
      {
        LOG(Warn, "Lookup table " << name << " contains multiple " << nr << " values");
      }
      return keys[0];
    }
  }
  LOG(Warn, "Unable to reverse look up " << nr << " in " << name);
  return std::string();
}

Handle ForwardTranslator::reverseLookup(QMap<Handle,int> map, int nr, const char *name)
{
  if(nr > 0)
  {
    QList<Handle> keys = map.keys(nr);
    if(keys.size()>0)
    {
      if(keys.size()>1)
      {
        LOG(Warn, "Lookup table " << name << " contains multiple " << nr << " values");
      }
      return keys[0];
    }
  }
  LOG(Warn, "Unable to reverse look up " << nr << " in " << name);
  return Handle();
}

void findAFEs(QMap<QString,int> &afeMap, QMap<QString,int> &extWallAFE, QMap<QString,int> &intWallAFE, 
              QMap<QString,int> &floorAFE, QMap<QString,int> &roofAFE)
{
  /*
  QStringList grade, wallExt, wallInt, floor, roof;
  grade << "Leaky" << "Average" << "Tight";
  wallExt << "ExtWallLeaky" << "ExtWallAvg" << "ExtWallTight";
  wallInt << "IntWallLeaky" << "IntWallAvg" << "IntWallTight";
  floor << "FloorLeaky" << "FloorAvg" << "FloorTight";
  roof << "RoofLeaky" << "RoofAvg" << "RoofTight";
  for(int i=0;i<grade.size();i++)
  {
    extWallAFE[grade[i]] = afeMap[wallExt[i]];
    intWallAFE[grade[i]] = afeMap[wallInt[i]];
    floorAFE[grade[i]] = afeMap[floor[i]];
    roofAFE[grade[i]] = afeMap[roof[i]];
  }
  */
}

bool ForwardTranslator::setAirtightnessLevel(std::string level)
{
  // For this to work, the "standard" names must be in the PRJ data
  std::map<std::string,int> afeMap;
  QList<std::string> grade, wallExt, wallInt, floor, roof;
  grade << "Leaky" << "Average" << "Tight";
  wallExt << "ExtWallLeaky" << "ExtWallAvg" << "ExtWallTight";
  wallInt << "IntWallLeaky" << "IntWallAvg" << "IntWallTight";
  floor << "FloorLeaky" << "FloorAvg" << "FloorTight";
  roof << "RoofLeaky" << "RoofAvg" << "RoofTight";
  int index = grade.indexOf(level);
  if(index == -1) // Default to average, unknown leakage level
  {
    LOG(Warn, "Unknown airtightness label '" << level << "', defaulting to 'Average'");
    index = 1;
  }
  int nr;
  // Exterior walls
  nr = airflowElementNrByName(wallExt[index]);
  if(!nr)
  {
    return false;
  }
  afeMap["exterior"] = nr;
  // Interior walls
  nr = airflowElementNrByName(wallInt[index]);
  if(!nr)
  {
    return false;
  }
  afeMap["interior"] =  nr;
  // Floors
  nr = airflowElementNrByName(floor[index]);
  if(!nr)
  {
    return false;
  }
  afeMap["floor"] = nr;
  // Roof
  nr = airflowElementNrByName(roof[index]);
  if(!nr)
  {
    return false;
  }
  afeMap["roof"] = nr;
  m_afeMap = afeMap;
  return true;
}

bool ForwardTranslator::setExteriorFlowRate(double flow,double n,double deltaP)
{
  std::map<std::string,int> afeMap;
  afeMap["exterior"] = addNewAirflowElement("CustomExterior",flow,n,deltaP);
  afeMap["roof"] = addNewAirflowElement("CustomRoof",flow,n,deltaP);
  afeMap["interior"] = addNewAirflowElement("CustomInterior",2*flow,n,deltaP);
  afeMap["floor"] = addNewAirflowElement("CustomFloor",2*flow,n,deltaP);
  m_afeMap = afeMap;
  return true;
}

bool ForwardTranslator::toPrj(const openstudio::path& path)
{
  boost::optional<std::string> output = this->toString();
  if(output)
  {
    QFile file(toQString(path));
    if(file.open(QFile::WriteOnly))
    {
      QTextStream textStream(&file);
      textStream << *output;
      file.close();
      return true;
    }
  }
  return false;
}

bool ForwardTranslator::modelToPrj(const openstudio::model::Model& model, const openstudio::path& path,
  bool translateHVAC, std::string leakageDescriptor, ProgressBar* progressBar)
{
  ForwardTranslator translator;

  boost::optional<std::string> output;
  output = translator.translateToString(model,translateHVAC,leakageDescriptor);
  if (!output)
  {
    return false;
  }
  QFile file(toQString(path));
  if(file.open(QFile::WriteOnly))
  {
    QTextStream textStream(&file);
    textStream << *output;
    file.close();
  }
  else
  {
    return false;
  }

  return false;
}

boost::optional<std::string> ForwardTranslator::translateToString(const openstudio::model::Model& model,
                                                                  bool translateHVAC, std::string leakageDescriptor)
{
  setAirtightnessLevel(leakageDescriptor);
  if(translate(model,translateHVAC))
    return boost::optional<std::string>(toString());
  return false;
}

//bool ForwardTranslator::translate(const openstudio::model::Model& model, bool translateHVAC, std::string leakageDescriptor,
 // ProgressBar *progressBar)
//{
  /*
  m_progressBar = progressBar;
  m_logSink.setThreadId(QThread::currentThread());
  m_logSink.resetStringStream();
  QString output;
  int nr;
  // Load the template
  //openstudio::contam::prj::Data data(":/templates/template.prj",false);
  //m_data.read(":/templates/template.prj",false);
  //if(!m_data.valid)
  //{
  //  return false;
  //}
  // The template is a legal PRJ file, so it has one level. Not for long.
  //m_data.levels.clear();
  // Verify that the leakageDescriptor is one we know about
  QList<std::string> known = QList<std::string>() << std::string("Average") << std::string("Tight")
    << std::string("Leaky");
  QString descriptor = openstudio::toQString(leakageDescriptor);
  if(!known.contains(leakageDescriptor))
  {
    LOG(Warn, "Unknown leakage descriptor '" << leakageDescriptor << "' using 'Average'");
    descriptor = "Average";
  }
  // Build the airflow element lookup table
  for(int i=0;i<m_data.airflowElements.size();i++)
  {
    m_afeMap[m_data.airflowElements[i]->name] = m_data.airflowElements[i]->nr;
    //std::cout << m_data.airflowElements[i]->name.toStdString() << " " << m_afeMap[m_data.airflowElements[i]->name] <<std::endl;
  }
  // Build up data for setting wall leakage
  QMap<QString,int> extWallAFE;
  QMap<QString,int> intWallAFE;
  QMap<QString,int> floorAFE;
  QMap<QString,int> roofAFE;
  findAFEs(m_afeMap,extWallAFE,intWallAFE,floorAFE,roofAFE);
  // Set top-level model info
  boost::optional<model::Building> building = model.getOptionalUniqueModelObject<model::Building>();
  QString modelDescr = QString("Automatically generated OpenStudio model");
  if(building)
  {
    boost::optional<std::string> name = building->name();
    if(name)
      modelDescr = QString("Automatically generated from \"%1\" OpenStudio model").arg(openstudio::toQString(name.get()));
  }
  m_data.rc.setPrjdesc(modelDescr);
  std::vector<openstudio::model::BuildingStory> stories = model.getModelObjects<openstudio::model::BuildingStory>();
  // Translate each building story into a level and generate a lookup table by handle.
  if (m_progressBar)
  {
    m_progressBar->setWindowTitle(openstudio::toString("Translating Stories"));
    m_progressBar->setMinimum(0);
    m_progressBar->setMaximum(stories.size());
    m_progressBar->setValue(0);
  }
  nr=1;
  double totalHeight = 0;
  BOOST_FOREACH(const openstudio::model::BuildingStory& buildingStory, stories)
  {
    openstudio::contam::prj::Level level;
    level.setName(QString("<%1>").arg(nr).toStdString());
    m_levelMap[buildingStory.handle()] = nr;
    double ht = buildingStory.nominalFloortoFloorHeight();
    totalHeight += ht;
    boost::optional<double> elevation = buildingStory.nominalZCoordinate();
    double z = totalHeight;
    if(elevation)
      z = *elevation;
    level.setNr(nr);
    level.setRefht(QString("%1").arg(z).toStdString());
    level.setDelht(QString("%1").arg(ht).toStdString());
    m_data.levels().push_back(level);
    nr++;
    if (m_progressBar)
    {
      m_progressBar->setValue(m_progressBar->value() + 1);
    }
  }
  m_data.rc.setWind_H(QString().sprintf("%g",totalHeight),toStdString());
  // Check for levels - translation can't proceed without levels
  if(m_data.levels.size() == 0)
  {
    LOG(Error, "Failed to find building stories in model, translation aborted");
    m_valid = false;
    return false;
  }
  // Translate each thermal zone and generate a lookup table by name.
  std::vector<openstudio::model::ThermalZone> thermalZones = model.getConcreteModelObjects<openstudio::model::ThermalZone>();
  if (m_progressBar)
  {
    m_progressBar->setWindowTitle(openstudio::toString("Translating Zones"));
    m_progressBar->setMinimum(0);
    m_progressBar->setMaximum(thermalZones.size());
    m_progressBar->setValue(0);
  }
  nr=0;
  BOOST_FOREACH(openstudio::model::ThermalZone thermalZone, thermalZones)
  {
    nr++;
    openstudio::contam::prj::Zone zone;
    m_zoneMap[thermalZone.handle()] = nr;
    //volumeMap[thermalZone.name().get()] = nr;
    zone.setNr(nr);
    zone.setName(QString("Zone_%1").arg(nr).toStdString());
    boost::optional<double> volume = thermalZone.volume();
    QString volString("0.0");
    if(volume)
    {
      volString = QString("%1").arg(*volume);
    }
    else
    {
      // Since it seems this is a pretty common thing, no warning unless we can't get a value
      // LOG(Warn, "Zone '" << name.toStdString() << "' has zero volume, trying to sum space volumes");
      double vol=0.0;
      BOOST_FOREACH(openstudio::model::Space space, thermalZone.spaces())
      {
        vol += space.volume();
      }
      if(vol == 0.0)
      {
        LOG(Warn, "Failed to compute volume for Zone '" << thermalZone.name().get() << "'");
      }
      else
      {
        volString = QString("%1").arg(vol);
      }
    }
    zone.setVol(volString.toStdString());
    zone.setVariablePressure(true);
    zone.setVariableContaminants(true);
    // Set level - this is not great and will fail to create a legitimate model in cases
    // where a zone is on more than one level. There are ugly workarounds - will need to
    // think about
    int levelNr = 0;
    BOOST_FOREACH(openstudio::model::Space space, thermalZone.spaces())
    {
      boost::optional<openstudio::model::BuildingStory> story = space.buildingStory();
      if(story)
      {
        levelNr = tableLookup(m_levelMap,(*story).handle(),"levelMap");
        break;
      }
    }
    if(levelNr)
    {
      zone.setPl(levelNr);
    }
    else
    {
      LOG(Error, "Unable to set level for zone '" << thermalZone.name().get() << "', translation aborted");
      m_valid = false;
      return false;
    }
    // set T0
    zone.setT0(QString("293.15").toStdString());
    m_data.zones().push_back(zone);
    if (m_progressBar)
    {
      m_progressBar->setValue(m_progressBar->value() + 1);
    }
  }

  // Create paths and generate a lookup table by name
  std::vector<openstudio::model::Surface> surfaces = model.getConcreteModelObjects<openstudio::model::Surface>();
  if (m_progressBar)
  {
    m_progressBar->setWindowTitle(openstudio::toString("Translating Surfaces"));
    m_progressBar->setMinimum(0);
    m_progressBar->setMaximum(surfaces.size());
    m_progressBar->setValue(0);
  }
  nr = 0;
  // Loop over surfaces and generate paths
  QList <openstudio::Handle>used;
  double wind_H = QString().fromStdString(m_data.rc.wind_H()).toDouble();
  BOOST_FOREACH(openstudio::model::Surface surface,surfaces)
  {
    openstudio::contam::prj::Path path;
    std::string bc = surface.outsideBoundaryCondition();
    if(!used.contains(surface.handle()) && bc != "Ground")
    {
      // Get the associated thermal zone
      boost::optional<openstudio::model::Space> space = surface.space();
      if(!space)
      {
        LOG(Warn, "Unattached surface '" << surface.name().get() << "'");
        if (m_progressBar)
        {
          m_progressBar->setValue(m_progressBar->value() + 1);
        }
        continue;
      }
      boost::optional<openstudio::model::ThermalZone> thermalZone = space->thermalZone();
      if(!thermalZone)
      {
        LOG(Warn, "Unattached space '" << space->name().get() << "'");
        if (m_progressBar)
        {
          m_progressBar->setValue(m_progressBar->value() + 1);
        }
        continue;
      }
      // Use the lookup table to get the zone info
      int zoneNr;
      if(!(zoneNr=tableLookup(m_zoneMap,thermalZone->handle(),"zoneMap")))
      {
        // Maybe this needs a warning?
        if (m_progressBar)
        {
          m_progressBar->setValue(m_progressBar->value() + 1);
        }
        continue;
      }
      openstudio::contam::prj::Zone *zone = &(m_data.zones()[zoneNr-1]);
      // Get the surface area - will need to do more work here later if large openings are present
      double area = surface.grossArea();
      std::string type = surface.surfaceType();
      double averageZ = 0;
      double numVertices = surface.vertices().size();
      BOOST_FOREACH(const Point3d& point, surface.vertices())
      {
        averageZ += point.z();
      }
      // Now set the path info
      path.setRelHt(QString().sprintf("%g",averageZ / numVertices - QString().fromStdString(m_data.levels[zone->pl-1].refht()).toDouble()).toStdString());
      path.setPld(zone->pl());
      path.setMult(QString().sprintf("%g",area).toStdString());
      // Now for the type specific info
      if(bc == "Outdoors")
      {
        // Make an exterior flow path
        path.setPzn(zone->nr());
        path.setPzm(-1);
        // Set the wind-related stuff here
        path.setWazm(QString().sprintf("%g",openstudio::radToDeg(surface.azimuth())).toStdString());
        path.setWindPressure(true);
        path.setWPmod(QString().sprintf("%g",openstudio::wind::pressureModifier(openstudio::wind::Default,wind_H)).toStdString());
        path.setPw(4); // Assume standard template
        // Set flow element
        if(type == "RoofCeiling")
        {
          path.pe = roofAFE[descriptor];
          path.pw = 5; // Assume standard template
        }
        else
        {
          path.pe = extWallAFE[descriptor];
        }
        path.nr = ++nr;
        m_surfaceMap[surface.handle()] = path.nr;
        m_data.paths << path;
      }
      else if (bc == "Surface")
      {
        boost::optional<openstudio::model::Surface> adjacentSurface = surface.adjacentSurface();
        if(!adjacentSurface)
        {
          LOG(Error, "Unable to find adjacent surface for surface '" << surface.name().get() << "'");
          m_valid = false;
          return false;
        }
        boost::optional<openstudio::model::Space> adjacentSpace = adjacentSurface->space();
        if(!adjacentSpace)
        {
          LOG(Error, "Unattached adjacent surface '" << adjacentSurface->name().get() << "'");
          m_valid = false;
          return false;
        }
        boost::optional<openstudio::model::ThermalZone> adjacentZone = adjacentSpace->thermalZone();
        if(!adjacentZone)
        {
          LOG(Error, "Unattached adjacent space '" << adjacentSpace->name().get() << "'");
          m_valid = false;
          return false;
        }
        if(adjacentZone.get() != thermalZone.get()) // I don't really like doing this
        {
          // Make an interior flow path
          path.pzn = zone->nr;
          path.pzm = m_zoneMap[adjacentZone->handle()];
          // Set flow element
          if(type == "Floor" || type == "RoofCeiling")
          {
            path.pe = floorAFE[descriptor];
          }
          else
          {
            path.pe = intWallAFE[descriptor];
          }
          path.nr = ++nr;
          m_surfaceMap[surface.handle()] = path.nr;
          m_data.paths << path;
          used << adjacentSurface->handle();
        }
      }
    }
    if (m_progressBar)
    {
      m_progressBar->setValue(m_progressBar->value() + 1);
    }
  }

  if(translateHVAC)
  {
    // Generate air handling systems
    std::vector<openstudio::model::AirLoopHVAC> systems = model.getConcreteModelObjects<openstudio::model::AirLoopHVAC>();
    if (m_progressBar)
    {
      m_progressBar->setWindowTitle(openstudio::toString("Translating AirLoops"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum(systems.size());
      m_progressBar->setValue(0);
    }
    nr = 0;
    BOOST_FOREACH(openstudio::model::AirLoopHVAC airloop,systems)
    {
      // Skip loops with no zones attached
      if(!airloop.thermalZones().size())
      {
        if (m_progressBar)
        {
          m_progressBar->setValue(m_progressBar->value() + 1);
        }
        continue;
      }
      openstudio::contam::prj::Ahs ahs;
      ahs.nr = ++nr;
      m_ahsMap[airloop.handle()] = nr;
      ahs.name = QString("AHS_%1").arg(nr);
      // Create supply and return zones
      openstudio::contam::prj::Zone rz;
      rz.nr = m_data.zones.size()+1;
      rz.pl = 1;
      rz.T0 = QString("293.15");
      rz.setSystem(true);
      rz.setVariableContaminants(true);
      rz.name = QString("AHS_%1(Rec)").arg(nr);
      //volumeMap[rz.name.toStdString()] = rz.nr;
      openstudio::contam::prj::Zone sz;
      sz.nr = rz.nr+1;
      sz.pl = 1;
      sz.T0 = QString("293.15");
      sz.setSystem(true);
      sz.setVariableContaminants(true);
      sz.name = QString("AHS_%1(Sup)").arg(nr);
      //volumeMap[sz.name.toStdString()] = sz.nr;
      // Store the zone numbers in the ahs
      ahs.zone_r = rz.nr;
      ahs.zone_s = sz.nr;
      // Add them to the zone list
      m_data.zones << rz << sz;
      // Now hook the served zones up to the supply and return zones
      BOOST_FOREACH(openstudio::model::ThermalZone thermalZone, airloop.thermalZones())
      {
        int zoneNr = tableLookup(m_zoneMap,thermalZone.handle(),"zoneMap");
        // Supply path
        openstudio::contam::prj::Path sp;
        sp.nr = m_data.paths.size()+1;
        sp.pld = 1;
        sp.pzn = ahs.zone_s;
        sp.pzm = zoneNr;
        sp.pa = ahs.nr;
        sp.setSystem(true);
        m_pathMap[(thermalZone.name().get()+" supply")] = sp.nr;
        // Return path
        openstudio::contam::prj::Path rp;
        rp.nr = sp.nr+1;
        rp.pld = 1;
        rp.pzn = zoneNr;
        rp.pzm = ahs.zone_r;
        rp.pa = ahs.nr;
        rp.setSystem(true);
        m_pathMap[(thermalZone.name().get()+" return")] = rp.nr;
        // Add the paths to the path list
        m_data.paths << sp << rp;
      }
      m_data.ahs << ahs;
      if (m_progressBar)
      {
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    if (m_progressBar)
    {
      m_progressBar->setWindowTitle(openstudio::toString("Connecting AHS to zones"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum(m_data.ahs.size());
      m_progressBar->setValue(0);
    }
    // Now loop back through the AHS list and connect the supply and return zones together
    for(int i=0;i<m_data.ahs.size();i++)
    {
      std::string loopName = QString("AHS_%1").arg(i+1).toStdString();
      // Recirculation path
      openstudio::contam::prj::Path recirc;
      recirc.nr = m_data.paths.size()+1;
      recirc.pld = 1;
      // Set the OA fraction schedule here
      //recirc.ps = ?
      recirc.pzn = m_data.ahs[i].zone_r;
      recirc.pzm = m_data.ahs[i].zone_s;
      recirc.setRecirculation(true);
      m_pathMap[loopName + " recirculation"] = recirc.nr;
      // Outside air path
      openstudio::contam::prj::Path oa;
      oa.nr = recirc.nr+1;
      oa.pld = 1;
      oa.pzn = -1;
      oa.pzm = m_data.ahs[i].zone_s;
      oa.setOutsideAir(true);
      m_pathMap[loopName + " oa"] = oa.nr;
      // Exhaust path;
      openstudio::contam::prj::Path exhaust;
      exhaust.nr = oa.nr+1;
      exhaust.pld = 1;
      exhaust.pzn = m_data.ahs[i].zone_r;
      exhaust.pzm = -1;
      exhaust.setExhaust(true);
      m_pathMap[loopName + " exhaust"] = exhaust.nr;
      // Add the paths to the path list
      m_data.paths << recirc << oa << exhaust;
      // Store the nrs in the ahs
      m_data.ahs[i].path_r = recirc.nr;
      m_data.ahs[i].path_s = oa.nr;
      m_data.ahs[i].path_x = exhaust.nr;
      if (m_progressBar)
      {
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    // The rest of this isn't hooked into the progress bar yet, will need to do that at some point

    // Try to use E+ results to set flow rates. The supply and return flow paths are in the path
    // lookup table under the names thermalZone.name + supply|return (see above)
    boost::optional<openstudio::SqlFile> sqlFile = model.sqlFile();
    if(sqlFile)
    {
      std::string envPeriod; 
      BOOST_FOREACH(std::string t, sqlFile->availableEnvPeriods())
      {
        envPeriod = t; // should only ever be one
        break;
      }
      // get sizing results, get flow rate schedules for each zone's inlet, return, and exhaust nodes
      // This should be moved to inside the contam translator
      BOOST_FOREACH(model::ThermalZone thermalZone, model.getModelObjects<model::ThermalZone>())
      {
        LOG(Warn, "Zone equipment not yet accounted for.");
        // todo: this does not include OA from zone equipment (PTAC, PTHP, etc) or exhaust fans
        boost::optional<model::Node> returnAirNode;
        boost::optional<model::ModelObject> returnAirModelObject = thermalZone.returnAirModelObject();
        if (returnAirModelObject)
        {
          returnAirNode = returnAirModelObject->optionalCast<model::Node>();
        }
        if (returnAirNode)
        {
          std::string keyValue = returnAirNode->name().get();
          keyValue = boost::regex_replace(keyValue, boost::regex("([a-z])"),"\\u$1");
          boost::optional<TimeSeries> timeSeries = sqlFile->timeSeries(envPeriod, "Hourly", 
            "System Node MassFlowRate", keyValue);
          if (timeSeries)
          {
            openstudio::Vector values = timeSeries->values();
          }
        }

        boost::optional<model::Node> supplyAirNode;
        boost::optional<model::ModelObject> supplyAirModelObject = thermalZone.inletPortList().airLoopHVACModelObject();
        if (supplyAirModelObject)
        {
          supplyAirNode = supplyAirModelObject->optionalCast<model::Node>();
        }
        if (supplyAirNode)
        {
          std::string keyValue = supplyAirNode->name().get();
          keyValue = boost::regex_replace(keyValue, boost::regex("([a-z])"),"\\u$1");
          boost::optional<TimeSeries> timeSeries = sqlFile->timeSeries(envPeriod, "Hourly",
            "System Node MassFlowRate", keyValue);
          if (timeSeries)
          {
            openstudio::Vector values = timeSeries->values();
          }
        }
      }
    }
    else
    {
      LOG(Warn, "Simulation results not available, using 1 scfm/ft^2 to set supply flows");
      // Use the 1 scfm/ft^2 approximation with 90% return
      BOOST_FOREACH(openstudio::model::ThermalZone thermalZone,
        model.getConcreteModelObjects<openstudio::model::ThermalZone>())
      {
        double area=0.0;
        BOOST_FOREACH(openstudio::model::Space space, thermalZone.spaces())
        {
          area += space.floorArea();
        }
        if(area == 0.0)
        {
          LOG(Warn, "Failed to compute floor area for Zone '" << thermalZone.name().get() << "'");
        }
        else
        {
          double flowRate = area*0.00508*1.2041;  // Assume 1 scfm/ft^2 as an approximation
          std::string supplyName = thermalZone.name().get() + " supply";
          std::string returnName = thermalZone.name().get() + " return";
          int supplyNr = m_pathMap.value(supplyName,0);
          if(supplyNr)
          {
            m_data.paths[supplyNr-1].Fahs = QString().sprintf("%g",flowRate);
          }
          int returnNr = m_pathMap.value(returnName,0);
          if(returnNr)
          {
            m_data.paths[returnNr-1].Fahs = QString().sprintf("%g",0.9*flowRate);
          }
        }
      }
    }
  }
  */
//  m_valid = true;

 // return true;

  // these are probably useful, will have to ask Kyle
  // Kyle, should these functions be const?
  //boost::optional<model::ModelObject> airInletModelObject = zone.airInletModelObject();
  //boost::optional<model::ModelObject> returnAirModelObject = zone.returnAirModelObject();
  //boost::optional<model::Node> exhaustAirNode = zone.exhaustAirNode();
  //boost::optional<model::Node> zoneAirNode = zone.zoneAirNode();

  //ZoneData *afz = zoneList.at(i);
  //double flowRate = afz->area*0.00508*1.2041;  // Assume 1 scfm/ft^2 as an approximation

//}

bool ForwardTranslator::translate(const openstudio::model::Model& model, bool translateHVAC, ProgressBar *progressBar)
{
  m_progressBar = progressBar;
  m_logSink.setThreadId(QThread::currentThread());
  m_logSink.resetStringStream();
  QString output;
  int nr;
  //
  // All of the model specific elements should probably be cleared here 
  //
  // Set top-level model info
  boost::optional<model::Building> building = model.getOptionalUniqueModelObject<model::Building>();
  QString modelDescr = QString("Automatically generated OpenStudio model");
  if(building)
  {
    boost::optional<std::string> name = building->name();
    if(name)
      modelDescr = QString("Automatically generated from \"%1\" OpenStudio model").arg(openstudio::toQString(name.get()));
  }
  rc().setPrjdesc(modelDescr.toStdString());
  std::vector<openstudio::model::BuildingStory> stories = model.getModelObjects<openstudio::model::BuildingStory>();
  // Translate each building story into a level and generate a lookup table by handle.
  if (m_progressBar)
  {
    m_progressBar->setWindowTitle(openstudio::toString("Translating Stories"));
    m_progressBar->setMinimum(0);
    m_progressBar->setMaximum(stories.size());
    m_progressBar->setValue(0);
  }
  nr=1;
  double totalHeight = 0;
  BOOST_FOREACH(const openstudio::model::BuildingStory& buildingStory, stories)
  {
    openstudio::contam::prj::Level level;
    level.setName(QString("<%1>").arg(nr).toStdString());
    m_levelMap[buildingStory.handle()] = nr;
    double ht = buildingStory.nominalFloortoFloorHeight();
    totalHeight += ht;
    boost::optional<double> elevation = buildingStory.nominalZCoordinate();
    double z = totalHeight;
    if(elevation)
      z = *elevation;
    level.setNr(nr);
    level.setRefht(QString("%1").arg(z).toStdString());
    level.setDelht(QString("%1").arg(ht).toStdString());
    addLevel(level);
    nr++;
    if (m_progressBar)
    {
      m_progressBar->setValue(m_progressBar->value() + 1);
    }
  }
  rc().setWind_H(QString().sprintf("%g",totalHeight).toStdString());
  // Check for levels - translation can't proceed without levels
  if(levels().size() == 0)
  {
    LOG(Error, "Failed to find building stories in model, translation aborted");
    m_ready = false;
    return false;
  }
  // Translate each thermal zone and generate a lookup table by name.
  std::vector<openstudio::model::ThermalZone> thermalZones = model.getConcreteModelObjects<openstudio::model::ThermalZone>();
  if (m_progressBar)
  {
    m_progressBar->setWindowTitle(openstudio::toString("Translating Zones"));
    m_progressBar->setMinimum(0);
    m_progressBar->setMaximum(thermalZones.size());
    m_progressBar->setValue(0);
  }
  nr=0;
  BOOST_FOREACH(openstudio::model::ThermalZone thermalZone, thermalZones)
  {
    nr++;
    openstudio::contam::prj::Zone zone;
    m_zoneMap[thermalZone.handle()] = nr;
    //volumeMap[thermalZone.name().get()] = nr;
    zone.setNr(nr);
    zone.setName(QString("Zone_%1").arg(nr).toStdString());
    boost::optional<double> volume = thermalZone.volume();
    QString volString("0.0");
    if(volume)
    {
      volString = QString("%1").arg(*volume);
    }
    else
    {
      // Since it seems this is a pretty common thing, no warning unless we can't get a value
      // LOG(Warn, "Zone '" << name.toStdString() << "' has zero volume, trying to sum space volumes");
      double vol=0.0;
      BOOST_FOREACH(openstudio::model::Space space, thermalZone.spaces())
      {
        vol += space.volume();
      }
      if(vol == 0.0)
      {
        LOG(Warn, "Failed to compute volume for Zone '" << thermalZone.name().get() << "'");
      }
      else
      {
        volString = QString("%1").arg(vol);
      }
    }
    zone.setVol(volString.toStdString());
    zone.setVariablePressure(true);
    zone.setVariableContaminants(true);
    // Set level - this is not great and will fail to create a legitimate model in cases
    // where a zone is on more than one level. There are ugly workarounds - will need to
    // think about
    int levelNr = 0;
    BOOST_FOREACH(openstudio::model::Space space, thermalZone.spaces())
    {
      boost::optional<openstudio::model::BuildingStory> story = space.buildingStory();
      if(story)
      {
        levelNr = tableLookup(m_levelMap,(*story).handle(),"levelMap");
        break;
      }
    }
    if(levelNr)
    {
      zone.setPl(levelNr);
    }
    else
    {
      LOG(Error, "Unable to set level for zone '" << thermalZone.name().get() << "', translation aborted");
      m_ready = false;
      return false;
    }
    // set T0
    zone.setT0(QString("293.15").toStdString());
    addZone(zone);
    if (m_progressBar)
    {
      m_progressBar->setValue(m_progressBar->value() + 1);
    }
  }

  // Create paths and generate a lookup table by name
  std::vector<openstudio::model::Surface> surfaces = model.getConcreteModelObjects<openstudio::model::Surface>();
  if (m_progressBar)
  {
    m_progressBar->setWindowTitle(openstudio::toString("Translating Surfaces"));
    m_progressBar->setMinimum(0);
    m_progressBar->setMaximum(surfaces.size());
    m_progressBar->setValue(0);
  }
  nr = 0;
  // Loop over surfaces and generate paths
  QList <openstudio::Handle>used;
  double wind_H = QString().fromStdString(rc().wind_H()).toDouble();
  BOOST_FOREACH(openstudio::model::Surface surface,surfaces)
  {
    openstudio::contam::prj::Path path;
    std::string bc = surface.outsideBoundaryCondition();
    if(!used.contains(surface.handle()) && bc != "Ground")
    {
      // Get the associated thermal zone
      boost::optional<openstudio::model::Space> space = surface.space();
      if(!space)
      {
        LOG(Warn, "Unattached surface '" << surface.name().get() << "'");
        if (m_progressBar)
        {
          m_progressBar->setValue(m_progressBar->value() + 1);
        }
        continue;
      }
      boost::optional<openstudio::model::ThermalZone> thermalZone = space->thermalZone();
      if(!thermalZone)
      {
        LOG(Warn, "Unattached space '" << space->name().get() << "'");
        if (m_progressBar)
        {
          m_progressBar->setValue(m_progressBar->value() + 1);
        }
        continue;
      }
      // Use the lookup table to get the zone info
      int zoneNr;
      if(!(zoneNr=tableLookup(m_zoneMap,thermalZone->handle(),"zoneMap")))
      {
        // Maybe this needs a warning?
        if (m_progressBar)
        {
          m_progressBar->setValue(m_progressBar->value() + 1);
        }
        continue;
      }
      openstudio::contam::prj::Zone zone = zones()[zoneNr-1];
      // Get the surface area - will need to do more work here later if large openings are present
      double area = surface.grossArea();
      std::string type = surface.surfaceType();
      double averageZ = 0;
      double numVertices = surface.vertices().size();
      BOOST_FOREACH(const Point3d& point, surface.vertices())
      {
        averageZ += point.z();
      }
      // Now set the path info
      path.setRelHt(QString().sprintf("%g",averageZ / numVertices - QString().fromStdString(levels()[zone.pl()-1].refht()).toDouble()).toStdString());
      path.setPld(zone.pl());
      path.setMult(QString().sprintf("%g",area).toStdString());
      // Now for the type specific info
      if(bc == "Outdoors")
      {
        // Make an exterior flow path
        path.setPzn(zone.nr());
        path.setPzm(-1);
        // Set the wind-related stuff here
        path.setWazm(QString().sprintf("%g",openstudio::radToDeg(surface.azimuth())).toStdString());
        path.setWindPressure(true);
        path.setWPmod(QString().sprintf("%g",openstudio::wind::pressureModifier(openstudio::wind::Default,wind_H)).toStdString());
        path.setPw(4); // Assume standard template
        // Set flow element
        if(type == "RoofCeiling")
        {
          path.setPe(m_afeMap["roof"]);
          path.setPw(5); // Assume standard template
        }
        else
        {
          path.setPe(m_afeMap["exterior"]);
        }
        path.setNr(++nr);
        m_surfaceMap[surface.handle()] = path.nr();
        addPath(path);
      }
      else if (bc == "Surface")
      {
        boost::optional<openstudio::model::Surface> adjacentSurface = surface.adjacentSurface();
        if(!adjacentSurface)
        {
          LOG(Error, "Unable to find adjacent surface for surface '" << surface.name().get() << "'");
          m_ready = false;
          return false;
        }
        boost::optional<openstudio::model::Space> adjacentSpace = adjacentSurface->space();
        if(!adjacentSpace)
        {
          LOG(Error, "Unattached adjacent surface '" << adjacentSurface->name().get() << "'");
          m_ready = false;
          return false;
        }
        boost::optional<openstudio::model::ThermalZone> adjacentZone = adjacentSpace->thermalZone();
        if(!adjacentZone)
        {
          LOG(Error, "Unattached adjacent space '" << adjacentSpace->name().get() << "'");
          m_ready = false;
          return false;
        }
        if(adjacentZone.get() != thermalZone.get()) // I don't really like doing this
        {
          // Make an interior flow path
          path.setPzn(zone.nr());
          path.setPzm (m_zoneMap[adjacentZone->handle()]);
          // Set flow element
          if(type == "Floor" || type == "RoofCeiling")
          {
            path.setPe(m_afeMap["floor"]);
          }
          else
          {
            path.setPe(m_afeMap["interior"]);
          }
          path.setNr(++nr);
          m_surfaceMap[surface.handle()] = path.nr();
          addPath(path);
          used << adjacentSurface->handle();
        }
      }
    }
    if (m_progressBar)
    {
      m_progressBar->setValue(m_progressBar->value() + 1);
    }
  }

  if(translateHVAC)
  {
    // Generate air handling systems
    std::vector<openstudio::model::AirLoopHVAC> systems = model.getConcreteModelObjects<openstudio::model::AirLoopHVAC>();
    if (m_progressBar)
    {
      m_progressBar->setWindowTitle(openstudio::toString("Translating AirLoops"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum(systems.size());
      m_progressBar->setValue(0);
    }
    nr = 0;
    BOOST_FOREACH(openstudio::model::AirLoopHVAC airloop,systems)
    {
      // Skip loops with no zones attached
      if(!airloop.thermalZones().size())
      {
        if (m_progressBar)
        {
          m_progressBar->setValue(m_progressBar->value() + 1);
        }
        continue;
      }
      openstudio::contam::prj::Ahs ahs;
      ahs.setNr(++nr);
      m_ahsMap[airloop.handle()] = nr;
      ahs.setName(QString("AHS_%1").arg(nr).toStdString());
      // Create supply and return zones
      openstudio::contam::prj::Zone rz;
      rz.setNr(zones().size()+1);
      rz.setPl(1);
      rz.setT0(QString("293.15").toStdString());
      rz.setSystem(true);
      rz.setVariableContaminants(true);
      rz.setName(QString("AHS_%1(Rec)").arg(nr).toStdString());
      //volumeMap[rz.name.toStdString()] = rz.nr;
      openstudio::contam::prj::Zone sz;
      sz.setNr(rz.nr()+1);
      sz.setPl(1);
      sz.setT0(QString("293.15").toStdString());
      sz.setSystem(true);
      sz.setVariableContaminants(true);
      sz.setName(QString("AHS_%1(Sup)").arg(nr).toStdString());
      //volumeMap[sz.name.toStdString()] = sz.nr;
      // Store the zone numbers in the ahs
      ahs.setZone_r(rz.nr());
      ahs.setZone_s(sz.nr());
      // Add them to the zone list
      addZone(rz);
      addZone(sz);
      // Now hook the served zones up to the supply and return zones
      BOOST_FOREACH(openstudio::model::ThermalZone thermalZone, airloop.thermalZones())
      {
        int zoneNr = tableLookup(m_zoneMap,thermalZone.handle(),"zoneMap");
        // Supply path
        openstudio::contam::prj::Path sp;
        sp.setNr(paths().size()+1);
        sp.setPld(1);
        sp.setPzn(ahs.zone_s());
        sp.setPzm(zoneNr);
        sp.setPa(ahs.nr());
        sp.setSystem(true);
        m_pathMap[(thermalZone.name().get()+" supply")] = sp.nr();
        // Return path
        openstudio::contam::prj::Path rp;
        rp.setNr(sp.nr()+1);
        rp.setPld(1);
        rp.setPzn(zoneNr);
        rp.setPzm(ahs.zone_r());
        rp.setPa(ahs.nr());
        rp.setSystem(true);
        m_pathMap[(thermalZone.name().get()+" return")] = rp.nr();
        // Add the paths to the path list
        addPath(sp);
        addPath(rp);
      }
      addAhs(ahs);
      if (m_progressBar)
      {
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    if (m_progressBar)
    {
      m_progressBar->setWindowTitle(openstudio::toString("Connecting AHS to zones"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum(ahs().size());
      m_progressBar->setValue(0);
    }
    // Now loop back through the AHS list and connect the supply and return zones together
    for(unsigned int i=0;i<ahs().size();i++)
    {
      std::string loopName = QString("AHS_%1").arg(i+1).toStdString();
      // Recirculation path
      openstudio::contam::prj::Path recirc;
      recirc.setNr(paths().size()+1);
      recirc.setPld(1);
      // Set the OA fraction schedule here
      //recirc.ps = ?
      recirc.setPzn(ahs()[i].zone_r());
      recirc.setPzm(ahs()[i].zone_s());
      recirc.setRecirculation(true);
      m_pathMap[loopName + " recirculation"] = recirc.nr();
      // Outside air path
      openstudio::contam::prj::Path oa;
      oa.setNr(recirc.nr()+1);
      oa.setPld(1);
      oa.setPzn(-1);
      oa.setPzm(ahs()[i].zone_s());
      oa.setOutsideAir(true);
      m_pathMap[loopName + " oa"] = oa.nr();
      // Exhaust path;
      openstudio::contam::prj::Path exhaust;
      exhaust.setNr(oa.nr()+1);
      exhaust.setPld(1);
      exhaust.setPzn(ahs()[i].zone_r());
      exhaust.setPzm(-1);
      exhaust.setExhaust(true);
      m_pathMap[loopName + " exhaust"] = exhaust.nr();
      // Add the paths to the path list
      addPath(recirc);
      addPath(oa); 
      addPath(exhaust);
      // Store the nrs in the ahs
      ahs()[i].setPath_r(recirc.nr());
      ahs()[i].setPath_s(oa.nr());
      ahs()[i].setPath_x(exhaust.nr());
      if (m_progressBar)
      {
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    // The rest of this isn't hooked into the progress bar yet, will need to do that at some point

    // Try to use E+ results to set temperatures and flow rates. The supply and return flow paths are in the path
    // lookup table under the names thermalZone.name + supply|return (see above)
    boost::optional<openstudio::SqlFile> sqlFile = model.sqlFile();
    if(sqlFile)
    {
      std::vector<std::string> available = sqlFile->availableTimeSeries();
      BOOST_FOREACH(std::string var, available)
      {
        std::cout << '\t' << var << std::endl;
      }
      std::string envPeriod; 
      BOOST_FOREACH(std::string t, sqlFile->availableEnvPeriods())
      {
        envPeriod = t; // should only ever be one
        break;
      }
      // bool setTime=false;
      if(std::find(available.begin(), available.end(), "Zone Mean Air Temperature")!=available.end())
      {
        // Loop through and get a time series for each zone we can find
        BOOST_FOREACH(model::ThermalZone thermalZone, model.getModelObjects<model::ThermalZone>())
        {
          boost::optional<std::string> name = thermalZone.name();
          if(!name)
          {
            LOG(Warn,"Zone " << openstudio::toString(thermalZone.handle()) << " has no name and will have constant temperature");
            continue;
          }
          std::string keyValue = name.get();
          keyValue = boost::regex_replace(keyValue, boost::regex("([a-z])"),"\\u$1");
          boost::optional<TimeSeries> timeSeries = sqlFile->timeSeries(envPeriod, "Hourly", 
            "Zone Mean Air Temperature", keyValue);
          if(timeSeries)
          {
            int nr =  m_zoneMap[thermalZone.handle()];
            // std::cout << "Found time series for zone " << name.get() << ", CONTAM index " << nr << std::endl;
            // Create a control node
            std::string controlName = QString("ctrl_z_%1").arg(nr).toStdString();
            std::string valueName = QString("temp_%1").arg(nr).toStdString();
            m_cvf.addTimeSeries(valueName,*timeSeries);
            prj::CvfDat ctrl;
            ctrl.setName(controlName);
            ctrl.setValuename(valueName);
            addControlNode(ctrl);
            // Connect to the zone
            zones()[nr-1].setPc(ctrl.nr());
          }
          else
          {
            LOG(Warn,"Zone '" << name.get() << "' has no Zone Mean Air Temperature time series");
          }
        }
      }
      else
      {
        LOG(Warn, "Activate \"Zone Mean Air Temperature\" output to set zone temperature controls.");
      }
      if(std::find(available.begin(), available.end(), "System Node MassFlowRate")!=available.end())
      {
        LOG(Warn, "Zone equipment not yet accounted for.");
        // get sizing results, get flow rate schedules for each zone's inlet, return, and exhaust nodes
        // This should be moved to inside the contam translator
        BOOST_FOREACH(model::ThermalZone thermalZone, model.getModelObjects<model::ThermalZone>())
        {
          // todo: this does not include OA from zone equipment (PTAC, PTHP, etc) or exhaust fans

          boost::optional<model::Node> supplyAirNode;
          boost::optional<model::ModelObject> supplyAirModelObject = thermalZone.inletPortList().airLoopHVACModelObject();
          if (supplyAirModelObject)
          {
            supplyAirNode = supplyAirModelObject->optionalCast<model::Node>();
          }
          if (supplyAirNode)
          {
            std::string keyValue = supplyAirNode->name().get();
            keyValue = boost::regex_replace(keyValue, boost::regex("([a-z])"),"\\u$1");
            boost::optional<TimeSeries> timeSeries = sqlFile->timeSeries(envPeriod, "Hourly",
              "System Node MassFlowRate", keyValue);
            if (timeSeries)
            {
              std::cout << "Found time series for supply to zone " << thermalZone.name().get() << std::endl;
              nr = m_pathMap.value(thermalZone.name().get()+" supply",0);
              // There really should not be a case of missing number here, but it is better to be safe
              if(!nr)
              {
                LOG(Error,"Supply node for zone '" << thermalZone.name().get() << "' has no associated CONTAM path");
                continue;
              }
              // Create a control node
              std::string controlName = QString("ctrl_p_%1").arg(nr).toStdString();
              std::string valueName = QString("supply_%1").arg(nr).toStdString();
              m_cvf.addTimeSeries(valueName,*timeSeries);
              prj::CvfDat ctrl;
              ctrl.setName(controlName);
              ctrl.setValuename(valueName);
              addControlNode(ctrl);
              // Connect to the path
              paths()[nr-1].setPc(ctrl.nr());
              if(m_ratioOverride) // This assumes that there *is* a return, which could be wrong? maybe?
              {
                // Create a new time series
                TimeSeries returnSeries = (*timeSeries)*m_returnSupplyRatio;
                nr = m_pathMap.value(thermalZone.name().get()+" return",0);
                // There really should not be a case of missing number here, but it is better to be safe
                if(!nr)
                {
                  LOG(Error,"Failed to find return path for zone '" << thermalZone.name().get() << "'");
                  continue;
                }
                // Create a control node
                std::string controlName = QString("ctrl_p_%1r").arg(nr).toStdString();
                std::string valueName = QString("return_%1").arg(nr).toStdString();
                m_cvf.addTimeSeries(valueName,*timeSeries);
                prj::CvfDat ctrl;
                ctrl.setName(controlName);
                ctrl.setValuename(valueName);
                addControlNode(ctrl);
                // Connect to the path
                paths()[nr-1].setPc(ctrl.nr());
              }
            }
          }

          if(!m_ratioOverride)
          {
            boost::optional<model::Node> returnAirNode;
            boost::optional<model::ModelObject> returnAirModelObject = thermalZone.returnAirModelObject();
            if (returnAirModelObject)
            {
              returnAirNode = returnAirModelObject->optionalCast<model::Node>();
            }
            if (returnAirNode)
            {
              std::string keyValue = returnAirNode->name().get();
              keyValue = boost::regex_replace(keyValue, boost::regex("([a-z])"),"\\u$1");
              boost::optional<TimeSeries> timeSeries = sqlFile->timeSeries(envPeriod, "Hourly", 
                "System Node MassFlowRate", keyValue);
              if (timeSeries)
              {
                std::cout << "Found time series for return from zone " << thermalZone.name().get() << std::endl;
                nr = m_pathMap.value(thermalZone.name().get()+" return",0);
                // There really should not be a case of missing number here, but it is better to be safe
                if(!nr)
                {
                  LOG(Error,"Return node for zone '" << thermalZone.name().get() << "' has no associated CONTAM path");
                  continue;
                }
                // Create a control node
                std::string controlName = QString("ctrl_p_%1").arg(nr).toStdString();
                std::string valueName = QString("return_%1").arg(nr).toStdString();
                m_cvf.addTimeSeries(valueName,*timeSeries);
                prj::CvfDat ctrl;
                ctrl.setName(controlName);
                ctrl.setValuename(valueName);
                addControlNode(ctrl);
                // Connect to the path
                paths()[nr-1].setPc(ctrl.nr());
              }
            }
          }
        }
      }
      else
      {
        LOG(Warn, "Activate \"System Node MassFlowRate\" output to set zone supply/return flows.");
      }
      // The modification of the PRJ for transient simulation probably should be here
    }
    else
    {
      LOG(Warn, "Simulation results not available, using 1 scfm/ft^2 to set supply flows");
      // Use the 1 scfm/ft^2 approximation with 90% return
      BOOST_FOREACH(openstudio::model::ThermalZone thermalZone,
        model.getConcreteModelObjects<openstudio::model::ThermalZone>())
      {
        double area=0.0;
        BOOST_FOREACH(openstudio::model::Space space, thermalZone.spaces())
        {
          area += space.floorArea();
        }
        if(area == 0.0)
        {
          LOG(Warn, "Failed to compute floor area for Zone '" << thermalZone.name().get() << "'");
        }
        else
        {
          double flowRate = area*0.00508*1.2041;  // Assume 1 scfm/ft^2 as an approximation
          std::string supplyName = thermalZone.name().get() + " supply";
          std::string returnName = thermalZone.name().get() + " return";
          int supplyNr = m_pathMap.value(supplyName,0);
          if(supplyNr)
          {
            paths()[supplyNr-1].setFahs(QString().sprintf("%g",flowRate).toStdString());
          }
          int returnNr = m_pathMap.value(returnName,0);
          if(returnNr)
          {
            paths()[returnNr-1].setFahs(QString().sprintf("%g",m_returnSupplyRatio*flowRate).toStdString());
          }
        }
      }
    }
  }
  m_ready = true;

  return true;

  // these are probably useful, will have to ask Kyle
  // Kyle, should these functions be const?
  //boost::optional<model::ModelObject> airInletModelObject = zone.airInletModelObject();
  //boost::optional<model::ModelObject> returnAirModelObject = zone.returnAirModelObject();
  //boost::optional<model::Node> exhaustAirNode = zone.exhaustAirNode();
  //boost::optional<model::Node> zoneAirNode = zone.zoneAirNode();

  //ZoneData *afz = zoneList.at(i);
  //double flowRate = afz->area*0.00508*1.2041;  // Assume 1 scfm/ft^2 as an approximation

}

bool ForwardTranslator::translateEpw(const openstudio::model::Model& model, openstudio::path outpath)
{
  boost::optional<model::WeatherFile> weatherFile = model.weatherFile();
  if(!weatherFile)
  {
    LOG(Warn,"No weather file object to process");
    return false;
  }
  boost::optional<EpwFile> epwFile = weatherFile->file();
  if(!epwFile)
  {
    LOG(Warn,"No weather file path to process");
    return false;
  }
  try
  {
    epwFile->translateToWth(outpath);
  }
  catch(...)
  {
    LOG(Error,"Translation of EPW file failed, weather will be steady state");
    return false;
  }
  return true;
}

bool ForwardTranslator::translateEpw(openstudio::path epwpath, openstudio::path outpath)
{
  std::cout << openstudio::toString(epwpath) << std::endl;
  try
  {
    EpwFile epwFile(epwpath,true);
    try
    {
      epwFile.translateToWth(outpath);
    }
    catch(...) // Is this going to work?
    {
      LOG(Error,"Translation of EPW file failed, weather will be steady state");
      return false;
    }
  }
  catch(...)
  {
    LOG(Error,"Failed to correctly load EPW file, weather will be steady state");
    return false;
  }
  return true;
}

std::string ForwardTranslator::toString()
{
  if(ready())
    return prj::Model::toString();
  return std::string();
}


//bool ForwardTranslator::translate(const openstudio::model::Model& model, double leakageRate, bool translateHVAC,
//  ProgressBar *progressBar)
//{
  // Use the leakage rate to add new leakage paths
  /*
  std::map<std::string,int> afeMap;
  afeMap["exterior"] = addAirflowElement("CustomExterior",leakageRate);
  afeMap["roof"] = addAirflowElement("CustomRoof",leakageRate);
  afeMap["interior"] = addAirflowElement("CustomInterior",2*leakageRate);
  afeMap["floor"] = addAirflowElement("CustomFloor",2*leakageRate);

  return translate(model, afeMap, translateHVAC, progressBar);
  */
 // return false;
//}

bool ForwardTranslator::setSteadyWeather(double windSpeed, double windDirection)
{
  if(windSpeed < 0)
  {
    LOG(Warn, "Steady state wind speed is negative, using absolute value.");
    windSpeed = -windSpeed; // Maybe should return false in this case?
  }
  // Is a negative wind direction allowed? Will have to check
  rc().ssWeather().setWindspd(QString().sprintf("%g",windSpeed).toStdString());
  rc().ssWeather().setWinddir(QString().sprintf("%g",windDirection).toStdString());
  return true;
}

static double laminarCoefficient(double Ct, double x)
{
  // Ct  turbulent flow coefficient
  // x   exponent
  double Clam;                // laminar flow coefficient
  double F;                   // mass flow rate at L-T transition
  double dP=0;                // pressure drop at L-T transition
  double RHOAIR = 1.20410;    // density of standard air
  double SRHO = 1.097315;     // sqrt( RHOAIR )
  double SQRT2 = 1.414213562; // sqrt( 2.0 )
  double MUAIR = 1.81625e-5;  // viscosity of standard air
  double DPTMIN = 1.0e-10;    // minimum L-T transition dP
  double Re = 30.0;           // Reynolds number of L-T transition

  double A = Ct/(0.6*SQRT2);  // flow area
  double D = sqrt(A);         // hydraulic diameter

  // Re = rho * V * D / mu; F = rho * V * A; therefore
  F = MUAIR * Re * A / D;
  // F = Ct * sqrt(rho) * dP^x
  dP = std::pow( F / (Ct * SRHO), 1.0 / x );
  if( dP < DPTMIN )
  {
    dP = DPTMIN;   // 1998/07/17 - dPt > 0
  }
  // F = Clam * (rho / mu) * dP
  Clam = ( (MUAIR * F) / (RHOAIR * dP) );

  return Clam;

}

int ForwardTranslator::addNewAirflowElement(std::string name, double flow,double n,double deltaP)
{
  // flow - volume flow rate in m^3/h
  // deltaP - pressure difference in Pa
  // n - exponent  

  double RHOAIR = 1.20410;    // density of standard air
  double SRHO = 1.097315;     // sqrt( RHOAIR )
    
  double F = RHOAIR*flow/3600.0;  // mass flow in kg/s

  double Ct = F/(SRHO*std::pow(deltaP,n));
  double Cl = laminarCoefficient(Ct,n);

  std::string lam = QString().sprintf("%g",Cl).toStdString();
  std::string turb = QString().sprintf("%g",Ct).toStdString();
  std::string expt = QString().sprintf("%g",n).toStdString();
  std::string dP = QString().sprintf("%g",deltaP).toStdString();
  std::string Flow = QString().sprintf("%g",F).toStdString();
  int u_P = 0;
  int u_F = 1; // Display units are m^3/h

  // Create a 1-point test element with display units of m^3/h
  prj::PlrTest1 afe(0, 0, name, " ", lam, turb, expt, dP, Flow, u_P, u_F);

  addAirflowElement(afe);

  return afe.nr();
}

bool ForwardTranslator::writeCvFile(openstudio::path filepath)
{
  if(!m_cvf.isEmpty())
  {
    return m_cvf.write(filepath);
  }
  return false;
}

std::vector<LogMessage> ForwardTranslator::warnings() const
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

std::vector<LogMessage> ForwardTranslator::errors() const
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
