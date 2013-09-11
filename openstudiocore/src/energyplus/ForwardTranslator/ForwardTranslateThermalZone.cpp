/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <energyplus/ForwardTranslator.hpp>

#include <model/Model.hpp>

#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/PortList.hpp>
#include <model/PortList_Impl.hpp>
#include <model/ZoneHVACEquipmentList.hpp>
#include <model/ZoneHVACEquipmentList_Impl.hpp>
#include <model/SizingZone.hpp>
#include <model/SizingZone_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/ScheduleCompact.hpp>
#include <model/ScheduleCompact_Impl.hpp>
#include <model/SetpointManagerMixedAir.hpp>
#include <model/SetpointManagerSingleZoneReheat.hpp>
#include <model/AirLoopHVAC.hpp>
#include <model/AirLoopHVAC_Impl.hpp>
#include <model/ThermostatSetpointDualSetpoint.hpp>
#include <model/ThermostatSetpointDualSetpoint_Impl.hpp>
#include <model/SizingZone.hpp>
#include <model/SizingZone_Impl.hpp>
#include <model/DesignSpecificationOutdoorAir.hpp>
#include <model/DesignSpecificationOutdoorAir_Impl.hpp>
#include <model/SizingPeriod.hpp>
#include <model/SizingPeriod_Impl.hpp>

#include <model/Space.hpp>
#include <model/Space_Impl.hpp>
#include <model/SpaceType.hpp>
#include <model/SpaceType_Impl.hpp>
#include <model/ShadingSurfaceGroup.hpp>
#include <model/ShadingSurfaceGroup_Impl.hpp>
#include <model/ShadingSurface.hpp>
#include <model/ShadingSurface_Impl.hpp>
#include <model/InteriorPartitionSurfaceGroup.hpp>
#include <model/InteriorPartitionSurfaceGroup_Impl.hpp>
#include <model/InteriorPartitionSurface.hpp>
#include <model/InteriorPartitionSurface_Impl.hpp>
#include <model/Surface.hpp>
#include <model/Surface_Impl.hpp>
#include <model/SubSurface.hpp>
#include <model/SubSurface_Impl.hpp>
#include <model/ConstructionBase.hpp>
#include <model/ConstructionBase_Impl.hpp>
#include <model/InternalMass.hpp>
#include <model/InternalMass_Impl.hpp>
#include <model/InternalMassDefinition.hpp>
#include <model/InternalMassDefinition_Impl.hpp>
#include <model/People.hpp>
#include <model/People_Impl.hpp>
#include <model/PeopleDefinition.hpp>
#include <model/PeopleDefinition_Impl.hpp>
#include <model/Lights.hpp>
#include <model/Lights_Impl.hpp>
#include <model/LightsDefinition.hpp>
#include <model/LightsDefinition_Impl.hpp>
#include <model/Luminaire.hpp>
#include <model/Luminaire_Impl.hpp>
#include <model/LuminaireDefinition.hpp>
#include <model/LuminaireDefinition_Impl.hpp>
#include <model/ElectricEquipment.hpp>
#include <model/ElectricEquipment_Impl.hpp>
#include <model/ElectricEquipmentDefinition.hpp>
#include <model/ElectricEquipmentDefinition_Impl.hpp>
#include <model/GasEquipment.hpp>
#include <model/GasEquipment_Impl.hpp>
#include <model/GasEquipmentDefinition.hpp>
#include <model/GasEquipmentDefinition_Impl.hpp>
#include <model/HotWaterEquipment.hpp>
#include <model/HotWaterEquipment_Impl.hpp>
#include <model/SteamEquipment.hpp>
#include <model/SteamEquipment_Impl.hpp>
#include <model/OtherEquipment.hpp>
#include <model/OtherEquipment_Impl.hpp>
#include <model/DaylightingControl.hpp>
#include <model/DaylightingControl_Impl.hpp>
#include <model/IlluminanceMap.hpp>
#include <model/IlluminanceMap_Impl.hpp>
#include <model/SpaceInfiltrationDesignFlowRate.hpp>
#include <model/SpaceInfiltrationDesignFlowRate_Impl.hpp>
#include <model/SpaceInfiltrationEffectiveLeakageArea.hpp>
#include <model/SpaceInfiltrationEffectiveLeakageArea_Impl.hpp>
#include <model/DesignSpecificationOutdoorAir.hpp>
#include <model/DesignSpecificationOutdoorAir_Impl.hpp>
#include <model/GlareSensor.hpp>
#include <model/GlareSensor_Impl.hpp>
#include <model/LifeCycleCost.hpp>

#include <utilities/idf/IdfExtensibleGroup.hpp>
#include <utilities/idf/Workspace.hpp>
#include <utilities/idf/WorkspaceObjectOrder.hpp>
#include <utilities/core/Logger.hpp>
#include <utilities/core/Assert.hpp>
#include <utilities/idd/Zone_FieldEnums.hxx>
#include <utilities/idd/HVACTemplate_Zone_IdealLoadsAirSystem_FieldEnums.hxx>
#include <utilities/idd/Daylighting_Controls_FieldEnums.hxx>
#include <utilities/idd/Output_IlluminanceMap_FieldEnums.hxx>
#include <utilities/idd/Schedule_Compact_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_EquipmentConnections_FieldEnums.hxx>
#include <utilities/idd/ZoneControl_Thermostat_FieldEnums.hxx>
#include <utilities/idd/Sizing_Zone_FieldEnums.hxx>
#include <utilities/idd/DesignSpecification_OutdoorAir_FieldEnums.hxx>
#include <utilities/idd/ZoneVentilation_DesignFlowRate_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include <utilities/geometry/Geometry.hpp>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateThermalZone( ThermalZone & modelObject )
{
  std::string s;

  // ThermalZone

  // create, register, and name object
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Zone,
                                                       modelObject);

  BOOST_FOREACH(LifeCycleCost lifeCycleCost, modelObject.lifeCycleCosts()){
    translateAndMapModelObject(lifeCycleCost);
  }

  idfObject.setString(openstudio::ZoneFields::Type,"");

  if (!modelObject.isMultiplierDefaulted()){
    idfObject.setInt(openstudio::ZoneFields::Multiplier,modelObject.multiplier());
  }

  if (modelObject.ceilingHeight()){
    idfObject.setDouble(openstudio::ZoneFields::CeilingHeight,modelObject.ceilingHeight().get());
  }

  if (modelObject.volume()){
    idfObject.setDouble(openstudio::ZoneFields::Volume,modelObject.volume().get());
  }

  if (modelObject.zoneInsideConvectionAlgorithm()){
    idfObject.setString(openstudio::ZoneFields::ZoneInsideConvectionAlgorithm,modelObject.zoneInsideConvectionAlgorithm().get());
  }

  if (modelObject.zoneOutsideConvectionAlgorithm()){
    idfObject.setString(openstudio::ZoneFields::ZoneOutsideConvectionAlgorithm,modelObject.zoneOutsideConvectionAlgorithm().get());
  }

  // Spaces

  std::vector<Space> spaces = modelObject.spaces();
  if (spaces.empty()){
    LOG(Warn, "ThermalZone " << modelObject.name().get() << " does not have any geometry or loads associated with it.");
  }else{
    OS_ASSERT(spaces.size() == 1);

    if (!spaces[0].isDirectionofRelativeNorthDefaulted()){
      idfObject.setDouble(openstudio::ZoneFields::DirectionofRelativeNorth, spaces[0].directionofRelativeNorth());
    }

    if (!spaces[0].isXOriginDefaulted()){
      idfObject.setDouble(openstudio::ZoneFields::XOrigin, spaces[0].xOrigin());
    }

    if (!spaces[0].isYOriginDefaulted()){
      idfObject.setDouble(openstudio::ZoneFields::YOrigin, spaces[0].yOrigin());
    }

    if (!spaces[0].isZOriginDefaulted()){
      idfObject.setDouble(openstudio::ZoneFields::ZOrigin, spaces[0].zOrigin());
    }

    if (!spaces[0].isPartofTotalFloorAreaDefaulted()){
      if (spaces[0].partofTotalFloorArea()){
        idfObject.setString(openstudio::ZoneFields::PartofTotalFloorArea,"Yes");
      }else{
        idfObject.setString(openstudio::ZoneFields::PartofTotalFloorArea,"No");
      }
    }

    // translate the space now
    translateAndMapModelObject(spaces[0]);

    // translate shading groups
    ShadingSurfaceGroupVector shadingSurfaceGroups = spaces[0].shadingSurfaceGroups();
    std::sort(shadingSurfaceGroups.begin(), shadingSurfaceGroups.end(), WorkspaceObjectNameLess());
    BOOST_FOREACH(ShadingSurfaceGroup& shadingSurfaceGroup, shadingSurfaceGroups){
      translateAndMapModelObject(shadingSurfaceGroup);
    }

    // translate interior surface partition groups
    InteriorPartitionSurfaceGroupVector interiorPartitionSurfaceGroups = spaces[0].interiorPartitionSurfaceGroups();
    std::sort(interiorPartitionSurfaceGroups.begin(), interiorPartitionSurfaceGroups.end(), WorkspaceObjectNameLess());
    BOOST_FOREACH(InteriorPartitionSurfaceGroup& interiorPartitionSurfaceGroup, interiorPartitionSurfaceGroups){
      translateAndMapModelObject(interiorPartitionSurfaceGroup);
    }

    // translate surfaces
    SurfaceVector surfaces = spaces[0].surfaces();
    std::sort(surfaces.begin(), surfaces.end(), WorkspaceObjectNameLess());
    BOOST_FOREACH(Surface& surface, surfaces){
      translateAndMapModelObject(surface);
    }
    
    // translate internal mass
    InternalMassVector internalMasses = spaces[0].internalMass();
    std::sort(internalMasses.begin(), internalMasses.end(), WorkspaceObjectNameLess());
    BOOST_FOREACH(InternalMass& internalMass, internalMasses){
      translateAndMapModelObject(internalMass);
    }

    // translate lights
    LightsVector lights = spaces[0].lights();
    std::sort(lights.begin(), lights.end(), WorkspaceObjectNameLess());
    BOOST_FOREACH(Lights& light, lights){
      translateAndMapModelObject(light);
    }

    // translate luminaires
    LuminaireVector luminaires = spaces[0].luminaires();
    std::sort(luminaires.begin(), luminaires.end(), WorkspaceObjectNameLess());
    BOOST_FOREACH(Luminaire& luminaire, luminaires){
      translateAndMapModelObject(luminaire);
    }

    // translate people
    PeopleVector people = spaces[0].people();
    std::sort(people.begin(), people.end(), WorkspaceObjectNameLess());
    BOOST_FOREACH(People& person, people){
      translateAndMapModelObject(person);
    }

    // translate electric equipment
    ElectricEquipmentVector electricEquipment = spaces[0].electricEquipment();
    std::sort(electricEquipment.begin(), electricEquipment.end(), WorkspaceObjectNameLess());
    BOOST_FOREACH(ElectricEquipment& equipment, electricEquipment){
      translateAndMapModelObject(equipment);
    }

    // translate gas equipment
    GasEquipmentVector gasEquipment = spaces[0].gasEquipment();
    std::sort(gasEquipment.begin(), gasEquipment.end(), WorkspaceObjectNameLess());
    BOOST_FOREACH(GasEquipment& equipment, gasEquipment){
      translateAndMapModelObject(equipment);
    }

    // translate hot water equipment
    HotWaterEquipmentVector hotWaterEquipment = spaces[0].hotWaterEquipment();
    std::sort(hotWaterEquipment.begin(), hotWaterEquipment.end(), WorkspaceObjectNameLess());
    BOOST_FOREACH(HotWaterEquipment& equipment, hotWaterEquipment){
      translateAndMapModelObject(equipment);
    }

    // translate steam equipment
    SteamEquipmentVector steamEquipment = spaces[0].steamEquipment();
    std::sort(steamEquipment.begin(), steamEquipment.end(), WorkspaceObjectNameLess());
    BOOST_FOREACH(SteamEquipment& equipment, steamEquipment){
      translateAndMapModelObject(equipment);
    }

    // translate other equipment
    OtherEquipmentVector otherEquipment = spaces[0].otherEquipment();
    std::sort(otherEquipment.begin(), otherEquipment.end(), WorkspaceObjectNameLess());
    BOOST_FOREACH(OtherEquipment& equipment, otherEquipment){
      translateAndMapModelObject(equipment);
    }

    // translate daylighting controls
    boost::optional<DaylightingControl> primaryDaylightingControl = modelObject.primaryDaylightingControl();
    if (primaryDaylightingControl){

      boost::optional<DaylightingControl> secondaryDaylightingControl = modelObject.secondaryDaylightingControl();

      int numPoints = 1;
      if (secondaryDaylightingControl){
        numPoints = 2;
      }

      IdfObject daylightingControlObject(openstudio::IddObjectType::Daylighting_Controls);
      m_idfObjects.push_back(daylightingControlObject);

      daylightingControlObject.setString(
          Daylighting_ControlsFields::ZoneName,
          modelObject.name().get());

      daylightingControlObject.setInt(
          Daylighting_ControlsFields::TotalDaylightingReferencePoints,
          numPoints);

      // Primary Control
      daylightingControlObject.setDouble(
          Daylighting_ControlsFields::XCoordinateofFirstReferencePoint,
          primaryDaylightingControl->positionXCoordinate());
      daylightingControlObject.setDouble(
          Daylighting_ControlsFields::YCoordinateofFirstReferencePoint,
          primaryDaylightingControl->positionYCoordinate());
      daylightingControlObject.setDouble(
          Daylighting_ControlsFields::ZCoordinateofFirstReferencePoint,
          primaryDaylightingControl->positionZCoordinate());
      
      if (istringEqual("None", primaryDaylightingControl->lightingControlType())){
        daylightingControlObject.setDouble(
            Daylighting_ControlsFields::FractionofZoneControlledbyFirstReferencePoint,
            0.0);
      }else{
        daylightingControlObject.setDouble(
            Daylighting_ControlsFields::FractionofZoneControlledbyFirstReferencePoint,
            modelObject.fractionofZoneControlledbyPrimaryDaylightingControl());
      }
      
      if (!primaryDaylightingControl->isIlluminanceSetpointDefaulted()){
        daylightingControlObject.setDouble(
            Daylighting_ControlsFields::IlluminanceSetpointatFirstReferencePoint,
            primaryDaylightingControl->illuminanceSetpoint());
      }

      // Secondary Control
      if (secondaryDaylightingControl){
        daylightingControlObject.setDouble(
            Daylighting_ControlsFields::XCoordinateofSecondReferencePoint,
            secondaryDaylightingControl->positionXCoordinate());
        daylightingControlObject.setDouble(
            Daylighting_ControlsFields::YCoordinateofSecondReferencePoint,
            secondaryDaylightingControl->positionYCoordinate());
        daylightingControlObject.setDouble(
            Daylighting_ControlsFields::ZCoordinateofSecondReferencePoint,
            secondaryDaylightingControl->positionZCoordinate());
        
        if (istringEqual("None", secondaryDaylightingControl->lightingControlType())){
          daylightingControlObject.setDouble(
              Daylighting_ControlsFields::FractionofZoneControlledbySecondReferencePoint,
              0.0);
        }else{
          daylightingControlObject.setDouble(
              Daylighting_ControlsFields::FractionofZoneControlledbySecondReferencePoint,
              modelObject.fractionofZoneControlledbySecondaryDaylightingControl());
        }

        if (!secondaryDaylightingControl->isIlluminanceSetpointDefaulted()){
          daylightingControlObject.setDouble(
              Daylighting_ControlsFields::IlluminanceSetpointatSecondReferencePoint,
              secondaryDaylightingControl->illuminanceSetpoint());
        }
      }

      // Shared Data
      std::string lightingControlType = primaryDaylightingControl->lightingControlType();
      if (istringEqual("None", lightingControlType)){
        // fraction for first point have been set to 0, get control type from second point
        lightingControlType = secondaryDaylightingControl->lightingControlType();
      }

      if (istringEqual("None", lightingControlType)){
        // both fractions are 0
      }else if (istringEqual("Continuous", lightingControlType)){
        daylightingControlObject.setInt(Daylighting_ControlsFields::LightingControlType, 1);
      }else if(istringEqual("Stepped", lightingControlType)){
        daylightingControlObject.setInt(Daylighting_ControlsFields::LightingControlType, 2);
      }else if(istringEqual("Continuous/Off", lightingControlType)){
        daylightingControlObject.setInt(Daylighting_ControlsFields::LightingControlType, 3);
      }else{
        LOG(Warn, "Unknown lighting control type '" << lightingControlType << "' for OS:Daylighting:Control " << primaryDaylightingControl->name().get());
      }

      if (primaryDaylightingControl->psiRotationAroundXAxis() != 0.0){
        LOG(Warn, "Rotation of " << primaryDaylightingControl->psiRotationAroundXAxis() << " degrees about X axis not mapped for OS:Daylighting:Control " << primaryDaylightingControl->name().get());
      }

      if (primaryDaylightingControl->phiRotationAroundZAxis() != 0.0){
        LOG(Warn, "Rotation of " << primaryDaylightingControl->phiRotationAroundZAxis() << " degrees about Z axis not mapped for OS:Daylighting:Control " << primaryDaylightingControl->name().get());
      }

      double glareAngle = -openstudio::radToDeg(primaryDaylightingControl->thetaRotationAroundYAxis());
      daylightingControlObject.setDouble(
          Daylighting_ControlsFields::GlareCalculationAzimuthAngleofViewDirectionClockwisefromZoneyAxis,
          glareAngle);

      if (OptionalDouble d = primaryDaylightingControl->maximumAllowableDiscomfortGlareIndex()){
        daylightingControlObject.setDouble(
            Daylighting_ControlsFields::MaximumAllowableDiscomfortGlareIndex,
            *d);
      }

      if (!primaryDaylightingControl->isMinimumInputPowerFractionforContinuousDimmingControlDefaulted()){
        daylightingControlObject.setDouble(
            Daylighting_ControlsFields::MinimumInputPowerFractionforContinuousDimmingControl,
            primaryDaylightingControl->minimumInputPowerFractionforContinuousDimmingControl());
      }

      if (!primaryDaylightingControl->isMinimumLightOutputFractionforContinuousDimmingControlDefaulted()) {
        daylightingControlObject.setDouble(
            Daylighting_ControlsFields::MinimumLightOutputFractionforContinuousDimmingControl,
            primaryDaylightingControl->minimumLightOutputFractionforContinuousDimmingControl());
      }

      if (!primaryDaylightingControl->isNumberofSteppedControlStepsDefaulted()){
        daylightingControlObject.setInt(
            Daylighting_ControlsFields::NumberofSteppedControlSteps,
            primaryDaylightingControl->numberofSteppedControlSteps());
      }

      if (!primaryDaylightingControl->isProbabilityLightingwillbeResetWhenNeededinManualSteppedControlDefaulted()){
        daylightingControlObject.setDouble(
            Daylighting_ControlsFields::ProbabilityLightingwillbeResetWhenNeededinManualSteppedControl,
            primaryDaylightingControl->probabilityLightingwillbeResetWhenNeededinManualSteppedControl());
      }
      
    }

    // translate illuminance map
    boost::optional<IlluminanceMap> illuminanceMap = modelObject.illuminanceMap();
    if (illuminanceMap){

      if (!primaryDaylightingControl){
        LOG(Warn, "Daylighting:Controls object is required to trigger daylighting calculations in EnergyPlus, adding a minimal one to Zone " << modelObject.name().get());

        IdfObject daylightingControlObject(openstudio::IddObjectType::Daylighting_Controls);
        m_idfObjects.push_back(daylightingControlObject);

        daylightingControlObject.setString(Daylighting_ControlsFields::ZoneName, modelObject.name().get());
        daylightingControlObject.setInt(Daylighting_ControlsFields::TotalDaylightingReferencePoints, 1);
        daylightingControlObject.setDouble(Daylighting_ControlsFields::XCoordinateofFirstReferencePoint, 0.0);
        daylightingControlObject.setDouble(Daylighting_ControlsFields::YCoordinateofFirstReferencePoint, 0.0);
        daylightingControlObject.setDouble(Daylighting_ControlsFields::FractionofZoneControlledbyFirstReferencePoint, 0.0);
      }


      IdfObject illuminanceMapObject(openstudio::IddObjectType::Output_IlluminanceMap);
      m_idfObjects.push_back(illuminanceMapObject);

      illuminanceMapObject.setString(Output_IlluminanceMapFields::Name, illuminanceMap->name().get());

      illuminanceMapObject.setString(Output_IlluminanceMapFields::ZoneName, modelObject.name().get());

      illuminanceMapObject.setDouble(Output_IlluminanceMapFields::Zheight, illuminanceMap->originZCoordinate());

      illuminanceMapObject.setDouble(Output_IlluminanceMapFields::XMinimumCoordinate, illuminanceMap->originXCoordinate());
      illuminanceMapObject.setDouble(Output_IlluminanceMapFields::XMaximumCoordinate, illuminanceMap->originXCoordinate() + illuminanceMap->xLength());
      illuminanceMapObject.setInt(Output_IlluminanceMapFields::NumberofXGridPoints, illuminanceMap->numberofXGridPoints());
      
      illuminanceMapObject.setDouble(Output_IlluminanceMapFields::YMinimumCoordinate, illuminanceMap->originYCoordinate());
      illuminanceMapObject.setDouble(Output_IlluminanceMapFields::YMaximumCoordinate, illuminanceMap->originYCoordinate() + illuminanceMap->yLength());
      illuminanceMapObject.setInt(Output_IlluminanceMapFields::NumberofYGridPoints, illuminanceMap->numberofYGridPoints());
      
      if (illuminanceMap->psiRotationAroundXAxis() != 0.0){
        LOG(Warn, "Rotation of " << illuminanceMap->psiRotationAroundXAxis() << " degrees about X axis not mapped for OS:IlluminanceMap " << illuminanceMap->name().get());
      }

      if (illuminanceMap->thetaRotationAroundYAxis() != 0.0){
        LOG(Warn, "Rotation of " << illuminanceMap->thetaRotationAroundYAxis() << " degrees about Y axis not mapped for OS:IlluminanceMap " << illuminanceMap->name().get());
      }

      if (illuminanceMap->phiRotationAroundZAxis() != 0.0){
        LOG(Warn, "Rotation of " << illuminanceMap->phiRotationAroundZAxis() << " degrees about Z axis not mapped for OS:IlluminanceMap " << illuminanceMap->name().get());
      }

    }

    // translate SpaceInfiltration_DesignFlowRate
    SpaceInfiltrationDesignFlowRateVector spaceInfiltrationDesignFlowRates = spaces[0].spaceInfiltrationDesignFlowRates();
    std::sort(spaceInfiltrationDesignFlowRates.begin(), spaceInfiltrationDesignFlowRates.end(), WorkspaceObjectNameLess());
    BOOST_FOREACH(SpaceInfiltrationDesignFlowRate& spaceInfiltrationDesignFlowRate, spaceInfiltrationDesignFlowRates){
      translateAndMapModelObject(spaceInfiltrationDesignFlowRate);
    }

    // translate SpaceInfiltration_EffectiveLeakageArea
    SpaceInfiltrationEffectiveLeakageAreaVector spaceInfiltrationEffectiveLeakageAreas = spaces[0].spaceInfiltrationEffectiveLeakageAreas();
    std::sort(spaceInfiltrationEffectiveLeakageAreas.begin(), spaceInfiltrationEffectiveLeakageAreas.end(), WorkspaceObjectNameLess());
    BOOST_FOREACH(SpaceInfiltrationEffectiveLeakageArea& spaceInfiltrationEffectiveLeakageArea, spaceInfiltrationEffectiveLeakageAreas){
      translateAndMapModelObject(spaceInfiltrationEffectiveLeakageArea);
    }
  }

  // translate thermostat
  if( ( modelObject.equipment().size() > 0 ) || modelObject.useIdealAirLoads() )
  {
    // Thermostat
    boost::optional<ThermostatSetpointDualSetpoint> thermostat;
    boost::optional<IdfObject> idfThermostat;
    if((thermostat = modelObject.thermostatSetpointDualSetpoint()))
    {
      IdfObject zoneControlThermostat(openstudio::IddObjectType::ZoneControl_Thermostat);
      zoneControlThermostat.setString(ZoneControl_ThermostatFields::Name,modelObject.name().get() + " Thermostat");
      zoneControlThermostat.setString(ZoneControl_ThermostatFields::ZoneorZoneListName,modelObject.name().get());
      m_idfObjects.push_back(zoneControlThermostat);

      IdfObject scheduleCompact(openstudio::IddObjectType::Schedule_Compact);
      scheduleCompact.setName(modelObject.name().get() + " Thermostat Schedule");
      m_idfObjects.push_back(scheduleCompact);
      scheduleCompact.setString(1,modelObject.name().get() + " Thermostat Schedule Type Limits");
      scheduleCompact.setString(2,"Through: 12/31");
      scheduleCompact.setString(3,"For: AllDays");
      scheduleCompact.setString(4,"Until: 24:00");
      scheduleCompact.setString(5,"4");

      IdfObject scheduleTypeLimits(openstudio::IddObjectType::ScheduleTypeLimits);
      scheduleTypeLimits.setName(modelObject.name().get() + " Thermostat Schedule Type Limits");
      m_idfObjects.push_back(scheduleTypeLimits);
      scheduleTypeLimits.setString(1,"0");
      scheduleTypeLimits.setString(2,"4");
      scheduleTypeLimits.setString(3,"DISCRETE");

      zoneControlThermostat.setString(ZoneControl_ThermostatFields::ControlTypeScheduleName,scheduleCompact.name().get());

      idfThermostat = translateAndMapModelObject(thermostat.get());

      if( idfThermostat )
      {
        StringVector values(zoneControlThermostat.iddObject().properties().numExtensible);
        values[ZoneControl_ThermostatExtensibleFields::ControlObjectType] = idfThermostat->iddObject().name();
        values[ZoneControl_ThermostatExtensibleFields::ControlName] = idfThermostat->name().get();
        IdfExtensibleGroup eg = zoneControlThermostat.pushExtensibleGroup(values);
      }
    }
  }

  // Ideal air loads
  if( modelObject.useIdealAirLoads() )
  {
    IdfObject idealLoadsAirSystem(IddObjectType::HVACTemplate_Zone_IdealLoadsAirSystem);

    idealLoadsAirSystem.setString(HVACTemplate_Zone_IdealLoadsAirSystemFields::ZoneName,modelObject.name().get());

    m_idfObjects.push_back(idealLoadsAirSystem); 
  }

  ModelObjectVector zoneEquipment = modelObject.equipment();

  if( zoneEquipment.size() > 0 )
  {
    // ZoneHVAC_EquipmentConnections

    IdfObject connectionsObject(openstudio::IddObjectType::ZoneHVAC_EquipmentConnections);
    m_idfObjects.push_back(connectionsObject);

    s = modelObject.name().get();
    connectionsObject.setString(openstudio::ZoneHVAC_EquipmentConnectionsFields::ZoneName,s);

    //set the inlet port list
    PortList inletPortList = modelObject.inletPortList();
    if (inletPortList.modelObjects().size() > 0 )
    {
      boost::optional<IdfObject> _inletNodeList = translateAndMapModelObject(inletPortList);
      if(_inletNodeList)
      {
        s = _inletNodeList->name().get();
        connectionsObject.setString(openstudio::ZoneHVAC_EquipmentConnectionsFields::ZoneAirInletNodeorNodeListName,s);
      }
    }

    //set the exhaust port list
    PortList exhaustPortList = modelObject.exhaustPortList();
    if( exhaustPortList.modelObjects().size() > 0 )
    {
      boost::optional<IdfObject> _exhaustNodeList = translateAndMapModelObject(exhaustPortList);
      if(_exhaustNodeList)
      {
        s = _exhaustNodeList->name().get();
        connectionsObject.setString(openstudio::ZoneHVAC_EquipmentConnectionsFields::ZoneAirExhaustNodeorNodeListName,s);
      }
    }

    //set the zone air node
    Node node = modelObject.zoneAirNode();
    connectionsObject.setString(openstudio::ZoneHVAC_EquipmentConnectionsFields::ZoneAirNodeName,node.name().get());

    //set the zone return air node
    boost::optional<ModelObject> optObj = modelObject.returnAirModelObject();
    if(optObj)
    {
      s = optObj->name().get();
      connectionsObject.setString(openstudio::ZoneHVAC_EquipmentConnectionsFields::ZoneReturnAirNodeName,s);
    }
    else
    {
      s = modelObject.name().get() + " Return Air Node";

      connectionsObject.setString(openstudio::ZoneHVAC_EquipmentConnectionsFields::ZoneReturnAirNodeName,s);
    }

    // ZoneHVAC_EquipmentList
    
    ZoneHVACEquipmentList equipmentList = modelObject.getImpl<model::detail::ThermalZone_Impl>()->zoneHVACEquipmentList(); 

    boost::optional<IdfObject> _equipmentList = translateAndMapModelObject(equipmentList);

    if( _equipmentList )
    {
      s = _equipmentList->name().get();

      connectionsObject.setString(openstudio::ZoneHVAC_EquipmentConnectionsFields::ZoneConditioningEquipmentListName,s);
    }
  }

  // SizingZone 

  if( (zoneEquipment.size() > 0) || modelObject.useIdealAirLoads() )
  {
    // get sizing period objects from the model
    std::vector<SizingPeriod> sizingPeriod = modelObject.model().getModelObjects<SizingPeriod>();
    // map the sizing object only if a sizing period object exists
    boost::optional<IdfObject> sizingZoneIdf;
    if (sizingPeriod.size() > 0){
      SizingZone sizingZone = modelObject.sizingZone();
      sizingZoneIdf = translateAndMapModelObject(sizingZone);
      OS_ASSERT(sizingZoneIdf);
    }

    // map the design specification outdoor air
    boost::optional<DesignSpecificationOutdoorAir> designSpecificationOutdoorAir;
    if (!spaces.empty()){
      designSpecificationOutdoorAir = spaces[0].designSpecificationOutdoorAir();
      if (designSpecificationOutdoorAir){

        translateAndMapModelObject(*designSpecificationOutdoorAir);

        // point the sizing object to the outdoor air spec
        if (sizingZoneIdf){
          sizingZoneIdf->setString(Sizing_ZoneFields::DesignSpecificationOutdoorAirObjectName, designSpecificationOutdoorAir->name().get());
        }

        // create zone ventilation if needed
        // TODO: we could remove all this code if we used ZoneHVAC:IdealLoadsAirSystem instead of HVACTemplate:Zone:IdealLoadsAirSystem
        if (zoneEquipment.empty()){

          double outdoorAirFlowperPerson = designSpecificationOutdoorAir->outdoorAirFlowperPerson();
          double outdoorAirFlowperFloorArea = designSpecificationOutdoorAir->outdoorAirFlowperFloorArea();
          double outdoorAirFlowRate = designSpecificationOutdoorAir->outdoorAirFlowRate();
          double outdoorAirFlowAirChangesperHour = designSpecificationOutdoorAir->outdoorAirFlowAirChangesperHour();

          std::string outdoorAirMethod = designSpecificationOutdoorAir->outdoorAirMethod();
          if (istringEqual(outdoorAirMethod, "Max")){

            double rateForPeople = spaces[0].numberOfPeople()*outdoorAirFlowperPerson;
            double rateForArea = spaces[0].floorArea()*outdoorAirFlowperFloorArea;
            double rate = outdoorAirFlowRate;
            double rateForVolume = spaces[0].volume()*outdoorAirFlowAirChangesperHour;

            double biggestRate = std::max(rateForPeople, std::max(rateForArea, std::max(rate, rateForVolume)));

            if (rateForPeople == biggestRate){
              //outdoorAirFlowperPerson = 0;
              outdoorAirFlowperFloorArea = 0;
              outdoorAirFlowRate = 0;
              outdoorAirFlowAirChangesperHour = 0;
            }else if(rateForArea == biggestRate){
              outdoorAirFlowperPerson = 0;
              //outdoorAirFlowperFloorArea = 0;
              outdoorAirFlowRate = 0;
              outdoorAirFlowAirChangesperHour = 0;
            }else if(rate == biggestRate){
              outdoorAirFlowperPerson = 0;
              outdoorAirFlowperFloorArea = 0;
              //outdoorAirFlowRate = 0;
              outdoorAirFlowAirChangesperHour = 0;
            }else{
              //rateForVolume == biggestRate
              outdoorAirFlowperPerson = 0;
              outdoorAirFlowperFloorArea = 0;
              outdoorAirFlowRate = 0;
              //outdoorAirFlowAirChangesperHour = 0;
            }
          
          }else{
            // sum
          }

          if (outdoorAirFlowperPerson > 0){

            // todo: improve this?
            // find first people schedule
            std::vector<People> allPeople;
            BOOST_FOREACH(People people, spaces[0].people()){
              allPeople.push_back(people);
            }
            if (spaces[0].spaceType()){
              BOOST_FOREACH(People people, spaces[0].spaceType()->people()){
                allPeople.push_back(people);
              }
            }

            boost::optional<Schedule> peopleSchedule;
            BOOST_FOREACH(People people, allPeople){
              peopleSchedule = people.numberofPeopleSchedule();
              if (peopleSchedule){
                break;
              }
            }

            if (peopleSchedule){
              IdfObject zoneVentilation(IddObjectType::ZoneVentilation_DesignFlowRate);
              zoneVentilation.setName(modelObject.name().get() + " Ventilation per Person");
              zoneVentilation.setString(ZoneVentilation_DesignFlowRateFields::ZoneorZoneListName, modelObject.name().get());
              zoneVentilation.setString(ZoneVentilation_DesignFlowRateFields::ScheduleName, peopleSchedule->name().get());
              zoneVentilation.setString(ZoneVentilation_DesignFlowRateFields::DesignFlowRateCalculationMethod, "Flow/Person");
              zoneVentilation.setDouble(ZoneVentilation_DesignFlowRateFields::FlowRateperPerson, outdoorAirFlowperPerson);
              m_idfObjects.push_back(zoneVentilation);
            }else{
              LOG(Warn, "No People found in ThermalZone '" << modelObject.name().get() << "', outdoor air per person will not be added");
            }
          }

          if (outdoorAirFlowperFloorArea > 0){
            IdfObject zoneVentilation(IddObjectType::ZoneVentilation_DesignFlowRate);
            zoneVentilation.setName(modelObject.name().get() + " Ventilation per Floor Area");
            zoneVentilation.setString(ZoneVentilation_DesignFlowRateFields::ZoneorZoneListName, modelObject.name().get());
            zoneVentilation.setString(ZoneVentilation_DesignFlowRateFields::ScheduleName, this->alwaysOnSchedule().name().get()); 
            zoneVentilation.setString(ZoneVentilation_DesignFlowRateFields::DesignFlowRateCalculationMethod, "Flow/Area");
            zoneVentilation.setDouble(ZoneVentilation_DesignFlowRateFields::FlowRateperZoneFloorArea, outdoorAirFlowperFloorArea);
            m_idfObjects.push_back(zoneVentilation);
          }

          if (outdoorAirFlowRate > 0){
            IdfObject zoneVentilation(IddObjectType::ZoneVentilation_DesignFlowRate);
            zoneVentilation.setName(modelObject.name().get() + " Ventilation Rate");
            zoneVentilation.setString(ZoneVentilation_DesignFlowRateFields::ZoneorZoneListName, modelObject.name().get());
            zoneVentilation.setString(ZoneVentilation_DesignFlowRateFields::ScheduleName, this->alwaysOnSchedule().name().get()); 
            zoneVentilation.setString(ZoneVentilation_DesignFlowRateFields::DesignFlowRateCalculationMethod, "Flow/Zone");
            zoneVentilation.setDouble(ZoneVentilation_DesignFlowRateFields::DesignFlowRate, outdoorAirFlowRate);
            m_idfObjects.push_back(zoneVentilation);
          }

          if (outdoorAirFlowAirChangesperHour > 0){
            IdfObject zoneVentilation(IddObjectType::ZoneVentilation_DesignFlowRate);
            zoneVentilation.setName(modelObject.name().get() + " Ventilation Air Changes per Hour");
            zoneVentilation.setString(ZoneVentilation_DesignFlowRateFields::ZoneorZoneListName, modelObject.name().get());
            zoneVentilation.setString(ZoneVentilation_DesignFlowRateFields::ScheduleName, this->alwaysOnSchedule().name().get()); 
            zoneVentilation.setString(ZoneVentilation_DesignFlowRateFields::DesignFlowRateCalculationMethod, "AirChanges/Hour");
            zoneVentilation.setDouble(ZoneVentilation_DesignFlowRateFields::AirChangesperHour, outdoorAirFlowAirChangesperHour);
            m_idfObjects.push_back(zoneVentilation);
          }
        }
      }
    }
  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

