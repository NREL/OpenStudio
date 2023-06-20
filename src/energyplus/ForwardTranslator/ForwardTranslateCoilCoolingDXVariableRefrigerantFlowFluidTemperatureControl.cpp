/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl.hpp"
#include "../../model/CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl.hpp"

#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"

#include <utilities/idd/Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControl_FieldEnums.hxx>
// #include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl(
    model::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;

    IdfObject idfObject(IddObjectType::Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControl);

    m_idfObjects.push_back(idfObject);

    // Name
    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }

    // Availability Schedule Name: Optional Object
    if (boost::optional<Schedule> _availabilitySchedule = modelObject.availabilitySchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_availabilitySchedule.get())) {
        idfObject.setString(Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::AvailabilityScheduleName, _owo->nameString());
      }
    }

    if (modelObject.isRatedTotalCoolingCapacityAutosized()) {
      idfObject.setString(Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedTotalCoolingCapacity, "Autosize");
    } else {
      // Rated Total Cooling Capacity: boost::optional<double>
      if (boost::optional<double> _ratedTotalCoolingCapacity = modelObject.ratedTotalCoolingCapacity()) {
        idfObject.setDouble(Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedTotalCoolingCapacity,
                            _ratedTotalCoolingCapacity.get());
      }
    }

    if (modelObject.isRatedSensibleHeatRatioAutosized()) {
      idfObject.setString(Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedSensibleHeatRatio, "Autosize");
    } else {
      // Rated Sensible Heat Ratio: boost::optional<double>
      if (boost::optional<double> _ratedSensibleHeatRatio = modelObject.ratedSensibleHeatRatio()) {
        idfObject.setDouble(Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedSensibleHeatRatio,
                            _ratedSensibleHeatRatio.get());
      }
    }

    // Indoor Unit Reference Superheating: Optional Double
    double indoorUnitReferenceSuperheating = modelObject.indoorUnitReferenceSuperheating();
    idfObject.setDouble(Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::IndoorUnitReferenceSuperheating,
                        indoorUnitReferenceSuperheating);

    // Indoor Unit Evaporating Temperature Function of Superheating Curve Name: Required Object
    Curve indoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve = modelObject.indoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve();
    if (boost::optional<IdfObject> _owo = translateAndMapModelObject(indoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve)) {
      idfObject.setString(
        Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::IndoorUnitEvaporatingTemperatureFunctionofSuperheatingCurveName,
        _owo->nameString());
    }

    return idfObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio