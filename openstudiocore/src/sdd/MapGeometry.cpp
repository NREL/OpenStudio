/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../model/ScheduleDay.hpp"
#include "../model/ScheduleDay_Impl.hpp"
#include "../model/ScheduleConstant.hpp"
#include "../model/ScheduleConstant_Impl.hpp"
#include "../model/ScheduleRuleset.hpp"
#include "../model/ScheduleRuleset_Impl.hpp"
#include "../model/ScheduleTypeLimits.hpp"
#include "../model/ScheduleTypeLimits_Impl.hpp"
#include "../model/SurfacePropertyConvectionCoefficients.hpp"
#include "../model/SurfacePropertyConvectionCoefficients_Impl.hpp"
#include "../model/PlantLoop.hpp"
#include "../model/PlantLoop_Impl.hpp"
#include "../model/WaterUseConnections.hpp"
#include "../model/WaterUseConnections_Impl.hpp"
#include "../model/WaterUseEquipment.hpp"
#include "../model/WaterUseEquipment_Impl.hpp"
#include "../model/WaterUseEquipmentDefinition.hpp"
#include "../model/WaterUseEquipmentDefinition_Impl.hpp"
#include "../model/ThermostatSetpointDualSetpoint.hpp"
#include "../model/AirLoopHVAC.hpp"
#include "../model/AirLoopHVAC_Impl.hpp"

#include "../utilities/geometry/Transformation.hpp"
#include "../utilities/geometry/Geometry.hpp"
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

    if (nameElement.isNull()){
      LOG(Error, "Bldg element 'Name' is empty.")
    } else {
      building.setName(escapeName(nameElement.text()));
    }

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
        if (!exteriorShading){
          LOG(Error, "Failed to translate 'ExtShdgObj' element " << i);
        }
      }
    }

    // create all spaces
    for (int i = 0; i < spaceElements.count(); i++){
      QDomElement spaceElement = spaceElements.at(i).toElement();
      boost::optional<model::ModelObject> space = createSpace(spaceElement, doc, model);
      if (!space){
        LOG(Error, "Failed to translate 'Spc' element " << i);
      }
    }

    // create all thermal zones
    for (int i = 0; i < thermalZoneElements.count(); i++){

      if (thermalZoneElements.at(i).firstChildElement("Name").isNull()){
        LOG(Error, "ThrmlZn element 'Name' is empty, object will not be translated.")
        continue;
      }

      QDomElement thermalZoneElement = thermalZoneElements.at(i).toElement();

      boost::optional<model::ModelObject> thermalZone = createThermalZone(thermalZoneElement, doc, model);
      if (!thermalZone){
        LOG(Error, "Failed to translate 'ThrmlZn' element " << i);
      }
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
      if (!buildingStory){
        LOG(Error, "Failed to translate 'Story' element " << i);
      }

      if (m_progressBar){
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    // remove unused CFactor constructions
    for (model::CFactorUndergroundWallConstruction cFactorConstruction : model.getConcreteModelObjects<model::CFactorUndergroundWallConstruction>()){
      if (cFactorConstruction.directUseCount(true) == 0){
        cFactorConstruction.remove();
      }
    }

    // remove unused FFactor constructions
    for (model::FFactorGroundFloorConstruction fFactorConstruction : model.getConcreteModelObjects<model::FFactorGroundFloorConstruction>()){
      if (fFactorConstruction.directUseCount(true) == 0){
        fFactorConstruction.remove();
      }
    }

    return building;
  }

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::createThermalZone(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model)
  {
    QDomElement nameElement = element.firstChildElement("Name");

    model::ThermalZone thermalZone(model);

    if (nameElement.isNull()){
      LOG(Error, "ThrmlZn element 'Name' is empty.");
    } else{
      thermalZone.setName(escapeName(nameElement.text()));
    }

    return thermalZone;
  }

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateBuildingStory(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model)
  {
    QDomElement nameElement = element.firstChildElement("Name");
    QDomNodeList spaceElements = element.elementsByTagName("Spc");

    model::BuildingStory buildingStory(model);

    std::string name;
    if (nameElement.isNull()){
      LOG(Error, "Story element 'Name' is empty.");
    } else{
      name = escapeName(nameElement.text());
    }
    buildingStory.setName(name);

    for (int i = 0; i < spaceElements.count(); i++){
      QDomElement spaceElement = spaceElements.at(i).toElement();
      boost::optional<model::ModelObject> space = translateSpace(spaceElement, doc, buildingStory);
      if (!space){
        LOG(Error, "Failed to translate 'Spc' element " << i << " under Story '" << name << "'");
      }
    }

    return buildingStory;
  }

  boost::optional<model::ModelObject> ReverseTranslator::createSpace(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model)
  {
    QDomElement nameElement = element.firstChildElement("Name");

    model::Space space(model);

    if (nameElement.isNull()){
      LOG(Error, "Spc element 'Name' is empty.")
    } else{
      space.setName(escapeName(nameElement.text()));
    }

    return space;
  }

  boost::optional<model::ModelObject> ReverseTranslator::translateSpace(const QDomElement& element, const QDomDocument& doc, openstudio::model::BuildingStory& buildingStory)
  {
    QDomElement nameElement = element.firstChildElement("Name");
    QDomElement hotWtrHtgRtElement = element.firstChildElement("HotWtrHtgRtSim");
    QDomElement hotWtrHtgSchRefElement = element.firstChildElement("HotWtrHtgSchRef");
    QDomElement shwFluidSegRefElement = element.firstChildElement("SHWFluidSegRef");
    QDomElement hotWtrSupTempElement = element.firstChildElement("HotWtrSupTemp");
    QDomNodeList exteriorWallElements = element.elementsByTagName("ExtWall");
    QDomNodeList exteriorFloorElements = element.elementsByTagName("ExtFlr");
    QDomNodeList roofElements = element.elementsByTagName("Roof");
    QDomNodeList undergroundFloorElements = element.elementsByTagName("UndgrFlr");
    QDomNodeList undergroundWallElements = element.elementsByTagName("UndgrWall");
    QDomNodeList ceilingElements = element.elementsByTagName("Ceiling");
    QDomNodeList interiorWallElements = element.elementsByTagName("IntWall");
    QDomNodeList interiorFloorElements = element.elementsByTagName("IntFlr");

    std::string spaceName;
    if (nameElement.isNull()){
      LOG(Error, "Spc element 'Name' is empty.");
    } else{
      spaceName = escapeName(nameElement.text());
    }

    boost::optional<model::Space> space = buildingStory.model().getModelObjectByName<model::Space>(spaceName);
    if (!space){
      LOG(Error, "Could not retrieve Space named '" << spaceName << "'.");
      return boost::none;
    }

    space->setBuildingStory(buildingStory);

    QDomElement thermalZoneElement = element.firstChildElement("ThrmlZnRef");
    std::string thermalZoneName;
    if (thermalZoneElement.isNull()){
      LOG(Error, "Spc element 'ThrmlZnRef' is empty for Space named '" << spaceName << "'.");
    } else{
      thermalZoneName = escapeName(thermalZoneElement.text());
    }

    boost::optional<model::ThermalZone> thermalZone = space->model().getModelObjectByName<model::ThermalZone>(thermalZoneName);
    if (thermalZone){
      space->setThermalZone(*thermalZone);
    } else{
      LOG(Error, "Could not retrieve ThermalZone named '" << thermalZoneName << "'.");
      LOG(Error, "ThermalZone not set for Space named '" << spaceName << "'.");
    }

    translateLoads(element, doc, *space);

    for (int i = 0; i < exteriorWallElements.count(); i++){
      QDomElement exteriorWallElement = exteriorWallElements.at(i).toElement();
      boost::optional<model::ModelObject> surface = translateSurface(exteriorWallElement, doc, *space);
      if (!surface){
        LOG(Error, "Failed to translate 'ExtWall' element " << i << " for Space named '" << spaceName << "'.");
      }
    }

    for (int i = 0; i < exteriorFloorElements.count(); i++){
      QDomElement exteriorFloorElement = exteriorFloorElements.at(i).toElement();
      boost::optional<model::ModelObject> surface = translateSurface(exteriorFloorElement, doc, *space);
      if (!surface){
        LOG(Error, "Failed to translate 'ExtFlr' element " << i << " for Space named '" << spaceName << "'.");
      }
    }

    for (int i = 0; i < roofElements.count(); i++){
      QDomElement roofElement = roofElements.at(i).toElement();
      boost::optional<model::ModelObject> surface = translateSurface(roofElement, doc, *space);
      if (!surface){
        LOG(Error, "Failed to translate 'Roof' element " << i << " for Space named '" << spaceName << "'.");
      }
    }

    for (int i = 0; i < undergroundFloorElements.count(); i++){
      QDomElement undergroundFloorElement = undergroundFloorElements.at(i).toElement();
      boost::optional<model::ModelObject> surface = translateSurface(undergroundFloorElement, doc, *space);
      if (!surface){
        LOG(Error, "Failed to translate 'UndgrFlr' element " << i << " for Space named '" << spaceName << "'.");
      }
    }

    for (int i = 0; i < undergroundWallElements.count(); i++){
      QDomElement undergroundWallElement = undergroundWallElements.at(i).toElement();
      boost::optional<model::ModelObject> surface = translateSurface(undergroundWallElement, doc, *space);
      if (!surface){
        LOG(Error, "Failed to translate 'UndgrWall' element " << i << " for Space named '" << spaceName << "'.");
      }
    }

    for (int i = 0; i < ceilingElements.count(); i++){
      QDomElement ceilingElement = ceilingElements.at(i).toElement();
      boost::optional<model::ModelObject> surface = translateSurface(ceilingElement, doc, *space);
      if (!surface){
        LOG(Error, "Failed to translate 'Ceiling' element " << i << " for Space named '" << spaceName << "'.");
      }
    }

    for (int i = 0; i < interiorWallElements.count(); i++){
      QDomElement interiorWallElement = interiorWallElements.at(i).toElement();
      boost::optional<model::ModelObject> surface = translateSurface(interiorWallElement, doc, *space);
      if (!surface){
        LOG(Error, "Failed to translate 'IntWall' element " << i << " for Space named '" << spaceName << "'.");
      }
    }

    for (int i = 0; i < interiorFloorElements.count(); i++){
      QDomElement interiorFloorElement = interiorFloorElements.at(i).toElement();
      boost::optional<model::ModelObject> surface = translateSurface(interiorFloorElement, doc, *space);
      if (!surface){
        LOG(Error, "Failed to translate 'IntFlr' element " << i << " for Space named '" << spaceName << "'.");
      }
    }

    // translate shadingSurfaces
    QDomNodeList exteriorShadingElements = element.elementsByTagName("ExtShdgObj");
    model::ShadingSurfaceGroup shadingSurfaceGroup(space->model());
    shadingSurfaceGroup.setName(spaceName + " ShadingGroup");
    shadingSurfaceGroup.setSpace(*space);
    for (int i = 0; i < exteriorShadingElements.count(); ++i){
      if (exteriorShadingElements.at(i).parentNode() == element){
        boost::optional<model::ModelObject> exteriorShading = translateShadingSurface(exteriorShadingElements.at(i).toElement(), doc, shadingSurfaceGroup);
        if (!exteriorShading){
          LOG(Error, "Failed to translate 'ExtShdgObj' element " << i << " for Space named '" << spaceName << "'.");
        }
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

      value = hotWtrSupTempElement.text().toDouble(&ok);
      if( ok ) {
        value = unitToUnit(value,"F","C").get();
        model::ScheduleRuleset schedule(model);
        schedule.setName(spaceName + " Target SHW Temp");
        auto scheduleDay = schedule.defaultDaySchedule();
        scheduleDay.addValue(Time(1.0),value);
        definition.setTargetTemperatureSchedule(schedule);
      }

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

      QDomElement occDensElement = element.firstChildElement("OccDensSim");
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
          openstudio::model::ScheduleRuleset activitySchedule(model, totalHeatRateSI);
          activitySchedule.setName(name + " People Activity Level");

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

      //InfMthd = {AirChangesPerHour, FlowArea, FlowExteriorArea, FlowExteriorWallArea, FlowSpace}
      QDomNodeList infMthdNodes = element.elementsByTagName("InfMthd");
      for (int i = 0; i < infMthdNodes.count(); i++){

        QDomElement infMthdElement = infMthdNodes.at(i).toElement();

        bool hasIndex;
        int infIndex = infMthdElement.attribute("index").toInt(&hasIndex);

        QDomElement dsgnInfRtElement = elementByTagNameAndIndex(element,"DsgnInfRt",hasIndex,infIndex);

        if ((!infMthdElement.isNull()) && (!dsgnInfRtElement.isNull())){


          if ((infMthdElement.text() == "AirChangesPerHour") ||
              (infMthdElement.text() == "FlowArea") ||
              (infMthdElement.text() == "FlowExteriorArea") ||
              (infMthdElement.text() == "FlowExteriorWallArea") ||
              (infMthdElement.text() == "FlowSpace")){

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

            double dsnInfRt = dsgnInfRtElement.text().toDouble();

            openstudio::Quantity dsgnInfRtIP(dsnInfRt, openstudio::createUnit("cfm", UnitSystem::BTU).get());
            OptionalQuantity dsgnInfRtSI = QuantityConverter::instance().convert(dsgnInfRtIP, siSys);
            OS_ASSERT(dsgnInfRtSI);
            OS_ASSERT(dsgnInfRtSI->units() == SIUnit(SIExpnt(0, 3, -1)));

            openstudio::Quantity dsgnInfRtAreaIP(dsnInfRt, openstudio::createUnit("cfm/ft^2", UnitSystem::BTU).get());
            OptionalQuantity dsgnInfRtAreaSI = QuantityConverter::instance().convert(dsgnInfRtAreaIP, siSys);
            OS_ASSERT(dsgnInfRtAreaSI);
            OS_ASSERT(dsgnInfRtAreaSI->units() == SIUnit(SIExpnt(0, 1, -1)));

            if (infMthdElement.text() == "AirChangesPerHour") {
              spaceInfiltrationDesignFlowRate.setAirChangesperHour(dsnInfRt);
            }else if (infMthdElement.text() == "FlowArea") {
                spaceInfiltrationDesignFlowRate.setFlowperSpaceFloorArea(dsgnInfRtAreaSI->value());
            }else if (infMthdElement.text() == "FlowExteriorArea") {
              spaceInfiltrationDesignFlowRate.setFlowperExteriorSurfaceArea(dsgnInfRtAreaSI->value());
            }else if(infMthdElement.text() == "FlowExteriorWallArea") {
              spaceInfiltrationDesignFlowRate.setFlowperExteriorWallArea(dsgnInfRtAreaSI->value());
            }else if (infMthdElement.text() == "FlowSpace") {
              spaceInfiltrationDesignFlowRate.setDesignFlowRate(dsgnInfRtSI->value());
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


      QDomElement intLPDRegSimElement = element.firstChildElement("IntLPDRegSim");
      QDomElement intLtgRegSchRefElement = element.firstChildElement("IntLtgRegSchRef");
      QDomElement intLtgRegHtGnSpcFracSimElement = element.firstChildElement("IntLtgRegHtGnSpcFracSim");
      QDomElement intLtgRegHtGnRadFracSimElement = element.firstChildElement("IntLtgRegHtGnRadFracSim");
      QDomElement intLtgRegEndUseElement = element.firstChildElement("IntLtgRegEndUseCat");
      if (!intLPDRegSimElement.isNull() && (intLPDRegSimElement.text().toDouble() > 0)){

        openstudio::Quantity lightingDensityIP(intLPDRegSimElement.text().toDouble(), openstudio::createUnit("W/ft^2").get());
        OptionalQuantity lightingDensitySI = QuantityConverter::instance().convert(lightingDensityIP, whSys);
        OS_ASSERT(lightingDensitySI);
        OS_ASSERT(lightingDensitySI->units() == WhUnit(WhExpnt(1,0,-2)));

        openstudio::model::LightsDefinition lightsDefinition(model);
        lightsDefinition.setName(name + " Regulated Lights Definition");
        lightsDefinition.setWattsperSpaceFloorArea(lightingDensitySI->value()); // W/m2

        openstudio::model::Lights lights(lightsDefinition);
        lights.setName(name + " Regulated Lights");
        lights.setSpace(space);

        std::string subCategory = "ComplianceLtg";
        if (!intLtgRegEndUseElement.isNull()){
          subCategory = intLtgRegEndUseElement.text().toStdString();
        }
        lights.setEndUseSubcategory(subCategory);

        if (!intLtgRegSchRefElement.isNull()){
          std::string scheduleName = escapeName(intLtgRegSchRefElement.text());
          boost::optional<model::Schedule> schedule = model.getModelObjectByName<model::Schedule>(scheduleName);
          if (schedule){
            lights.setSchedule(*schedule);
          }else{
            LOG(Error, "Could not find schedule '" << scheduleName << "'");
          }
        }

        if (!intLtgRegHtGnSpcFracSimElement.isNull()){
          double spaceFraction = intLtgRegHtGnSpcFracSimElement.text().toDouble();
          double returnAirFraction = 1.0 - spaceFraction;
          lightsDefinition.setReturnAirFraction(returnAirFraction);

          if (!intLtgRegHtGnRadFracSimElement.isNull()){
            double fractionRadiant = intLtgRegHtGnRadFracSimElement.text().toDouble() * spaceFraction;
            lightsDefinition.setFractionRadiant(fractionRadiant);
          }
        }else if (!intLtgRegHtGnRadFracSimElement.isNull()){
          LOG(Warn, "IntLtgRegHtGnRadFracSimElement is specified for space '" << name << "' but IntLtgRegHtGnSpcFracSimElement is not, IntLtgNonRegHtGnRadFracSimElement will be ignored.");
        }
      }

      QDomElement intLPDNonRegSimElement = element.firstChildElement("IntLPDNonRegSim");
      QDomElement intLtgNonRegSchRefElement = element.firstChildElement("IntLtgNonRegSchRef");
      QDomElement intLtgNonRegHtGnSpcFracSimElement = element.firstChildElement("IntLtgNonRegHtGnSpcFracSim");
      QDomElement intLtgNonRegHtGnRadFracSimElement = element.firstChildElement("IntLtgNonRegHtGnRadFracSim");
      QDomElement intLtgNonRegEndUseElement = element.firstChildElement("IntLtgNonRegEndUseCat");
      if (!intLPDNonRegSimElement.isNull() && (intLPDNonRegSimElement.text().toDouble() > 0)){

        openstudio::Quantity lightingDensityIP(intLPDNonRegSimElement.text().toDouble(), openstudio::createUnit("W/ft^2").get());
        OptionalQuantity lightingDensitySI = QuantityConverter::instance().convert(lightingDensityIP, whSys);
        OS_ASSERT(lightingDensitySI);
        OS_ASSERT(lightingDensitySI->units() == WhUnit(WhExpnt(1,0,-2)));

        openstudio::model::LightsDefinition lightsDefinition(model);
        lightsDefinition.setName(name + " Non-Regulated Lights Definition");
        lightsDefinition.setWattsperSpaceFloorArea(lightingDensitySI->value()); // W/m2

        openstudio::model::Lights lights(lightsDefinition);
        lights.setName(name + " Non-Regulated Lights");
        lights.setSpace(space);

        std::string subCategory = "NonComplianceLtg";
        if (!intLtgRegEndUseElement.isNull()){
          subCategory = intLtgNonRegEndUseElement.text().toStdString();
        }
        lights.setEndUseSubcategory(subCategory);

        if (!intLtgNonRegSchRefElement.isNull()){
          std::string scheduleName = escapeName(intLtgNonRegSchRefElement.text());
          boost::optional<model::Schedule> schedule = model.getModelObjectByName<model::Schedule>(scheduleName);
          if (schedule){
            lights.setSchedule(*schedule);
          }else{
            LOG(Error, "Could not find schedule '" << scheduleName << "'");
          }
        }

        if (!intLtgNonRegHtGnSpcFracSimElement.isNull()){
          double spaceFraction = intLtgNonRegHtGnSpcFracSimElement.text().toDouble();
          double returnAirFraction = 1.0 - spaceFraction;
          lightsDefinition.setReturnAirFraction(returnAirFraction);

          if (!intLtgNonRegHtGnRadFracSimElement.isNull()){
            double fractionRadiant = intLtgNonRegHtGnRadFracSimElement.text().toDouble() * spaceFraction;
            lightsDefinition.setFractionRadiant(fractionRadiant);
          }
        }else if (!intLtgNonRegHtGnRadFracSimElement.isNull()){
          LOG(Warn, "IntLtgNonRegHtGnRadFracSimElement is specified for space '" << name << "' but IntLtgNonRegHtGnSpcFracSimElement is not, IntLtgNonRegHtGnRadFracSimElement will be ignored.");
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

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateConvectionCoefficients(const QDomElement& element, const QDomDocument& doc, openstudio::model::PlanarSurface& surface)
  {
    boost::optional<std::string> convectionCoefficient1Location;
    boost::optional<std::string> convectionCoefficient1Type;
    boost::optional<double> convectionCoefficient1;
    boost::optional<std::string> convectionCoefficient2Location;
    boost::optional<std::string> convectionCoefficient2Type;
    boost::optional<double> convectionCoefficient2;

    QDomElement insideConvCoefElement = element.firstChildElement("InsideConvCoef");
    if (!insideConvCoefElement.isNull()){

      // sdd IP units (Btu/h-ft2-F), os SI units (W/m2-K)
      Quantity coefIP(insideConvCoefElement.text().toDouble(), BTUUnit(BTUExpnt(1, -2, -1, -1)));
      OptionalQuantity coefSI = QuantityConverter::instance().convert(coefIP, UnitSystem(UnitSystem::Wh));
      OS_ASSERT(coefSI);
      OS_ASSERT(coefSI->units() == WhUnit(WhExpnt(1, 0, -2, -1)));

      convectionCoefficient1Location = "Inside";
      convectionCoefficient1 = coefSI->value();
    }

    QDomElement outsideConvCoefElement = element.firstChildElement("OutsideConvCoef");
    if (!outsideConvCoefElement.isNull()){

      // sdd IP units (Btu/h-ft2-F), os SI units (W/m2-K)
      Quantity coefIP(outsideConvCoefElement.text().toDouble(), BTUUnit(BTUExpnt(1, -2, -1, -1)));
      OptionalQuantity coefSI = QuantityConverter::instance().convert(coefIP, UnitSystem(UnitSystem::Wh));
      OS_ASSERT(coefSI);
      OS_ASSERT(coefSI->units() == WhUnit(WhExpnt(1, 0, -2, -1)));

      if (convectionCoefficient1Location){
        convectionCoefficient2Location = "Outside";
        convectionCoefficient2 = coefSI->value();
      } else {
        convectionCoefficient1Location = "Outside";
        convectionCoefficient1 = coefSI->value();
      }
    }

    if (convectionCoefficient1Location){

      if( auto derivedSurface = surface.optionalCast<model::Surface>() ) {
        model::SurfacePropertyConvectionCoefficients surfacePropertyConvectionCoefficients(derivedSurface.get());

        surfacePropertyConvectionCoefficients.setConvectionCoefficient1Location(*convectionCoefficient1Location);
        surfacePropertyConvectionCoefficients.setConvectionCoefficient1Type("Value");
        surfacePropertyConvectionCoefficients.setConvectionCoefficient1(*convectionCoefficient1);

        if (convectionCoefficient2Location){
          surfacePropertyConvectionCoefficients.setConvectionCoefficient2Location(*convectionCoefficient2Location);
          surfacePropertyConvectionCoefficients.setConvectionCoefficient2Type("Value");
          surfacePropertyConvectionCoefficients.setConvectionCoefficient2(*convectionCoefficient2);
        }

        return surfacePropertyConvectionCoefficients;
      }
    }

    return boost::none;
  }

  boost::optional<model::ModelObject> ReverseTranslator::translateSurface(const QDomElement& element, const QDomDocument& doc, openstudio::model::Space& space)
  {
    boost::optional<model::ModelObject> result;

    UnitSystem siSys(UnitSystem::SI);

    std::vector<openstudio::Point3d> vertices;

    QDomElement polyLoopElement = element.firstChildElement("PolyLp");
    if (polyLoopElement.isNull()){
      LOG(Error, "Surface element 'PolyLp' is empty, cannot create Surface.");
      return boost::none;
    }

    QDomNodeList cartesianPointElements = polyLoopElement.elementsByTagName("CartesianPt");
    for (int i = 0; i < cartesianPointElements.count(); i++){
      QDomNodeList coordinateElements = cartesianPointElements.at(i).toElement().elementsByTagName("Coord");
      if (coordinateElements.size() != 3){
        LOG(Error, "PolyLp element 'CartesianPt' does not have exactly 3 'Coord' elements, cannot create Surface.");
        return boost::none;
      }

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

    openstudio::model::Surface surface(vertices, space.model());
    surface.setSpace(space);

    QDomElement nameElement = element.firstChildElement("Name");
    std::string name;
    if (nameElement.isNull()){
      LOG(Error, "Surface element 'Name' is empty.")
    } else{
      name = escapeName(nameElement.text());
    }
    surface.setName(name);

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
      if (!subSurface){
        LOG(Error, "Failed to translate 'Win' element " << i << " for Surface named '" << name << "'");
      }
    }
    for (int i = 0; i < doorElements.count(); ++i){
      boost::optional<model::ModelObject> subSurface = translateSubSurface(doorElements.at(i).toElement(), doc, surface);
      if (!subSurface){
        LOG(Error, "Failed to translate 'Dr' element " << i << " for Surface named '" << name << "'");
      }
    }
    for (int i = 0; i < skylightElements.count(); ++i){
      boost::optional<model::ModelObject> subSurface = translateSubSurface(skylightElements.at(i).toElement(), doc, surface);
      if (!subSurface){
        LOG(Error, "Failed to translate 'Skylt' element " << i << " for Surface named '" << name << "'");
      }
    }

    // check for adjacent surface
    QDomElement adjacentSpaceElement = element.firstChildElement("AdjacentSpcRef");
    if (!adjacentSpaceElement.isNull()){
      std::string adjacentSpaceName = escapeName(adjacentSpaceElement.text());
      boost::optional<model::Space> otherSpace = space.model().getModelObjectByName<model::Space>(adjacentSpaceName);

      if (!otherSpace){
        LOG(Error, "Cannot retrieve adjacent Space '" << adjacentSpaceName << "' for Surface named '" << name << "'");

        // DLM: make adiabatic per David Reddy, 6/5/2015
        //surface.remove();
        //return boost::none;
        surface.setOutsideBoundaryCondition("Adiabatic");

      } else if (otherSpace->handle() == space.handle()){
        LOG(Error, "Adjacent Space '" << adjacentSpaceName << "' is same as parent Space for Surface named '" << name << "'.  Removing interior surface.");

        // DLM: make adiabatic per David Reddy, 6/5/2015
        //surface.remove();
        //return boost::none;
        surface.setOutsideBoundaryCondition("Adiabatic");

      } else{
        // clone the surface and sub surfaces with reverse vertices
        boost::optional<model::Surface> otherSurface = surface.createAdjacentSurface(*otherSpace);
        if (!otherSurface){
          LOG(Error, "Failed to create surface in adjacent Space '" << adjacentSpaceName << "' for Surface named '" << name << "'.  Removing surface.");

          // DLM: make adiabatic per David Reddy, 6/5/2015
          //surface.remove();
          //return boost::none;
          surface.setOutsideBoundaryCondition("Adiabatic");
        }
      }
    }

    return result;
  }

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateSubSurface(const QDomElement& element, const QDomDocument& doc, openstudio::model::Surface& surface)
  {
    std::vector<openstudio::Point3d> vertices;

    UnitSystem siSys(UnitSystem::SI);

    QDomElement polyLoopElement = element.firstChildElement("PolyLp");
    if (polyLoopElement.isNull()){
      LOG(Error, "SubSurface element 'PolyLp' is empty, cannot create SubSurface.");
      return boost::none;
    }

    QDomNodeList cartesianPointElements = polyLoopElement.elementsByTagName("CartesianPt");
    for (int i = 0; i < cartesianPointElements.count(); i++){
      QDomNodeList coordinateElements = cartesianPointElements.at(i).toElement().elementsByTagName("Coord");
      if (coordinateElements.size() != 3){
        LOG(Error, "PolyLp element 'CartesianPt' does not have exactly 3 'Coord' elements, cannot create SubSurface.");
        return boost::none;
      }

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

    model::SubSurface subSurface(vertices, surface.model());
    subSurface.setSurface(surface);

    QDomElement nameElement = element.firstChildElement("Name");
    std::string name;
    if (nameElement.isNull()){
      LOG(Error, "Surface element 'Name' is empty.")
    } else{
      name = escapeName(nameElement.text());
    }
    subSurface.setName(name);

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

      // Convert surface convection coefficients
      translateConvectionCoefficients(element, doc, subSurface);

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

      // Convert surface convection coefficients
      translateConvectionCoefficients(element, doc, subSurface);

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

      // Convert surface convection coefficients
      translateConvectionCoefficients(element, doc, subSurface);

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
    if (polyLoopElement.isNull()){
      LOG(Error, "ShadingSurface element 'PolyLp' is empty, cannot create ShadingSurface.");
      return boost::none;
    }

    QDomNodeList cartesianPointElements = polyLoopElement.elementsByTagName("CartesianPt");
    for (int i = 0; i < cartesianPointElements.count(); i++){
      QDomNodeList coordinateElements = cartesianPointElements.at(i).toElement().elementsByTagName("Coord");
      if (coordinateElements.size() != 3){
        LOG(Error, "PolyLp element 'CartesianPt' does not have exactly 3 'Coord' elements, cannot create ShadingSurface.");
        return boost::none;
      }

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
    model::ShadingSurface shadingSurface(vertices, model);
    shadingSurface.setShadingSurfaceGroup(shadingSurfaceGroup);

    QDomElement nameElement = element.firstChildElement("Name");
    std::string name;
    if (nameElement.isNull()){
      LOG(Error, "ShadingSurface element 'Name' is empty.")
    } else{
      name = escapeName(nameElement.text());
    }
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

      QDomElement transOptionElement = element.firstChildElement("TransOption");
      if (!transOptionElement.isNull()){

        boost::optional<model::Schedule> schedule;
        std::string scheduleName;

        // constant transmittance
        if (transOptionElement.text().compare("Constant", Qt::CaseInsensitive) == 0){

          QDomElement transElement = element.firstChildElement("Trans");
          if (!transElement.isNull()){
            schedule = shadingSchedule(model, transElement.text().toDouble());
            if (schedule){
              scheduleName = schedule->name().get();
            }
          } else {
            LOG(Error, "Cannot find shading transmittance for shading surface '" << name << "'");
          }

          // transmittance schedule
        } else if (transOptionElement.text().compare("Scheduled", Qt::CaseInsensitive) == 0){

          QDomElement scheduleReferenceElement = element.firstChildElement("TransSchRef");
          if (!scheduleReferenceElement.isNull()){
            scheduleName = escapeName(scheduleReferenceElement.text());
            schedule = model.getModelObjectByName<model::Schedule>(scheduleName);
            if (!schedule){
              LOG(Error, "Cannot find shading schedule '" << scheduleName << "' for shading surface '" << name << "'");
            }
          } else{
            LOG(Error, "Cannot find shading schedule for shading surface '" << name << "'");
          }

        } else{
          LOG(Error, "Unknown TransOption value for shading surface '" << name << "'");
        }

        if (schedule){
          bool test = shadingSurface.setTransmittanceSchedule(*schedule);
          if (!test){
            LOG(Error, "Failed to assign shading schedule '" << scheduleName << "' to shading surface '" << name << "'");
          }
        } else {
          // DLM: could warn here
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
    if (!test){
      LOG(Error, "Failed to assign material layers to Construction named '" << constructionName << "'");
    }

    m_shadingConstructionMap.insert(std::make_pair(key, construction));
    return construction;
  }

  model::Schedule ReverseTranslator::shadingSchedule(openstudio::model::Model& model, double trans)
  {
    auto it = m_shadingScheduleMap.find(trans);
    if (it != m_shadingScheduleMap.end()){
      return it->second;
    }

    std::string description = boost::lexical_cast<std::string>(trans);
    std::string scheduleName = "Shading Schedule " + description;

    // create a schedule with these properties
    model::ScheduleRuleset schedule(model, trans);
    schedule.setName(scheduleName);

    m_shadingScheduleMap.insert(std::make_pair(trans, schedule));
    return schedule;
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

    // SDD:
    // FuncClassMthd - optional, ignore
    // RelocPubSchoolBldg - optional, in progress
    // WholeBldgModeled - required, need to add
    // BldgAz - required, done
    // TotStoryCnt - required, in progress
    // TotStoryCntNew - optional, need to add?
    // TotStoryCntExisting - optional, need to add?
    // TotStoryCntAltered - optional, need to add?
    // AboveGrdStoryCnt - required, in progress
    // AboveGrdStoryCntNew - optional, need to add?
    // AboveGrdStoryCntExisting  - optional, need to add?
    // AboveGrdStoryCntAltered - optional, need to add?
    // LivingUnitCnt - defaulted, in progress
    // LivingUnitCntNew - optional, need to add?
    // LivingUnitCntExisting - optional, need to add?
    // LivingUnitCntAltered - optional, need to add?
    // TotFlrArea - defaulted, ignore
    // NonResFlrArea - defaulted, ignore
    // ResFlrArea - defaulted, ignore
    // TotCondVol - defaulted, ignore
    // PlantClgCap - defaulted, ignore
    // PlantHtgCap - defaulted, ignore
    // CoilClgCap - defaulted, ignore
    // CoilHtgCap - defaulted, ignore

    // building azimuth
    double buildingAzimuth = fixAngle(building.northAxis());
    QDomElement buildingAzimuthElement = doc.createElement("BldgAz");
    result.appendChild(buildingAzimuthElement);
    buildingAzimuthElement.appendChild(doc.createTextNode(QString::number(buildingAzimuth)));

    // TotStoryCnt - required, Standards Number of Stories
    // AboveGrdStoryCnt - required, Standards Number of Above Ground Stories
    // LivingUnitCnt - defaulted, Standards Number of Living Units

    // translate storys
    std::vector<model::BuildingStory> buildingStories = building.model().getConcreteModelObjects<model::BuildingStory>();
    std::sort(buildingStories.begin(), buildingStories.end(), WorkspaceObjectNameLess());

    if (m_progressBar){
      m_progressBar->setWindowTitle(toString("Translating Building Stories"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum((int)buildingStories.size());
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
      m_progressBar->setMaximum((int)shadingSurfaceGroups.size());
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
    std::string spacesWithoutStory;
    std::string spacesWithoutZone;
    for (const model::Space& space : spaces){
      if (!space.buildingStory()){
        spacesWithoutStory += " '" + space.name().get() + "',";
      }
      if (!space.thermalZone()){
        spacesWithoutZone += " '" + space.name().get() + "',";
      }
    }

    if (spacesWithoutStory.size() > 0){
      spacesWithoutStory.pop_back();
      LOG(Warn, "Model contains spaces which are not assigned to a building story, these have not been translated:" << spacesWithoutStory);
    }

    if (spacesWithoutZone.size() > 0){
      // DLM: desired workflow is to assign thermal zones in cbecc
      // DLM: Kyle, we will have to think about if we want to warn about this or not
      //Do not want this logged, http://code.google.com/p/cbecc/issues/detail?id=695
      //spacesWithoutZone.pop_back();
      //LOG(Warn, "Model contains spaces which are not assigned to a thermal zone, these have not been translated:" << spacesWithoutZone);
    }

    // issue warning if any surfaces not assigned to space
    std::vector<model::Surface> surfaces = building.model().getConcreteModelObjects<model::Surface>();
    std::sort(surfaces.begin(), surfaces.end(), WorkspaceObjectNameLess());
    std::string surfacesWithoutSpace;
    for (const model::Surface& surface : surfaces){
      if (!surface.space()){
        surfacesWithoutSpace += " '" + surface.name().get() + "',";
      }
    }

    if (surfacesWithoutSpace.size() > 0){
      surfacesWithoutSpace.pop_back();
      LOG(Warn, "Model contains surfaces which are not assigned to a space, these have not been translated:" << surfacesWithoutSpace);
    }

    // issue warning if any sub surfaces not assigned to surface
    std::vector<model::SubSurface> subSurfaces = building.model().getConcreteModelObjects<model::SubSurface>();
    std::sort(subSurfaces.begin(), subSurfaces.end(), WorkspaceObjectNameLess());
    std::string subSurfacesWithoutSurface;
    for (const model::SubSurface& subSurface : subSurfaces){
      if (!subSurface.surface()){
        subSurfacesWithoutSurface += " '" + subSurface.name().get() + "',";
      }
    }

    if (subSurfacesWithoutSurface.size() > 0){
      subSurfacesWithoutSurface.pop_back();
      LOG(Warn, "Model contains sub surfaces which are not assigned to a surface, these have not been translated:" << subSurfacesWithoutSurface);
    }

    // translate thermal zones
    std::vector<model::ThermalZone> thermalZones = building.model().getConcreteModelObjects<model::ThermalZone>();
    std::sort(thermalZones.begin(), thermalZones.end(), WorkspaceObjectNameLess());

    if (m_progressBar){
      m_progressBar->setWindowTitle(toString("Translating Thermal Zones"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum((int)thermalZones.size());
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

    // translate AirLoopHVAC systems
    auto airLoops = building.model().getConcreteModelObjects<model::AirLoopHVAC>();
    std::sort(airLoops.begin(),airLoops.end(),WorkspaceObjectNameLess());

    if (m_progressBar) {
      m_progressBar->setWindowTitle(toString("Translating AirLoopHVAC Systems"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum((int)airLoops.size());
      m_progressBar->setValue(0);
    }

    for (const auto & airLoop : airLoops) {
      auto airLoopElement = translateAirLoopHVAC(airLoop,doc);
      if (airLoopElement) {
        result.appendChild(*airLoopElement);
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

    // SDD:
    // Mult - defaulted, ignore (OS doesn't have this)
    // Z - only for simple geometry, ignore
    // FlrToFlrHgt - only for simple geometry, ignore
    // FlrToCeilingHgt - only for simple geometry, ignore

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

    // SDD:
    // Status - required, need to add
    // CondgType - required, in progress
    // SupPlenumSpcRef - optional, in progress
    // RetPlenumSpcRef - optional, in progress
    // ThrmlZnRef - required, done
    // Area - only for simple geometry, done, can we remove?
    // FlrArea - optional, do we need this?
    // FlrZ - optional, do we need this?
    // FlrToCeilingHgt - optional, do we need this?
    // Vol - required, done, can we remove?
    // SpcFuncDefaultsRef - optional, do with space types
    // SpcFunc - compulsory, do with space types
    // FuncSchGrp - optional, do with space types
    // OccDensSim - optional, do with space types
    // OccSensHtRt - optional, do with space types
    // OccLatHtRt - optional, do with space types
    // OccSchRef - optional, do with space types
    // InfMthd - defaulted, do with space types
    // DsgnInfRt - defaulted, do with space types
    // InfSchRef - defaulted, do with space types
    // InfModelCoefA - required, do with space types
    // InfModelCoefB - required, do with space types
    // InfModelCoefC - required, do with space types
    // InfModelCoefD - required, do with space types
    // EnvStatus - optional, do with space types
    // LtgStatus - optional, do with space types
    // IntLtgSpecMthd - required, do with space types
    // IntLPDReg - optional, do with space types
    // IntLtgRegSchRef - optional, do with space types
    // IntLtgRegHtGnSpcFrac - optional, do with space types
    // IntLtgRegHtGnRadFrac - optional, do with space types
    // IntLPDNonReg - optional, do with space types
    // IntLtgNonRegSchRef - optional, do with space types
    // IntLtgNonRegHtGnSpcFrac - optional, do with space types
    // IntLtgNonRegHtGnRadFrac - optional, do with space types
    // SkylitDayltgInstalledLtgPwr - optional, do we need this?
    // PriSideDayltgInstalledLtgPwr - optional, do we need this?
    // SecSideDayltgInstalledLtgPwr - optional, do we need this?
    // Skylit100PctControlled - optional, do we need this?
    // PriSide100PctControlled - optional, do we need this?
    // SecSide100PctControlled - optional, do we need this?
    // SkylitDayltgRefPtCoord - optional, do we need this?
    // SkylitDayltgCtrlLtgPwr - optional, do we need this?
    // SkylitDayltgCtrlLtgFrac - optional, do we need this?
    // SkylitDayltgIllumSetpt - optional, do we need this?
    // PriSideDayltgRefPtCoord - optional, do we need this?
    // PriSideDayltgCtrlLtgPwr - optional, do we need this?
    // PriSideDayltgCtrlLtgFrac - optional, do we need this?
    // PriSideDayltgIllumSetpt - optional, do we need this?
    // SecSideDayltgRefPtCoord - optional, do we need this?
    // SecSideDayltgCtrlLtgPwr - optional, do we need this?
    // SecSideDayltgCtrlLtgFrac - optional, do we need this?
    // SecSideDayltgIllumSetpt - optional, do we need this?
    // DayltgCtrlType - optional, do we need this?
    // MinDimLtgFrac - optional, do we need this?
    // MinDimPwrFrac - optional, do we need this?
    // NumOfCtrlSteps - optional, do we need this?
    // GlrAz - optional, do we need this?
    // MaxGlrIdx - optional, do we need this?
    // SkyltReqExcpt - optional, do we need this?
    // SkyltReqExcptArea - optional, do we need this?
    // SkyltReqExcptFrac - optional, do we need this?
    // RecptPwrDens - defaulted, do with space types
    // RecptSchRef - defaulted, do with space types
    // RecptRadFrac - defaulted, do with space types
    // RecptLatFrac - defaulted, do with space types
    // RecptLostFrac - defaulted, do with space types
    // GasEqpPwrDens - defaulted, do with space types
    // GasEqpSchRef - defaulted, do with space types
    // GasEqpRadFrac - defaulted, do with space types
    // GasEqpLatFrac - defaulted, do with space types
    // GasEqpLostFrac - defaulted, do with space types
    // ProcElecPwrDens - optional, do with space types
    // ProcElecSchRef - optional, do with space types
    // ProcElecRadFrac - optional, do with space types
    // ProcElecLatFrac - optional, do with space types
    // ProcElecLostFrac - optional, do with space types
    // ProcGasPwrDens - optional, do with space types
    // ProcGasSchRef - optional, do with space types
    // ProcGasRadFrac - optional, do with space types
    // ProcGasLatFrac - optional, do with space types
    // ProcGasLostFrac - optional, do with space types
    // CommRfrgEPD - defaulted, do with space types
    // CommRfrgEqpSchRef - defaulted, do with space types
    // CommRfrgRadFrac - defaulted, do with space types
    // CommRfrgLatFrac - defaulted, do with space types
    // CommRfrgLostFrac - defaulted, do with space types
    // ElevCnt - optional, do with space types
    // ElevPwr - optional, do with space types
    // ElevSchRef - defaulted, do with space types
    // ElevRadFrac - optional, do with space types
    // ElevLatFrac - optional, do with space types
    // ElevLostFrac - optional, do with space types
    // EscalCnt - optional, do with space types
    // EscalPwr - optional, do with space types
    // EscalSchRef - defaulted, do with space types
    // EscalRadFrac - optional, do with space types
    // EscalLatFrac - optional, do with space types
    // EscalLostFrac - optional, do with space types
    // SHWFluidSegRef - optional, do with space types
    // RecircDHWSysRef - optional, do with space types
    // HotWtrHtgRt - defaulted, do with space types
    // RecircHotWtrHtgRt - optional, do with space types
    // HotWtrHtgSchRef - optional, do with space types
    // VentPerPerson - defaulted, do with space types
    // VentPerArea - defaulted, do with space types
    // VentACH - optional, do with space types
    // VentPerSpc - optional, do with space types
    // ExhPerArea - optional, do we need this?
    // ExhACH - optional, do we need this?
    // ExhPerSpc - optional, do we need this?
    // KitExhHoodLen - optional, do we need this?
    // KitExhHoodStyle - optional, do we need this?
    // KitExhHoodDuty - optional, do we need this?
    // KitExhHoodFlow - optional, do we need this?
    // LabExhRtType - optional, do we need this?
    // IntLPDPrescrip - optional, do we need this?
    // IsPlenumRet - optional, do we need this?
    // HighRiseResInt - optional, do we need this?
    // HighRiseResCondFlrArea - optional, do we need this?


    // volume
    double volume = space.volume();
    Quantity volumeSI(volume, SIUnit(SIExpnt(0,3,0)));
    OptionalQuantity volumeIP = QuantityConverter::instance().convert(volumeSI, ipSys);
    OS_ASSERT(volumeIP);
    OS_ASSERT(volumeIP->units() == IPUnit(IPExpnt(0,3,0)));
    QDomElement volumeElement = doc.createElement("Vol");
    result.appendChild(volumeElement);
    volumeElement.appendChild(doc.createTextNode(QString::number(volumeIP->value())));

    // log warning if volume is 0
    if (volumeIP->value() < std::numeric_limits<double>::epsilon()){
      LOG(Warn, "Space '" << name << "' has zero volume.");
    }

    // area
    double floorArea = space.floorArea();
    Quantity floorAreaSI(floorArea, SIUnit(SIExpnt(0,2,0)));
    OptionalQuantity floorAreaIP = QuantityConverter::instance().convert(floorAreaSI, ipSys);
    OS_ASSERT(floorAreaIP);
    OS_ASSERT(floorAreaIP->units() == IPUnit(IPExpnt(0,2,0)));
    QDomElement floorAreaElement = doc.createElement("Area");  // SAC 3/14/14
    result.appendChild(floorAreaElement);
    floorAreaElement.appendChild(doc.createTextNode(QString::number(floorAreaIP->value())));

    // log warning if area is 0
    if (floorAreaIP->value() < std::numeric_limits<double>::epsilon()){
      LOG(Warn, "Space '" << name << "' has zero floor area.");
    }

    // translate floorPrint
    Transformation transformation = space.siteTransformation();

    Point3dVector vertices = transformation*space.floorPrint();
    boost::optional<double> floorPrintArea = openstudio::getArea(vertices);
    if (vertices.empty() || !floorPrintArea){
      LOG(Warn, "Cannot compute floor print for space '" << name << "'.");
    }else{
      Quantity floorPrintAreaSI(*floorPrintArea, SIUnit(SIExpnt(0, 2, 0)));
      OptionalQuantity floorPrintAreaIP = QuantityConverter::instance().convert(floorPrintAreaSI, ipSys);
      OS_ASSERT(floorPrintAreaIP);
      OS_ASSERT(floorPrintAreaIP->units() == IPUnit(IPExpnt(0, 2, 0)));

      // log warning if floor print area is 0
      if (floorPrintAreaIP->value() < std::numeric_limits<double>::epsilon()){
        LOG(Warn, "Space '" << name << "' has zero floor print area.");
      }
    }

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

      // CondgType - required
      // SupPlenumSpcRef - optional
      // RetPlenumSpcRef - optional
      // ThrmlZnRef - required
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

    unsigned numFloors = 0;
    unsigned numWalls = 0;
    unsigned numRoofCeilings = 0;
    for (const model::Surface& surface : surfaces){
      std::string surfaceType = surface.surfaceType();
      if (istringEqual("Wall", surfaceType)){
        ++numWalls;
      } else if (istringEqual("RoofCeiling", surfaceType)){
        ++numRoofCeilings;
      } else if (istringEqual("Floor", surfaceType)){
        ++numFloors;
      }

      boost::optional<QDomElement> surfaceElement = translateSurface(surface, transformation, doc);
      if (surfaceElement){
        result.appendChild(*surfaceElement);
      }
    }

    if (numFloors < 1){
      LOG(Warn, "Space '" << name << "' has less than 1 floor surfaces.")
    }
    if (numWalls < 3){
      LOG(Warn, "Space '" << name << "' has less than 3 wall surfaces.")
    }
    if (numRoofCeilings < 1){
      LOG(Warn, "Space '" << name << "' has less than 1 roof or ceiling surfaces.")
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

    // SDD:
    // Status - required (ExtFlr, ExtWall, IntWall, Roof, UndgrFlr, UndgrWall), need to add
    // ConsAssmRef - optional (Ceiling, ExtFlr, ExtWall, IntFlr, IntWall, Roof, UndgrFlr, UndgrWall), done
    // AdjacentSpcRef - optional (Ceiling, IntFlr, IntWall), done
    // Area - simplified geometry only (Ceiling, ExtFlr, ExtWall, IntFlr, IntWall, Roof, UndgrFlr, UndgrWall), ignore
    // Hgt - optional (UndgrWall), done, requires unique CFactor construction per surface
    // PerimExposed - optional (UndgrFlr), done requires unique FFactor construction per surface
    // DisplayPerim - optional (ExtWall), need to add?
    // Az - simplified geometry only (ExtWall, Roof), ignore
    // Tilt - simplified geometry only (Roof), ignore
    // ExtSolAbs - required (Ceiling, ExtFlr, ExtWall, IntWall), ignore, do at construction level
    // ExtThrmlAbs - required (Ceiling, ExtFlr, ExtWall, IntWall), ignore, do at construction level
    // ExtVisAbs - required (Ceiling, ExtFlr, ExtWall, IntWall), ignore, do at construction level
    // IntSolAbs - optional (Ceiling, ExtFlr, ExtWall, IntFlr, IntWall, Roof, UndgrFlr, UndgrWall), ignore, do at construction level
    // IntThrmlAbs - optional (Ceiling, ExtFlr, ExtWall, IntFlr, IntWall, Roof, UndgrFlr, UndgrWall), ignore, do at construction level
    // IntVisAbs - optional (Ceiling, ExtFlr, ExtWall, IntFlr, IntWall, Roof, UndgrFlr, UndgrWall), ignore, do at construction level
    // FieldAppliedCoating - optional (Roof), ignore, do at construction level
    // CRRCInitialRefl - optional (Roof), ignore, do at construction level
    // CRRCAgedRefl - optional (Roof), ignore, do at construction level
    // CRRCInitialEmittance - optional (Roof)), ignore, do at construction level
    // CRRCAgedEmittance - optional (Roof), ignore, do at construction level
    // CRRCInitialSRI - optional (Roof), ignore, do at construction level
    // CRRCAgedSRI - optional (Roof), ignore, do at construction level
    // CRRCProdID - optional (Roof), ignore, do at construction level

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
        //Do not want this logged, http://code.google.com/p/cbecc/issues/detail?id=695
        //LOG(Error, "Surface '" << name << "' uses construction '" << constructionName << "' which has not been translated");
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
          //Do not want this logged, http://code.google.com/p/cbecc/issues/detail?id=695
          //LOG(Error, "Cannot compute exposed perimeter for surface '" << name << "'.");
        }
      }else{
        //Do not want this logged, http://code.google.com/p/cbecc/issues/detail?id=695
        //LOG(Error, "Cannot compute exposed perimeter for surface '" << name << "'.");
      }
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
          //Do not want this logged, http://code.google.com/p/cbecc/issues/detail?id=695
          //LOG(Error, "Cannot compute height for surface '" << name << "'.");
        }
      }else{
        //Do not want this logged, http://code.google.com/p/cbecc/issues/detail?id=695
        //LOG(Error, "Cannot compute height for surface '" << name << "'.");
      }
    }

    // check area
    double grossArea = surface.grossArea();
    Quantity grossAreaSI(grossArea, SIUnit(SIExpnt(0, 2, 0)));
    OptionalQuantity grossAreaIP = QuantityConverter::instance().convert(grossAreaSI, ipSys);
    OS_ASSERT(grossAreaIP);
    OS_ASSERT(grossAreaIP->units() == IPUnit(IPExpnt(0, 2, 0)));

    // log warning if area is 0
    if (grossAreaIP->value() < std::numeric_limits<double>::epsilon()){
      LOG(Warn, "Surface '" << name << "' has zero area.")
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

    // SDD:
    // Status - required (Win, Skylt, Dr), need to add
    // FenConsRef - optional (Win, Skylt), done
    // DrConsRef - optional (Dr), done
    // Oper - optional (Dr), in progress
    // Area - simple geometry only (Win, Skylt, Dr), ignore

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
        //Do not want this logged, http://code.google.com/p/cbecc/issues/detail?id=695
        //LOG(Error, "SubSurface '" << name << "' uses construction '" << constructionName << "' which has not been translated");
      }
    }

    // check area
    double grossArea = subSurface.grossArea();
    Quantity grossAreaSI(grossArea, SIUnit(SIExpnt(0, 2, 0)));
    OptionalQuantity grossAreaIP = QuantityConverter::instance().convert(grossAreaSI, ipSys);
    OS_ASSERT(grossAreaIP);
    OS_ASSERT(grossAreaIP->units() == IPUnit(IPExpnt(0, 2, 0)));

    // log warning if area is 0
    if (grossAreaIP->value() < std::numeric_limits<double>::epsilon()){
      LOG(Warn, "Sub Surface '" << name << "' has zero area.")
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

    // SDD:
    // Status - required, need to add
    // TransSchRef - optional, in progress
    // SolRefl - optional, done
    // VisRefl - optional, done

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

    // DLM: Not input
    // Mult
    //QDomElement multElement = doc.createElement("Mult");
    //result.appendChild(multElement);
    //multElement.appendChild(doc.createTextNode(QString::number(thermalZone.multiplier())));

    return result;
  }


} // sdd
} // openstudio
