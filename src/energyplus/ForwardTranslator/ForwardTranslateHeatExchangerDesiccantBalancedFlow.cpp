/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/HeatExchangerDesiccantBalancedFlow.hpp"

#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/HeatExchangerDesiccantBalancedFlowPerformanceDataType1.hpp"
#include "../../model/HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl.hpp"

#include <utilities/idd/HeatExchanger_Desiccant_BalancedFlow_FieldEnums.hxx>
// #include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateHeatExchangerDesiccantBalancedFlow(HeatExchangerDesiccantBalancedFlow& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;
    OptionalModelObject temp;

    // HeatExchangerPerformance, is required, so start by that
    HeatExchangerDesiccantBalancedFlowPerformanceDataType1 performance = modelObject.heatExchangerPerformance();
    boost::optional<IdfObject> _performance = translateAndMapModelObject(performance);
    if (!_performance) {
      LOG(Warn, modelObject.briefDescription() << " cannot be translated as its performance object cannot be translated: "
                                               << performance.briefDescription() << ".");
      return boost::none;
    }

    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::HeatExchanger_Desiccant_BalancedFlow, modelObject);

    // AvailabilityScheduleName
    Schedule sched = modelObject.availabilitySchedule();
    translateAndMapModelObject(sched);
    idfObject.setString(HeatExchanger_Desiccant_BalancedFlowFields::AvailabilityScheduleName, sched.name().get());

    // RegenerationAirInletNodeName
    temp = modelObject.primaryAirInletModelObject();
    if (temp) {
      idfObject.setString(HeatExchanger_Desiccant_BalancedFlowFields::RegenerationAirInletNodeName, temp->nameString());
    }

    // RegenerationAirOutletNodeName
    temp = modelObject.primaryAirOutletModelObject();
    if (temp) {
      idfObject.setString(HeatExchanger_Desiccant_BalancedFlowFields::RegenerationAirOutletNodeName, temp->nameString());
    }

    // ProcessAirInletNodeName
    temp = modelObject.secondaryAirInletModelObject();
    if (temp) {
      idfObject.setString(HeatExchanger_Desiccant_BalancedFlowFields::ProcessAirInletNodeName, temp->nameString());
    }

    // ProcessAirOutletNodeName
    temp = modelObject.secondaryAirOutletModelObject();
    if (temp) {
      idfObject.setString(HeatExchanger_Desiccant_BalancedFlowFields::ProcessAirOutletNodeName, temp->nameString());
    }

    // HeatExchangerPerformanceObjectType
    idfObject.setString(HeatExchanger_Desiccant_BalancedFlowFields::HeatExchangerPerformanceObjectType, _performance->iddObject().name());

    // HeatExchangerPerformanceName`
    idfObject.setString(HeatExchanger_Desiccant_BalancedFlowFields::HeatExchangerPerformanceName, _performance->name().get());

    // EconomizerLockout
    if (modelObject.economizerLockout()) {
      idfObject.setString(HeatExchanger_Desiccant_BalancedFlowFields::EconomizerLockout, "Yes");
    } else {
      idfObject.setString(HeatExchanger_Desiccant_BalancedFlowFields::EconomizerLockout, "No");
    }

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
