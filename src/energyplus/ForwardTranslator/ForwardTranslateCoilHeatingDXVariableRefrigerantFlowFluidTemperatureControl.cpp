/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl.hpp"
#include "../../model/CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl.hpp"

#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"

#include <utilities/idd/Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControl_FieldEnums.hxx>
// #include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl(
    model::CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;

    IdfObject idfObject(IddObjectType::Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControl);

    m_idfObjects.push_back(idfObject);

    // Name
    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }

    // Availability Schedule: Optional Object
    if (boost::optional<Schedule> _availabilitySchedule = modelObject.availabilitySchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_availabilitySchedule.get())) {
        idfObject.setString(Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::AvailabilitySchedule, _owo->nameString());
      }
    }

    if (modelObject.isRatedTotalHeatingCapacityAutosized()) {
      idfObject.setString(Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedTotalHeatingCapacity, "Autosize");
    } else {
      // Rated Total Heating Capacity: boost::optional<double>
      if (boost::optional<double> _ratedTotalHeatingCapacity = modelObject.ratedTotalHeatingCapacity()) {
        idfObject.setDouble(Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedTotalHeatingCapacity,
                            _ratedTotalHeatingCapacity.get());
      }
    }

    // Indoor Unit Reference Subcooling: Optional Double
    double indoorUnitReferenceSubcooling = modelObject.indoorUnitReferenceSubcooling();
    idfObject.setDouble(Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::IndoorUnitReferenceSubcooling,
                        indoorUnitReferenceSubcooling);

    // Indoor Unit Condensing Temperature Function of Subcooling Curve Name: Required Object
    Curve indoorUnitCondensingTemperatureFunctionofSubcoolingCurve = modelObject.indoorUnitCondensingTemperatureFunctionofSubcoolingCurve();
    if (boost::optional<IdfObject> _owo = translateAndMapModelObject(indoorUnitCondensingTemperatureFunctionofSubcoolingCurve)) {
      idfObject.setString(
        Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::IndoorUnitCondensingTemperatureFunctionofSubcoolingCurveName,
        _owo->nameString());
    }

    return idfObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio