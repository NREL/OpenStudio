/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/CoilHeatingDXVariableRefrigerantFlow.hpp"
#include "../../model/CoilHeatingDXVariableRefrigerantFlow_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"
#include "../../model/CurveBiquadratic.hpp"
#include "../../model/CurveBiquadratic_Impl.hpp"
#include "../../model/CurveCubic.hpp"
#include "../../model/CurveCubic_Impl.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/Coil_Heating_DX_VariableRefrigerantFlow_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCoilHeatingDXVariableRefrigerantFlow(CoilHeatingDXVariableRefrigerantFlow& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;

    IdfObject idfObject(IddObjectType::Coil_Heating_DX_VariableRefrigerantFlow);

    m_idfObjects.push_back(idfObject);

    // Name

    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }

    // AvailabilitySchedule

    if (boost::optional<model::Schedule> schedule = modelObject.availabilitySchedule()) {
      if (boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get())) {
        idfObject.setString(Coil_Heating_DX_VariableRefrigerantFlowFields::AvailabilitySchedule, _schedule->name().get());
      }
    }

    // RatedTotalHeatingCapacity

    if (modelObject.isRatedTotalHeatingCapacityAutosized()) {
      idfObject.setString(Coil_Heating_DX_VariableRefrigerantFlowFields::GrossRatedHeatingCapacity, "Autosize");
    } else if ((value = modelObject.ratedTotalHeatingCapacity())) {
      idfObject.setDouble(Coil_Heating_DX_VariableRefrigerantFlowFields::GrossRatedHeatingCapacity, value.get());
    }

    // RatedAirFlowRate

    if (modelObject.isRatedAirFlowRateAutosized()) {
      idfObject.setString(Coil_Heating_DX_VariableRefrigerantFlowFields::RatedAirFlowRate, "Autosize");
    } else if ((value = modelObject.ratedAirFlowRate())) {
      idfObject.setDouble(Coil_Heating_DX_VariableRefrigerantFlowFields::RatedAirFlowRate, value.get());
    }

    // HeatingCapacityRatioModifierFunctionofTemperatureCurveName

    if (boost::optional<model::Curve> curve = modelObject.heatingCapacityRatioModifierFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(Coil_Heating_DX_VariableRefrigerantFlowFields::HeatingCapacityRatioModifierFunctionofTemperatureCurveName,
                            _curve->name().get());
      }
    }

    // HeatingCapacityModifierFunctionofFlowFractionCurveName

    if (boost::optional<model::Curve> curve = modelObject.heatingCapacityModifierFunctionofFlowFractionCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(Coil_Heating_DX_VariableRefrigerantFlowFields::HeatingCapacityModifierFunctionofFlowFractionCurveName,
                            _curve->name().get());
      }
    }

    // TODO CoilAirInletNode
    // TODO CoilAirOutletNode

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
