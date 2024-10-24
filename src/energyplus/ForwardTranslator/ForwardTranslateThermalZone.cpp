/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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
#include "../../model/ZoneHVACIdealLoadsAirSystem.hpp"
#include "../../model/ZoneHVACIdealLoadsAirSystem_Impl.hpp"
#include "../../model/ZoneVentilationDesignFlowRate.hpp"
#include "../../model/ZoneVentilationDesignFlowRate_Impl.hpp"
#include "../../model/ZoneVentilationWindandStackOpenArea.hpp"
#include "../../model/ZoneVentilationWindandStackOpenArea_Impl.hpp"
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
#include "../../model/AirLoopHVACReturnPlenum.hpp"
#include "../../model/AirLoopHVACReturnPlenum_Impl.hpp"
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
#include "../../model/ElectricEquipmentITEAirCooled.hpp"
#include "../../model/ElectricEquipmentITEAirCooled_Impl.hpp"
#include "../../model/ElectricEquipmentITEAirCooledDefinition.hpp"
#include "../../model/ElectricEquipmentITEAirCooledDefinition_Impl.hpp"
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
#include "../../model/SpaceInfiltrationFlowCoefficient.hpp"
#include "../../model/SpaceInfiltrationFlowCoefficient_Impl.hpp"
#include "../../model/GlareSensor.hpp"
#include "../../model/GlareSensor_Impl.hpp"
#include "../../model/LifeCycleCost.hpp"
#include "../../model/ZoneMixing.hpp"
#include "../../model/ConstructionAirBoundary.hpp"

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
#include <numeric>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateThermalZone(ThermalZone& modelObject) {

    std::string tzName = modelObject.nameString();

    std::vector<Space> spaces = modelObject.spaces();
    // INCLUDE SPACE TRANSLATION (new behavior)
    if (spaces.empty()) {
      LOG(Warn, "ThermalZone " << tzName << " does not have any geometry or loads associated with it. It will not be translated");
      return boost::none;
    }

    // ThermalZone

    // create, register, and name object
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Zone, modelObject);

    for (LifeCycleCost lifeCycleCost : modelObject.lifeCycleCosts()) {
      translateAndMapModelObject(lifeCycleCost);
    }

    idfObject.setString(openstudio::ZoneFields::Type, "");

    idfObject.setInt(openstudio::ZoneFields::Multiplier, modelObject.multiplier());

    if (modelObject.ceilingHeight()) {
      idfObject.setDouble(openstudio::ZoneFields::CeilingHeight, modelObject.ceilingHeight().get());
    }

    if (modelObject.volume()) {
      idfObject.setDouble(openstudio::ZoneFields::Volume, modelObject.volume().get());
    }

    // DLM: currently there is no setter for floorArea and the getter does not return the value from this field
    if (modelObject.getDouble(openstudio::OS_ThermalZoneFields::FloorArea)) {
      idfObject.setDouble(openstudio::ZoneFields::FloorArea, modelObject.getDouble(openstudio::OS_ThermalZoneFields::FloorArea).get());
    }

    if (modelObject.zoneInsideConvectionAlgorithm()) {
      idfObject.setString(openstudio::ZoneFields::ZoneInsideConvectionAlgorithm, modelObject.zoneInsideConvectionAlgorithm().get());
    }

    if (modelObject.zoneOutsideConvectionAlgorithm()) {
      idfObject.setString(openstudio::ZoneFields::ZoneOutsideConvectionAlgorithm, modelObject.zoneOutsideConvectionAlgorithm().get());
    }

    // Spaces

    // Figure out the x, y, z and whether we write floor area or not
    if (m_forwardTranslatorOptions.excludeSpaceTranslation()) {
      OS_ASSERT(spaces.size() == 1);
      auto& space = spaces[0];

      idfObject.setDouble(openstudio::ZoneFields::DirectionofRelativeNorth, spaces[0].directionofRelativeNorth());
      idfObject.setDouble(openstudio::ZoneFields::XOrigin, space.xOrigin());
      idfObject.setDouble(openstudio::ZoneFields::YOrigin, space.yOrigin());
      idfObject.setDouble(openstudio::ZoneFields::ZOrigin, space.zOrigin());

      if (space.partofTotalFloorArea()) {
        idfObject.setString(openstudio::ZoneFields::PartofTotalFloorArea, "Yes");
      } else {
        idfObject.setString(openstudio::ZoneFields::PartofTotalFloorArea, "No");
      }

    } else {
      // sort by space name
      std::sort(spaces.begin(), spaces.end(), WorkspaceObjectNameLess());
      boost::optional<double> directionofRelativeNorth;
      if (!spaces[0].isDirectionofRelativeNorthDefaulted()) {
        directionofRelativeNorth = spaces[0].directionofRelativeNorth();
      }
      double xOrigin = spaces[0].xOrigin();
      double yOrigin = spaces[0].yOrigin();
      double zOrigin = spaces[0].zOrigin();
      double totalFloorArea = 0.0;  // only area included in total floor area
      bool needToSetFloorArea = false;
      bool anyNotPartofTotalFloorArea = false;
      bool partofTotalFloorArea = spaces[0].partofTotalFloorArea();

      for (const Space& space : spaces) {

        // if all spaces have same directionOfRelativeNorth use that, otherwise clear it
        if (!space.isDirectionofRelativeNorthDefaulted()) {
          if (directionofRelativeNorth && (*directionofRelativeNorth == space.directionofRelativeNorth())) {
            // no-op
          } else {
            directionofRelativeNorth.reset();
          }
        }

        // pick the lower left corner if specified
        xOrigin = std::min(xOrigin, space.xOrigin());
        yOrigin = std::min(yOrigin, space.yOrigin());
        zOrigin = std::min(zOrigin, space.zOrigin());

        double floorArea = space.floorArea();

        // space floor area is counted if any space is part of floor area
        if (space.partofTotalFloorArea()) {
          partofTotalFloorArea = true;
          totalFloorArea += floorArea;

          if (anyNotPartofTotalFloorArea) {
            needToSetFloorArea = true;
          }

        } else {
          anyNotPartofTotalFloorArea = true;

          if (partofTotalFloorArea) {
            needToSetFloorArea = true;
          }
        }
      }  // End loop on space

      // set the origin the Thermal Zone (lower-left corner of all spaces)
      idfObject.setDouble(openstudio::ZoneFields::XOrigin, xOrigin);
      idfObject.setDouble(openstudio::ZoneFields::YOrigin, yOrigin);
      idfObject.setDouble(openstudio::ZoneFields::ZOrigin, zOrigin);

      // Important: we need to call Space::changeTransformation to align for the xOrigin, yOrigin, zOrigin (lower left corner) we identified above
      // Because in E+ all coordinates are still relative to the Zone. The E+ 'Space' object has no specific origin x, y, z coordinates
      auto newTranslation = Transformation::translation(Vector3d(xOrigin, yOrigin, zOrigin));
      double degNorth = 0.0;
      if (directionofRelativeNorth) {
        idfObject.setDouble(openstudio::ZoneFields::DirectionofRelativeNorth, directionofRelativeNorth.get());
        degNorth = directionofRelativeNorth.get();
      }
      // rotate negative amount around the z axis, EnergyPlus defines rotation clockwise
      auto newRotation = Transformation::rotation(Vector3d(0, 0, 1), -openstudio::degToRad(degNorth));
      auto newTransformation = newTranslation * newRotation;
      // This will also change the transformation for surfaces, daylighting controls, etc
      for (auto& space : spaces) {
        space.changeTransformation(newTransformation);
      }

      if (partofTotalFloorArea) {
        idfObject.setString(openstudio::ZoneFields::PartofTotalFloorArea, "Yes");
      } else {
        idfObject.setString(openstudio::ZoneFields::PartofTotalFloorArea, "No");
      }

      if (needToSetFloorArea) {
        if (modelObject.getDouble(openstudio::OS_ThermalZoneFields::FloorArea)) {
          LOG(Info, "ThermalZone " << tzName << " has a user-specified Floor Area, using this number");
          idfObject.setDouble(openstudio::ZoneFields::FloorArea, modelObject.getDouble(openstudio::OS_ThermalZoneFields::FloorArea).get());
        } else {
          LOG(Info, "ThermalZone '" << tzName << "' has spaces with mis-matched 'Part of Total Floor Area' flags. "
                                    << "Setting the flag to 'Yes', but hard-coding the total floor area to only take into account the spaces "
                                    << "that are part of total Floor Area");
          idfObject.setDouble(openstudio::ZoneFields::FloorArea, totalFloorArea);
        }
      }

      // TODO: JM 2021-10-15: Aren't we having a lot of problems with DaylightingControls? Yes we are!
      // Sooooo. Our API prevents us from effectively writing the daylightng objects to a Space because the ThermalZone itself bears the
      // Primary/Secondary daylighting control + fraction of lights controlled.
      // E+ will yell if you have space enclosures but the Daylighting objects are on Zone-level (also, it currently does not work, IlluminanceMap
      // only accepts Zone level, and Daylighting:ReferencePoint should support Space Name but it doesn't)
      // So here if the zone has daylighting control, we make an airwall between spaces and convert non airwalls to InternalMass

      // another awesome thing is that z.primaryDaylightingControl() || z.secondaryDaylightingControl() || z.illuminanceMap() is probably not safe, since a
      // DaylightingControl that isn't directly referenced to a zone might be translated too, because DaylightingControl does reference a space itself

      int nDayligthingObjects = std::accumulate(spaces.cbegin(), spaces.cend(), 0, [](int sum, const Space& space) {
        return sum + space.daylightingControls().size() + space.illuminanceMaps().size();
      });
      if (nDayligthingObjects > 0) {

        boost::optional<InteriorPartitionSurfaceGroup> interiorPartitionSurfaceGroup;
        std::set<Surface> mergedSurfaces;

        std::vector<Surface> zoneSurfaces;
        for (const auto& space : spaces) {
          for (const auto& surface : space.surfaces()) {
            zoneSurfaces.emplace_back(surface);
          }
        }

        // sort by surface name, for repeatability
        std::sort(zoneSurfaces.begin(), zoneSurfaces.end(), WorkspaceObjectNameLess());

        for (const auto& surface : zoneSurfaces) {

          // Already processed?
          auto it = mergedSurfaces.find(surface);
          if (it != mergedSurfaces.end()) {
            continue;
          }

          auto adjacentSurface_ = surface.adjacentSurface();
          if (!adjacentSurface_) {
            continue;
          }

          auto adjacentSpace_ = adjacentSurface_->space();
          if (!adjacentSpace_) {
            continue;
          }

          // Adjacent Space must be part of this thermalZone as well
          if (std::find(spaces.cbegin(), spaces.cend(), adjacentSpace_.get()) == spaces.end()) {
            continue;
          }

          // handling both the surface and the adjacentSurface
          mergedSurfaces.insert(surface);
          mergedSurfaces.insert(adjacentSurface_.get());

          // don't make interior partitions for interior air walls
          bool isAirWall = surface.isAirWall();
          bool isAdjacentAirWall = adjacentSurface_->isAirWall();

          if (isAirWall && isAdjacentAirWall) {
            continue;
          } else if (isAirWall) {
            LOG(Warn, "Interior surface '" << surface.nameString() << "' is an air wall but adjacent surface '" << adjacentSurface_->nameString()
                                           << "' is not, ignoring internal mass.")
            continue;
          } else if (isAdjacentAirWall) {
            LOG(Warn, "Interior surface '" << adjacentSurface_->nameString() << "' is an air wall but adjacent surface '" << surface.nameString()
                                           << "' is not, ignoring internal mass.")
            continue;
          }

          if (!interiorPartitionSurfaceGroup) {
            interiorPartitionSurfaceGroup = InteriorPartitionSurfaceGroup(modelObject.model());
            // TODO: I don't think I care about which space I assign it to, since currently that's only written at Zone Level but this might change
            interiorPartitionSurfaceGroup->setSpace(adjacentSpace_.get());
          }

          // is there a better way to pick which vertices to keep based on outward normal?
          InteriorPartitionSurface interiorPartitionSurface(surface.vertices(), modelObject.model());
          interiorPartitionSurface.setName("Merged " + surface.name().get() + " - " + adjacentSurface_->name().get());
          interiorPartitionSurface.setInteriorPartitionSurfaceGroup(*interiorPartitionSurfaceGroup);

          boost::optional<ConstructionBase> construction = surface.construction();
          if (construction) {
            interiorPartitionSurface.setConstruction(*construction);
          }
        }

        if (!mergedSurfaces.empty()) {
          LOG(Warn, modelObject.briefDescription() << " has DaylightingControl Objects assigned. The interior walls between Spaces will be merged. "
                                                      "Make sure these are correctly Matched!");
        }

        ConstructionAirBoundary airWallConstruction(modelObject.model());

        // std::set forces a const iterator, hence why I take a copy...
        for (Surface mergedSurface : mergedSurfaces) {
          mergedSurface.setConstruction(airWallConstruction);
        }

      }  // End Combine surfaces if daylighting
    }

    for (Space& space : spaces) {

      // translate the space now: it will translate it's geometry children (ShadingSurface/InteriorPartition Groups + Surfaces)
      // and all SpaceLoad directly associated with the space
      translateAndMapModelObject(space);

    }  // end spaces

    // translate daylighting controls
    // TODO: move this out somewhere, figure out the right format.
    // Maybe `translateDaylightingControl(DaylightingControl primaryDaylightingControl, boost::optional<DaylightingControl> secondaryDaylightingControl)` ?
    auto translateDaylightingControls = [this, &tzName](model::ThermalZone& thermalZone) {
      boost::optional<DaylightingControl> primaryDaylightingControl = thermalZone.primaryDaylightingControl();
      if (primaryDaylightingControl) {

        boost::optional<DaylightingControl> secondaryDaylightingControl = thermalZone.secondaryDaylightingControl();

        IdfObject daylightingControlObject(openstudio::IddObjectType::Daylighting_Controls);
        // Name it like the Zone name + " DaylightingControls"
        daylightingControlObject.setName(tzName + " DaylightingControls");
        m_idfObjects.push_back(daylightingControlObject);

        // Zone Name
        daylightingControlObject.setString(Daylighting_ControlsFields::ZoneorSpaceName, tzName);

        // Availability Schedule Name
        if (boost::optional<Schedule> sched = thermalZone.daylightingControlsAvailabilitySchedule()) {
          if (boost::optional<IdfObject> idfo = translateAndMapModelObject(sched.get())) {
            daylightingControlObject.setString(Daylighting_ControlsFields::AvailabilityScheduleName, idfo->nameString());
          }
        }

        // Primary Control
        IdfObject primaryReferencePoint(openstudio::IddObjectType::Daylighting_ReferencePoint);
        // Name it like the OS:Daylighting:Control corresponding to the Primary Reference Point
        primaryReferencePoint.setName(primaryDaylightingControl->nameString());
        m_idfObjects.push_back(primaryReferencePoint);

        OptionalSpace refSpace = primaryDaylightingControl->space();
        if (refSpace) {
          OptionalThermalZone refThermalZone = refSpace->thermalZone();
          // TODO: I think we should catch the case when refThermalZone != thermalZone...
          if (refThermalZone) {
            primaryReferencePoint.setString(Daylighting_ReferencePointFields::ZoneorSpaceName, refThermalZone->nameString());
          }
        }

        primaryReferencePoint.setDouble(Daylighting_ReferencePointFields::XCoordinateofReferencePoint,
                                        primaryDaylightingControl->positionXCoordinate());
        primaryReferencePoint.setDouble(Daylighting_ReferencePointFields::YCoordinateofReferencePoint,
                                        primaryDaylightingControl->positionYCoordinate());
        primaryReferencePoint.setDouble(Daylighting_ReferencePointFields::ZCoordinateofReferencePoint,
                                        primaryDaylightingControl->positionZCoordinate());

        double primaryFrac = thermalZone.fractionofZoneControlledbyPrimaryDaylightingControl();
        if (istringEqual("None", primaryDaylightingControl->lightingControlType())) {
          if (primaryFrac > 0.0) {
            primaryFrac = 0.0;
            LOG(Warn, "Fraction of Zone Controlled by Primary Daylight Control is "
                        << primaryFrac << " but lighting control type is 'None'. Reseting Primary Fraction to " << 0.0);
          }
        }

        std::vector<std::string> firstGroup{primaryReferencePoint.nameString(), toString(primaryFrac),
                                            toString(primaryDaylightingControl->illuminanceSetpoint())};
        daylightingControlObject.pushExtensibleGroup(firstGroup);

        // Secondary Control
        if (secondaryDaylightingControl) {
          IdfObject secondaryReferencePoint(openstudio::IddObjectType::Daylighting_ReferencePoint);
          // Name it like the OS:Daylighting:Control corresponding to the Secondary Reference Point
          secondaryReferencePoint.setName(secondaryDaylightingControl->nameString());
          m_idfObjects.push_back(secondaryReferencePoint);

          refSpace = secondaryDaylightingControl->space();
          if (refSpace) {
            OptionalThermalZone refThermalZone = refSpace->thermalZone();
            // TODO: catch if refThermalZone != thermalZone
            if (refThermalZone) {
              secondaryReferencePoint.setString(Daylighting_ReferencePointFields::ZoneorSpaceName, refThermalZone->nameString());
            }
          }

          secondaryReferencePoint.setString(Daylighting_ReferencePointFields::ZoneorSpaceName, tzName);
          secondaryReferencePoint.setDouble(Daylighting_ReferencePointFields::XCoordinateofReferencePoint,
                                            secondaryDaylightingControl->positionXCoordinate());
          secondaryReferencePoint.setDouble(Daylighting_ReferencePointFields::YCoordinateofReferencePoint,
                                            secondaryDaylightingControl->positionYCoordinate());
          secondaryReferencePoint.setDouble(Daylighting_ReferencePointFields::ZCoordinateofReferencePoint,
                                            secondaryDaylightingControl->positionZCoordinate());

          double secondaryFrac = thermalZone.fractionofZoneControlledbySecondaryDaylightingControl();
          if (istringEqual("None", secondaryDaylightingControl->lightingControlType())) {
            if (secondaryFrac > 0.0) {
              secondaryFrac = 0.0;
              LOG(Warn, "Fraction of Zone Controlled by Secondary Daylight Control is "
                          << secondaryFrac << " but lighting control type is 'None'. Reseting Secondary Fraction to " << 0.0);
            }
          } else {
            if ((primaryFrac + secondaryFrac) > 1) {
              // Reset secondary to 1 - Primary
              secondaryFrac = std::max(1.0 - primaryFrac, 0.0);
              LOG(Warn, "Fraction of Zone Controlled by Primary Daylight Control is "
                          << primaryFrac << " while Secondary Fraction is " << secondaryFrac << ". Reseting Secondary Fraction to " << secondaryFrac);
            }
          }

          std::vector<std::string> secondGroup{secondaryReferencePoint.nameString(), toString(secondaryFrac),
                                               toString(secondaryDaylightingControl->illuminanceSetpoint())};
          daylightingControlObject.pushExtensibleGroup(secondGroup);
        }

        // Shared Data
        std::string lightingControlType = primaryDaylightingControl->lightingControlType();
        if (istringEqual("None", lightingControlType)) {
          // fraction for first point have been set to 0, try to get control type from second point
          if (secondaryDaylightingControl) {
            lightingControlType = secondaryDaylightingControl->lightingControlType();
          }
        }

        if (istringEqual("None", lightingControlType)) {
          // both fractions are 0
        } else if (istringEqual("Continuous", lightingControlType)) {
          daylightingControlObject.setString(Daylighting_ControlsFields::LightingControlType, "Continuous");
        } else if (istringEqual("Stepped", lightingControlType)) {
          daylightingControlObject.setString(Daylighting_ControlsFields::LightingControlType, "Stepped");
        } else if (istringEqual("Continuous/Off", lightingControlType)) {
          daylightingControlObject.setString(Daylighting_ControlsFields::LightingControlType, "ContinuousOff");
        } else {
          LOG(Warn,
              "Unknown lighting control type '" << lightingControlType << "' for OS:Daylighting:Control " << primaryDaylightingControl->nameString());
        }

        if (primaryDaylightingControl->psiRotationAroundXAxis() != 0.0) {
          LOG(Warn, "Rotation of " << primaryDaylightingControl->psiRotationAroundXAxis()
                                   << " degrees about X axis not mapped for OS:Daylighting:Control " << primaryDaylightingControl->nameString());
        }

        if (primaryDaylightingControl->thetaRotationAroundYAxis() != 0.0) {
          LOG(Warn, "Rotation of " << primaryDaylightingControl->thetaRotationAroundYAxis()
                                   << " degrees about Y axis not mapped for OS:Daylighting:Control " << primaryDaylightingControl->nameString());
        }

        // glare:
        // * openstudio uses the right-hand rule, y points North and x points east.
        //   So a positive rotation around the z-axis is counter-clockwise in the horizontal plane
        // * E+ on the other hand is looking for something that is clockwise:
        //   > Field: Glare Calculation Azimuth Angle of View Direction Clockwise from Zone y-Axis
        //   > It is the angle, measured clockwise in the horizontal plane, between the zone y-axis and the occupant view direction

        // 3D View:
        //          z
        //          ▲
        //          │
        //          │
        //        | │ ▲
        //       +└►├─┘
        //          │      . OS convention for ϕ
        //          │    .
        //          │  .<-◝
        //          │.     ) ϕ
        //          ○─────────────────► y
        //         ╱  *     )
        //        ╱     *<-◞ E+ Field
        //       ╱        *
        //      ╱
        //     ╱
        //    x
        //
        // 2D View, from the top:
        //
        //            y
        //            ▲         Glare Calculation Azimuth Angle of View Direction Clockwise from Zone y axis
        //   \        │  E+    /
        //    \  OS ϕ ├─────┐ /
        //     \ ┌────┤     ▼/
        //      \▼    │     /
        //       \    │    /
        //        \   │   /
        //         \  │  /
        //          \ | /
        //           (◯)───────────────►x
        //            z
        //

        double glareAngle = -primaryDaylightingControl->phiRotationAroundZAxis();
        // Force [0,360[
        glareAngle = normalizeAngle0to360(glareAngle);
        daylightingControlObject.setDouble(Daylighting_ControlsFields::GlareCalculationAzimuthAngleofViewDirectionClockwisefromZoneyAxis, glareAngle);

        if (OptionalDouble d = primaryDaylightingControl->maximumAllowableDiscomfortGlareIndex()) {
          daylightingControlObject.setDouble(Daylighting_ControlsFields::MaximumAllowableDiscomfortGlareIndex, *d);
        }

        daylightingControlObject.setString(Daylighting_ControlsFields::GlareCalculationDaylightingReferencePointName,
                                           primaryReferencePoint.nameString());

        daylightingControlObject.setDouble(Daylighting_ControlsFields::MinimumInputPowerFractionforContinuousorContinuousOffDimmingControl,
                                           primaryDaylightingControl->minimumInputPowerFractionforContinuousDimmingControl());

        daylightingControlObject.setDouble(Daylighting_ControlsFields::MinimumLightOutputFractionforContinuousorContinuousOffDimmingControl,
                                           primaryDaylightingControl->minimumLightOutputFractionforContinuousDimmingControl());

        daylightingControlObject.setInt(Daylighting_ControlsFields::NumberofSteppedControlSteps,
                                        primaryDaylightingControl->numberofSteppedControlSteps());

        daylightingControlObject.setDouble(Daylighting_ControlsFields::ProbabilityLightingwillbeResetWhenNeededinManualSteppedControl,
                                           primaryDaylightingControl->probabilityLightingwillbeResetWhenNeededinManualSteppedControl());
      }
    };

    translateDaylightingControls(modelObject);

    // translate illuminance map
    boost::optional<IlluminanceMap> illuminanceMap = modelObject.illuminanceMap();
    if (illuminanceMap) {
      if (!modelObject.primaryDaylightingControl()) {
        LOG(Warn,
            "Daylighting:Controls object is required to trigger daylighting calculations in EnergyPlus, adding a minimal one to Zone " << tzName);

        IdfObject referencePoint(openstudio::IddObjectType::Daylighting_ReferencePoint);
        referencePoint.setName(tzName + " Daylighting Reference Point");
        m_idfObjects.push_back(referencePoint);
        referencePoint.setString(Daylighting_ReferencePointFields::ZoneorSpaceName, tzName);
        referencePoint.setDouble(Daylighting_ReferencePointFields::XCoordinateofReferencePoint,
                                 illuminanceMap->originXCoordinate() + 0.5 * illuminanceMap->xLength());
        referencePoint.setDouble(Daylighting_ReferencePointFields::YCoordinateofReferencePoint,
                                 illuminanceMap->originYCoordinate() + 0.5 * illuminanceMap->yLength());
        referencePoint.setDouble(Daylighting_ReferencePointFields::ZCoordinateofReferencePoint, illuminanceMap->originZCoordinate());

        IdfObject daylightingControlObject(openstudio::IddObjectType::Daylighting_Controls);
        // Name it like the Zone name + " DaylightingControls"
        daylightingControlObject.setName(tzName + " DaylightingControls");
        m_idfObjects.push_back(daylightingControlObject);

        daylightingControlObject.setString(Daylighting_ControlsFields::ZoneorSpaceName, tzName);
        std::vector<std::string> group{referencePoint.nameString(),  // ref point name
                                       "0.0",                        // fraction controlled
                                       ""};                          // illuminance setpoint
        daylightingControlObject.pushExtensibleGroup(group);
      }

      // TODO: move this to its own file
      auto translateIlluminanceMap = [this](model::IlluminanceMap& illuminanceMap) -> IdfObject {
        IdfObject illuminanceMapObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Output_IlluminanceMap, illuminanceMap);

        illuminanceMapObject.setDouble(Output_IlluminanceMapFields::Zheight, illuminanceMap.originZCoordinate());

        illuminanceMapObject.setDouble(Output_IlluminanceMapFields::XMinimumCoordinate, illuminanceMap.originXCoordinate());
        illuminanceMapObject.setDouble(Output_IlluminanceMapFields::XMaximumCoordinate,
                                       illuminanceMap.originXCoordinate() + illuminanceMap.xLength());
        illuminanceMapObject.setInt(Output_IlluminanceMapFields::NumberofXGridPoints, illuminanceMap.numberofXGridPoints());

        illuminanceMapObject.setDouble(Output_IlluminanceMapFields::YMinimumCoordinate, illuminanceMap.originYCoordinate());
        illuminanceMapObject.setDouble(Output_IlluminanceMapFields::YMaximumCoordinate,
                                       illuminanceMap.originYCoordinate() + illuminanceMap.yLength());
        illuminanceMapObject.setInt(Output_IlluminanceMapFields::NumberofYGridPoints, illuminanceMap.numberofYGridPoints());

        if (illuminanceMap.psiRotationAroundXAxis() != 0.0) {
          LOG(Warn, "Rotation of " << illuminanceMap.psiRotationAroundXAxis() << " degrees about X axis not mapped for OS:IlluminanceMap "
                                   << illuminanceMap.nameString());
        }

        if (illuminanceMap.thetaRotationAroundYAxis() != 0.0) {
          LOG(Warn, "Rotation of " << illuminanceMap.thetaRotationAroundYAxis() << " degrees about Y axis not mapped for OS:IlluminanceMap "
                                   << illuminanceMap.nameString());
        }

        if (illuminanceMap.phiRotationAroundZAxis() != 0.0) {
          LOG(Warn, "Rotation of " << illuminanceMap.phiRotationAroundZAxis() << " degrees about Z axis not mapped for OS:IlluminanceMap "
                                   << illuminanceMap.nameString());
        }

        return illuminanceMapObject;
      };

      auto illuminanceMapObject = translateIlluminanceMap(illuminanceMap.get());
      illuminanceMapObject.setString(Output_IlluminanceMapFields::ZoneorSpaceName, tzName);
    }

    // TODO: this is definitely shared between both paths
    // translate zone mixing objects which supply air to this zone
    ZoneMixingVector supplyZoneMixing = modelObject.supplyZoneMixing();
    std::sort(supplyZoneMixing.begin(), supplyZoneMixing.end(), WorkspaceObjectNameLess());
    for (ZoneMixing& mixing : supplyZoneMixing) {
      translateAndMapModelObject(mixing);
    }

    auto zoneEquipment = modelObject.equipment();

    // In OS ZoneVentilationDesignFlowRate and ZoneVentilationWindandStackOpenArea are considered zone equipment,
    // but for the E+ perspective it is not so we have to remove them and treat them differently.
    auto isZoneVentilationObject = [](const ModelObject& mo) {
      return ((mo.iddObjectType() == ZoneVentilationDesignFlowRate::iddObjectType())
              || (mo.iddObjectType() == ZoneVentilationWindandStackOpenArea::iddObjectType()));
    };

    std::vector<model::ModelObject> zoneVentilationObjects;
    std::copy_if(zoneEquipment.begin(), zoneEquipment.end(), std::back_inserter(zoneVentilationObjects), isZoneVentilationObject);

    auto zoneVentilationBegin = std::remove_if(zoneEquipment.begin(), zoneEquipment.end(), isZoneVentilationObject);
    zoneEquipment.erase(zoneVentilationBegin, zoneEquipment.end());

    // translate thermostat and/or humidistat
    if ((!zoneEquipment.empty()) || modelObject.useIdealAirLoads()) {
      // Thermostat
      if (auto thermostat = modelObject.thermostat()) {
        if (thermostat->iddObjectType() == ZoneControlThermostatStagedDualSetpoint::iddObjectType()) {
          // This one we translate already
          translateAndMapModelObject(thermostat.get());
        } else {

          // This is a OS:ThermostatSetpoint:DualSetpoint as it's the only other choice.
          auto dualSetpoint = thermostat->cast<ThermostatSetpointDualSetpoint>();

          auto createZoneControlThermostat = [&]() {
            IdfObject zoneControlThermostat(openstudio::IddObjectType::ZoneControl_Thermostat);
            zoneControlThermostat.setString(ZoneControl_ThermostatFields::Name, tzName + " Thermostat");
            zoneControlThermostat.setString(ZoneControl_ThermostatFields::ZoneorZoneListName, tzName);
            m_idfObjects.push_back(zoneControlThermostat);

            // Need to handle the control type base don thermostat type (1: Single heating, 2: single cooling, 4: Dual setpoint)
            IdfObject scheduleCompact(openstudio::IddObjectType::Schedule_Compact);
            scheduleCompact.setName(tzName + " Thermostat Schedule");
            m_idfObjects.push_back(scheduleCompact);
            scheduleCompact.setString(1, tzName + " Thermostat Schedule Type Limits");
            scheduleCompact.setString(2, "Through: 12/31");
            scheduleCompact.setString(3, "For: AllDays");
            scheduleCompact.setString(4, "Until: 24:00");
            scheduleCompact.setString(5, "4");

            IdfObject scheduleTypeLimits(openstudio::IddObjectType::ScheduleTypeLimits);
            scheduleTypeLimits.setName(tzName + " Thermostat Schedule Type Limits");
            m_idfObjects.push_back(scheduleTypeLimits);
            scheduleTypeLimits.setString(1, "0");
            scheduleTypeLimits.setString(2, "4");
            scheduleTypeLimits.setString(3, "DISCRETE");

            zoneControlThermostat.setString(ZoneControl_ThermostatFields::ControlTypeScheduleName, scheduleCompact.nameString());

            if (boost::optional<IdfObject> idfThermostat = translateAndMapModelObject(dualSetpoint)) {
              // TODO: JM 2019-09-04 switch back to an extensible object once/if https://github.com/NREL/EnergyPlus/issues/7484 is addressed and the
              // 'Temperature Difference Between Cutout And Setpoint' field is moved before the extensible fields
              // For now, we revert to a non extensible object, so we can still write that field

              //StringVector values(zoneControlThermostat.iddObject().properties().numExtensible);
              //values[ZoneControl_ThermostatExtensibleFields::ControlObjectType] = idfThermostat->iddObject().name();
              //values[ZoneControl_ThermostatExtensibleFields::ControlName] = idfThermostat->nameString();
              //IdfExtensibleGroup eg = zoneControlThermostat.pushExtensibleGroup(values);

              zoneControlThermostat.setString(ZoneControl_ThermostatFields::Control1ObjectType, idfThermostat->iddObject().name());
              zoneControlThermostat.setString(ZoneControl_ThermostatFields::Control1Name, idfThermostat->nameString());

              if (idfThermostat->iddObject().name() == "ThermostatSetpoint:SingleHeating") {
                scheduleCompact.setString(5, "1");
              } else if (idfThermostat->iddObject().name() == "ThermostatSetpoint:SingleCooling") {
                scheduleCompact.setString(5, "2");
              } else {
                // DualSetpoint
                scheduleCompact.setString(5, "4");
              }

              // Thermostat's Temperature Difference Between Cutout And Setpoint is placed here on the ZoneControl:Thermostat
              zoneControlThermostat.setDouble(ZoneControl_ThermostatFields::TemperatureDifferenceBetweenCutoutAndSetpoint,
                                              dualSetpoint.temperatureDifferenceBetweenCutoutAndSetpoint());
            }
          };

          // Only translate ThermostatSetpointDualSetpoint if there is at least one schedule attached
          // The translation to SingleHeating, SingleCooling, or DualSetpoint as appropriate is handled in ForwardTranslateThermostatSetpointDualSetpoint
          if (dualSetpoint.heatingSetpointTemperatureSchedule() || dualSetpoint.coolingSetpointTemperatureSchedule()) {
            createZoneControlThermostat();
          }
        }
      }

      // Humidistat
      if (boost::optional<ZoneControlHumidistat> humidistat = modelObject.zoneControlHumidistat()) {
        if (boost::optional<IdfObject> idfHumidistat = translateAndMapModelObject(humidistat.get())) {
          idfHumidistat->setString(ZoneControl_HumidistatFields::ZoneName, tzName);
        }
      }

      // ZoneControlContaminantController
      if (auto controller = modelObject.zoneControlContaminantController()) {
        translateAndMapModelObject(controller.get());
      }
    }

    // Ideal air loads
    if (modelObject.useIdealAirLoads()) {
      auto& idealLoadsAirSystem = m_idfObjects.emplace_back(IddObjectType::HVACTemplate_Zone_IdealLoadsAirSystem);
      idealLoadsAirSystem.setString(HVACTemplate_Zone_IdealLoadsAirSystemFields::ZoneName, tzName);
    }

    // ZoneVentilationDesignFlowRate and ZoneVentilationWindandStackOpenArea do not go on equipment connections or associated list
    for (auto& zone_vent : zoneVentilationObjects) {
      translateAndMapModelObject(zone_vent);
    }

    bool zoneHVACIdealWorkaround = false;
    boost::optional<ZoneHVACIdealLoadsAirSystem> ideal;
    if (zoneEquipment.size() == 1) {
      ideal = zoneEquipment.front().optionalCast<model::ZoneHVACIdealLoadsAirSystem>();
      if (ideal) {
        auto returnPlenum = ideal->returnPlenum();
        if (returnPlenum) {
          auto allIdealHVAC = returnPlenum->getImpl<model::detail::AirLoopHVACReturnPlenum_Impl>()->zoneHVACIdealLoadsAirSystems();
          if (!allIdealHVAC.empty()) {
            zoneHVACIdealWorkaround = true;
          }
        }
      }
    }

    if (zoneHVACIdealWorkaround) {
      // ZoneHVAC_EquipmentConnections
      IdfObject connectionsObject(openstudio::IddObjectType::ZoneHVAC_EquipmentConnections);
      m_idfObjects.push_back(connectionsObject);

      connectionsObject.setString(openstudio::ZoneHVAC_EquipmentConnectionsFields::ZoneName, tzName);

      //set the inlet port list
      PortList inletPortList = modelObject.inletPortList();
      if (!inletPortList.modelObjects().empty()) {
        boost::optional<IdfObject> _inletNodeList = translateAndMapModelObject(inletPortList);
        if (_inletNodeList) {
          _inletNodeList->setName(tzName + " Inlet Node List");
          connectionsObject.setString(openstudio::ZoneHVAC_EquipmentConnectionsFields::ZoneAirInletNodeorNodeListName, _inletNodeList->nameString());
        }
      }

      //set the zone air node
      Node node = modelObject.zoneAirNode();
      connectionsObject.setString(openstudio::ZoneHVAC_EquipmentConnectionsFields::ZoneAirNodeName, node.nameString());

      // Use the exhaust node as the zone return node in this workaround
      //set the zone return air node
      auto exhaustPortList = modelObject.exhaustPortList();
      auto exhaustNodes = subsetCastVector<model::Node>(exhaustPortList.modelObjects());
      OS_ASSERT(exhaustNodes.size() == 1);
      connectionsObject.setString(openstudio::ZoneHVAC_EquipmentConnectionsFields::ZoneReturnAirNodeorNodeListName,
                                  exhaustNodes.front().nameString());

      // ZoneHVAC_EquipmentList
      ZoneHVACEquipmentList equipmentList = modelObject.getImpl<model::detail::ThermalZone_Impl>()->zoneHVACEquipmentList();
      boost::optional<IdfObject> _equipmentList = translateAndMapModelObject(equipmentList);

      if (_equipmentList) {
        connectionsObject.setString(openstudio::ZoneHVAC_EquipmentConnectionsFields::ZoneConditioningEquipmentListName, _equipmentList->nameString());
      }
    } else if (!zoneEquipment.empty()) {
      // ZoneHVAC_EquipmentConnections
      IdfObject connectionsObject(openstudio::IddObjectType::ZoneHVAC_EquipmentConnections);
      m_idfObjects.push_back(connectionsObject);

      connectionsObject.setString(openstudio::ZoneHVAC_EquipmentConnectionsFields::ZoneName, tzName);

      //set the inlet port list
      PortList inletPortList = modelObject.inletPortList();
      if (!inletPortList.modelObjects().empty()) {
        boost::optional<IdfObject> _inletNodeList = translateAndMapModelObject(inletPortList);
        if (_inletNodeList) {
          _inletNodeList->setName(tzName + " Inlet Node List");
          connectionsObject.setString(openstudio::ZoneHVAC_EquipmentConnectionsFields::ZoneAirInletNodeorNodeListName, _inletNodeList->nameString());
        }
      }

      //set the exhaust port list
      PortList exhaustPortList = modelObject.exhaustPortList();
      if (!exhaustPortList.modelObjects().empty()) {
        boost::optional<IdfObject> _exhaustNodeList = translateAndMapModelObject(exhaustPortList);
        if (_exhaustNodeList) {
          _exhaustNodeList->setName(tzName + " Exhaust Node List");
          connectionsObject.setString(openstudio::ZoneHVAC_EquipmentConnectionsFields::ZoneAirExhaustNodeorNodeListName,
                                      _exhaustNodeList->nameString());
        }
      }

      //set the zone air node
      Node node = modelObject.zoneAirNode();
      connectionsObject.setString(openstudio::ZoneHVAC_EquipmentConnectionsFields::ZoneAirNodeName, node.nameString());

      //set the zone return air node
      auto returnPortList = modelObject.returnPortList();
      if (!returnPortList.modelObjects().empty()) {
        auto _returnNodeList = translateAndMapModelObject(returnPortList);
        if (_returnNodeList) {
          _returnNodeList->setName(tzName + " Return Node List");
          connectionsObject.setString(openstudio::ZoneHVAC_EquipmentConnectionsFields::ZoneReturnAirNodeorNodeListName,
                                      _returnNodeList->nameString());
        }
      }

      // ZoneHVAC_EquipmentList

      ZoneHVACEquipmentList equipmentList = modelObject.getImpl<model::detail::ThermalZone_Impl>()->zoneHVACEquipmentList();

      boost::optional<IdfObject> _equipmentList = translateAndMapModelObject(equipmentList);

      if (_equipmentList) {
        connectionsObject.setString(openstudio::ZoneHVAC_EquipmentConnectionsFields::ZoneConditioningEquipmentListName, _equipmentList->nameString());
      }
    }

    // SizingZone

    if ((!zoneEquipment.empty()) || modelObject.useIdealAirLoads()) {
      // get sizing period objects from the model
      std::vector<SizingPeriod> sizingPeriod = modelObject.model().getModelObjects<SizingPeriod>();
      // map the sizing object only if a sizing period object exists
      boost::optional<IdfObject> sizingZoneIdf;
      if (!sizingPeriod.empty()) {
        SizingZone sizingZone = modelObject.sizingZone();
        sizingZoneIdf = translateAndMapModelObject(sizingZone);
        OS_ASSERT(sizingZoneIdf);
      }

      boost::optional<IdfObject> dsoaList;
      bool needToRegisterDSOAList = false;
      bool atLeastOneDSOAWasWritten = true;

      if (!m_forwardTranslatorOptions.excludeSpaceTranslation() && sizingZoneIdf) {
        // DO not register it yet! E+ will crash if the DSOA Space List ends up empty
        dsoaList = IdfObject(openstudio::IddObjectType::DesignSpecification_OutdoorAir_SpaceList);
        needToRegisterDSOAList = true;
        atLeastOneDSOAWasWritten = false;
        dsoaList->setName(tzName + " DSOA Space List");
      }

      // map the design specification outdoor air
      boost::optional<DesignSpecificationOutdoorAir> designSpecificationOutdoorAir;

      // For the ZoneVentilation workaround
      bool createZvs = false;
      double zvRateForPeople = 0.0;
      double zvRateForArea = 0.0;
      double zvRate = 0.0;
      double zvRateForVolume = 0.0;
      double totVolume = 0.0;

      for (const Space& space : spaces) {
        designSpecificationOutdoorAir = space.designSpecificationOutdoorAir();
        if (designSpecificationOutdoorAir) {

          // TODO: We definitely need to do something here...
          // TODO: this isn't good. We also need to check the SpaceType-level DSOA...
          boost::optional<IdfObject> thisDSOA = translateAndMapModelObject(*designSpecificationOutdoorAir);
          if (sizingZoneIdf) {
            if (m_forwardTranslatorOptions.excludeSpaceTranslation()) {
              // point the sizing object to the outdoor air spec
              sizingZoneIdf->setString(Sizing_ZoneFields::DesignSpecificationOutdoorAirObjectName, designSpecificationOutdoorAir->nameString());
            } else {
              if (needToRegisterDSOAList) {
                m_idfObjects.emplace_back(dsoaList.get());
                sizingZoneIdf->setString(Sizing_ZoneFields::DesignSpecificationOutdoorAirObjectName, dsoaList->nameString());
                needToRegisterDSOAList = false;
              }

              // push an extensible group on the DSOA:SpaceList
              dsoaList->pushExtensibleGroup(std::vector<std::string>{space.nameString(), thisDSOA->nameString()});
              atLeastOneDSOAWasWritten = true;
            }
          }

          // create zone ventilation if needed
          // TODO: we could remove all this code if we used ZoneHVAC:IdealLoadsAirSystem instead of HVACTemplate:Zone:IdealLoadsAirSystem
          // We have space level stuff, that we need to write at Zone-level. So we compute the rate for each component (per Person, Floor Area,
          // absolute and ACH) by looping on spaces. Then we'll write that by dividing by the total zone number of people, floor area, 1, and volume
          // This amount to computing a weighted average
          if (zoneEquipment.empty()) {
            createZvs = true;

            totVolume += space.volume();

            double rateForPeople = space.numberOfPeople() * designSpecificationOutdoorAir->outdoorAirFlowperPerson();
            double rateForArea = space.floorArea() * designSpecificationOutdoorAir->outdoorAirFlowperFloorArea();
            double rate = designSpecificationOutdoorAir->outdoorAirFlowRate();
            // ACH * volume = m3/hour, divide by 3600 s/hr to get m3/s
            double rateForVolume = space.volume() * designSpecificationOutdoorAir->outdoorAirFlowAirChangesperHour() / 3600.0;

            std::string outdoorAirMethod = designSpecificationOutdoorAir->outdoorAirMethod();
            if (istringEqual(outdoorAirMethod, "Maximum")) {

              double biggestRate = std::max({rateForPeople, rateForArea, rate, rateForVolume});

              if (rateForPeople == biggestRate) {
                //rateForPeople = 0.0;
                rateForArea = 0.0;
                rate = 0.0;
                rateForVolume = 0.0;
              } else if (rateForArea == biggestRate) {
                rateForPeople = 0.0;
                //rateForArea = 0.0;
                rate = 0.0;
                rateForVolume = 0.0;
              } else if (rate == biggestRate) {
                rateForPeople = 0.0;
                rateForArea = 0.0;
                //rate = 0.0;
                rateForVolume = 0.0;
              } else {
                //rateForVolume == biggestRate
                rateForPeople = 0.0;
                rateForArea = 0.0;
                rate = 0.0;
                //rateForVolume = 0.0;
              }

            } else {
              // sum
            }

            zvRateForPeople += rateForPeople;
            zvRateForArea += rateForArea;
            zvRate += rate;
            zvRateForVolume += rateForVolume;
          }  // if zoneEquipment.empty()
        }    // if dsoa
      }      // loop on spaces

      if (!atLeastOneDSOAWasWritten && sizingZoneIdf) {
        // Controller:MechnicalVentilation: Design Specification Outdoor Air Object Name <x>
        // > If this field is blank, the corresponding DesignSpecification:OutdoorAir object for the zone will come from
        // > the DesignSpecification:OutdoorAir object referenced by the Sizing:Zone object for the same zone.
        // > ***If no such zone match is found, default values from the IDD will be used for the DesignSpecification:OutdoorAir object
        // > which is 0.0094 m3/s-person.***
        // Apparently I need to set an empty one or something
        auto& dsoaObject = m_idfObjects.emplace_back(IddObjectType::DesignSpecification_OutdoorAir);

        dsoaObject.setString(DesignSpecification_OutdoorAirFields::Name, modelObject.nameString() + " Zero air DSOA");
        dsoaObject.setString(DesignSpecification_OutdoorAirFields::OutdoorAirMethod, "Sum");
        dsoaObject.setDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowperPerson, 0.0);
        dsoaObject.setDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowperZoneFloorArea, 0.0);
        dsoaObject.setDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowperZone, 0.0);
        dsoaObject.setDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowAirChangesperHour, 0.0);

        sizingZoneIdf->setString(Sizing_ZoneFields::DesignSpecificationOutdoorAirObjectName, dsoaObject.nameString());
      }

      if (createZvs) {
        if (zvRateForPeople > 0) {
          // TODO: improve this?
          // find first people schedule
          std::vector<People> allPeople;
          for (const auto& space : spaces) {
            for (const People& people : space.people()) {
              allPeople.push_back(people);
            }
            if (space.spaceType()) {
              for (const People& people : space.spaceType()->people()) {
                allPeople.push_back(people);
              }
            }
          }

          boost::optional<Schedule> peopleSchedule;
          for (const People& people : allPeople) {
            peopleSchedule = people.numberofPeopleSchedule();
            if (peopleSchedule) {
              break;
            }
          }

          if (!allPeople.empty()) {
            auto& zoneVentilation = m_idfObjects.emplace_back(IddObjectType::ZoneVentilation_DesignFlowRate);
            zoneVentilation.setName(tzName + " Ventilation per Person");
            zoneVentilation.setString(ZoneVentilation_DesignFlowRateFields::ZoneorZoneListorSpaceorSpaceListName, tzName);
            if (peopleSchedule) {
              zoneVentilation.setString(ZoneVentilation_DesignFlowRateFields::ScheduleName, peopleSchedule->nameString());
            }
            zoneVentilation.setString(ZoneVentilation_DesignFlowRateFields::DesignFlowRateCalculationMethod, "Flow/Person");
            zoneVentilation.setDouble(ZoneVentilation_DesignFlowRateFields::FlowRateperPerson, zvRateForPeople / modelObject.numberOfPeople());
          } else {
            LOG(Warn, "No People found in ThermalZone '" << tzName << "', outdoor air per person will not be added");
          }
        }

        if (zvRateForArea > 0) {
          auto& zoneVentilation = m_idfObjects.emplace_back(IddObjectType::ZoneVentilation_DesignFlowRate);
          zoneVentilation.setName(tzName + " Ventilation per Floor Area");
          zoneVentilation.setString(ZoneVentilation_DesignFlowRateFields::ZoneorZoneListorSpaceorSpaceListName, tzName);
          zoneVentilation.setString(ZoneVentilation_DesignFlowRateFields::ScheduleName, modelObject.model().alwaysOnDiscreteSchedule().nameString());
          zoneVentilation.setString(ZoneVentilation_DesignFlowRateFields::DesignFlowRateCalculationMethod, "Flow/Area");
          zoneVentilation.setDouble(ZoneVentilation_DesignFlowRateFields::FlowRateperFloorArea, zvRateForArea / modelObject.floorArea());
        }

        if (zvRate > 0) {
          auto& zoneVentilation = m_idfObjects.emplace_back(IddObjectType::ZoneVentilation_DesignFlowRate);
          zoneVentilation.setName(tzName + " Ventilation Rate");
          zoneVentilation.setString(ZoneVentilation_DesignFlowRateFields::ZoneorZoneListorSpaceorSpaceListName, tzName);
          zoneVentilation.setString(ZoneVentilation_DesignFlowRateFields::ScheduleName, modelObject.model().alwaysOnDiscreteSchedule().nameString());
          zoneVentilation.setString(ZoneVentilation_DesignFlowRateFields::DesignFlowRateCalculationMethod, "Flow/Zone");
          zoneVentilation.setDouble(ZoneVentilation_DesignFlowRateFields::DesignFlowRate, zvRate);
        }

        if (zvRateForVolume > 0) {
          auto& zoneVentilation = m_idfObjects.emplace_back(IddObjectType::ZoneVentilation_DesignFlowRate);
          zoneVentilation.setName(tzName + " Ventilation Air Changes per Hour");
          zoneVentilation.setString(ZoneVentilation_DesignFlowRateFields::ZoneorZoneListorSpaceorSpaceListName, tzName);
          zoneVentilation.setString(ZoneVentilation_DesignFlowRateFields::ScheduleName, modelObject.model().alwaysOnDiscreteSchedule().nameString());
          zoneVentilation.setString(ZoneVentilation_DesignFlowRateFields::DesignFlowRateCalculationMethod, "AirChanges/Hour");
          zoneVentilation.setDouble(ZoneVentilation_DesignFlowRateFields::AirChangesperHour, 3600.0 * zvRateForVolume / totVolume);
        }
      }  // zone.equipment().emptpy()
    }    // End Sizing:Zone / ZV block

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
