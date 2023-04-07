/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
