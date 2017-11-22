/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"

#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/PortList.hpp"
#include "../../model/PortList_Impl.hpp"
#include "../../model/ZoneHVACEquipmentList.hpp"
#include "../../model/ZoneHVACEquipmentList_Impl.hpp"
#include "../../model/ZoneVentilationDesignFlowRate.hpp"
#include "../../model/ZoneVentilationDesignFlowRate_Impl.hpp"
#include "../../model/SizingZone.hpp"
#include "../../model/SizingZone_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ScheduleCompact.hpp"
#include "../../model/ScheduleCompact_Impl.hpp"
#include "../../model/SetpointManagerMixedAir.hpp"
#include "../../model/SetpointManagerSingleZoneReheat.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVAC_Impl.hpp"
#include "../../model/Thermostat.hpp"
#include "../../model/Thermostat_Impl.hpp"
#include "../../model/ThermostatSetpointDualSetpoint.hpp"
#include "../../model/ThermostatSetpointDualSetpoint_Impl.hpp"
#include "../../model/ZoneControlContaminantController.hpp"
#include "../../model/ZoneControlHumidistat.hpp"
#include "../../model/ZoneControlThermostatStagedDualSetpoint.hpp"
#include "../../model/DesignSpecificationOutdoorAir.hpp"
#include "../../model/DesignSpecificationOutdoorAir_Impl.hpp"
#include "../../model/SizingPeriod.hpp"
#include "../../model/SizingPeriod_Impl.hpp"
#include "../../model/ModelObject.hpp"
#include "../../model/ModelObject_Impl.hpp"

#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/ShadingSurfaceGroup.hpp"
#include "../../model/ShadingSurfaceGroup_Impl.hpp"
#include "../../model/ShadingSurface.hpp"
#include "../../model/ShadingSurface_Impl.hpp"
#include "../../model/InteriorPartitionSurfaceGroup.hpp"
#include "../../model/InteriorPartitionSurfaceGroup_Impl.hpp"
#include "../../model/InteriorPartitionSurface.hpp"
#include "../../model/InteriorPartitionSurface_Impl.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/SubSurface.hpp"
#include "../../model/SubSurface_Impl.hpp"
#include "../../model/ConstructionBase.hpp"
#include "../../model/ConstructionBase_Impl.hpp"
#include "../../model/InternalMass.hpp"
#include "../../model/InternalMass_Impl.hpp"
#include "../../model/InternalMassDefinition.hpp"
#include "../../model/InternalMassDefinition_Impl.hpp"
#include "../../model/People.hpp"
#include "../../model/People_Impl.hpp"
#include "../../model/PeopleDefinition.hpp"
#include "../../model/PeopleDefinition_Impl.hpp"
#include "../../model/Lights.hpp"
#include "../../model/Lights_Impl.hpp"
#include "../../model/LightsDefinition.hpp"
#include "../../model/LightsDefinition_Impl.hpp"
#include "../../model/Luminaire.hpp"
#include "../../model/Luminaire_Impl.hpp"
#include "../../model/LuminaireDefinition.hpp"
#include "../../model/LuminaireDefinition_Impl.hpp"
#include "../../model/ElectricEquipment.hpp"
#include "../../model/ElectricEquipment_Impl.hpp"
#include "../../model/ElectricEquipmentDefinition.hpp"
#include "../../model/ElectricEquipmentDefinition_Impl.hpp"
#include "../../model/GasEquipment.hpp"
#include "../../model/GasEquipment_Impl.hpp"
#include "../../model/GasEquipmentDefinition.hpp"
#include "../../model/GasEquipmentDefinition_Impl.hpp"
#include "../../model/HotWaterEquipment.hpp"
#include "../../model/HotWaterEquipment_Impl.hpp"
#include "../../model/SteamEquipment.hpp"
#include "../../model/SteamEquipment_Impl.hpp"
#include "../../model/OtherEquipment.hpp"
#include "../../model/OtherEquipment_Impl.hpp"
#include "../../model/DaylightingControl.hpp"
#include "../../model/DaylightingControl_Impl.hpp"
#include "../../model/IlluminanceMap.hpp"
#include "../../model/IlluminanceMap_Impl.hpp"
#include "../../model/SpaceInfiltrationDesignFlowRate.hpp"
#include "../../model/SpaceInfiltrationDesignFlowRate_Impl.hpp"
#include "../../model/SpaceInfiltrationEffectiveLeakageArea.hpp"
#include "../../model/SpaceInfiltrationEffectiveLeakageArea_Impl.hpp"
#include "../../model/GlareSensor.hpp"
#include "../../model/GlareSensor_Impl.hpp"
#include "../../model/LifeCycleCost.hpp"
#include "../../model/ZoneMixing.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/WorkspaceObjectOrder.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/OS_ThermalZone_FieldEnums.hxx>
#include <utilities/idd/Zone_FieldEnums.hxx>
#include <utilities/idd/HVACTemplate_Zone_IdealLoadsAirSystem_FieldEnums.hxx>
#include <utilities/idd/Daylighting_Controls_FieldEnums.hxx>
#include <utilities/idd/Daylighting_ReferencePoint_FieldEnums.hxx>
#include <utilities/idd/Output_IlluminanceMap_FieldEnums.hxx>
#include <utilities/idd/Schedule_Compact_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_EquipmentConnections_FieldEnums.hxx>
#include <utilities/idd/ZoneControl_Thermostat_FieldEnums.hxx>
#include <utilities/idd/ZoneControl_Humidistat_FieldEnums.hxx>
#include <utilities/idd/Sizing_Zone_FieldEnums.hxx>
#include <utilities/idd/DesignSpecification_OutdoorAir_FieldEnums.hxx>
#include <utilities/idd/ZoneVentilation_DesignFlowRate_FieldEnums.hxx>

#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include "../../utilities/geometry/Geometry.hpp"
#include <algorithm>

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

  for (LifeCycleCost lifeCycleCost : modelObject.lifeCycleCosts()){
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

  // DLM: currently there is no setter for floorArea and the getter does not return the value from this field
  if (modelObject.getDouble(openstudio::OS_ThermalZoneFields::FloorArea)){
    idfObject.setDouble(openstudio::ZoneFields::FloorArea, modelObject.getDouble(openstudio::OS_ThermalZoneFields::FloorArea).get());
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
    for (ShadingSurfaceGroup& shadingSurfaceGroup : shadingSurfaceGroups){
      translateAndMapModelObject(shadingSurfaceGroup);
    }

    // translate interior surface partition groups
    InteriorPartitionSurfaceGroupVector interiorPartitionSurfaceGroups = spaces[0].interiorPartitionSurfaceGroups();
    std::sort(interiorPartitionSurfaceGroups.begin(), interiorPartitionSurfaceGroups.end(), WorkspaceObjectNameLess());
    for (InteriorPartitionSurfaceGroup& interiorPartitionSurfaceGroup : interiorPartitionSurfaceGroups){
      translateAndMapModelObject(interiorPartitionSurfaceGroup);
    }

    // translate surfaces
    SurfaceVector surfaces = spaces[0].surfaces();
    std::sort(surfaces.begin(), surfaces.end(), WorkspaceObjectNameLess());
    for (Surface& surface : surfaces){
      translateAndMapModelObject(surface);
    }

    // translate internal mass
    InternalMassVector internalMasses = spaces[0].internalMass();
    std::sort(internalMasses.begin(), internalMasses.end(), WorkspaceObjectNameLess());
    for (InternalMass& internalMass : internalMasses){
      translateAndMapModelObject(internalMass);
    }

    // translate lights
    LightsVector lights = spaces[0].lights();
    std::sort(lights.begin(), lights.end(), WorkspaceObjectNameLess());
    for (Lights& light : lights){
      translateAndMapModelObject(light);
    }

    // translate luminaires
    LuminaireVector luminaires = spaces[0].luminaires();
    std::sort(luminaires.begin(), luminaires.end(), WorkspaceObjectNameLess());
    for (Luminaire& luminaire : luminaires){
      translateAndMapModelObject(luminaire);
    }

    // translate people
    PeopleVector people = spaces[0].people();
    std::sort(people.begin(), people.end(), WorkspaceObjectNameLess());
    for (People& person : people){
      translateAndMapModelObject(person);
    }

    // translate electric equipment
    ElectricEquipmentVector electricEquipment = spaces[0].electricEquipment();
    std::sort(electricEquipment.begin(), electricEquipment.end(), WorkspaceObjectNameLess());
    for (ElectricEquipment& equipment : electricEquipment){
      translateAndMapModelObject(equipment);
    }

    // translate gas equipment
    GasEquipmentVector gasEquipment = spaces[0].gasEquipment();
    std::sort(gasEquipment.begin(), gasEquipment.end(), WorkspaceObjectNameLess());
    for (GasEquipment& equipment : gasEquipment){
      translateAndMapModelObject(equipment);
    }

    // translate hot water equipment
    HotWaterEquipmentVector hotWaterEquipment = spaces[0].hotWaterEquipment();
    std::sort(hotWaterEquipment.begin(), hotWaterEquipment.end(), WorkspaceObjectNameLess());
    for (HotWaterEquipment& equipment : hotWaterEquipment){
      translateAndMapModelObject(equipment);
    }

    // translate steam equipment
    SteamEquipmentVector steamEquipment = spaces[0].steamEquipment();
    std::sort(steamEquipment.begin(), steamEquipment.end(), WorkspaceObjectNameLess());
    for (SteamEquipment& equipment : steamEquipment){
      translateAndMapModelObject(equipment);
    }

    // translate other equipment
    OtherEquipmentVector otherEquipment = spaces[0].otherEquipment();
    std::sort(otherEquipment.begin(), otherEquipment.end(), WorkspaceObjectNameLess());
    for (OtherEquipment& equipment : otherEquipment){
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
          modelObject.nameString());

      // Primary Control
      IdfObject primaryReferencePoint(openstudio::IddObjectType::Daylighting_ReferencePoint);
      primaryReferencePoint.setName(primaryDaylightingControl->nameString());
      m_idfObjects.push_back(primaryReferencePoint);

      OptionalSpace refSpace = primaryDaylightingControl->space();
      if (refSpace){
        OptionalThermalZone refThermalZone = refSpace->thermalZone();
        if (refThermalZone){
          primaryReferencePoint.setString(Daylighting_ReferencePointFields::ZoneName, refThermalZone->nameString());
        }
      }

      primaryReferencePoint.setDouble(
          Daylighting_ReferencePointFields::XCoordinateofReferencePoint,
          primaryDaylightingControl->positionXCoordinate());
      primaryReferencePoint.setDouble(
          Daylighting_ReferencePointFields::YCoordinateofReferencePoint,
          primaryDaylightingControl->positionYCoordinate());
      primaryReferencePoint.setDouble(
          Daylighting_ReferencePointFields::ZCoordinateofReferencePoint,
          primaryDaylightingControl->positionZCoordinate());

      double primaryFrac = modelObject.fractionofZoneControlledbyPrimaryDaylightingControl();
      if (istringEqual("None", primaryDaylightingControl->lightingControlType())){
        if (primaryFrac > 0.0){
          primaryFrac = 0.0;
          LOG(Warn, "Fraction of Zone Controlled by Primary Daylight Control is " << primaryFrac << " but lighting control type is 'None'. Reseting Primary Fraction to " << 0.0);
        }
      }
      std::string fractionofZoneControlledbyFirstReferencePoint = toString(primaryFrac);
      std::string illuminanceSetpointatFirstReferencePoint = toString(primaryDaylightingControl->illuminanceSetpoint());

      std::vector<std::string> firstGroup;
      firstGroup.push_back(primaryReferencePoint.nameString());
      firstGroup.push_back(fractionofZoneControlledbyFirstReferencePoint);
      firstGroup.push_back(illuminanceSetpointatFirstReferencePoint);
      daylightingControlObject.pushExtensibleGroup(firstGroup);

      // Secondary Control
      if (secondaryDaylightingControl){
        IdfObject secondaryReferencePoint(openstudio::IddObjectType::Daylighting_ReferencePoint);
        secondaryReferencePoint.setName(secondaryDaylightingControl->nameString());
        m_idfObjects.push_back(secondaryReferencePoint);

        refSpace = secondaryDaylightingControl->space();
        if (refSpace){
          OptionalThermalZone refThermalZone = refSpace->thermalZone();
          if (refThermalZone){
            secondaryReferencePoint.setString(Daylighting_ReferencePointFields::ZoneName, refThermalZone->nameString());
          }
        }

        secondaryReferencePoint.setString(Daylighting_ReferencePointFields::ZoneName, modelObject.nameString());
        secondaryReferencePoint.setDouble(
            Daylighting_ReferencePointFields::XCoordinateofReferencePoint,
            secondaryDaylightingControl->positionXCoordinate());
        secondaryReferencePoint.setDouble(
            Daylighting_ReferencePointFields::YCoordinateofReferencePoint,
            secondaryDaylightingControl->positionYCoordinate());
        secondaryReferencePoint.setDouble(
            Daylighting_ReferencePointFields::ZCoordinateofReferencePoint,
            secondaryDaylightingControl->positionZCoordinate());

        
        double secondaryFrac = modelObject.fractionofZoneControlledbySecondaryDaylightingControl();
        if (istringEqual("None", secondaryDaylightingControl->lightingControlType())){
          if (secondaryFrac > 0.0){
            secondaryFrac = 0.0;
            LOG(Warn, "Fraction of Zone Controlled by Secondary Daylight Control is " << secondaryFrac << " but lighting control type is 'None'. Reseting Secondary Fraction to " << 0.0);
          }
        }else{
          if ((primaryFrac + secondaryFrac) > 1) {
            // Reset secondary to 1 - Primary
            secondaryFrac = std::max(1.0 - primaryFrac, 0.0);

            LOG(Warn, "Fraction of Zone Controlled by Primary Daylight Control is " << primaryFrac << " while Secondary Fraction is "
                  << secondaryFrac << ". Reseting Secondary Fraction to " << secondaryFrac);
            
          }
        }
        std::string fractionofZoneControlledbySecondReferencePoint = toString(secondaryFrac);
        std::string illuminanceSetpointatSecondReferencePoint = toString(secondaryDaylightingControl->illuminanceSetpoint());

        std::vector<std::string> secondGroup;
        secondGroup.push_back(secondaryReferencePoint.nameString());
        secondGroup.push_back(fractionofZoneControlledbySecondReferencePoint);
        secondGroup.push_back(illuminanceSetpointatSecondReferencePoint);
        daylightingControlObject.pushExtensibleGroup(secondGroup);

      }

      // Shared Data
      std::string lightingControlType = primaryDaylightingControl->lightingControlType();
      if (istringEqual("None", lightingControlType)){
        // fraction for first point have been set to 0, try to get control type from second point
        if (secondaryDaylightingControl){
          lightingControlType = secondaryDaylightingControl->lightingControlType();
        }
      }

      if (istringEqual("None", lightingControlType)){
        // both fractions are 0
      }else if (istringEqual("Continuous", lightingControlType)){
        daylightingControlObject.setString(Daylighting_ControlsFields::LightingControlType, "Continuous");
      }else if(istringEqual("Stepped", lightingControlType)){
        daylightingControlObject.setString(Daylighting_ControlsFields::LightingControlType, "Stepped");
      }else if(istringEqual("Continuous/Off", lightingControlType)){
        daylightingControlObject.setString(Daylighting_ControlsFields::LightingControlType, "ContinuousOff");
      }else{
        LOG(Warn, "Unknown lighting control type '" << lightingControlType << "' for OS:Daylighting:Control " << primaryDaylightingControl->name().get());
      }

      if (primaryDaylightingControl->psiRotationAroundXAxis() != 0.0){
        LOG(Warn, "Rotation of " << primaryDaylightingControl->psiRotationAroundXAxis() << " degrees about X axis not mapped for OS:Daylighting:Control " << primaryDaylightingControl->name().get());
      }

      if (primaryDaylightingControl->phiRotationAroundZAxis() != 0.0){
        LOG(Warn, "Rotation of " << primaryDaylightingControl->phiRotationAroundZAxis() << " degrees about Z axis not mapped for OS:Daylighting:Control " << primaryDaylightingControl->name().get());
      }

      // glare
      double glareAngle = -openstudio::radToDeg(primaryDaylightingControl->thetaRotationAroundYAxis());
      daylightingControlObject.setDouble(
          Daylighting_ControlsFields::GlareCalculationAzimuthAngleofViewDirectionClockwisefromZoneyAxis,
          glareAngle);

      if (OptionalDouble d = primaryDaylightingControl->maximumAllowableDiscomfortGlareIndex()){
        daylightingControlObject.setDouble(
          Daylighting_ControlsFields::MaximumAllowableDiscomfortGlareIndex,
          *d);
      }

      daylightingControlObject.setString(
        Daylighting_ControlsFields::GlareCalculationDaylightingReferencePointName,
        primaryReferencePoint.nameString());


      if (!primaryDaylightingControl->isMinimumInputPowerFractionforContinuousDimmingControlDefaulted()){
        daylightingControlObject.setDouble(
            Daylighting_ControlsFields::MinimumInputPowerFractionforContinuousorContinuousOffDimmingControl,
            primaryDaylightingControl->minimumInputPowerFractionforContinuousDimmingControl());
      }

      if (!primaryDaylightingControl->isMinimumLightOutputFractionforContinuousDimmingControlDefaulted()) {
        daylightingControlObject.setDouble(
            Daylighting_ControlsFields::MinimumLightOutputFractionforContinuousorContinuousOffDimmingControl,
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

        IdfObject referencePoint(openstudio::IddObjectType::Daylighting_ReferencePoint);
        referencePoint.setName(modelObject.nameString() + " Daylighting Reference Point");
        m_idfObjects.push_back(referencePoint);
        referencePoint.setString(Daylighting_ReferencePointFields::ZoneName, modelObject.nameString());
        referencePoint.setDouble(Daylighting_ReferencePointFields::XCoordinateofReferencePoint, illuminanceMap->originXCoordinate() + 0.5*illuminanceMap->xLength());
        referencePoint.setDouble(Daylighting_ReferencePointFields::YCoordinateofReferencePoint, illuminanceMap->originYCoordinate() + 0.5*illuminanceMap->yLength());
        referencePoint.setDouble(Daylighting_ReferencePointFields::ZCoordinateofReferencePoint, illuminanceMap->originZCoordinate());

        IdfObject daylightingControlObject(openstudio::IddObjectType::Daylighting_Controls);
        m_idfObjects.push_back(daylightingControlObject);

        daylightingControlObject.setString(Daylighting_ControlsFields::ZoneName, modelObject.nameString());
        std::vector<std::string> group;
        group.push_back(referencePoint.nameString()); // ref point name
        group.push_back("0.0"); // fraction controlled
        group.push_back(""); // illuminance setpoint
        daylightingControlObject.pushExtensibleGroup(group);
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
    for (SpaceInfiltrationDesignFlowRate& spaceInfiltrationDesignFlowRate : spaceInfiltrationDesignFlowRates){
      translateAndMapModelObject(spaceInfiltrationDesignFlowRate);
    }

    // translate SpaceInfiltration_EffectiveLeakageArea
    SpaceInfiltrationEffectiveLeakageAreaVector spaceInfiltrationEffectiveLeakageAreas = spaces[0].spaceInfiltrationEffectiveLeakageAreas();
    std::sort(spaceInfiltrationEffectiveLeakageAreas.begin(), spaceInfiltrationEffectiveLeakageAreas.end(), WorkspaceObjectNameLess());
    for (SpaceInfiltrationEffectiveLeakageArea& spaceInfiltrationEffectiveLeakageArea : spaceInfiltrationEffectiveLeakageAreas){
      translateAndMapModelObject(spaceInfiltrationEffectiveLeakageArea);
    }
  }

  // translate zone mixing objects which supply air to this zone
  ZoneMixingVector supplyZoneMixing = modelObject.supplyZoneMixing();
  std::sort(supplyZoneMixing.begin(), supplyZoneMixing.end(), WorkspaceObjectNameLess());
  for (ZoneMixing& mixing : supplyZoneMixing){
    translateAndMapModelObject(mixing);
  }

  auto zoneEquipment = modelObject.equipment();

  // In OS ZoneVentilationDesignFlowRate is considered zone equipment,
  // but for the E+ perspective it is not so we have to remove them,
  // and treat them differently.
  auto isZoneVentilationDesignFlowRate = [](const ModelObject & mo) {
    return (mo.iddObjectType() == ZoneVentilationDesignFlowRate::iddObjectType());
  };

  std::vector<model::ModelObject> zoneVentilationObjects;
  std::copy_if(zoneEquipment.begin(),zoneEquipment.end(),std::back_inserter(zoneVentilationObjects),isZoneVentilationDesignFlowRate);

  auto zoneVentilationBegin = std::remove_if(zoneEquipment.begin(),zoneEquipment.end(),isZoneVentilationDesignFlowRate);
  zoneEquipment.erase(zoneVentilationBegin,zoneEquipment.end());

  // translate thermostat and/or humidistat
  if( ( zoneEquipment.size() > 0 ) || modelObject.useIdealAirLoads() )
  {
    // Thermostat
    if( auto thermostat = modelObject.thermostat() )
    {
      if( thermostat->iddObjectType() == ZoneControlThermostatStagedDualSetpoint::iddObjectType() )
      {
        translateAndMapModelObject(thermostat.get());
      } else {
        auto createZoneControlThermostat = [&]() {
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

          if( boost::optional<IdfObject> idfThermostat = translateAndMapModelObject(thermostat.get()) )
          {
            StringVector values(zoneControlThermostat.iddObject().properties().numExtensible);
            values[ZoneControl_ThermostatExtensibleFields::ControlObjectType] = idfThermostat->iddObject().name();
            values[ZoneControl_ThermostatExtensibleFields::ControlName] = idfThermostat->name().get();
            IdfExtensibleGroup eg = zoneControlThermostat.pushExtensibleGroup(values);
          }
        };

        // Only translate ThermostatSetpointDualSetpoint if there are schedules attached
        if( auto dualSetpoint = thermostat->optionalCast<ThermostatSetpointDualSetpoint>() ) {
          if( dualSetpoint->heatingSetpointTemperatureSchedule() && dualSetpoint->coolingSetpointTemperatureSchedule() ) {
            createZoneControlThermostat();
          }
        } else {
          createZoneControlThermostat();
        }
      }
    }

    // Humidistat
    if( boost::optional<ZoneControlHumidistat> humidistat = modelObject.zoneControlHumidistat() )
    {
      if( boost::optional<IdfObject> idfHumidistat = translateAndMapModelObject(humidistat.get()) ) {
        idfHumidistat->setString(ZoneControl_HumidistatFields::ZoneName,modelObject.name().get());
      }
    }

    // ZoneControlContaminantController
    if( auto controller = modelObject.zoneControlContaminantController() ) {
      translateAndMapModelObject(controller.get());
    }
  }

  // Ideal air loads
  if( modelObject.useIdealAirLoads() )
  {
    IdfObject idealLoadsAirSystem(IddObjectType::HVACTemplate_Zone_IdealLoadsAirSystem);

    idealLoadsAirSystem.setString(HVACTemplate_Zone_IdealLoadsAirSystemFields::ZoneName,modelObject.name().get());

    m_idfObjects.push_back(idealLoadsAirSystem);
  }

  // ZoneVentilationDesignFlowRate does not go on equipment connections or associated list
  for( auto & zone_vent : zoneVentilationObjects ) {
    translateAndMapModelObject(zone_vent);
  }

  if( zoneEquipment.size() > 0 ) {
    // ZoneHVAC_EquipmentConnections
    IdfObject connectionsObject(openstudio::IddObjectType::ZoneHVAC_EquipmentConnections);
    m_idfObjects.push_back(connectionsObject);

    s = modelObject.name().get();
    std::string name = s;
    connectionsObject.setString(openstudio::ZoneHVAC_EquipmentConnectionsFields::ZoneName,s);

    //set the inlet port list
    PortList inletPortList = modelObject.inletPortList();
    if (inletPortList.modelObjects().size() > 0 )
    {
      boost::optional<IdfObject> _inletNodeList = translateAndMapModelObject(inletPortList);
      if(_inletNodeList)
      {
        _inletNodeList->setName(name + " Inlet Node List");
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
        _exhaustNodeList->setName(name + " Exhaust Node List");
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
      connectionsObject.setString(openstudio::ZoneHVAC_EquipmentConnectionsFields::ZoneReturnAirNodeorNodeListName,s);
    }
    else
    {
      s = modelObject.name().get() + " Return Air Node";

      connectionsObject.setString(openstudio::ZoneHVAC_EquipmentConnectionsFields::ZoneReturnAirNodeorNodeListName,s);
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
            for (People people : spaces[0].people()){
              allPeople.push_back(people);
            }
            if (spaces[0].spaceType()){
              for (People people : spaces[0].spaceType()->people()){
                allPeople.push_back(people);
              }
            }

            boost::optional<Schedule> peopleSchedule;
            for (People people : allPeople){
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

