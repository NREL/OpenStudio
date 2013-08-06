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
#include <contam/PrjData.hpp>
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

#include <utilities/sql/SqlFile.hpp>
#include <utilities/core/Logger.hpp>
#include <utilities/geometry/Geometry.hpp>

#include <boost/foreach.hpp>
#include <boost/math/constants/constants.hpp>


#include <QFile>
#include <QTextStream>
#include <QMap>

namespace openstudio 
{
namespace contam
{

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
  }

  bool ForwardTranslator::modelToContam(const openstudio::model::Model& model, const openstudio::path& path,
    const openstudio::path& mapPath)
  {
    ForwardTranslator translator;

    boost::optional<QString> output;
    output = translator.translateToPrj(model);
    if (!output)
      return false;

    QFile file(toQString(path));
    if(file.open(QFile::WriteOnly))
    {
      QTextStream textStream(&file);
      textStream << *output;
      file.close();
    }
    else
      return false;

    if(translator.writeMaps(mapPath))
      return true;

    return false;
  }

  bool ForwardTranslator::modelToContam(const openstudio::model::Model& model, const openstudio::path& path)
  {
    ForwardTranslator translator;

    boost::optional<QString> output;
    output = translator.translateToPrj(model);
    if (!output)
      return false;

    QFile file(toQString(path));
    if(file.open(QFile::WriteOnly))
    {
      QTextStream textStream(&file);
      textStream << *output;
      file.close();
    }
    else
      return false;

    return false;
  }

  ForwardTranslator::ForwardTranslator()
  {
  }

  boost::optional<QString> ForwardTranslator::translateToPrj(const openstudio::model::Model& model,
    bool translateHVAC)
  {
    QString output;
    int nr;
    // Load the template
    //openstudio::contam::prj::Data data(":/templates/template.prj",false);
    data.read(":/templates/template.prj",false);
    if(!data.valid)
      return false;
    // The template is a legal PRJ file, so it has one level. Not for long.
    data.levels.clear();
    // Build the airflow element lookup table
    for(int i=0;i<data.airflowElements.size();i++)
      afeMap[data.airflowElements[i]->name] = data.airflowElements[i]->nr;
    // Build up data for setting wall leakage
    QMap<QString,int> extWallAFE;
    QMap<QString,int> intWallAFE;
    QMap<QString,int> floorAFE;
    QMap<QString,int> roofAFE;
    findAFEs(afeMap,extWallAFE,intWallAFE,floorAFE,roofAFE);
    // Set top-level model info
    boost::optional<model::Building> building = model.getOptionalUniqueModelObject<model::Building>();
    QString modelDescr = QString("Automatically generated OpenStudio model");
    if(building)
    {
      boost::optional<std::string> name = building->name();
      if(name)
        modelDescr = QString("Automatically generated from \"%1\" OpenStudio model").arg(openstudio::toQString(name.get()));
    }
    data.rc.prjdesc = modelDescr;
    // Translate each building story into a level and generate a lookup table by handle.
    // Probably need to add something here to complain about lack of stories
    nr=1;
    double totalHeight = 0;
    BOOST_FOREACH(const openstudio::model::BuildingStory& buildingStory, model.getModelObjects<openstudio::model::BuildingStory>())
    {
      openstudio::contam::prj::Level level;
      level.name = QString("<%1>").arg(nr);
      levelMap[buildingStory.handle()] = nr;
      double ht = buildingStory.nominalFloortoFloorHeight();
      totalHeight += ht;
      boost::optional<double> elevation = buildingStory.nominalZCoordinate();
      double z = totalHeight;
      if(elevation)
        z = *elevation;
      level.nr = nr;
      level.refht = QString("%1").arg(z);
      level.delht = QString("%1").arg(ht);
      data.levels << level;
      nr++;
    }
    data.rc.wind_H = QString().sprintf("%g",totalHeight);
    // Check for levels - translation can't proceed without levels
    if(data.levels.size() == 0)
    {
      LOG(Error, "Failed to find building stories in model, translation aborted");
      return false;
    }
    // Translate each thermal zone and generate a lookup table by name.
    nr=0;
    BOOST_FOREACH(openstudio::model::ThermalZone thermalZone,
      model.getConcreteModelObjects<openstudio::model::ThermalZone>())
    {
      nr++;
      openstudio::contam::prj::Zone zone;
      zoneMap[thermalZone.handle()] = nr;
      //volumeMap[thermalZone.name().get()] = nr;
      zone.nr = nr;
      zone.name = QString("Zone_%1").arg(nr);
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
          volString = QString("%1").arg(vol);
      }
      zone.Vol = volString;
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
          levelNr = tableLookup(levelMap,(*story).handle(),"levelMap");
          break;
        }
      }
      if(levelNr)
      {
        zone.pl = levelNr;
      }
      else
      {
        LOG(Error, "Unable to set level for zone '" << thermalZone.name().get() << "', translation aborted");
        return false;
      }
      // set T0
      zone.T0 = QString("293.15");
      data.zones << zone;
    }

    // Create paths and generate a lookup table by name
    nr = 0;
    // Loop over surfaces and generate paths
    QList <openstudio::Handle>used;
    double wind_H = data.rc.wind_H.toDouble();
    BOOST_FOREACH(openstudio::model::Surface surface, model.getConcreteModelObjects<openstudio::model::Surface>())
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
          continue;
        }
        boost::optional<openstudio::model::ThermalZone> thermalZone = space->thermalZone();
        if(!thermalZone)
        {
          LOG(Warn, "Unattached space '" << space->name().get() << "'");
          continue;
        }
        // Use the lookup table to get the zone info
        int zoneNr;
        if(!(zoneNr=tableLookup(zoneMap,thermalZone->handle(),"zoneMap")))
          continue;
        openstudio::contam::prj::Zone *zone = &(data.zones[zoneNr-1]);
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
        path.relHt = QString().sprintf("%g",averageZ / numVertices - data.levels[zone->pl-1].refht.toDouble());
        path.pld = zone->pl;
        path.mult = QString().sprintf("%g",area);
        // Now for the type specific info
        if(bc == "Outdoors")
        {
          // Make an exterior flow path
          path.pzn = zone->nr;
          path.pzm = -1;
          // Set the wind-related stuff here
          path.wazm = QString().sprintf("%g",openstudio::radToDeg(surface.azimuth()));
          path.setWindPressure(true);
          path.wPmod = QString().sprintf("%g",openstudio::wind::pressureModifier(openstudio::wind::Default,wind_H));
          path.pw = 4; // Assume standard template
          // Set flow element
          if(type == "RoofCeiling")
          {
            path.pe = roofAFE["Average"];
            path.pw = 5; // Assume standard template
          }
          else
          {
            path.pe = extWallAFE["Average"];
          }
          path.nr = ++nr;
          surfaceMap[surface.handle()] = path.nr;
          data.paths << path;
        }
        else if (bc == "Surface")
        {
          boost::optional<openstudio::model::Surface> adjacentSurface = surface.adjacentSurface();
          if(adjacentSurface)
          {
            boost::optional<openstudio::model::Space> adjacentSpace = adjacentSurface->space();
            if(adjacentSpace)
            {
              boost::optional<openstudio::model::ThermalZone> adjacentZone = adjacentSpace->thermalZone();
              if(adjacentZone)
              {
                if(adjacentZone.get() != thermalZone.get()) // I don't really like doing this
                {
                  // Make an interior flow path
                  path.pzn = zone->nr;
                  path.pzm = zoneMap[adjacentZone->handle()];
                  // Set flow element
                  if(type == "Floor" || type == "RoofCeiling")
                  {
                    path.pe = floorAFE["Average"];
                  }
                  else
                  {
                    path.pe = intWallAFE["Average"];
                  }
                  path.nr = ++nr;
                  surfaceMap[surface.handle()] = path.nr;
                  data.paths << path;
                  used << adjacentSurface->handle();
                }
              }
              else
              {
                LOG(Error, "Unattached adjacent space '" << adjacentSpace->name().get() << "'");
                return false;
              }
            }
            else
            {
              LOG(Error, "Unattached adjacent surface '" << adjacentSurface->name().get() << "'");
              return false;
            }
          }
          else
          {
            LOG(Error, "Unable to find adjacent surface for surface '" << surface.name().get() << "'");
            return false;
          }
        }
      }
    }

    if(translateHVAC)
    {
      // Generate air handling systems
      nr = 0;
      BOOST_FOREACH(openstudio::model::AirLoopHVAC airloop,
        model.getConcreteModelObjects<openstudio::model::AirLoopHVAC>())
      {
        // Skip loops with no zones attached
        if(!airloop.thermalZones().size())
          continue;
        openstudio::contam::prj::Ahs ahs;
        ahs.nr = ++nr;
        ahsMap[airloop.handle()] = nr;
        ahs.name = QString("AHS_%1").arg(nr);
        // Create supply and return zones
        openstudio::contam::prj::Zone rz;
        rz.nr = data.zones.size()+1;
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
        data.zones << rz << sz;
        // Now hook the served zones up to the supply and return zones
        BOOST_FOREACH(openstudio::model::ThermalZone thermalZone, airloop.thermalZones())
        {
          int zoneNr = tableLookup(zoneMap,thermalZone.handle(),"zoneMap");
          // Supply path
          openstudio::contam::prj::Path sp;
          sp.nr = data.paths.size()+1;
          sp.pld = 1;
          sp.pzn = ahs.zone_s;
          sp.pzm = zoneNr;
          sp.pa = ahs.nr;
          sp.setSystem(true);
          pathMap[(thermalZone.name().get()+" supply")] = sp.nr;
          // Return path
          openstudio::contam::prj::Path rp;
          rp.nr = sp.nr+1;
          rp.pld = 1;
          rp.pzn = zoneNr;
          rp.pzm = ahs.zone_r;
          rp.pa = ahs.nr;
          rp.setSystem(true);
          pathMap[(thermalZone.name().get()+" return")] = rp.nr;
          // Add the paths to the path list
          data.paths << sp << rp;
        }
        data.ahs << ahs;
      }

      // Now loop back through the AHS list and connect the supply and return zones together
      for(int i=0;i<data.ahs.size();i++)
      {
        std::string loopName = QString("AHS_%1").arg(i+1).toStdString();
        // Recirculation path
        openstudio::contam::prj::Path recirc;
        recirc.nr = data.paths.size()+1;
        recirc.pld = 1;
        // Set the OA fraction schedule here
        //recirc.ps = ?
        recirc.pzn = data.ahs[i].zone_r;
        recirc.pzm = data.ahs[i].zone_s;
        recirc.setRecirculation(true);
        pathMap[loopName + " recirculation"] = recirc.nr;
        // Outside air path
        openstudio::contam::prj::Path oa;
        oa.nr = recirc.nr+1;
        oa.pld = 1;
        oa.pzn = -1;
        oa.pzm = data.ahs[i].zone_s;
        oa.setOutsideAir(true);
        pathMap[loopName + " oa"] = oa.nr;
        // Exhaust path;
        openstudio::contam::prj::Path exhaust;
        exhaust.nr = oa.nr+1;
        exhaust.pld = 1;
        exhaust.pzn = data.ahs[i].zone_r;
        exhaust.pzm = -1;
        exhaust.setExhaust(true);
        pathMap[loopName + " exhaust"] = exhaust.nr;
        // Add the paths to the path list
        data.paths << recirc << oa << exhaust;
        // Store the nrs in the ahs
        data.ahs[i].path_r = recirc.nr;
        data.ahs[i].path_s = oa.nr;
        data.ahs[i].path_x = exhaust.nr;
      }

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
            int supplyNr,returnNr;
            if(supplyNr = pathMap.value(supplyName,0))
              data.paths[supplyNr-1].Fahs = QString().sprintf("%g",flowRate);
            if(returnNr = pathMap.value(returnName,0))
              data.paths[returnNr-1].Fahs = QString().sprintf("%g",0.9*flowRate);
          }
        }
      }
    }

    return boost::optional<QString>(data.print());

    // these are probably useful, will have to ask Kyle
    // Kyle, should these functions be const?
    //boost::optional<model::ModelObject> airInletModelObject = zone.airInletModelObject();
    //boost::optional<model::ModelObject> returnAirModelObject = zone.returnAirModelObject();
    //boost::optional<model::Node> exhaustAirNode = zone.exhaustAirNode();
    //boost::optional<model::Node> zoneAirNode = zone.zoneAirNode();

    //ZoneData *afz = zoneList.at(i);
    //double flowRate = afz->area*0.00508*1.2041;  // Assume 1 scfm/ft^2 as an approximation

  }

  bool ForwardTranslator::writeMaps(const openstudio::path& path)
  {
    //  QFile file(QString(toQString(path)));
    //  if(file.open(QFile::WriteOnly))
    //  {
    //  QTextStream textStream(&file);
    //  QList<std::string> keys = zoneMap.keys();
    //  BOOST_FOREACH(std::string name,keys)
    //  {
    //    textStream << QString::fromStdString(name) + QString(",%1\n").arg(zoneMap[name]);
    //  }
    //  file.close();
    //  return true;
    //}
    return false;
  }
} // contam
} // openstudio
