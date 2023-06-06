/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ModelObject.hpp"
#include "../../model/ModelObject_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/ZoneHVACBaseboardRadiantConvectiveWater.hpp"
#include "../../model/ZoneHVACBaseboardRadiantConvectiveWater_Impl.hpp"
#include "../../model/ZoneHVACEquipmentList.hpp"
#include "../../model/ZoneHVACEquipmentList_Impl.hpp"
#include "../../model/ZoneHVACComponent.hpp"
#include "../../model/ZoneHVACComponent_Impl.hpp"
#include "../../model/CoilHeatingWaterBaseboardRadiant.hpp"
#include "../../model/CoilHeatingWaterBaseboardRadiant_Impl.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include <utilities/idd/ZoneHVAC_Baseboard_RadiantConvective_Water_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_Baseboard_RadiantConvective_Water_Design_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateZoneHVACBaseboardRadiantConvectiveWater(ZoneHVACBaseboardRadiantConvectiveWater& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;
    boost::optional<ModelObject> temp;

    // Name
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::ZoneHVAC_Baseboard_RadiantConvective_Water, modelObject);

    // E+ 9.5.0: This object got broken into two: a zonehvac object, and a design object
    IdfObject designObject(openstudio::IddObjectType::ZoneHVAC_Baseboard_RadiantConvective_Water_Design);
    m_idfObjects.push_back(designObject);
    designObject.setName(idfObject.nameString() + " Design");

    idfObject.setString(ZoneHVAC_Baseboard_RadiantConvective_WaterFields::DesignObject, designObject.nameString());

    // AvailabilityScheduleName
    {
      auto schedule = modelObject.availabilitySchedule();
      if (auto _schedule = translateAndMapModelObject(schedule)) {
        idfObject.setString(ZoneHVAC_Baseboard_RadiantConvective_WaterFields::AvailabilityScheduleName, _schedule->name().get());
      }
    }

    if (auto heatingCoil = modelObject.heatingCoil().optionalCast<CoilHeatingWaterBaseboardRadiant>()) {

      // InletNodeName
      if (auto node = heatingCoil->inletModelObject()) {
        idfObject.setString(ZoneHVAC_Baseboard_RadiantConvective_WaterFields::InletNodeName, node->name().get());
      }

      // OutletNodeName
      if (auto node = heatingCoil->outletModelObject()) {
        idfObject.setString(ZoneHVAC_Baseboard_RadiantConvective_WaterFields::OutletNodeName, node->name().get());
      }

      // RatedAverageWaterTemperature
      if ((value = heatingCoil->ratedAverageWaterTemperature())) {
        idfObject.setDouble(ZoneHVAC_Baseboard_RadiantConvective_WaterFields::RatedAverageWaterTemperature, value.get());
      }

      // RatedWaterMassFlowRate
      if ((value = heatingCoil->ratedWaterMassFlowRate())) {
        idfObject.setDouble(ZoneHVAC_Baseboard_RadiantConvective_WaterFields::RatedWaterMassFlowRate, value.get());
      }

      // HeatingDesignCapacity
      if (heatingCoil->isHeatingDesignCapacityAutosized()) {
        idfObject.setString(ZoneHVAC_Baseboard_RadiantConvective_WaterFields::HeatingDesignCapacity, "AutoSize");
      } else if ((value = heatingCoil->heatingDesignCapacity())) {
        idfObject.setDouble(ZoneHVAC_Baseboard_RadiantConvective_WaterFields::HeatingDesignCapacity, value.get());
      }

      // MaximumWaterFlowRate
      if (heatingCoil->isMaximumWaterFlowRateAutosized()) {
        idfObject.setString(ZoneHVAC_Baseboard_RadiantConvective_WaterFields::MaximumWaterFlowRate, "AutoSize");
      } else if ((value = heatingCoil->maximumWaterFlowRate())) {
        idfObject.setDouble(ZoneHVAC_Baseboard_RadiantConvective_WaterFields::MaximumWaterFlowRate, value.get());
      }

      // Starting Here: all these fields are now on the Design object

      // HeatingDesignCapacityMethod
      if ((s = heatingCoil->heatingDesignCapacityMethod())) {
        designObject.setString(ZoneHVAC_Baseboard_RadiantConvective_Water_DesignFields::HeatingDesignCapacityMethod, s.get());
      }

      // HeatingDesignCapacityPerFloorArea
      if ((value = heatingCoil->heatingDesignCapacityPerFloorArea())) {
        designObject.setDouble(ZoneHVAC_Baseboard_RadiantConvective_Water_DesignFields::HeatingDesignCapacityPerFloorArea, value.get());
      }

      // FractionofAutosizedHeatingDesignCapacity
      if ((value = heatingCoil->fractionofAutosizedHeatingDesignCapacity())) {
        designObject.setDouble(ZoneHVAC_Baseboard_RadiantConvective_Water_DesignFields::FractionofAutosizedHeatingDesignCapacity, value.get());
      }

      // ConvergenceTolerance
      if ((value = heatingCoil->convergenceTolerance())) {
        designObject.setDouble(ZoneHVAC_Baseboard_RadiantConvective_Water_DesignFields::ConvergenceTolerance, value.get());
      }
    }

    // FractionRadiant: On Design object
    if ((value = modelObject.fractionRadiant())) {
      designObject.setDouble(ZoneHVAC_Baseboard_RadiantConvective_Water_DesignFields::FractionRadiant, value.get());
    }

    // FractionofRadiantEnergyIncidentonPeople: On Design object
    double fractionofRadiantEnergyIncidentonPeople = modelObject.fractionofRadiantEnergyIncidentonPeople();
    {
      designObject.setDouble(ZoneHVAC_Baseboard_RadiantConvective_Water_DesignFields::FractionofRadiantEnergyIncidentonPeople,
                             fractionofRadiantEnergyIncidentonPeople);
    }

    //get rid of any existing surface (just to be safe)
    idfObject.clearExtensibleGroups();

    //aggregator for total area; will be used to create weighted area
    double totalAreaOfWallSurfaces = 0;
    double totalAreaOfCeilingSurfaces = 0;
    double totalAreaOfFloorSurfaces = 0;

    //loop through all surfaces, adding up their area
    auto const& surfaces = modelObject.getImpl<model::detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>()->surfaces();

    for (auto const& surface : surfaces) {
      if (istringEqual(surface.surfaceType(), "Floor")) {
        totalAreaOfFloorSurfaces += surface.grossArea();
      } else if (istringEqual(surface.surfaceType(), "RoofCeiling")) {
        totalAreaOfCeilingSurfaces += surface.grossArea();
      } else {
        totalAreaOfWallSurfaces += surface.grossArea();
      }
    }

    // Assume that 5% of what is not on people is on the floor
    double fractionOnFloor = (1.0 - fractionofRadiantEnergyIncidentonPeople) * 0.05;
    // Assume that 55% of what is not on people is on the walls
    double fractionOnWall = (1.0 - fractionofRadiantEnergyIncidentonPeople) * 0.55;
    // Assume that 40% of what is not on people is on the ceiling
    double fractionOnCeiling = (1.0 - fractionofRadiantEnergyIncidentonPeople) * 0.40;
    //loop through all the surfaces, adding them and their flow fractions (weighted per-area)
    for (auto const& surface : surfaces) {
      IdfExtensibleGroup group = idfObject.pushExtensibleGroup();
      group.setString(ZoneHVAC_Baseboard_RadiantConvective_WaterExtensibleFields::SurfaceName, surface.name().get());
      if (istringEqual(surface.surfaceType(), "Floor")) {
        group.setDouble(ZoneHVAC_Baseboard_RadiantConvective_WaterExtensibleFields::FractionofRadiantEnergytoSurface,
                        (surface.grossArea() / totalAreaOfFloorSurfaces * fractionOnFloor));
      } else if (istringEqual(surface.surfaceType(), "RoofCeiling")) {
        group.setDouble(ZoneHVAC_Baseboard_RadiantConvective_WaterExtensibleFields::FractionofRadiantEnergytoSurface,
                        (surface.grossArea() / totalAreaOfCeilingSurfaces * fractionOnCeiling));
      } else {
        group.setDouble(ZoneHVAC_Baseboard_RadiantConvective_WaterExtensibleFields::FractionofRadiantEnergytoSurface,
                        (surface.grossArea() / totalAreaOfWallSurfaces * fractionOnWall));
      }
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
