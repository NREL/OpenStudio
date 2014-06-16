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
#include "ReverseTranslator.hpp"
#include "ForwardTranslator.hpp"

#include "../model/Model.hpp"
#include "../model/ModelObject.hpp"
#include "../model/ModelObject_Impl.hpp"
#include "../model/ConstructionBase.hpp"
#include "../model/ConstructionBase_Impl.hpp"
#include "../model/Construction.hpp"
#include "../model/Construction_Impl.hpp"
#include "../model/MasslessOpaqueMaterial.hpp"
#include "../model/MasslessOpaqueMaterial_Impl.hpp"
#include "../model/StandardOpaqueMaterial.hpp"
#include "../model/StandardOpaqueMaterial_Impl.hpp"
#include "../model/FFactorGroundFloorConstruction.hpp"
#include "../model/FFactorGroundFloorConstruction_Impl.hpp"
#include "../model/CFactorUndergroundWallConstruction.hpp"
#include "../model/CFactorUndergroundWallConstruction_Impl.hpp"
#include "../model/Building.hpp"
#include "../model/Building_Impl.hpp"
#include "../model/ThermalZone.hpp"
#include "../model/ThermalZone_Impl.hpp"
#include "../model/BuildingStory.hpp"
#include "../model/BuildingStory_Impl.hpp"
#include "../model/Space.hpp"
#include "../model/Space_Impl.hpp"
#include "../model/Surface.hpp"
#include "../model/Surface_Impl.hpp"
#include "../model/SubSurface.hpp"
#include "../model/SubSurface_Impl.hpp"
#include "../model/ShadingSurface.hpp"
#include "../model/ShadingSurface_Impl.hpp"
#include "../model/ShadingSurfaceGroup.hpp"
#include "../model/ShadingSurfaceGroup_Impl.hpp"
#include "../model/PeopleDefinition.hpp"
#include "../model/People.hpp"
#include "../model/LightsDefinition.hpp"
#include "../model/Lights.hpp"
#include "../model/ElectricEquipmentDefinition.hpp"
#include "../model/ElectricEquipment.hpp"
#include "../model/GasEquipmentDefinition.hpp"
#include "../model/GasEquipment.hpp"
#include "../model/HotWaterEquipmentDefinition.hpp"
#include "../model/HotWaterEquipment.hpp"
#include "../model/DesignSpecificationOutdoorAir.hpp"
#include "../model/SpaceInfiltrationDesignFlowRate.hpp"
#include "../model/Schedule.hpp"
#include "../model/Schedule_Impl.hpp"
#include "../model/ScheduleConstant.hpp"
#include "../model/ScheduleConstant_Impl.hpp"
#include "../model/ScheduleTypeLimits.hpp"
#include "../model/ScheduleTypeLimits_Impl.hpp"
#include "../model/PlantLoop.hpp"
#include "../model/PlantLoop_Impl.hpp"
#include "../model/WaterUseConnections.hpp"
#include "../model/WaterUseConnections_Impl.hpp"
#include "../model/WaterUseEquipment.hpp"
#include "../model/WaterUseEquipment_Impl.hpp"
#include "../model/WaterUseEquipmentDefinition.hpp"
#include "../model/WaterUseEquipmentDefinition_Impl.hpp"
#include "../model/ThermostatSetpointDualSetpoint.hpp"

#include "../utilities/geometry/Transformation.hpp"
#include "../utilities/units/QuantityConverter.hpp"
#include "../utilities/units/IPUnit.hpp"
#include "../utilities/units/SIUnit.hpp"
#include "../utilities/units/BTUUnit.hpp"
#include "../utilities/units/MPHUnit.hpp"
#include "../utilities/units/WhUnit.hpp"
#include "../utilities/units/UnitFactory.hpp"
#include "../utilities/units/TemperatureUnit.hpp"
#include "../utilities/units/TemperatureUnit_Impl.hpp"
#include "../utilities/plot/ProgressBar.hpp"
#include "../utilities/core/Assert.hpp"

#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QStringList>

namespace openstudio {
namespace sdd {

  const double footToMeter =  0.3048;
  const double meterToFoot = 1.0/0.3048;

  double fixAngle(double angle){
    while (angle >= 360){
      angle -= 360;
    }
    while (angle < 0){
      angle += 360;
    }
    return angle;
  }

  QDomElement elementByTagNameAndIndex(const QDomElement & root, const QString & tagName, bool useIndex, const int & index){
    QDomElement result;
    if( useIndex ){
      bool ok;
      QDomNodeList nodes = root.elementsByTagName(tagName);
      for(int i = 0; i < nodes.count(); i++){
        QDomElement e = nodes.at(i).toElement();
        int thisIndex = e.attribute("index").toInt(&ok);
        if( ok && (thisIndex == index) ) {
          result = e;
          break;
        }
      }
    }else{
      result = root.firstChildElement(tagName); 
    }
    return result;
  }

  boost::optional<model::ModelObject> ReverseTranslator::translateBuilding(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model)
  {
    openstudio::model::Building building = model.getUniqueModelObject<openstudio::model::Building>();

    QDomElement nameElement = element.firstChildElement("Name");

    // http://code.google.com/p/cbecc/issues/detail?id=378
    // The angle between the model Y-Axis and True North, measured clockwise from the model Y-Axis in Degrees. 
    QDomElement northAngleElement = element.firstChildElement("NAng");
    // The angle between True North and the the model Y-Axis, measured clockwise from True North in Degrees.  
    QDomElement buildingAzimuthElement = element.firstChildElement("BldgAz"); // this corresponds to Building::North Axis

    QDomNodeList spaceElements = element.elementsByTagName("Spc");
    QDomNodeList thermalZoneElements = element.elementsByTagName("ThrmlZn");
    QDomNodeList buildingStoryElements = element.elementsByTagName("Story");

    OS_ASSERT(!nameElement.isNull());
    building.setName(escapeName(nameElement.text()));

    if(!buildingAzimuthElement.isNull()){
      double buildingAzimuth = fixAngle(buildingAzimuthElement.text().toDouble());
      building.setNorthAxis(buildingAzimuth);
    }else if(!northAngleElement.isNull()){
      // use NAng for backwards compatibility with SDD's only having NAng
      double northAngle = fixAngle(northAngleElement.text().toDouble());
      double buildingAzimuth = 360.0 - northAngle;
      building.setNorthAxis(buildingAzimuth);
    }

    // translate shadingSurfaces
    QDomNodeList exteriorShadingElements = element.elementsByTagName("ExtShdgObj");
    model::ShadingSurfaceGroup shadingSurfaceGroup(model);
    shadingSurfaceGroup.setName("Building ShadingGroup");
    shadingSurfaceGroup.setShadingSurfaceType("Building");
    for (int i = 0; i < exteriorShadingElements.count(); ++i){
      if (exteriorShadingElements.at(i).parentNode() == element){
        boost::optional<model::ModelObject> exteriorShading = translateShadingSurface(exteriorShadingElements.at(i).toElement(), doc, shadingSurfaceGroup);
        OS_ASSERT(exteriorShading);
      }
    }

    // create all spaces
    for (int i = 0; i < spaceElements.count(); i++){
      QDomElement spaceElement = spaceElements.at(i).toElement();
      boost::optional<model::ModelObject> space = createSpace(spaceElement, doc, model);
      OS_ASSERT(space); // what type of error handling do we want?
    }

    // create all thermal zones
    for (int i = 0; i < thermalZoneElements.count(); i++){

      if (thermalZoneElements.at(i).firstChildElement("Name").isNull()){
        continue;
      }

      QDomElement thermalZoneElement = thermalZoneElements.at(i).toElement();

      boost::optional<model::ModelObject> thermalZone = createThermalZone(thermalZoneElement, doc, model);
      OS_ASSERT(thermalZone); // what type of error handling do we want?
    }

    // translate building stories
    if (m_progressBar){
      m_progressBar->setWindowTitle(toString("Translating Storys"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum(buildingStoryElements.count());
      m_progressBar->setValue(0);
    }

    for (int i = 0; i < buildingStoryElements.count(); i++){
      QDomElement buildingStoryElement = buildingStoryElements.at(i).toElement();
      boost::optional<model::ModelObject> buildingStory = translateBuildingStory(buildingStoryElement, doc, model);
      OS_ASSERT(buildingStory); // what type of error handling do we want?

      if (m_progressBar){
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    // remove unused CFactor constructions
    for (model::CFactorUndergroundWallConstruction cFactorConstruction : model.getConcreteModelObjects<model::CFactorUndergroundWallConstruction>()){
      if (cFactorConstruction.directUseCount() == 0){
        cFactorConstruction.remove();
      }
    }

    // remove unused FFactor constructions
    for (model::FFactorGroundFloorConstruction fFactorConstruction : model.getConcreteModelObjects<model::FFactorGroundFloorConstruction>()){
      if (fFactorConstruction.directUseCount() == 0){
        fFactorConstruction.remove();
      }
    }

    return building;
  }

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::createThermalZone(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model)
  {
    QDomElement nameElement = element.firstChildElement("Name");

    model::ThermalZone thermalZone(model);

    OS_ASSERT(!nameElement.isNull());
    thermalZone.setName(escapeName(nameElement.text()));

    return thermalZone;
  }

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateBuildingStory(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model)
  {
    QDomElement nameElement = element.firstChildElement("Name");
    QDomNodeList spaceElements = element.elementsByTagName("Spc");

    model::BuildingStory buildingStory(model);

    OS_ASSERT(!nameElement.isNull());
    buildingStory.setName(escapeName(nameElement.text()));

    for (int i = 0; i < spaceElements.count(); i++){
      QDomElement spaceElement = spaceElements.at(i).toElement();
      boost::optional<model::ModelObject> space = translateSpace(spaceElement, doc, buildingStory);
      OS_ASSERT(space); // what type of error handling do we want?
    }

    return buildingStory;
  }

  boost::optional<model::ModelObject> ReverseTranslator::createSpace(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model)
  {
    QDomElement nameElement = element.firstChildElement("Name");

    model::Space space(model);

    OS_ASSERT(!nameElement.isNull());
    space.setName(escapeName(nameElement.text()));

    return space;
  }

  boost::optional<model::ModelObject> ReverseTranslator::translateSpace(const QDomElement& element, const QDomDocument& doc, openstudio::model::BuildingStory& buildingStory)
  {
    QDomElement nameElement = element.firstChildElement("Name");
    QDomElement hotWtrHtgRtElement = element.firstChildElement("HotWtrHtgRtSim");
    QDomElement hotWtrHtgSchRefElement = element.firstChildElement("HotWtrHtgSchRef");
    QDomElement shwFluidSegRefElement = element.firstChildElement("SHWFluidSegRef");
    QDomNodeList exteriorWallElements = element.elementsByTagName("ExtWall");
    QDomNodeList exteriorFloorElements = element.elementsByTagName("ExtFlr");
    QDomNodeList roofElements = element.elementsByTagName("Roof");
    QDomNodeList undergroundFloorElements = element.elementsByTagName("UndgrFlr");
    QDomNodeList undergroundWallElements = element.elementsByTagName("UndgrWall");
    QDomNodeList ceilingElements = element.elementsByTagName("Ceiling");
    QDomNodeList interiorWallElements = element.elementsByTagName("IntWall");
    QDomNodeList interiorFloorElements = element.elementsByTagName("IntFlr");

    OS_ASSERT(!nameElement.isNull());
    std::string spaceName = escapeName(nameElement.text());
    boost::optional<model::Space> space = buildingStory.model().getModelObjectByName<model::Space>(spaceName);
    OS_ASSERT(space); // what type of error handling do we want?

    space->setBuildingStory(buildingStory);

    QDomElement thermalZoneElement = element.firstChildElement("ThrmlZnRef");
    OS_ASSERT(!thermalZoneElement.isNull());
    std::string thermalZoneName = escapeName(thermalZoneElement.text());
    boost::optional<model::ThermalZone> thermalZone = space->model().getModelObjectByName<model::ThermalZone>(thermalZoneName);
    OS_ASSERT(thermalZone);
    space->setThermalZone(*thermalZone);

    translateLoads(element, doc, *space);

    for (int i = 0; i < exteriorWallElements.count(); i++){
      QDomElement exteriorWallElement = exteriorWallElements.at(i).toElement();
      boost::optional<model::ModelObject> surface = translateSurface(exteriorWallElement, doc, *space);
      OS_ASSERT(surface); // what type of error handling do we want?
    }

    for (int i = 0; i < exteriorFloorElements.count(); i++){
      QDomElement exteriorFloorElement = exteriorFloorElements.at(i).toElement();
      boost::optional<model::ModelObject> surface = translateSurface(exteriorFloorElement, doc, *space);
      OS_ASSERT(surface); // what type of error handling do we want?
    }

    for (int i = 0; i < roofElements.count(); i++){
      QDomElement roofElement = roofElements.at(i).toElement();
      boost::optional<model::ModelObject> surface = translateSurface(roofElement, doc, *space);
      OS_ASSERT(surface); // what type of error handling do we want?
    }

    for (int i = 0; i < undergroundFloorElements.count(); i++){
      QDomElement undergroundFloorElement = undergroundFloorElements.at(i).toElement();
      boost::optional<model::ModelObject> surface = translateSurface(undergroundFloorElement, doc, *space);
      OS_ASSERT(surface); // what type of error handling do we want?
    }

    for (int i = 0; i < undergroundWallElements.count(); i++){
      QDomElement undergroundWallElement = undergroundWallElements.at(i).toElement();
      boost::optional<model::ModelObject> surface = translateSurface(undergroundWallElement, doc, *space);
      OS_ASSERT(surface); // what type of error handling do we want?
    }

    for (int i = 0; i < ceilingElements.count(); i++){
      QDomElement ceilingElement = ceilingElements.at(i).toElement();
      boost::optional<model::ModelObject> surface = translateSurface(ceilingElement, doc, *space);
      OS_ASSERT(surface); // what type of error handling do we want?
    }

    for (int i = 0; i < interiorWallElements.count(); i++){
      QDomElement interiorWallElement = interiorWallElements.at(i).toElement();
      boost::optional<model::ModelObject> surface = translateSurface(interiorWallElement, doc, *space);
      OS_ASSERT(surface); // what type of error handling do we want?
    }

    for (int i = 0; i < interiorFloorElements.count(); i++){
      QDomElement interiorFloorElement = interiorFloorElements.at(i).toElement();
      boost::optional<model::ModelObject> surface = translateSurface(interiorFloorElement, doc, *space);
      OS_ASSERT(surface); // what type of error handling do we want?
    }

    // translate shadingSurfaces
    QDomNodeList exteriorShadingElements = element.elementsByTagName("ExtShdgObj");
    model::ShadingSurfaceGroup shadingSurfaceGroup(space->model());
    shadingSurfaceGroup.setName(spaceName + " ShadingGroup");
    shadingSurfaceGroup.setSpace(*space);
    for (int i = 0; i < exteriorShadingElements.count(); ++i){
      if (exteriorShadingElements.at(i).parentNode() == element){
        boost::optional<model::ModelObject> exteriorShading = translateShadingSurface(exteriorShadingElements.at(i).toElement(), doc, shadingSurfaceGroup);
       OS_ASSERT(exteriorShading);
      }
    }

    // DLM: volume is now a property associated with Thermal Zone, http://code.google.com/p/cbecc/issues/detail?id=490
    //// volume
    //if (!volElement.isNull()){
    //  // sdd units = ft^3, os units = m^3
    //  Quantity spaceVolumeIP(volElement.text().toDouble(), BTUUnit(BTUExpnt(0,3,0,0)));
    //  OptionalQuantity spaceVolumeSI = QuantityConverter::instance().convert(spaceVolumeIP, UnitSystem(UnitSystem::Wh));
    //  OS_ASSERT(spaceVolumeSI);
    //  OS_ASSERT(spaceVolumeSI->units() == WhUnit(WhExpnt(0,0,3,0)));
    //
    //  if (thermalZone->isVolumeDefaulted()){
    //    thermalZone->setVolume(spaceVolumeSI->value());
    //  }else{
    //    boost::optional<double> zoneVolume = thermalZone->volume();
    //    OS_ASSERT(zoneVolume);
    //    zoneVolume = *zoneVolume + spaceVolumeSI->value();
    //    thermalZone->setVolume(zoneVolume);
    // }
    //}

    // Service Hot Water

    bool ok;

    double value = hotWtrHtgRtElement.text().toDouble(&ok);

    model::Model model = buildingStory.model();

    boost::optional<model::PlantLoop> shwSys = serviceHotWaterLoopForSupplySegment(shwFluidSegRefElement.text(),doc,model);

    if( ok && shwSys )
    {
      model::WaterUseConnections connections(model);

      connections.setName(spaceName + " Water Use Connection");

      model::WaterUseEquipmentDefinition definition(model);

      definition.setName(spaceName + " Water Use Definition");

      definition.setPeakFlowRate(unitToUnit(value,"gal/h","m^3/s").get());

      model::WaterUseEquipment equipment(definition);

      equipment.setName(spaceName + " Water Use Equipment");

      if( boost::optional<model::Schedule> schedule = model.getModelObjectByName<model::Schedule>(hotWtrHtgSchRefElement.text().toStdString()) )
      {
        equipment.setFlowRateFractionSchedule(schedule.get());
      }

      connections.addWaterUseEquipment(equipment);

      shwSys->addDemandBranchForComponent(connections);
    }

    return boost::optional<model::ModelObject>(space);
  }

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateLoads(const QDomElement& element, const QDomDocument& doc, openstudio::model::Space& space)
  {
    UnitSystem siSys(UnitSystem::SI);
    UnitSystem whSys(UnitSystem::Wh);

    openstudio::model::Model model = space.model();
    std::string name = space.name().get();

    // DLM: what to do with these?
    //<SpcOcc>Office - Open</SpcOcc>
    //<SchOcc>Table G-I Office Occupancy</SchOcc>

    //***** OCCUPANCY *****
    {
      //<OccDens>6.67</OccDens> - person per thousand ft2
      //<OccSensHtRt>250</OccSensHtRt> - Btu per h person
      //<OccLatHtRt>200</OccLatHtRt> - Btu per h person
      //<OccSchRef>Office Occup Sched</OccSchRef>

      QDomElement occDensElement = element.firstChildElement("OccDens");
      QDomElement occSensHtRtElement = element.firstChildElement("OccSensHtRt");
      QDomElement occLatHtRtElement = element.firstChildElement("OccLatHtRt");
      QDomElement occSchRefElement = element.firstChildElement("OccSchRef");

      if (!occDensElement.isNull() && (occDensElement.text().toDouble() > 0)){
        if (!occSensHtRtElement.isNull() && !occLatHtRtElement.isNull()){

          openstudio::Quantity peopleDensityIP(occDensElement.text().toDouble() / 1000.0, openstudio::createUnit("people/ft^2",UnitSystem::BTU).get());
          OptionalQuantity peopleDensitySI = QuantityConverter::instance().convert(peopleDensityIP, whSys);
          OS_ASSERT(peopleDensitySI);
          OS_ASSERT(peopleDensitySI->units() == WhUnit(WhExpnt(0,0,-2,0,0,0,0,0,0,1)));

          openstudio::model::PeopleDefinition peopleDefinition(model);
          peopleDefinition.setName(name + " People Definition");
          peopleDefinition.setPeopleperSpaceFloorArea(peopleDensitySI->value()); // people/m2

          openstudio::Quantity sensibleHeatRateIP(occSensHtRtElement.text().toDouble(), openstudio::createUnit("Btu/h*person", UnitSystem::BTU).get());
          OptionalQuantity sensibleHeatRateSI = QuantityConverter::instance().convert(sensibleHeatRateIP, whSys);
          OS_ASSERT(sensibleHeatRateSI);
          OS_ASSERT(sensibleHeatRateSI->units() == WhUnit(WhExpnt(1,0,0,0,0,0,0,0,0,-1)));

          openstudio::Quantity latentHeatRateIP(occLatHtRtElement.text().toDouble(), openstudio::createUnit("Btu/h*person", UnitSystem::BTU).get());
          OptionalQuantity latentHeatRateSI = QuantityConverter::instance().convert(latentHeatRateIP, whSys);
          OS_ASSERT(latentHeatRateSI);
          OS_ASSERT(latentHeatRateSI->units() == WhUnit(WhExpnt(1,0,0,0,0,0,0,0,0,-1)));

          double totalHeatRateSI = sensibleHeatRateSI->value() + latentHeatRateSI->value();
          if (totalHeatRateSI > 0){
            double sensibleHeatFraction = sensibleHeatRateSI->value() / totalHeatRateSI;
            peopleDefinition.setSensibleHeatFraction(sensibleHeatFraction);
          }

          openstudio::model::People people(peopleDefinition);
          people.setName(name + " People");
          people.setSpace(space);

          // activity schedule
          openstudio::model::ScheduleConstant activitySchedule(model);
          activitySchedule.setName(name + " People Activity Level");
          activitySchedule.setValue(totalHeatRateSI);

          //boost::optional<model::ScheduleTypeLimits> scheduleTypeLimits = model.getModelObjectByName<model::ScheduleTypeLimits>("Activity Level");
          //if (!scheduleTypeLimits){
          //  scheduleTypeLimits = model::ScheduleTypeLimits(model);
          //  scheduleTypeLimits->setName("Activity Level");
          //  scheduleTypeLimits->setNumericType("Continuous");
          //  scheduleTypeLimits->setUnitType("ActivityLevel");
          //}
          //if (scheduleTypeLimits){
          //  activitySchedule.setScheduleTypeLimits(*scheduleTypeLimits);
          //}

          people.setActivityLevelSchedule(activitySchedule);

          // number of people schedule

          if (!occSchRefElement.isNull()){
            std::string scheduleName = escapeName(occSchRefElement.text());
            boost::optional<model::Schedule> schedule = model.getModelObjectByName<model::Schedule>(scheduleName);
            if (schedule){
              people.setNumberofPeopleSchedule(*schedule);
            }else{
              LOG(Error, "Could not find schedule '" << scheduleName << "'");
            }
          }
        }else{
          LOG(Error, "People density is non-zero but sensible and latent heat rates are not given");
        }
      }
    }

    //***** Infiltration *****
    {
      //<InfMthd>FlowExteriorWallArea</InfMthd>
      //<DsgnInfRt>0.0448</DsgnInfRt>
      //<InfSchRef>School Inf Sch</InfSchRef>
      //<InfModelCoefA>0</InfModelCoefA>
      //<InfModelCoefC>0.10016</InfModelCoefC>
      //<InfModelCoefD>0</InfModelCoefD>

      //InfMthd = {AirChangesPerHour, FlowArea, FlowExteriorArea, FlowExteriorWallArea, FlowZone}
      QDomNodeList infMthdNodes = element.elementsByTagName("InfMthd");
      for (int i = 0; i < infMthdNodes.count(); i++){
        
        QDomElement infMthdElement = infMthdNodes.at(i).toElement();

        bool hasIndex;
        int infIndex = infMthdElement.attribute("index").toInt(&hasIndex);

        QDomElement dsgnInfRtElement = elementByTagNameAndIndex(element,"DsgnInfRt",hasIndex,infIndex);

        if ((!infMthdElement.isNull()) && (!dsgnInfRtElement.isNull())){
          
          // DLM: for now this is the only method supported
          if ((infMthdElement.text() == "FlowExteriorWallArea") ||
              (infMthdElement.text() == "FlowArea")){

            QDomElement infSchRefElement = elementByTagNameAndIndex(element,"InfSchRef",hasIndex,infIndex);
            QDomElement infModelCoefAElement = elementByTagNameAndIndex(element,"InfModelCoefA",hasIndex,infIndex); // unitless
            QDomElement infModelCoefBElement = elementByTagNameAndIndex(element,"InfModelCoefB",hasIndex,infIndex); // 1/deltaF
            QDomElement infModelCoefCElement = elementByTagNameAndIndex(element,"InfModelCoefC",hasIndex,infIndex); // hr/mile
            QDomElement infModelCoefDElement = elementByTagNameAndIndex(element,"InfModelCoefD",hasIndex,infIndex); // hr^2/mile^2

            openstudio::model::SpaceInfiltrationDesignFlowRate spaceInfiltrationDesignFlowRate(model);
            std::string infName;
            if( hasIndex ) {
              infName = name + " Space Infiltration Design Flow Rate " + QString::number(infIndex + 1).toStdString();
            }
            else
            {
              infName = name + " Space Infiltration Design Flow Rate";
            }
            spaceInfiltrationDesignFlowRate.setName(infName);
            spaceInfiltrationDesignFlowRate.setSpace(space);

            openstudio::Quantity dsgnInfRtIP(dsgnInfRtElement.text().toDouble(), openstudio::createUnit("cfm/ft^2",UnitSystem::BTU).get());
            OptionalQuantity dsgnInfRtSI = QuantityConverter::instance().convert(dsgnInfRtIP, siSys);
            OS_ASSERT(dsgnInfRtSI);
            OS_ASSERT(dsgnInfRtSI->units() == SIUnit(SIExpnt(0,1,-1)));
            if( infMthdElement.text() == "FlowArea" ) {
              spaceInfiltrationDesignFlowRate.setFlowperSpaceFloorArea(dsgnInfRtSI->value());
            }else{ // Assume FlowExteriorWallArea
              spaceInfiltrationDesignFlowRate.setFlowperExteriorWallArea(dsgnInfRtSI->value());
            }

            if (!infSchRefElement.isNull()){
              std::string scheduleName = escapeName(infSchRefElement.text());
              boost::optional<model::Schedule> schedule = model.getModelObjectByName<model::Schedule>(scheduleName);
              if (schedule){
                spaceInfiltrationDesignFlowRate.setSchedule(*schedule);
              }else{
                LOG(Error, "Could not find schedule '" << scheduleName << "'");
              }
            }

            if (!infModelCoefAElement.isNull()){
              // unitless
              double infModelCoefA = infModelCoefAElement.text().toDouble();
              spaceInfiltrationDesignFlowRate.setConstantTermCoefficient(infModelCoefA);
            }

            if (!infModelCoefBElement.isNull()){
              // convert 1/deltaF to 1/detlaC
              spaceInfiltrationDesignFlowRate.setTemperatureTermCoefficient(infModelCoefBElement.text().toDouble() * 5.0 / 9.0);
            }

            if (!infModelCoefCElement.isNull()){
              // SDD: hr/mile, OpenStudio: s/m
              openstudio::Quantity infModelCoefCIP(infModelCoefCElement.text().toDouble(), MPHUnit(MPHExpnt(0,-1,1)));
              OptionalQuantity infModelCoefCSI = QuantityConverter::instance().convert(infModelCoefCIP, siSys);
              OS_ASSERT(infModelCoefCSI);
              OS_ASSERT(infModelCoefCSI->units() == SIUnit(SIExpnt(0,-1,1)));
              spaceInfiltrationDesignFlowRate.setVelocityTermCoefficient(infModelCoefCSI->value());
            }

            if (!infModelCoefDElement.isNull()){
              // SDD: hr^2/mile^2, OpenStudio: s^2/m^2
              openstudio::Quantity infModelCoefDIP(infModelCoefDElement.text().toDouble(), MPHUnit(MPHExpnt(0,-2,2)));
              OptionalQuantity infModelCoefDSI = QuantityConverter::instance().convert(infModelCoefDIP, siSys);
              OS_ASSERT(infModelCoefDSI);
              OS_ASSERT(infModelCoefDSI->units() == SIUnit(SIExpnt(0,-2,2)));
              spaceInfiltrationDesignFlowRate.setVelocitySquaredTermCoefficient(infModelCoefDSI->value());
            }
          }
        }
      } // end for infMthdNodes
    }

    //***** Interior Lights *****
    {
      //<IntLPDReg>1.1</IntLPDReg> - W per ft2
      //<IntLtgRegSchRef>Office Lighting Sched</IntLtgRegSchRef>
      //<IntLtgRegHtGnSpcFrac>0.61</IntLtgRegHtGnSpcFrac> - fraction to space, 1-Return Air Fraction
      //<IntLtgRegHtGnRadFrac>0.75</IntLtgRegHtGnRadFrac> - radiant fraction
      //<IntLPDNonReg>0</IntLPDNonReg> - W per ft2
      //<IntLtgNonRegSchRef>Office Lighting Sched</IntLtgNonRegSchRef>
      //<IntLtgNonRegHtGnSpcFrac>0.5</IntLtgNonRegHtGnSpcFrac> - fraction to space, 1-Return Air Fraction
      //<IntLtgNonRegHtGnRadFrac>0.55</IntLtgNonRegHtGnRadFrac> - radiant fraction


      QDomElement intLPDRegElement = element.firstChildElement("IntLPDReg");
      QDomElement intLtgRegSchRefElement = element.firstChildElement("IntLtgRegSchRef");
      QDomElement intLtgRegHtGnSpcFracElement = element.firstChildElement("IntLtgRegHtGnSpcFrac");
      QDomElement intLtgRegHtGnRadFracElement = element.firstChildElement("IntLtgRegHtGnRadFrac");
      if (!intLPDRegElement.isNull() && (intLPDRegElement.text().toDouble() > 0)){

        openstudio::Quantity lightingDensityIP(intLPDRegElement.text().toDouble(), openstudio::createUnit("W/ft^2").get());
        OptionalQuantity lightingDensitySI = QuantityConverter::instance().convert(lightingDensityIP, whSys);
        OS_ASSERT(lightingDensitySI);
        OS_ASSERT(lightingDensitySI->units() == WhUnit(WhExpnt(1,0,-2)));

        openstudio::model::LightsDefinition lightsDefinition(model);
        lightsDefinition.setName(name + " Regulated Lights Definition");
        lightsDefinition.setWattsperSpaceFloorArea(lightingDensitySI->value()); // W/m2

        openstudio::model::Lights lights(lightsDefinition);
        lights.setName(name + " Regulated Lights");
        lights.setSpace(space);
        lights.setEndUseSubcategory("Reg Ltg");

        if (!intLtgRegSchRefElement.isNull()){
          std::string scheduleName = escapeName(intLtgRegSchRefElement.text());
          boost::optional<model::Schedule> schedule = model.getModelObjectByName<model::Schedule>(scheduleName);
          if (schedule){
            lights.setSchedule(*schedule);
          }else{
            LOG(Error, "Could not find schedule '" << scheduleName << "'");
          }
        }

        if (!intLtgRegHtGnSpcFracElement.isNull()){
          double spaceFraction = intLtgRegHtGnSpcFracElement.text().toDouble();
          double returnAirFraction = 1.0 - spaceFraction;
          lightsDefinition.setReturnAirFraction(returnAirFraction);
        
          if (!intLtgRegHtGnRadFracElement.isNull()){
            double fractionRadiant = intLtgRegHtGnRadFracElement.text().toDouble() * spaceFraction;
            lightsDefinition.setFractionRadiant(fractionRadiant);
          }
        }else if (!intLtgRegHtGnRadFracElement.isNull()){
          LOG(Warn, "IntLtgRegHtGnRadFracElement is specified for space '" << name << "' but IntLtgRegHtGnSpcFracElement is not, IntLtgNonRegHtGnRadFracElement will be ignored.");
        }
      }

      QDomElement intLPDNonRegElement = element.firstChildElement("IntLPDNonReg");
      QDomElement intLtgNonRegSchRefElement = element.firstChildElement("IntLtgNonRegSchRef");
      QDomElement intLtgNonRegHtGnSpcFracElement = element.firstChildElement("IntLtgNonRegHtGnSpcFrac");
      QDomElement intLtgNonRegHtGnRadFracElement = element.firstChildElement("IntLtgNonRegHtGnRadFrac");
      if (!intLPDNonRegElement.isNull() && (intLPDNonRegElement.text().toDouble() > 0)){

        openstudio::Quantity lightingDensityIP(intLPDNonRegElement.text().toDouble(), openstudio::createUnit("W/ft^2").get());
        OptionalQuantity lightingDensitySI = QuantityConverter::instance().convert(lightingDensityIP, whSys);
        OS_ASSERT(lightingDensitySI);
        OS_ASSERT(lightingDensitySI->units() == WhUnit(WhExpnt(1,0,-2)));

        openstudio::model::LightsDefinition lightsDefinition(model);
        lightsDefinition.setName(name + " Non-Regulated Lights Definition");
        lightsDefinition.setWattsperSpaceFloorArea(lightingDensitySI->value()); // W/m2

        openstudio::model::Lights lights(lightsDefinition);
        lights.setName(name + " Non-Regulated Lights");
        lights.setSpace(space);
        lights.setEndUseSubcategory("NonReg Ltg");

        if (!intLtgNonRegSchRefElement.isNull()){
          std::string scheduleName = escapeName(intLtgNonRegSchRefElement.text());
          boost::optional<model::Schedule> schedule = model.getModelObjectByName<model::Schedule>(scheduleName);
          if (schedule){
            lights.setSchedule(*schedule);
          }else{
            LOG(Error, "Could not find schedule '" << scheduleName << "'");
          }
        }

        if (!intLtgNonRegHtGnSpcFracElement.isNull()){
          double spaceFraction = intLtgNonRegHtGnSpcFracElement.text().toDouble();
          double returnAirFraction = 1.0 - spaceFraction;
          lightsDefinition.setReturnAirFraction(returnAirFraction);

          if (!intLtgNonRegHtGnRadFracElement.isNull()){
            double fractionRadiant = intLtgNonRegHtGnRadFracElement.text().toDouble() * spaceFraction;
            lightsDefinition.setFractionRadiant(fractionRadiant);
          }
        }else if (!intLtgNonRegHtGnRadFracElement.isNull()){
          LOG(Warn, "IntLtgNonRegHtGnRadFracElement is specified for space '" << name << "' but IntLtgNonRegHtGnSpcFracElement is not, IntLtgNonRegHtGnRadFracElement will be ignored.");
        }
      }
    }

    //***** Recepticle Loads *****
    {

      //<RecptPwrDens>2.53</RecptPwrDens> - W per ft2
      //<RecptSchRef>Office Plugs Sched</RecptSchRef>
      //<RecptRadFrac>0.2</RecptRadFrac>
      //<RecptLatFrac>0</RecptLatFrac>
      //<RecptLostFrac>0</RecptLostFrac>

      QDomElement recptPwrDensElement = element.firstChildElement("RecptPwrDens");
      QDomElement recptPwrDensSchRefElement = element.firstChildElement("RecptSchRef");
      QDomElement recptRadFracElement = element.firstChildElement("RecptRadFrac");
      QDomElement recptLatFracElement = element.firstChildElement("RecptLatFrac");
      QDomElement recptLostFracElement = element.firstChildElement("RecptLostFrac");
      if (!recptPwrDensElement.isNull() && (recptPwrDensElement.text().toDouble() > 0)){

        openstudio::Quantity electricalDensityIP(recptPwrDensElement.text().toDouble(), openstudio::createUnit("W/ft^2").get());
        OptionalQuantity electricalDensitySI = QuantityConverter::instance().convert(electricalDensityIP, whSys);
        OS_ASSERT(electricalDensitySI);
        OS_ASSERT(electricalDensitySI->units() == WhUnit(WhExpnt(1,0,-2)));

        openstudio::model::ElectricEquipmentDefinition electricEquipmentDefinition(model);
        electricEquipmentDefinition.setName(name + " Recepticle Loads Definition");
        electricEquipmentDefinition.setWattsperSpaceFloorArea(electricalDensitySI->value()); // W/m2

        if (!recptRadFracElement.isNull()){
          electricEquipmentDefinition.setFractionRadiant(recptRadFracElement.text().toDouble());
        }
        if (!recptLatFracElement.isNull()){
          electricEquipmentDefinition.setFractionLatent(recptLatFracElement.text().toDouble());
        }
        if (!recptLostFracElement.isNull()){
          electricEquipmentDefinition.setFractionLost(recptLostFracElement.text().toDouble());
        }

        openstudio::model::ElectricEquipment electricEquipment(electricEquipmentDefinition);
        electricEquipment.setName(name + " Recepticle Loads");
        electricEquipment.setSpace(space);
        electricEquipment.setEndUseSubcategory("Receptacle");

        if (!recptPwrDensSchRefElement.isNull()){
          std::string scheduleName = escapeName(recptPwrDensSchRefElement.text());
          boost::optional<model::Schedule> schedule = model.getModelObjectByName<model::Schedule>(scheduleName);
          if (schedule){
            electricEquipment.setSchedule(*schedule);
          }else{
            LOG(Error, "Could not find schedule '" << scheduleName << "'");
          }
        }
      }
    }

    //***** Gas Equipment Loads *****
    {

      //<GasEqpPwrDens>17.5377</GasEqpPwrDens>
      //<GasEqpSchRef>RestaurantReceptacle</GasEqpSchRef>
      //<GasEqpRadFrac>0.2</GasEqpRadFrac>
      //<GasEqpLatFrac>0.4</GasEqpLatFrac>
      //<GasEqpLostFrac>0.2></GasEqpLostFrac>

      QDomElement gasEqpPwrDensElement = element.firstChildElement("GasEqpPwrDens");
      QDomElement gasEqpPwrDensSchRefElement = element.firstChildElement("GasEqpSchRef");
      QDomElement gasEqpRadFracElement = element.firstChildElement("GasEqpRadFrac");
      QDomElement gasEqpLatFracElement = element.firstChildElement("GasEqpLatFrac");
      QDomElement gasEqpLostFracElement = element.firstChildElement("GasEqpLostFrac");
      if (!gasEqpPwrDensElement.isNull() && (gasEqpPwrDensElement.text().toDouble() > 0)){

        openstudio::Quantity gasDensityIP(gasEqpPwrDensElement.text().toDouble(), openstudio::createUnit("Btu/h*ft^2").get());
        OptionalQuantity gasDensitySI = QuantityConverter::instance().convert(gasDensityIP, whSys);
        OS_ASSERT(gasDensitySI);
        OS_ASSERT(gasDensitySI->units() == WhUnit(WhExpnt(1,0,-2)));

        openstudio::model::GasEquipmentDefinition gasEquipmentDefinition(model);
        gasEquipmentDefinition.setName(name + " Gas Equipment Loads Definition");
        gasEquipmentDefinition.setWattsperSpaceFloorArea(gasDensitySI->value()); // W/m2

        if (!gasEqpRadFracElement.isNull()){
          gasEquipmentDefinition.setFractionRadiant(gasEqpRadFracElement.text().toDouble());
        }
        if (!gasEqpLatFracElement.isNull()){
          gasEquipmentDefinition.setFractionLatent(gasEqpLatFracElement.text().toDouble());
        }
        if (!gasEqpLostFracElement.isNull()){
          gasEquipmentDefinition.setFractionLost(gasEqpLostFracElement.text().toDouble());
        }

        openstudio::model::GasEquipment gasEquipment(gasEquipmentDefinition);
        gasEquipment.setName(name + " Gas Equipment Loads");
        gasEquipment.setSpace(space);
        gasEquipment.setEndUseSubcategory("Receptacle");

        if (!gasEqpPwrDensSchRefElement.isNull()){
          std::string scheduleName = escapeName(gasEqpPwrDensSchRefElement.text());
          boost::optional<model::Schedule> schedule = model.getModelObjectByName<model::Schedule>(scheduleName);
          if (schedule){
            gasEquipment.setSchedule(*schedule);
          }else{
            LOG(Error, "Could not find schedule '" << scheduleName << "'");
          }
        }
      }
    }

    //***** Process Electricity Loads *****
    {
      //<ProcElecPwrDens>0</ProcElecPwrDens> - W per ft2
      //<ProcElecSchRef>Office Plugs Sched</ProcElecSchRef>
      //<ProcElecRadFrac>0.2</ProcElecRadFrac>
      //<ProcElecLatFrac>0</ProcElecLatFrac>
      //<ProcElecLostFrac>0</ProcElecLostFrac>

      QDomElement procElecPwrDensElement = element.firstChildElement("ProcElecPwrDens");
      QDomElement procElecSchRefElement = element.firstChildElement("ProcElecSchRef");
      QDomElement procElecRadFracElement = element.firstChildElement("ProcElecRadFrac");
      QDomElement procElecLatFracElement = element.firstChildElement("ProcElecLatFrac");
      QDomElement procElecLostFracElement = element.firstChildElement("ProcElecLostFrac");
      if (!procElecPwrDensElement.isNull() && (procElecPwrDensElement.text().toDouble() > 0)){

        openstudio::Quantity electricalDensityIP(procElecPwrDensElement.text().toDouble(), openstudio::createUnit("W/ft^2").get());
        OptionalQuantity electricalDensitySI = QuantityConverter::instance().convert(electricalDensityIP, whSys);
        OS_ASSERT(electricalDensitySI);
        OS_ASSERT(electricalDensitySI->units() == WhUnit(WhExpnt(1,0,-2)));

        openstudio::model::ElectricEquipmentDefinition electricEquipmentDefinition(model);
        electricEquipmentDefinition.setName(name + " Process Electric Loads Definition");
        electricEquipmentDefinition.setWattsperSpaceFloorArea(electricalDensitySI->value()); // W/m2

        if (!procElecRadFracElement.isNull()){
          electricEquipmentDefinition.setFractionRadiant(procElecRadFracElement.text().toDouble());
        }
        if (!procElecLatFracElement.isNull()){
          electricEquipmentDefinition.setFractionLatent(procElecLatFracElement.text().toDouble());
        }
        if (!procElecLostFracElement.isNull()){
          electricEquipmentDefinition.setFractionLost(procElecLostFracElement.text().toDouble());
        }

        openstudio::model::ElectricEquipment electricEquipment(electricEquipmentDefinition);
        electricEquipment.setName(name + " Process Electric Loads");
        electricEquipment.setSpace(space);
        electricEquipment.setEndUseSubcategory("Process");

        if (!procElecSchRefElement.isNull()){
          std::string scheduleName = escapeName(procElecSchRefElement.text());
          boost::optional<model::Schedule> schedule = model.getModelObjectByName<model::Schedule>(scheduleName);
          if (schedule){
            electricEquipment.setSchedule(*schedule);
          }else{
            LOG(Error, "Could not find schedule '" << scheduleName << "'");
          }
        }
      }
    }

    //***** Refrigeration Loads *****
    {
      //<CommRfrgEPD>0.06</CommRfrgEPD> - W per ft2
      //<CommRfrgEqpSchRef>Office Plugs Sched</CommRfrgEqpSchRef>
      //<CommRfrgRadFrac>0.1</CommRfrgRadFrac>
      //<CommRfrgLatFrac>0.1</CommRfrgLatFrac>
      //<CommRfrgLostFrac>0.6</CommRfrgLostFrac>

      QDomElement commRfrgEPDElement = element.firstChildElement("CommRfrgEPD");
      QDomElement commRfrgEqpSchRefElement = element.firstChildElement("CommRfrgEqpSchRef");
      QDomElement commRfrgRadFracElement = element.firstChildElement("CommRfrgRadFrac");
      QDomElement commRfrgLatFracElement = element.firstChildElement("CommRfrgLatFrac");
      QDomElement commRfrgLostFracElement = element.firstChildElement("CommRfrgLostFrac");
      if (!commRfrgEPDElement.isNull() && (commRfrgEPDElement.text().toDouble() > 0)){

        openstudio::Quantity electricalDensityIP(commRfrgEPDElement.text().toDouble(), openstudio::createUnit("W/ft^2").get());
        OptionalQuantity electricalDensitySI = QuantityConverter::instance().convert(electricalDensityIP, whSys);
        OS_ASSERT(electricalDensitySI);
        OS_ASSERT(electricalDensitySI->units() == WhUnit(WhExpnt(1,0,-2)));

        openstudio::model::ElectricEquipmentDefinition electricEquipmentDefinition(model);
        electricEquipmentDefinition.setName(name + " Refrigeration Loads Definition");
        electricEquipmentDefinition.setWattsperSpaceFloorArea(electricalDensitySI->value()); // W/m2

        if (!commRfrgRadFracElement.isNull()){
          electricEquipmentDefinition.setFractionRadiant(commRfrgRadFracElement.text().toDouble());
        }
        if (!commRfrgLatFracElement.isNull()){
          electricEquipmentDefinition.setFractionLatent(commRfrgLatFracElement.text().toDouble());
        }
        if (!commRfrgLostFracElement.isNull()){
          electricEquipmentDefinition.setFractionLost(commRfrgLostFracElement.text().toDouble());
        }

        openstudio::model::ElectricEquipment electricEquipment(electricEquipmentDefinition);
        electricEquipment.setName(name + " Refrigeration Loads");
        electricEquipment.setSpace(space);
        electricEquipment.setEndUseSubcategory("Refrig");

        if (!commRfrgEqpSchRefElement.isNull()){
          std::string scheduleName = escapeName(commRfrgEqpSchRefElement.text());
          boost::optional<model::Schedule> schedule = model.getModelObjectByName<model::Schedule>(scheduleName);
          if (schedule){
            electricEquipment.setSchedule(*schedule);
          }else{
            LOG(Error, "Could not find schedule '" << scheduleName << "'");
          }
        }
      }
    }

    //***** Elevator Loads *****
    {
      //<ElevPwr>20000</ElevPwr> 
			//<ElevSchRef>OfficeElevator</ElevSchRef>
			//<ElevRadFrac>0</ElevRadFrac>
			//<ElevLatFrac>0</ElevLatFrac>
			//<ElevLostFrac>0.8</ElevLostFrac>

      QDomElement elevPwrElement = element.firstChildElement("ElevPwr");
      QDomElement elevSchRefElement = element.firstChildElement("ElevSchRef");
      QDomElement elevRadFracElement = element.firstChildElement("ElevRadFrac");
      QDomElement elevLatFracElement = element.firstChildElement("ElevLatFrac");
      QDomElement elevLostFracElement = element.firstChildElement("ElevLostFrac");
      if (!elevPwrElement.isNull() && (elevPwrElement.text().toDouble() > 0)){

        openstudio::model::ElectricEquipmentDefinition electricEquipmentDefinition(model);
        electricEquipmentDefinition.setName(name + " Elevator Definition");
        electricEquipmentDefinition.setDesignLevel(elevPwrElement.text().toDouble()); 

        if (!elevRadFracElement.isNull()){
          electricEquipmentDefinition.setFractionRadiant(elevRadFracElement.text().toDouble());
        }
        if (!elevLatFracElement.isNull()){
          electricEquipmentDefinition.setFractionLatent(elevLatFracElement.text().toDouble());
        }
        if (!elevLostFracElement.isNull()){
          electricEquipmentDefinition.setFractionLost(elevLostFracElement.text().toDouble());
        }

        openstudio::model::ElectricEquipment electricEquipment(electricEquipmentDefinition);
        electricEquipment.setName(name + " Elevator");
        electricEquipment.setSpace(space);
        electricEquipment.setEndUseSubcategory("Internal Transport");

        if (!elevSchRefElement.isNull()){
          std::string scheduleName = escapeName(elevSchRefElement.text());
          boost::optional<model::Schedule> schedule = model.getModelObjectByName<model::Schedule>(scheduleName);
          if (schedule){
            electricEquipment.setSchedule(*schedule);
          }else{
            LOG(Error, "Could not find schedule '" << scheduleName << "'");
          }
        }
      }
    }

    //***** Escalator Loads *****
    {
			//<EscalPwr>7860</EscalPwr>
			//<EscalSchRef>OfficeEscalator Sch</EscalSchRef>
			//<EscalRadFrac>0</EscalRadFrac>
			//<EscalLatFrac>0</EscalLatFrac>
			//<EscalLostFrac>0.2</EscalLostFrac>

      QDomElement escalPwrElement = element.firstChildElement("EscalPwr");
      QDomElement escalSchRefElement = element.firstChildElement("EscalSchRef");
      QDomElement escalRadFracElement = element.firstChildElement("EscalRadFrac");
      QDomElement escalLatFracElement = element.firstChildElement("EscalLatFrac");
      QDomElement escalLostFracElement = element.firstChildElement("EscalLostFrac");
      if (!escalPwrElement.isNull() && (escalPwrElement.text().toDouble() > 0)){

        openstudio::model::ElectricEquipmentDefinition electricEquipmentDefinition(model);
        electricEquipmentDefinition.setName(name + " Escalator Definition");
        electricEquipmentDefinition.setDesignLevel(escalPwrElement.text().toDouble()); 

        if (!escalRadFracElement.isNull()){
          electricEquipmentDefinition.setFractionRadiant(escalRadFracElement.text().toDouble());
        }
        if (!escalLatFracElement.isNull()){
          electricEquipmentDefinition.setFractionLatent(escalLatFracElement.text().toDouble());
        }
        if (!escalLostFracElement.isNull()){
          electricEquipmentDefinition.setFractionLost(escalLostFracElement.text().toDouble());
        }

        openstudio::model::ElectricEquipment electricEquipment(electricEquipmentDefinition);
        electricEquipment.setName(name + " Escalator");
        electricEquipment.setSpace(space);
        electricEquipment.setEndUseSubcategory("Internal Transport");

        if (!escalSchRefElement.isNull()){
          std::string scheduleName = escapeName(escalSchRefElement.text());
          boost::optional<model::Schedule> schedule = model.getModelObjectByName<model::Schedule>(scheduleName);
          if (schedule){
            electricEquipment.setSchedule(*schedule);
          }else{
            LOG(Error, "Could not find schedule '" << scheduleName << "'");
          }
        }
      }
    }

    //***** Process Gas Loads *****
    {
      //<ProcGasPwrDens>0.04</ProcGasPwrDens> - Btu per h ft2
      //<ProcGasSchRef>Office Plugs Sched</ProcGasSchRef>
      //<ProcGasRadFrac>0.2</ProcGasRadFrac>
      //<ProcGasLatFrac>0.4</ProcGasLatFrac>
      //<ProcGasLostFrac>0.2</ProcGasLostFrac>

      QDomElement procGasPwrDensElement = element.firstChildElement("ProcGasPwrDens");
      QDomElement procGasSchRefElement = element.firstChildElement("ProcGasSchRef");
      QDomElement procGasRadFracElement = element.firstChildElement("ProcGasRadFrac");
      QDomElement procGasLatFracElement = element.firstChildElement("ProcGasLatFrac");
      QDomElement procGasLostFracElement = element.firstChildElement("ProcGasLostFrac");
      if (!procGasPwrDensElement.isNull() && (procGasPwrDensElement.text().toDouble() > 0)){

        openstudio::Quantity gasDensityIP(procGasPwrDensElement.text().toDouble(), openstudio::createUnit("Btu/h*ft^2").get());
        OptionalQuantity gasDensitySI = QuantityConverter::instance().convert(gasDensityIP, whSys);
        OS_ASSERT(gasDensitySI);
        OS_ASSERT(gasDensitySI->units() == WhUnit(WhExpnt(1,0,-2)));

        openstudio::model::GasEquipmentDefinition gasEquipmentDefinition(model);
        gasEquipmentDefinition.setName(name + " Gas Loads Definition");
        gasEquipmentDefinition.setWattsperSpaceFloorArea(gasDensitySI->value()); // W/m2

        if (!procGasRadFracElement.isNull()){
          gasEquipmentDefinition.setFractionRadiant(procGasRadFracElement.text().toDouble());
        }
        if (!procGasLatFracElement.isNull()){
          gasEquipmentDefinition.setFractionLatent(procGasLatFracElement.text().toDouble());
        }
        if (!procGasLostFracElement.isNull()){
          gasEquipmentDefinition.setFractionLost(procGasLostFracElement.text().toDouble());
        }

        openstudio::model::GasEquipment gasEquipment(gasEquipmentDefinition);
        gasEquipment.setName(name + " Gas Loads");
        gasEquipment.setSpace(space);
        gasEquipment.setEndUseSubcategory("Process");

        if (!procGasSchRefElement.isNull()){
          std::string scheduleName = escapeName(procGasSchRefElement.text());
          boost::optional<model::Schedule> schedule = model.getModelObjectByName<model::Schedule>(scheduleName);
          if (schedule){
            gasEquipment.setSchedule(*schedule);
          }else{
            LOG(Error, "Could not find schedule '" << scheduleName << "'");
          }
        }
      }
    }

    return space;
  }

  boost::optional<model::ModelObject> ReverseTranslator::translateSurface(const QDomElement& element, const QDomDocument& doc, openstudio::model::Space& space)
  {
    boost::optional<model::ModelObject> result;

    UnitSystem siSys(UnitSystem::SI);

    std::vector<openstudio::Point3d> vertices;

    QDomElement polyLoopElement = element.firstChildElement("PolyLp");
    OS_ASSERT(!polyLoopElement.isNull());

    QDomNodeList cartesianPointElements = polyLoopElement.elementsByTagName("CartesianPt");
    for (int i = 0; i < cartesianPointElements.count(); i++){
      QDomNodeList coordinateElements = cartesianPointElements.at(i).toElement().elementsByTagName("Coord");
      OS_ASSERT(coordinateElements.size() == 3);

      /* DLM: these unit conversions are taking about 75% of the total time to translate a large model

      // sdd units = ft, os units = m
      Quantity xIP(coordinateElements.at(0).toElement().text().toDouble(), IPUnit(IPExpnt(0,1,0)));
      Quantity yIP(coordinateElements.at(1).toElement().text().toDouble(), IPUnit(IPExpnt(0,1,0)));
      Quantity zIP(coordinateElements.at(2).toElement().text().toDouble(), IPUnit(IPExpnt(0,1,0)));

      OptionalQuantity xSI = QuantityConverter::instance().convert(xIP, siSys);
      OS_ASSERT(xSI);
      OS_ASSERT(xSI->units() == SIUnit(SIExpnt(0,1,0)));

      OptionalQuantity ySI = QuantityConverter::instance().convert(yIP, siSys);
      OS_ASSERT(ySI);
      OS_ASSERT(ySI->units() == SIUnit(SIExpnt(0,1,0)));

      OptionalQuantity zSI = QuantityConverter::instance().convert(zIP, siSys);
      OS_ASSERT(zSI);
      OS_ASSERT(zSI->units() == SIUnit(SIExpnt(0,1,0)));

      vertices.push_back(openstudio::Point3d(xSI->value(), ySI->value(), zSI->value()));
      */

      double x = footToMeter*coordinateElements.at(0).toElement().text().toDouble();
      double y = footToMeter*coordinateElements.at(1).toElement().text().toDouble();
      double z = footToMeter*coordinateElements.at(2).toElement().text().toDouble();
      vertices.push_back(openstudio::Point3d(x,y,z));

    }

    QDomElement nameElement = element.firstChildElement("Name");

    openstudio::model::Surface surface(vertices, space.model());
    OS_ASSERT(!nameElement.isNull());
    std::string name = escapeName(nameElement.text());
    surface.setName(name);
    surface.setSpace(space);
    result = surface;

    QDomElement constructionReferenceElement = element.firstChildElement("ConsAssmRef");
    if(!constructionReferenceElement.isNull()){
      std::string constructionName = escapeName(constructionReferenceElement.text());
      boost::optional<model::ConstructionBase> construction = space.model().getModelObjectByName<model::ConstructionBase>(constructionName);
      if(construction){
        surface.setConstruction(*construction);
      }else{
        LOG(Error, "Cannot find construction '" << constructionName << "'");
      }
    }

    QString tagName = element.tagName();
    if (tagName == "ExtWall"){
      surface.setSurfaceType("Wall");
      surface.setOutsideBoundaryCondition("Outdoors");
      surface.setSunExposure("SunExposed");
      surface.setWindExposure("WindExposed");
    }else if (tagName == "ExtFlr"){
      surface.setSurfaceType("Floor");
      surface.setOutsideBoundaryCondition("Outdoors");
      surface.setSunExposure("SunExposed");
      surface.setWindExposure("WindExposed");
    }else if (tagName == "Roof"){
      surface.setSurfaceType("RoofCeiling");
      surface.setOutsideBoundaryCondition("Outdoors");
      surface.setSunExposure("SunExposed");
      surface.setWindExposure("WindExposed");
    }else if (tagName.contains("UndgrFlr")){
      surface.setSurfaceType("Floor");
      surface.setOutsideBoundaryCondition("Ground");
      surface.setSunExposure("NoSun");
      surface.setWindExposure("NoWind");
    }else if (tagName.contains("UndgrWall")){
      surface.setSurfaceType("Wall");
      surface.setOutsideBoundaryCondition("Ground");
      surface.setSunExposure("NoSun");
      surface.setWindExposure("NoWind");
    }else if (tagName.contains("Ceiling")){
      surface.setSurfaceType("RoofCeiling");
      surface.setOutsideBoundaryCondition("Adiabatic");
      surface.setSunExposure("NoSun");
      surface.setWindExposure("NoWind");
    }else if (tagName.contains("IntWall")){
      surface.setSurfaceType("Wall");
      surface.setOutsideBoundaryCondition("Adiabatic");
      surface.setSunExposure("NoSun");
      surface.setWindExposure("NoWind");
    }else if (tagName.contains("IntFlr")){
      surface.setSurfaceType("Floor");
      surface.setOutsideBoundaryCondition("Adiabatic");
      surface.setSunExposure("NoSun");
      surface.setWindExposure("NoWind");
    }else{
      LOG(Error, "Unknown surface type '" << toString(tagName) << "'");
    }

    QDomElement perimExposedElement = element.firstChildElement("PerimExposed");
    if (!perimExposedElement.isNull()){

      double perimeterExposedIP = perimExposedElement.text().toDouble();
      double perimeterExposedSI = footToMeter*perimeterExposedIP;
    
      boost::optional<model::ConstructionBase> construction = surface.construction();
      if (construction && construction->optionalCast<model::FFactorGroundFloorConstruction>()){
        // clone the existing FFactor construction, set perimeter, and assign to this surface
        model::FFactorGroundFloorConstruction fFactorConstruction = construction->cast<model::FFactorGroundFloorConstruction>();
        model::Model model = fFactorConstruction.model();
        model::FFactorGroundFloorConstruction clone = fFactorConstruction.clone(model).cast<model::FFactorGroundFloorConstruction>();
        
        std::string cloneName = name + " " + fFactorConstruction.name().get();
        clone.setName(cloneName);
        clone.setPerimeterExposed(perimeterExposedSI);
        clone.setArea(surface.grossArea());
        surface.setConstruction(clone);
        surface.setOutsideBoundaryCondition("GroundFCfactorMethod");
      }else{
        LOG(Error, "Cannot set exposed perimeter for surface '" << name << "''s construction.");
      }
    }

    QDomElement heightElement = element.firstChildElement("Hgt");
    if (!heightElement.isNull()){

      double heightIP = heightElement.text().toDouble();
      double heightSI = footToMeter*heightIP;
    
      boost::optional<model::ConstructionBase> construction = surface.construction();
      if (construction && construction->optionalCast<model::CFactorUndergroundWallConstruction>()){
        // clone the existing CFactorUndergroundWallConstruction, set height, and assign to this surface
        model::CFactorUndergroundWallConstruction cFactorConstruction = construction->cast<model::CFactorUndergroundWallConstruction>();
        model::Model model = cFactorConstruction.model();
        model::CFactorUndergroundWallConstruction clone = cFactorConstruction.clone(model).cast<model::CFactorUndergroundWallConstruction>();
        
        std::string cloneName = name + " " + cFactorConstruction.name().get();
        clone.setName(cloneName);
        clone.setHeight(heightSI);
        surface.setConstruction(clone);
        surface.setOutsideBoundaryCondition("GroundFCfactorMethod");
      }else{
        LOG(Error, "Cannot set height for surface '" << name << "''s construction.");
      }
    }

    // translate subSurfaces
    QDomNodeList windowElements = element.elementsByTagName("Win");
    QDomNodeList doorElements = element.elementsByTagName("Dr");
    QDomNodeList skylightElements = element.elementsByTagName("Skylt");

    for (int i = 0; i < windowElements.count(); ++i){
      boost::optional<model::ModelObject> subSurface = translateSubSurface(windowElements.at(i).toElement(), doc, surface);
      OS_ASSERT(subSurface);
    }
    for (int i = 0; i < doorElements.count(); ++i){
      boost::optional<model::ModelObject> subSurface = translateSubSurface(doorElements.at(i).toElement(), doc, surface);
      OS_ASSERT(subSurface);
    }
    for (int i = 0; i < skylightElements.count(); ++i){
      boost::optional<model::ModelObject> subSurface = translateSubSurface(skylightElements.at(i).toElement(), doc, surface);
      OS_ASSERT(subSurface);
    }

    // check for adjacent surface
    QDomElement adjacentSpaceElement = element.firstChildElement("AdjacentSpcRef");
    if (!adjacentSpaceElement.isNull()){
      std::string adjacentSpaceName = escapeName(adjacentSpaceElement.text());
      boost::optional<model::Space> otherSpace = space.model().getModelObjectByName<model::Space>(adjacentSpaceName);
      OS_ASSERT(otherSpace); // what type of error handling do we want?

      // clone the surface and sub surfaces with reverse vertices
      boost::optional<model::Surface> otherSurface = surface.createAdjacentSurface(*otherSpace);
      OS_ASSERT(otherSurface); // what type of error handling do we want?
    }

    return result;
  }

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateSubSurface(const QDomElement& element, const QDomDocument& doc, openstudio::model::Surface& surface)
  {
    std::vector<openstudio::Point3d> vertices;

    UnitSystem siSys(UnitSystem::SI);

    QDomElement polyLoopElement = element.firstChildElement("PolyLp");
    OS_ASSERT(!polyLoopElement.isNull());

    QDomNodeList cartesianPointElements = polyLoopElement.elementsByTagName("CartesianPt");
    for (int i = 0; i < cartesianPointElements.count(); i++){
      QDomNodeList coordinateElements = cartesianPointElements.at(i).toElement().elementsByTagName("Coord");
      OS_ASSERT(coordinateElements.size() == 3);

      /* DLM: there conversions were taking about 75% of the time it takes to convert a large model

      // sdd units = ft, os units = m
      Quantity xIP(coordinateElements.at(0).toElement().text().toDouble(), IPUnit(IPExpnt(0,1,0)));
      Quantity yIP(coordinateElements.at(1).toElement().text().toDouble(), IPUnit(IPExpnt(0,1,0)));
      Quantity zIP(coordinateElements.at(2).toElement().text().toDouble(), IPUnit(IPExpnt(0,1,0)));

      OptionalQuantity xSI = QuantityConverter::instance().convert(xIP, siSys);
      OS_ASSERT(xSI);
      OS_ASSERT(xSI->units() == SIUnit(SIExpnt(0,1,0)));

      OptionalQuantity ySI = QuantityConverter::instance().convert(yIP, siSys);
      OS_ASSERT(ySI);
      OS_ASSERT(ySI->units() == SIUnit(SIExpnt(0,1,0)));

      OptionalQuantity zSI = QuantityConverter::instance().convert(zIP, siSys);
      OS_ASSERT(zSI);
      OS_ASSERT(zSI->units() == SIUnit(SIExpnt(0,1,0)));

      vertices.push_back(openstudio::Point3d(xSI->value(), ySI->value(), zSI->value()));
      */

      double x = footToMeter*coordinateElements.at(0).toElement().text().toDouble();
      double y = footToMeter*coordinateElements.at(1).toElement().text().toDouble();
      double z = footToMeter*coordinateElements.at(2).toElement().text().toDouble();
      vertices.push_back(openstudio::Point3d(x,y,z));
    }

    QDomElement nameElement = element.firstChildElement("Name");
    model::SubSurface subSurface(vertices, surface.model());
    subSurface.setSurface(surface);
    subSurface.setName(escapeName(nameElement.text()));

    QString tagName = element.tagName();
    if (tagName == "Win"){

      subSurface.setSubSurfaceType("FixedWindow");

      QDomElement constructionReferenceElement = element.firstChildElement("FenConsRef");
      if(!constructionReferenceElement.isNull()){
        std::string constructionName = escapeName(constructionReferenceElement.text());
        boost::optional<model::ConstructionBase> construction = surface.model().getModelObjectByName<model::ConstructionBase>(constructionName);
        if(construction){
          subSurface.setConstruction(*construction);
        }else{
          LOG(Error, "Cannot find construction '" << constructionName << "'");
        }
      }

    }else if (tagName == "Dr"){

      subSurface.setSubSurfaceType("Door");

      QDomElement constructionReferenceElement = element.firstChildElement("DrConsRef");
      if(!constructionReferenceElement.isNull()){
        std::string constructionName = escapeName(constructionReferenceElement.text());
        boost::optional<model::ConstructionBase> construction = surface.model().getModelObjectByName<model::ConstructionBase>(constructionName);
        if(construction){
          subSurface.setConstruction(*construction);
        }else{
          LOG(Error, "Cannot find construction '" << constructionName << "'");
        }
      }

    }else if (tagName == "Skylt"){

      subSurface.setSubSurfaceType("Skylight");

      QDomElement constructionReferenceElement = element.firstChildElement("FenConsRef");
      if(!constructionReferenceElement.isNull()){
        std::string constructionName = escapeName(constructionReferenceElement.text());
        boost::optional<model::ConstructionBase> construction = surface.model().getModelObjectByName<model::ConstructionBase>(constructionName);
        if(construction){
          subSurface.setConstruction(*construction);
        }else{
          LOG(Error, "Cannot find construction '" << constructionName << "'");
        }
      }

    }else{  
      LOG(Error, "Unknown subsurface type '" << toString(tagName) << "'");
    }

    // DLM: currently unhandled
    // InternalShadingDevice

    return subSurface;
  }

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateShadingSurface(const QDomElement& element, const QDomDocument& doc, openstudio::model::ShadingSurfaceGroup& shadingSurfaceGroup)
  {
    std::vector<openstudio::Point3d> vertices;

    UnitSystem siSys(UnitSystem::SI);

    QDomElement polyLoopElement = element.firstChildElement("PolyLp");
    OS_ASSERT(!polyLoopElement.isNull());

    QDomNodeList cartesianPointElements = polyLoopElement.elementsByTagName("CartesianPt");
    for (int i = 0; i < cartesianPointElements.count(); i++){
      QDomNodeList coordinateElements = cartesianPointElements.at(i).toElement().elementsByTagName("Coord");
      OS_ASSERT(coordinateElements.size() == 3);

      /* DLM: there conversions were taking about 75% of the time it takes to convert a large model

      // sdd units = ft, os units = m
      Quantity xIP(coordinateElements.at(0).toElement().text().toDouble(), IPUnit(IPExpnt(0,1,0)));
      Quantity yIP(coordinateElements.at(1).toElement().text().toDouble(), IPUnit(IPExpnt(0,1,0)));
      Quantity zIP(coordinateElements.at(2).toElement().text().toDouble(), IPUnit(IPExpnt(0,1,0)));

      OptionalQuantity xSI = QuantityConverter::instance().convert(xIP, siSys);
      OS_ASSERT(xSI);
      OS_ASSERT(xSI->units() == SIUnit(SIExpnt(0,1,0)));

      OptionalQuantity ySI = QuantityConverter::instance().convert(yIP, siSys);
      OS_ASSERT(ySI);
      OS_ASSERT(ySI->units() == SIUnit(SIExpnt(0,1,0)));

      OptionalQuantity zSI = QuantityConverter::instance().convert(zIP, siSys);
      OS_ASSERT(zSI);
      OS_ASSERT(zSI->units() == SIUnit(SIExpnt(0,1,0)));

      vertices.push_back(openstudio::Point3d(xSI->value(), ySI->value(), zSI->value()));
      */

      double x = footToMeter*coordinateElements.at(0).toElement().text().toDouble();
      double y = footToMeter*coordinateElements.at(1).toElement().text().toDouble();
      double z = footToMeter*coordinateElements.at(2).toElement().text().toDouble();
      vertices.push_back(openstudio::Point3d(x,y,z));
    }

    model::Model model = shadingSurfaceGroup.model();

    QDomElement nameElement = element.firstChildElement("Name");
    std::string name = escapeName(nameElement.text());

    model::ShadingSurface shadingSurface(vertices, model);
    shadingSurface.setShadingSurfaceGroup(shadingSurfaceGroup);
    shadingSurface.setName(name);

    QString tagName = element.tagName();
    if (tagName == "ExtShdgObj"){

      // default to 0 reflectance
      // http://code.google.com/p/cbecc/issues/detail?id=344#c16
      double solRefl = 0.0;
      QDomElement solReflElement = element.firstChildElement("SolRefl");
      if(!solReflElement.isNull()){
        solRefl = solReflElement.text().toDouble();
      }

      double visRefl = 0.0;
      QDomElement visReflElement = element.firstChildElement("VisRefl");
      if(!visReflElement.isNull()){
        visRefl = visReflElement.text().toDouble();
      }

      model::ConstructionBase construction = shadingConstruction(model, solRefl, visRefl);
      shadingSurface.setConstruction(construction);

      QDomElement scheduleReferenceElement = element.firstChildElement("TransSchRef");
      if(!scheduleReferenceElement.isNull()){
        std::string scheduleName = escapeName(scheduleReferenceElement.text());
        boost::optional<model::Schedule> schedule = model.getModelObjectByName<model::Schedule>(scheduleName);
        if(schedule){
          bool test = shadingSurface.setTransmittanceSchedule(*schedule);
          if (!test){
            LOG(Error, "Failed to assign schedule '" << scheduleName << "' to shading surface '" << name << "'");
          }
        }else{
          LOG(Error, "Cannot find schedule '" << scheduleName << "'");
        }
      }

    }else{  
      LOG(Error, "Unknown shading surface type '" << toString(tagName) << "'");
    }

    return shadingSurface;
  }

  model::ConstructionBase ReverseTranslator::shadingConstruction(openstudio::model::Model& model, double solRefl, double visRefl)
  {
    std::pair<double, double> key = std::make_pair(solRefl, visRefl);
    auto it = m_shadingConstructionMap.find(key);
    if (it != m_shadingConstructionMap.end()){
      return it->second;
    }

    std::string description = boost::lexical_cast<std::string>(solRefl) + "-" + boost::lexical_cast<std::string>(visRefl);
    std::string constructionName = "Shading Construction " + description;
    std::string materialName = "Shading Material " + description;

    // create a construction with these properties
    model::Construction construction(model);
    construction.setName(constructionName);

    model::MasslessOpaqueMaterial material(model);
    material.setName(materialName);

    bool test = material.setSolarAbsorptance(1.0-solRefl);
    if (!test){
      LOG(Error, "Failed to assign solar absorptance '" << 1.0-solRefl << "' to material '" << materialName << "'");
    }

    test = material.setVisibleAbsorptance(1.0-visRefl);
    if (!test){
      LOG(Error, "Failed to assign visible absorptance '" << 1.0-visRefl << "' to material '" << materialName << "'");
    }

    std::vector<model::Material> materials;
    materials.push_back(material);
    test = construction.setLayers(materials);
    OS_ASSERT(test); // what type of error handling do we want?

    m_shadingConstructionMap.insert(std::make_pair(key, construction));
    return construction;
  }

  boost::optional<QDomElement> ForwardTranslator::translateBuilding(const openstudio::model::Building& building, QDomDocument& doc)
  {
    QDomElement result = doc.createElement("Bldg");
    m_translatedObjects[building.handle()] = result;

    // name
    std::string name = building.name().get();
    QDomElement nameElement = doc.createElement("Name");
    result.appendChild(nameElement);
    nameElement.appendChild(doc.createTextNode(escapeName(name)));

    double buildingAzimuth = fixAngle(building.northAxis());
    double northAngle = 360.0 - buildingAzimuth;

    // north angle
    QDomElement northAngleElement = doc.createElement("NAng");
    result.appendChild(northAngleElement);
    northAngleElement.appendChild(doc.createTextNode(QString::number(northAngle)));

    // building azimuth
    QDomElement buildingAzimuthElement = doc.createElement("BldgAz");
    result.appendChild(buildingAzimuthElement);
    buildingAzimuthElement.appendChild(doc.createTextNode(QString::number(buildingAzimuth)));

    // translate storys
    std::vector<model::BuildingStory> buildingStories = building.model().getConcreteModelObjects<model::BuildingStory>();
    std::sort(buildingStories.begin(), buildingStories.end(), WorkspaceObjectNameLess());

    if (m_progressBar){
      m_progressBar->setWindowTitle(toString("Translating Building Stories"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum(buildingStories.size());
      m_progressBar->setValue(0);
    }

    // DLM: do not translate aboveGradeStoryCount, Issue 243: Forward Translator - AboveGrdStryCount
    /*
    // aboveGradeStoryCount
    unsigned numAboveGroundStories = 0;
    for (const model::BuildingStory& buildingStory : buildingStories){
      boost::optional<double> nominalZCoordinate = buildingStory.nominalZCoordinate();
      if (nominalZCoordinate && *nominalZCoordinate >= 0){
        numAboveGroundStories += 1;
      }
    }

    QDomElement aboveGradeStoryCountElement = doc.createElement("AboveGrdStoryCnt");
    result.appendChild(aboveGradeStoryCountElement);
    aboveGradeStoryCountElement.appendChild(doc.createTextNode(QString::number(numAboveGroundStories)));
    */

    // translate building shading
    std::vector<model::ShadingSurfaceGroup> shadingSurfaceGroups = building.model().getConcreteModelObjects<model::ShadingSurfaceGroup>();
    std::sort(shadingSurfaceGroups.begin(), shadingSurfaceGroups.end(), WorkspaceObjectNameLess());

    if (m_progressBar){
      m_progressBar->setWindowTitle(toString("Translating Building Shading"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum(shadingSurfaceGroups.size()); 
      m_progressBar->setValue(0);
    }

    for (const model::ShadingSurfaceGroup& shadingSurfaceGroup : shadingSurfaceGroups){
      if (istringEqual(shadingSurfaceGroup.shadingSurfaceType(), "Building")){

        Transformation transformation = shadingSurfaceGroup.siteTransformation();

        for (const model::ShadingSurface& shadingSurface : shadingSurfaceGroup.shadingSurfaces()){
          boost::optional<QDomElement> shadingSurfaceElement = translateShadingSurface(shadingSurface, transformation, doc);
          if (shadingSurfaceElement){
            result.appendChild(*shadingSurfaceElement);
          }
        }
      }

      if (m_progressBar){
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    // translate building story
    for (const model::BuildingStory& buildingStory : buildingStories){

      boost::optional<QDomElement> buildingStoryElement = translateBuildingStory(buildingStory, doc);
      if (buildingStoryElement){
        result.appendChild(*buildingStoryElement);
      }

      if (m_progressBar){
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    // issue warning if any spaces not assigned to building story
    std::vector<model::Space> spaces = building.model().getConcreteModelObjects<model::Space>();
    std::sort(spaces.begin(), spaces.end(), WorkspaceObjectNameLess());

    for (const model::Space& space : spaces){
      if (!space.buildingStory()){
        LOG(Warn, "Model contains spaces which are not assigned to a building story, these have not been translated.");
        break;
      }
    }

    // translate thermal zones
    std::vector<model::ThermalZone> thermalZones = building.model().getConcreteModelObjects<model::ThermalZone>();
    std::sort(thermalZones.begin(), thermalZones.end(), WorkspaceObjectNameLess());

    if (m_progressBar){
      m_progressBar->setWindowTitle(toString("Translating Thermal Zones"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum(thermalZones.size());
      m_progressBar->setValue(0);
    }

    for (const model::ThermalZone& thermalZone : thermalZones){

      boost::optional<QDomElement> thermalZoneElement = translateThermalZone(thermalZone, doc);
      if (thermalZoneElement){
        result.appendChild(*thermalZoneElement);
      }

      if (m_progressBar){
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    return result;
  }

  boost::optional<QDomElement> ForwardTranslator::translateBuildingStory(const openstudio::model::BuildingStory& buildingStory, QDomDocument& doc)
  {
    QDomElement result = doc.createElement("Story");
    m_translatedObjects[buildingStory.handle()] = result;

    // name
    std::string name = buildingStory.name().get();
    QDomElement nameElement = doc.createElement("Name");
    result.appendChild(nameElement);
    nameElement.appendChild(doc.createTextNode(escapeName(name)));

    // translate spaces
    std::vector<model::Space> spaces = buildingStory.spaces();
    std::sort(spaces.begin(), spaces.end(), WorkspaceObjectNameLess());

    for (const model::Space& space : spaces){
      boost::optional<QDomElement> spaceElement = translateSpace(space, doc);
      if (spaceElement){
        result.appendChild(*spaceElement);
      }
    }

    return result;
  }

  boost::optional<QDomElement> ForwardTranslator::translateSpace(const openstudio::model::Space& space, QDomDocument& doc)
  {
    UnitSystem ipSys(UnitSystem::IP);
    UnitSystem btuSys(UnitSystem::BTU);

    QDomElement result = doc.createElement("Spc");
    m_translatedObjects[space.handle()] = result;

    // name
    std::string name = space.name().get();
    QDomElement nameElement = doc.createElement("Name");
    result.appendChild(nameElement);
    nameElement.appendChild(doc.createTextNode(escapeName(name)));

    // volume
    double volume = space.volume();
    Quantity volumeSI(volume, SIUnit(SIExpnt(0,3,0)));
    OptionalQuantity volumeIP = QuantityConverter::instance().convert(volumeSI, ipSys);
    OS_ASSERT(volumeIP);
    OS_ASSERT(volumeIP->units() == IPUnit(IPExpnt(0,3,0)));
    QDomElement volumeElement = doc.createElement("Vol");
    result.appendChild(volumeElement);
    volumeElement.appendChild(doc.createTextNode(QString::number(volumeIP->value())));

    // floorArea
    double floorArea = space.floorArea();
    Quantity floorAreaSI(floorArea, SIUnit(SIExpnt(0,2,0)));
    OptionalQuantity floorAreaIP = QuantityConverter::instance().convert(floorAreaSI, ipSys);
    OS_ASSERT(floorAreaIP);
    OS_ASSERT(floorAreaIP->units() == IPUnit(IPExpnt(0,2,0)));
    QDomElement floorAreaElement = doc.createElement("Area");  // SAC 3/14/14
    result.appendChild(floorAreaElement);
    floorAreaElement.appendChild(doc.createTextNode(QString::number(floorAreaIP->value())));

    // translate floorPrint
    Transformation transformation = space.siteTransformation();
   
    Point3dVector vertices = transformation*space.floorPrint();
    QDomElement polyLoopElement = doc.createElement("PolyLp");
    result.appendChild(polyLoopElement);
    for (const Point3d& vertex : vertices){
      QDomElement cartesianPointElement = doc.createElement("CartesianPt");
      polyLoopElement.appendChild(cartesianPointElement);

      QDomElement coordinateXElement = doc.createElement("Coord");
      cartesianPointElement.appendChild(coordinateXElement);
      coordinateXElement.appendChild(doc.createTextNode(QString::number(meterToFoot*vertex.x())));

      QDomElement coordinateYElement = doc.createElement("Coord");
      cartesianPointElement.appendChild(coordinateYElement);
      coordinateYElement.appendChild(doc.createTextNode(QString::number(meterToFoot*vertex.y())));

      QDomElement coordinateZElement = doc.createElement("Coord");
      cartesianPointElement.appendChild(coordinateZElement);
      coordinateZElement.appendChild(doc.createTextNode(QString::number(meterToFoot*vertex.z())));
    }

    // thermal zone
    boost::optional<model::ThermalZone> thermalZone = space.thermalZone();
    if (thermalZone){
      std::string thermalZoneName = thermalZone->name().get();
      QDomElement thermalZoneElement = doc.createElement("ThrmlZnRef");
      result.appendChild(thermalZoneElement);
      thermalZoneElement.appendChild(doc.createTextNode(escapeName(thermalZoneName)));
    }

    // translate space shading
    std::vector<model::ShadingSurfaceGroup> shadingSurfaceGroups = space.shadingSurfaceGroups();
    std::sort(shadingSurfaceGroups.begin(), shadingSurfaceGroups.end(), WorkspaceObjectNameLess());

    for (const model::ShadingSurfaceGroup& shadingSurfaceGroup : shadingSurfaceGroups){
      Transformation shadingTransformation = shadingSurfaceGroup.siteTransformation();

      for (const model::ShadingSurface& shadingSurface : shadingSurfaceGroup.shadingSurfaces()){
        boost::optional<QDomElement> shadingSurfaceElement = translateShadingSurface(shadingSurface, shadingTransformation, doc);
        if (shadingSurfaceElement){
          result.appendChild(*shadingSurfaceElement);
        }
      }
    }

    // translate surfaces
    std::vector<model::Surface> surfaces = space.surfaces();
    std::sort(surfaces.begin(), surfaces.end(), WorkspaceObjectNameLess());

    for (const model::Surface& surface : surfaces){
      boost::optional<QDomElement> surfaceElement = translateSurface(surface, transformation, doc);
      if (surfaceElement){
        result.appendChild(*surfaceElement);
      }
    }

    return result;
  }

  boost::optional<QDomElement> ForwardTranslator::translateSurface(const openstudio::model::Surface& surface, const openstudio::Transformation& transformation, QDomDocument& doc)
  {
    UnitSystem ipSys(UnitSystem::IP);

    boost::optional<QDomElement> result;

    // return if already translated
    if (m_translatedObjects.find(surface.handle()) != m_translatedObjects.end()){
      return boost::none;
    }

    std::string surfaceType = surface.surfaceType();
    std::string outsideBoundaryCondition = surface.outsideBoundaryCondition();
    if (istringEqual("Wall", surfaceType)){
      if (istringEqual("Outdoors", outsideBoundaryCondition)){
        result = doc.createElement("ExtWall");
      }else if (surface.isGroundSurface()){
        result = doc.createElement("UndgrWall");
      }else if (istringEqual("Surface", outsideBoundaryCondition) || 
                istringEqual("Adiabatic", outsideBoundaryCondition)){
        result = doc.createElement("IntWall");
      }
    }else if (istringEqual("RoofCeiling", surfaceType)){
      if (istringEqual("Outdoors", outsideBoundaryCondition)){
        result = doc.createElement("Roof");
      }else if (surface.isGroundSurface()){
        // DLM: what to do here?
      }else if (istringEqual("Surface", outsideBoundaryCondition) || 
                istringEqual("Adiabatic", outsideBoundaryCondition)){
        // DLM: we are not translating interior ceiling surfaces, the paired interior floor will be written instead
        //result = doc.createElement("Ceiling"); 
        return boost::none;
      }
    }else if (istringEqual("Floor", surfaceType)){
      if (surface.isGroundSurface()){
        result = doc.createElement("UndgrFlr");
      }else if (istringEqual("Surface", outsideBoundaryCondition) || 
                istringEqual("Adiabatic", outsideBoundaryCondition)){
        result = doc.createElement("IntFlr"); 
      }else if (istringEqual("Outdoors", outsideBoundaryCondition)){
        result = doc.createElement("ExtFlr");
      }
    }

    if (!result){
      LOG(Error, "Cannot map surface '" << surface.name().get() << "' to a known surfaceType");
      return boost::none;
    }

    m_translatedObjects[surface.handle()] = *result;

    // name
    std::string name = surface.name().get();
    QDomElement nameElement = doc.createElement("Name");
    result->appendChild(nameElement);
    nameElement.appendChild(doc.createTextNode(escapeName(name)));

    // adjacent surface
    boost::optional<model::Surface> adjacentSurface = surface.adjacentSurface();
    if (adjacentSurface){
      boost::optional<model::Space> adjacentSpace = adjacentSurface->space();
      if (adjacentSpace){
        std::string adjacentSpaceName = adjacentSpace->name().get();
        QDomElement adjacentSpaceElement = doc.createElement("AdjacentSpcRef");
        result->appendChild(adjacentSpaceElement);
        adjacentSpaceElement.appendChild(doc.createTextNode(escapeName(adjacentSpaceName)));

        // count adjacent surface as translated
        m_translatedObjects[adjacentSurface->handle()] = *result;
      }
    }

    // construction reference
    boost::optional<model::ConstructionBase> construction = surface.construction();
    if (construction){

      std::string constructionName = construction->name().get();

      // check that construction has been translated
      if (m_translatedObjects.find(construction->handle()) != m_translatedObjects.end()){
        QDomElement constructionReferenceElement = doc.createElement("ConsAssmRef");
        result->appendChild(constructionReferenceElement);
        constructionReferenceElement.appendChild(doc.createTextNode(escapeName(constructionName)));
      }else{
        LOG(Error, "Surface '" << name << "' uses construction '" << constructionName << "' which has not been translated");
      }
    }

    // if underground floor try to write out perimeter exposed
    if (istringEqual("Floor", surfaceType) && surface.isGroundSurface()){
      // DLM: for now we will get the exposed perimeter from the FFactor construction
      // this assumes one construction per surface, I don't really like this, maybe we can do better later 
      if (construction && construction->optionalCast<model::FFactorGroundFloorConstruction>()){
        model::FFactorGroundFloorConstruction fFactorConstruction = construction->cast<model::FFactorGroundFloorConstruction>();
        // check assumption of one surface per FFactor construction
        if (fFactorConstruction.getModelObjectSources<model::Surface>().size() == 1){
          double perimeterExposedSI = fFactorConstruction.perimeterExposed();
          double perimeterExposedIP = meterToFoot*perimeterExposedSI;
          QDomElement perimExposedElement = doc.createElement("PerimExposed");
          result->appendChild(perimExposedElement);
          perimExposedElement.appendChild(doc.createTextNode(QString::number(perimeterExposedIP)));
        }else{
          LOG(Error, "Cannot compute exposed perimeter for surface '" << name << "'.");
        }
      }else{
        LOG(Error, "Cannot compute exposed perimeter for surface '" << name << "'.");
      }
    }else{
      LOG(Error, "Cannot compute exposed perimeter for surface '" << name << "'.");
    }

    // if underground wall try to write out height
    if (istringEqual("Wall", surfaceType) && surface.isGroundSurface()){
      // DLM: for now we will get the height from the CFactor construction
      // this assumes one construction per surface, I don't really like this, maybe we can do better later 
      if (construction && construction->optionalCast<model::CFactorUndergroundWallConstruction>()){
        model::CFactorUndergroundWallConstruction cFactorConstruction = construction->cast<model::CFactorUndergroundWallConstruction>();
        // check assumption of one surface per CFactor construction
        if (cFactorConstruction.getModelObjectSources<model::Surface>().size() == 1){
          double heightSI = cFactorConstruction.height();
          double heightIP = meterToFoot*heightSI;
          QDomElement heightElement = doc.createElement("Hgt");
          result->appendChild(heightElement);
          heightElement.appendChild(doc.createTextNode(QString::number(heightIP)));
        }else{
          LOG(Error, "Cannot compute height for surface '" << name << "'.");
        }
      }else{
        LOG(Error, "Cannot compute height for surface '" << name << "'.");
      }
    }else{
      LOG(Error, "Cannot compute height for surface '" << name << "'.");
    }

    // translate vertices
    Point3dVector vertices = transformation*surface.vertices();
    QDomElement polyLoopElement = doc.createElement("PolyLp");
    result->appendChild(polyLoopElement);
    for (const Point3d& vertex : vertices){
      QDomElement cartesianPointElement = doc.createElement("CartesianPt");
      polyLoopElement.appendChild(cartesianPointElement);

      /* DLM: these conversions were taking about 75% of the time to convert a large model

      Quantity xSI(vertex.x(), SIUnit(SIExpnt(0,1,0)));
      Quantity ySI(vertex.y(), SIUnit(SIExpnt(0,1,0)));
      Quantity zSI(vertex.z(), SIUnit(SIExpnt(0,1,0)));

      OptionalQuantity xIP = QuantityConverter::instance().convert(xSI, ipSys);
      OS_ASSERT(xIP);
      OS_ASSERT(xIP->units() == IPUnit(IPExpnt(0,1,0)));

      OptionalQuantity yIP = QuantityConverter::instance().convert(ySI, ipSys);
      OS_ASSERT(yIP);
      OS_ASSERT(yIP->units() == IPUnit(IPExpnt(0,1,0)));

      OptionalQuantity zIP = QuantityConverter::instance().convert(zSI, ipSys);
      OS_ASSERT(zIP);
      OS_ASSERT(zIP->units() == IPUnit(IPExpnt(0,1,0)));

      QDomElement coordinateXElement = doc.createElement("Coord");
      cartesianPointElement.appendChild(coordinateXElement);
      coordinateXElement.appendChild(doc.createTextNode(QString::number(xIP->value())));

      QDomElement coordinateYElement = doc.createElement("Coord");
      cartesianPointElement.appendChild(coordinateYElement);
      coordinateYElement.appendChild(doc.createTextNode(QString::number(yIP->value())));

      QDomElement coordinateZElement = doc.createElement("Coord");
      cartesianPointElement.appendChild(coordinateZElement);
      coordinateZElement.appendChild(doc.createTextNode(QString::number(zIP->value())));
      */

      QDomElement coordinateXElement = doc.createElement("Coord");
      cartesianPointElement.appendChild(coordinateXElement);
      coordinateXElement.appendChild(doc.createTextNode(QString::number(meterToFoot*vertex.x())));

      QDomElement coordinateYElement = doc.createElement("Coord");
      cartesianPointElement.appendChild(coordinateYElement);
      coordinateYElement.appendChild(doc.createTextNode(QString::number(meterToFoot*vertex.y())));

      QDomElement coordinateZElement = doc.createElement("Coord");
      cartesianPointElement.appendChild(coordinateZElement);
      coordinateZElement.appendChild(doc.createTextNode(QString::number(meterToFoot*vertex.z())));
    }

    // translate sub surfaces
    std::vector<model::SubSurface> subSurfaces = surface.subSurfaces();
    std::sort(subSurfaces.begin(), subSurfaces.end(), WorkspaceObjectNameLess());

    for (const model::SubSurface& subSurface : subSurfaces){
      boost::optional<QDomElement> subSurfaceElement = translateSubSurface(subSurface, transformation, doc);
      if (subSurfaceElement){
        result->appendChild(*subSurfaceElement);
      }
    }

    return result;
  }

  boost::optional<QDomElement> ForwardTranslator::translateSubSurface(const openstudio::model::SubSurface& subSurface, const openstudio::Transformation& transformation, QDomDocument& doc)
  {
    UnitSystem ipSys(UnitSystem::IP);

    boost::optional<QDomElement> result;

    // return if already translated
    if (m_translatedObjects.find(subSurface.handle()) != m_translatedObjects.end()){
      return boost::none;
    }

    std::string subSurfaceType = subSurface.subSurfaceType();
    QString consRefElementName;
    if (istringEqual("FixedWindow", subSurfaceType) || 
        istringEqual("OperableWindow", subSurfaceType) ||
        istringEqual("GlassDoor", subSurfaceType)){
      consRefElementName = "FenConsRef";
      result = doc.createElement("Win");
    }else if (istringEqual("Door", subSurfaceType) || 
              istringEqual("OverheadDoor", subSurfaceType)){
      consRefElementName = "DrConsRef";
      result = doc.createElement("Dr");
    }else if (istringEqual("Skylight", subSurfaceType)){
      consRefElementName = "FenConsRef";
      result = doc.createElement("Skylt");
    }

    if (!result){
      LOG(Error, "Cannot map subsurface '" << subSurface.name().get() << "' to a known subsurfaceType");
      return boost::none;
    }

    m_translatedObjects[subSurface.handle()] = *result;

    // name
    std::string name = subSurface.name().get();
    QDomElement nameElement = doc.createElement("Name");
    result->appendChild(nameElement);
    nameElement.appendChild(doc.createTextNode(escapeName(name)));

    // construction
    boost::optional<model::ConstructionBase> construction = subSurface.construction();
    if (construction){

      std::string constructionName = construction->name().get();

      // check that construction has been translated
      if (m_translatedObjects.find(construction->handle()) != m_translatedObjects.end()){
        QDomElement constructionReferenceElement = doc.createElement(consRefElementName);
        result->appendChild(constructionReferenceElement);
        constructionReferenceElement.appendChild(doc.createTextNode(escapeName(constructionName)));
      }else{
        LOG(Error, "SubSurface '" << name << "' uses construction '" << constructionName << "' which has not been translated");
      }
    }

    // translate vertices
    Point3dVector vertices = transformation*subSurface.vertices();
    QDomElement polyLoopElement = doc.createElement("PolyLp");
    result->appendChild(polyLoopElement);
    for (const Point3d& vertex : vertices){
      QDomElement cartesianPointElement = doc.createElement("CartesianPt");
      polyLoopElement.appendChild(cartesianPointElement);

      /* DLM: these conversions were taking about 75% of the time it takes to convert a large model

      Quantity xSI(vertex.x(), SIUnit(SIExpnt(0,1,0)));
      Quantity ySI(vertex.y(), SIUnit(SIExpnt(0,1,0)));
      Quantity zSI(vertex.z(), SIUnit(SIExpnt(0,1,0)));

      OptionalQuantity xIP = QuantityConverter::instance().convert(xSI, ipSys);
      OS_ASSERT(xIP);
      OS_ASSERT(xIP->units() == IPUnit(IPExpnt(0,1,0)));

      OptionalQuantity yIP = QuantityConverter::instance().convert(ySI, ipSys);
      OS_ASSERT(yIP);
      OS_ASSERT(yIP->units() == IPUnit(IPExpnt(0,1,0)));

      OptionalQuantity zIP = QuantityConverter::instance().convert(zSI, ipSys);
      OS_ASSERT(zIP);
      OS_ASSERT(zIP->units() == IPUnit(IPExpnt(0,1,0)));

      QDomElement coordinateXElement = doc.createElement("Coord");
      cartesianPointElement.appendChild(coordinateXElement);
      coordinateXElement.appendChild(doc.createTextNode(QString::number(xIP->value())));

      QDomElement coordinateYElement = doc.createElement("Coord");
      cartesianPointElement.appendChild(coordinateYElement);
      coordinateYElement.appendChild(doc.createTextNode(QString::number(yIP->value())));

      QDomElement coordinateZElement = doc.createElement("Coord");
      cartesianPointElement.appendChild(coordinateZElement);
      coordinateZElement.appendChild(doc.createTextNode(QString::number(zIP->value())));
      */

      QDomElement coordinateXElement = doc.createElement("Coord");
      cartesianPointElement.appendChild(coordinateXElement);
      coordinateXElement.appendChild(doc.createTextNode(QString::number(meterToFoot*vertex.x())));

      QDomElement coordinateYElement = doc.createElement("Coord");
      cartesianPointElement.appendChild(coordinateYElement);
      coordinateYElement.appendChild(doc.createTextNode(QString::number(meterToFoot*vertex.y())));

      QDomElement coordinateZElement = doc.createElement("Coord");
      cartesianPointElement.appendChild(coordinateZElement);
      coordinateZElement.appendChild(doc.createTextNode(QString::number(meterToFoot*vertex.z())));
    }

    return result;
  }

  boost::optional<QDomElement> ForwardTranslator::translateShadingSurface(const openstudio::model::ShadingSurface& shadingSurface, const openstudio::Transformation& transformation, QDomDocument& doc)
  {
    UnitSystem ipSys(UnitSystem::IP);

    boost::optional<QDomElement> result;

    // return if already translated
    if (m_translatedObjects.find(shadingSurface.handle()) != m_translatedObjects.end()){
      return boost::none;
    }

    result = doc.createElement("ExtShdgObj");
    m_translatedObjects[shadingSurface.handle()] = *result;

    // name
    std::string name = shadingSurface.name().get();
    QDomElement nameElement = doc.createElement("Name");
    result->appendChild(nameElement);
    nameElement.appendChild(doc.createTextNode(escapeName(name)));

    // schedule
    boost::optional<model::Schedule> transmittanceSchedule = shadingSurface.transmittanceSchedule();
    if (transmittanceSchedule){

      std::string transmittanceScheduleName = transmittanceSchedule->name().get();

      // check that construction has been translated
      if (m_translatedObjects.find(transmittanceSchedule->handle()) != m_translatedObjects.end()){
        QDomElement transmittanceScheduleReferenceElement = doc.createElement("TransSchRef");
        result->appendChild(transmittanceScheduleReferenceElement);
        transmittanceScheduleReferenceElement.appendChild(doc.createTextNode(escapeName(transmittanceScheduleName)));
      }else{
        LOG(Error, "ShadingSurface '" << name << "' uses transmittance schedule '" << transmittanceScheduleName << "' which has not been translated");
      }
    }

    // default to 0 reflectance
    // http://code.google.com/p/cbecc/issues/detail?id=344#c16
    double solRefl = 0.0;
    double visRefl = 0.0;

    boost::optional<model::ConstructionBase> constructionBase = shadingSurface.construction();
    if (constructionBase){
      boost::optional<model::Construction> construction = constructionBase->optionalCast<model::Construction>();
      if (construction){

        std::vector<model::Material> layers = construction->layers();
        if (!layers.empty()){
          
          if (layers[0].optionalCast<model::StandardOpaqueMaterial>()){
            model::StandardOpaqueMaterial outerMaterial = layers[0].cast<model::StandardOpaqueMaterial>();
            if (!outerMaterial.isSolarAbsorptanceDefaulted()){
              boost::optional<double> test = outerMaterial.solarReflectance();
              if (test){
                solRefl = *test;
              }
            }
            if (!outerMaterial.isVisibleAbsorptanceDefaulted()){
              boost::optional<double> test = outerMaterial.visibleReflectance();
              if (test){
                visRefl = *test;
              }
            }
          }

          if (layers[0].optionalCast<model::MasslessOpaqueMaterial>()){
            model::MasslessOpaqueMaterial outerMaterial = layers[0].cast<model::MasslessOpaqueMaterial>();
            if (!outerMaterial.isSolarAbsorptanceDefaulted()){
              boost::optional<double> test = outerMaterial.solarReflectance();
              if (test){
                solRefl = *test;
              }
            }
            if (!outerMaterial.isVisibleAbsorptanceDefaulted()){
              boost::optional<double> test = outerMaterial.visibleReflectance();
              if (test){
                visRefl = *test;
              }
            }
          }
        }
      }
    }

    QDomElement solReflElement = doc.createElement("SolRefl");
    result->appendChild(solReflElement);
    solReflElement.appendChild(doc.createTextNode(QString::number(solRefl)));

    QDomElement visReflElement = doc.createElement("VisRefl");
    result->appendChild(visReflElement);
    visReflElement.appendChild(doc.createTextNode(QString::number(visRefl)));

    // translate vertices
    Point3dVector vertices = transformation*shadingSurface.vertices();
    QDomElement polyLoopElement = doc.createElement("PolyLp");
    result->appendChild(polyLoopElement);
    for (const Point3d& vertex : vertices){
      QDomElement cartesianPointElement = doc.createElement("CartesianPt");
      polyLoopElement.appendChild(cartesianPointElement);

      /* DLM: these conversions were taking about 75% of the time it takes to convert a large model

      Quantity xSI(vertex.x(), SIUnit(SIExpnt(0,1,0)));
      Quantity ySI(vertex.y(), SIUnit(SIExpnt(0,1,0)));
      Quantity zSI(vertex.z(), SIUnit(SIExpnt(0,1,0)));

      OptionalQuantity xIP = QuantityConverter::instance().convert(xSI, ipSys);
      OS_ASSERT(xIP);
      OS_ASSERT(xIP->units() == IPUnit(IPExpnt(0,1,0)));

      OptionalQuantity yIP = QuantityConverter::instance().convert(ySI, ipSys);
      OS_ASSERT(yIP);
      OS_ASSERT(yIP->units() == IPUnit(IPExpnt(0,1,0)));

      OptionalQuantity zIP = QuantityConverter::instance().convert(zSI, ipSys);
      OS_ASSERT(zIP);
      OS_ASSERT(zIP->units() == IPUnit(IPExpnt(0,1,0)));

      QDomElement coordinateXElement = doc.createElement("Coord");
      cartesianPointElement.appendChild(coordinateXElement);
      coordinateXElement.appendChild(doc.createTextNode(QString::number(xIP->value())));

      QDomElement coordinateYElement = doc.createElement("Coord");
      cartesianPointElement.appendChild(coordinateYElement);
      coordinateYElement.appendChild(doc.createTextNode(QString::number(yIP->value())));

      QDomElement coordinateZElement = doc.createElement("Coord");
      cartesianPointElement.appendChild(coordinateZElement);
      coordinateZElement.appendChild(doc.createTextNode(QString::number(zIP->value())));
      */

      QDomElement coordinateXElement = doc.createElement("Coord");
      cartesianPointElement.appendChild(coordinateXElement);
      coordinateXElement.appendChild(doc.createTextNode(QString::number(meterToFoot*vertex.x())));

      QDomElement coordinateYElement = doc.createElement("Coord");
      cartesianPointElement.appendChild(coordinateYElement);
      coordinateYElement.appendChild(doc.createTextNode(QString::number(meterToFoot*vertex.y())));

      QDomElement coordinateZElement = doc.createElement("Coord");
      cartesianPointElement.appendChild(coordinateZElement);
      coordinateZElement.appendChild(doc.createTextNode(QString::number(meterToFoot*vertex.z())));
    }

    return result;
  }

  boost::optional<QDomElement> ForwardTranslator::translateThermalZone(const openstudio::model::ThermalZone& thermalZone, QDomDocument& doc)
  {
    QDomElement result = doc.createElement("ThrmlZn");
    m_translatedObjects[thermalZone.handle()] = result;

    // Name
    std::string name = thermalZone.name().get();
    QDomElement nameElement = doc.createElement("Name");
    result.appendChild(nameElement);
    nameElement.appendChild(doc.createTextNode(escapeName(name)));

    // Type
    std::string type; // Conditioned, Unconditioned, Plenum
    if(thermalZone.thermostatSetpointDualSetpoint()){
      type = "Conditioned";
    }else {
      type = "Unconditioned";
    }
    QDomElement typeElement = doc.createElement("Type");
    result.appendChild(typeElement);
    typeElement.appendChild(doc.createTextNode(toQString(type)));


    // Mult
    QDomElement multElement = doc.createElement("Mult");
    result.appendChild(multElement);
    multElement.appendChild(doc.createTextNode(QString::number(thermalZone.multiplier())));

    return result;
  }


} // sdd
} // openstudio
