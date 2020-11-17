/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
