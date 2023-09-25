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
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ZoneHVACBaseboardConvectiveWater.hpp"
#include "../../model/ZoneHVACBaseboardConvectiveWater_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/StraightComponent.hpp"
#include "../../model/StraightComponent_Impl.hpp"
#include "../../model/CoilHeatingWaterBaseboard.hpp"
#include "../../model/CoilHeatingWaterBaseboard_Impl.hpp"

#include <utilities/idd/ZoneHVAC_Baseboard_Convective_Water_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateZoneHVACBaseboardConvectiveWater(ZoneHVACBaseboardConvectiveWater& modelObject) {
    // Makes sure the modelObject gets put in the map, and that the new idfObject gets put in
    // the final file. Also set's the idfObject's name.
    IdfObject idfObject = createRegisterAndNameIdfObject(IddObjectType::ZoneHVAC_Baseboard_Convective_Water, modelObject);

    boost::optional<std::string> s;
    boost::optional<double> value;
    boost::optional<ModelObject> temp;
    //get the heating coil as a straight component, cast it to optional HT coil, if cast is successful,
    //get the object of class CoilHeatingWaterBaseboard
    StraightComponent coilStraight = modelObject.heatingCoil();
    boost::optional<CoilHeatingWaterBaseboard> coilOptionalHeatBBConvWater = coilStraight.optionalCast<CoilHeatingWaterBaseboard>();

    // AvailabilityScheduleName
    Schedule availabilitySchedule = modelObject.availabilitySchedule();
    translateAndMapModelObject(availabilitySchedule);
    idfObject.setString(ZoneHVAC_Baseboard_Convective_WaterFields::AvailabilityScheduleName, availabilitySchedule.name().get());

    if (coilOptionalHeatBBConvWater) {
      CoilHeatingWaterBaseboard coilHeatBBConvWater = *coilOptionalHeatBBConvWater;

      // Heating Design Capacity Method - introduced in 8.2.0 and not yet supported in OS
      idfObject.setString(ZoneHVAC_Baseboard_Convective_WaterFields::HeatingDesignCapacityMethod, "HeatingDesignCapacity");

      // Heating Design Capacity - introduced in 8.2.0 and not yet supported in OS
      idfObject.setString(ZoneHVAC_Baseboard_Convective_WaterFields::HeatingDesignCapacity, "Autosize");

      // Inlet Node Name
      temp = coilHeatBBConvWater.inletModelObject();
      if (temp) {
        s = temp->name();
        if (s) {
          idfObject.setString(openstudio::ZoneHVAC_Baseboard_Convective_WaterFields::InletNodeName, *s);
        }
      }

      // Outlet Node Name
      temp = coilHeatBBConvWater.outletModelObject();
      if (temp) {
        s = temp->name();
        if (s) {
          idfObject.setString(openstudio::ZoneHVAC_Baseboard_Convective_WaterFields::OutletNodeName, *s);
        }
      }

      {
        auto value = coilHeatBBConvWater.heatingDesignCapacityMethod();
        idfObject.setString(ZoneHVAC_Baseboard_Convective_WaterFields::HeatingDesignCapacityMethod, value);
      }

      if (coilHeatBBConvWater.isHeatingDesignCapacityAutosized()) {
        idfObject.setString(ZoneHVAC_Baseboard_Convective_WaterFields::HeatingDesignCapacity, "Autosize");
      } else if (auto value = coilHeatBBConvWater.heatingDesignCapacity()) {
        idfObject.setDouble(ZoneHVAC_Baseboard_Convective_WaterFields::HeatingDesignCapacity, value.get());
      }

      {
        auto value = coilHeatBBConvWater.heatingDesignCapacityPerFloorArea();
        idfObject.setDouble(ZoneHVAC_Baseboard_Convective_WaterFields::HeatingDesignCapacityPerFloorArea, value);
      }

      {
        auto value = coilHeatBBConvWater.fractionofAutosizedHeatingDesignCapacity();
        idfObject.setDouble(ZoneHVAC_Baseboard_Convective_WaterFields::FractionofAutosizedHeatingDesignCapacity, value);
      }

      // UFactorTimesAreaValue
      if (coilHeatBBConvWater.isUFactorTimesAreaValueAutosized()) {
        idfObject.setString(ZoneHVAC_Baseboard_Convective_WaterFields::UFactorTimesAreaValue, "Autosize");
      } else if ((value = coilHeatBBConvWater.uFactorTimesAreaValue())) {
        idfObject.setDouble(ZoneHVAC_Baseboard_Convective_WaterFields::UFactorTimesAreaValue, value.get());
      }

      // MaximumWaterFlowRate

      if (coilHeatBBConvWater.isMaximumWaterFlowRateAutosized()) {
        idfObject.setString(ZoneHVAC_Baseboard_Convective_WaterFields::MaximumWaterFlowRate, "Autosize");
      } else if ((value = coilHeatBBConvWater.maximumWaterFlowRate())) {
        idfObject.setDouble(ZoneHVAC_Baseboard_Convective_WaterFields::MaximumWaterFlowRate, value.get());
      }

      // Convergence Tolerance

      if (coilHeatBBConvWater.isConvergenceToleranceDefaulted()) {
        idfObject.setDouble(ZoneHVAC_Baseboard_Convective_WaterFields::ConvergenceTolerance, 0.001);
      } else if ((value = coilHeatBBConvWater.convergenceTolerance())) {
        idfObject.setDouble(ZoneHVAC_Baseboard_Convective_WaterFields::ConvergenceTolerance, value.get());
      }
    }
    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
