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
#include "../../model/ZoneHVACCoolingPanelRadiantConvectiveWater.hpp"
#include "../../model/ZoneHVACCoolingPanelRadiantConvectiveWater_Impl.hpp"
#include "../../model/ZoneHVACEquipmentList.hpp"
#include "../../model/ZoneHVACEquipmentList_Impl.hpp"
#include "../../model/ZoneHVACComponent.hpp"
#include "../../model/ZoneHVACComponent_Impl.hpp"
#include "../../model/CoilCoolingWaterPanelRadiant.hpp"
#include "../../model/CoilCoolingWaterPanelRadiant_Impl.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include <utilities/idd/ZoneHVAC_CoolingPanel_RadiantConvective_Water_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateZoneHVACCoolingPanelRadiantConvectiveWater(ZoneHVACCoolingPanelRadiantConvectiveWater& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;
    boost::optional<ModelObject> temp;

    // Name
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::ZoneHVAC_CoolingPanel_RadiantConvective_Water, modelObject);

    // AvailabilityScheduleName
    {
      auto schedule = modelObject.availabilitySchedule();
      if (auto _schedule = translateAndMapModelObject(schedule)) {
        idfObject.setString(ZoneHVAC_CoolingPanel_RadiantConvective_WaterFields::AvailabilityScheduleName, _schedule->name().get());
      }
    }

    if (auto coolingCoil = modelObject.coolingCoil().optionalCast<CoilCoolingWaterPanelRadiant>()) {

      // InletNodeName
      if (auto node = coolingCoil->inletModelObject()) {
        idfObject.setString(ZoneHVAC_CoolingPanel_RadiantConvective_WaterFields::WaterInletNodeName, node->name().get());
      }

      // OutletNodeName
      if (auto node = coolingCoil->outletModelObject()) {
        idfObject.setString(ZoneHVAC_CoolingPanel_RadiantConvective_WaterFields::WaterOutletNodeName, node->name().get());
      }

      // RatedInletWaterTemperature
      if ((value = coolingCoil->ratedInletWaterTemperature())) {
        idfObject.setDouble(ZoneHVAC_CoolingPanel_RadiantConvective_WaterFields::RatedInletWaterTemperature, value.get());
      }

      // RatedInletSpaceTemperature
      if ((value = coolingCoil->ratedInletSpaceTemperature())) {
        idfObject.setDouble(ZoneHVAC_CoolingPanel_RadiantConvective_WaterFields::RatedInletSpaceTemperature, value.get());
      }

      // RatedWaterMassFlowRate
      if ((value = coolingCoil->ratedWaterMassFlowRate())) {
        idfObject.setDouble(ZoneHVAC_CoolingPanel_RadiantConvective_WaterFields::RatedWaterMassFlowRate, value.get());
      }

      // CoolingDesignCapacityMethod
      if ((s = coolingCoil->coolingDesignCapacityMethod())) {
        idfObject.setString(ZoneHVAC_CoolingPanel_RadiantConvective_WaterFields::CoolingDesignCapacityMethod, s.get());
      }

      // CoolingDesignCapacity
      if (coolingCoil->isCoolingDesignCapacityAutosized()) {
        idfObject.setString(ZoneHVAC_CoolingPanel_RadiantConvective_WaterFields::CoolingDesignCapacity, "AutoSize");
      } else if ((value = coolingCoil->coolingDesignCapacity())) {
        idfObject.setDouble(ZoneHVAC_CoolingPanel_RadiantConvective_WaterFields::CoolingDesignCapacity, value.get());
      }

      // CoolingDesignCapacityPerFloorArea
      if ((value = coolingCoil->coolingDesignCapacityPerFloorArea())) {
        idfObject.setDouble(ZoneHVAC_CoolingPanel_RadiantConvective_WaterFields::CoolingDesignCapacityPerFloorArea, value.get());
      }

      // FractionofAutosizedCoolingDesignCapacity
      if ((value = coolingCoil->fractionofAutosizedCoolingDesignCapacity())) {
        idfObject.setDouble(ZoneHVAC_CoolingPanel_RadiantConvective_WaterFields::FractionofAutosizedCoolingDesignCapacity, value.get());
      }

      // MaximumChilledWaterFlowRate
      if (coolingCoil->isMaximumChilledWaterFlowRateAutosized()) {
        idfObject.setString(ZoneHVAC_CoolingPanel_RadiantConvective_WaterFields::MaximumChilledWaterFlowRate, "AutoSize");
      } else if ((value = coolingCoil->maximumChilledWaterFlowRate())) {
        idfObject.setDouble(ZoneHVAC_CoolingPanel_RadiantConvective_WaterFields::MaximumChilledWaterFlowRate, value.get());
      }

      // ControlType
      if ((s = coolingCoil->controlType())) {
        idfObject.setString(ZoneHVAC_CoolingPanel_RadiantConvective_WaterFields::ControlType, s.get());
      }

      // CoolingControlThrottlingRange
      if ((value = coolingCoil->coolingControlThrottlingRange())) {
        idfObject.setDouble(ZoneHVAC_CoolingPanel_RadiantConvective_WaterFields::CoolingControlThrottlingRange, value.get());
      }

      // CoolingControlTemperatureScheduleName
      if (auto schedule = coolingCoil->coolingControlTemperatureSchedule()) {
        idfObject.setString(ZoneHVAC_CoolingPanel_RadiantConvective_WaterFields::CoolingControlTemperatureScheduleName, schedule->name().get());
      }

      // CondensationControlType
      if ((s = coolingCoil->condensationControlType())) {
        idfObject.setString(ZoneHVAC_CoolingPanel_RadiantConvective_WaterFields::CondensationControlType, s.get());
      }

      // CondensationControlDewpointOffset
      if ((value = coolingCoil->condensationControlDewpointOffset())) {
        idfObject.setDouble(ZoneHVAC_CoolingPanel_RadiantConvective_WaterFields::CondensationControlDewpointOffset, value.get());
      }
    }

    // FractionRadiant
    if ((value = modelObject.fractionRadiant())) {
      idfObject.setDouble(ZoneHVAC_CoolingPanel_RadiantConvective_WaterFields::FractionRadiant, value.get());
    }

    // FractionofRadiantEnergyIncidentonPeople
    double fractionofRadiantEnergyIncidentonPeople = modelObject.fractionofRadiantEnergyIncidentonPeople();
    {
      idfObject.setDouble(ZoneHVAC_CoolingPanel_RadiantConvective_WaterFields::FractionofRadiantEnergyIncidentonPeople,
                          fractionofRadiantEnergyIncidentonPeople);
    }

    //get rid of any existing surface (just to be safe)
    idfObject.clearExtensibleGroups();

    //aggregator for total area; will be used to create weighted area
    double totalAreaOfWallSurfaces = 0;
    double totalAreaOfCeilingSurfaces = 0;
    double totalAreaOfFloorSurfaces = 0;

    //loop through all surfaces, adding up their area
    auto const& surfaces = modelObject.getImpl<model::detail::ZoneHVACCoolingPanelRadiantConvectiveWater_Impl>()->surfaces();

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
      group.setString(ZoneHVAC_CoolingPanel_RadiantConvective_WaterExtensibleFields::SurfaceName, surface.name().get());
      if (istringEqual(surface.surfaceType(), "Floor")) {
        group.setDouble(ZoneHVAC_CoolingPanel_RadiantConvective_WaterExtensibleFields::FractionofRadiantEnergytoSurface,
                        (surface.grossArea() / totalAreaOfFloorSurfaces * fractionOnFloor));
      } else if (istringEqual(surface.surfaceType(), "RoofCeiling")) {
        group.setDouble(ZoneHVAC_CoolingPanel_RadiantConvective_WaterExtensibleFields::FractionofRadiantEnergytoSurface,
                        (surface.grossArea() / totalAreaOfCeilingSurfaces * fractionOnCeiling));
      } else {
        group.setDouble(ZoneHVAC_CoolingPanel_RadiantConvective_WaterExtensibleFields::FractionofRadiantEnergytoSurface,
                        (surface.grossArea() / totalAreaOfWallSurfaces * fractionOnWall));
      }
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
