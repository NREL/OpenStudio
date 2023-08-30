/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/ThermalStorageIceDetailed.hpp"
#include "../../model/ThermalStorageIceDetailed_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/ThermalStorage_Ice_Detailed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateThermalStorageIceDetailed(ThermalStorageIceDetailed& modelObject) {
    IdfObject idfObject(IddObjectType::ThermalStorage_Ice_Detailed);
    m_idfObjects.push_back(idfObject);

    // Name
    if (auto s = modelObject.name()) {
      idfObject.setName(*s);
    }

    // InletNodeName
    if (auto mo = modelObject.inletModelObject()) {
      if (auto node = mo->optionalCast<Node>()) {
        idfObject.setString(ThermalStorage_Ice_DetailedFields::InletNodeName, node->name().get());
      }
    }

    // OutletNodeName
    if (auto mo = modelObject.outletModelObject()) {
      if (auto node = mo->optionalCast<Node>()) {
        idfObject.setString(ThermalStorage_Ice_DetailedFields::OutletNodeName, node->name().get());
      }
    }

    // AvailabilityScheduleName
    if (auto schedule = modelObject.availabilitySchedule()) {
      idfObject.setString(ThermalStorage_Ice_DetailedFields::AvailabilityScheduleName, schedule->name().get());
    }

    // Capacity
    {
      auto value = modelObject.capacity();
      idfObject.setDouble(ThermalStorage_Ice_DetailedFields::Capacity, value);
    }

    // DischargingCurveObjectType
    // DischargingCurveName
    {
      auto curve = modelObject.dischargingCurve();
      auto idf = translateAndMapModelObject(curve);
      OS_ASSERT(idf);

      idfObject.setString(ThermalStorage_Ice_DetailedFields::DischargingCurveVariableSpecifications,
                          modelObject.dischargingCurveVariableSpecifications());
      idfObject.setString(ThermalStorage_Ice_DetailedFields::DischargingCurveName, idf->name().get());
    }

    // ChargingCurveObjectType
    // ChargingCurveName
    {
      auto curve = modelObject.chargingCurve();
      auto idf = translateAndMapModelObject(curve);
      OS_ASSERT(idf);

      idfObject.setString(ThermalStorage_Ice_DetailedFields::ChargingCurveVariableSpecifications, modelObject.chargingCurveVariableSpecifications());
      idfObject.setString(ThermalStorage_Ice_DetailedFields::ChargingCurveName, idf->name().get());
    }

    // TimestepoftheCurveData
    {
      auto value = modelObject.timestepoftheCurveData();
      idfObject.setDouble(ThermalStorage_Ice_DetailedFields::TimestepoftheCurveData, value);
    }

    // ParasiticElectricLoadDuringDischarging
    {
      auto value = modelObject.parasiticElectricLoadDuringDischarging();
      idfObject.setDouble(ThermalStorage_Ice_DetailedFields::ParasiticElectricLoadDuringDischarging, value);
    }

    // ParasiticElectricLoadDuringCharging
    {
      auto value = modelObject.parasiticElectricLoadDuringCharging();
      idfObject.setDouble(ThermalStorage_Ice_DetailedFields::ParasiticElectricLoadDuringCharging, value);
    }

    // TankLossCoefficient
    {
      auto value = modelObject.tankLossCoefficient();
      idfObject.setDouble(ThermalStorage_Ice_DetailedFields::TankLossCoefficient, value);
    }

    // FreezingTemperatureofStorageMedium
    {
      auto value = modelObject.freezingTemperatureofStorageMedium();
      idfObject.setDouble(ThermalStorage_Ice_DetailedFields::FreezingTemperatureofStorageMedium, value);
    }

    // ThawProcessIndicator
    {
      auto value = modelObject.thawProcessIndicator();
      idfObject.setString(ThermalStorage_Ice_DetailedFields::ThawProcessIndicator, value);
    }

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
