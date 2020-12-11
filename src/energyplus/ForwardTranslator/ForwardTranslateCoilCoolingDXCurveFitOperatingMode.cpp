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

#include "../../model/CoilCoolingDXCurveFitOperatingMode.hpp"

#include "../../model/CoilCoolingDXCurveFitSpeed.hpp"
#include "../../model/CoilCoolingDXCurveFitSpeed_Impl.hpp"

#include <utilities/idd/Coil_Cooling_DX_CurveFit_OperatingMode_FieldEnums.hxx>
// #include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCoilCoolingDXCurveFitOperatingMode(model::CoilCoolingDXCurveFitOperatingMode& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;

    IdfObject idfObject(openstudio::IddObjectType::Coil_Cooling_DX_CurveFit_OperatingMode);

    m_idfObjects.push_back(idfObject);

    // Name
    idfObject.setString(Coil_Cooling_DX_CurveFit_OperatingModeFields::Name, modelObject.name().get());

    // RatedGrossTotalCoolingCapacity
    value = modelObject.ratedGrossTotalCoolingCapacity();
    if (value) {
      idfObject.setDouble(Coil_Cooling_DX_CurveFit_OperatingModeFields::RatedGrossTotalCoolingCapacity, value.get());
    } else {
      idfObject.setString(Coil_Cooling_DX_CurveFit_OperatingModeFields::RatedGrossTotalCoolingCapacity, "Autosize");
    }

    // RatedEvaporatorAirFlowRate
    value = modelObject.ratedEvaporatorAirFlowRate();
    if (value) {
      idfObject.setDouble(Coil_Cooling_DX_CurveFit_OperatingModeFields::RatedEvaporatorAirFlowRate, value.get());
    } else {
      idfObject.setString(Coil_Cooling_DX_CurveFit_OperatingModeFields::RatedEvaporatorAirFlowRate, "Autosize");
    }

    // RatedCondenserAirFlowRate: Unused if condenserType = "AirCooled"
    value = modelObject.ratedCondenserAirFlowRate();
    if (value) {
      idfObject.setDouble(Coil_Cooling_DX_CurveFit_OperatingModeFields::RatedCondenserAirFlowRate, value.get());
    } else {
      idfObject.setString(Coil_Cooling_DX_CurveFit_OperatingModeFields::RatedCondenserAirFlowRate, "Autosize");
    }

    // MaximumCyclingRate
    idfObject.setDouble(Coil_Cooling_DX_CurveFit_OperatingModeFields::MaximumCyclingRate, modelObject.maximumCyclingRate());

    // RatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity
    idfObject.setDouble(Coil_Cooling_DX_CurveFit_OperatingModeFields::RatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity,
                        modelObject.ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity());

    // LatentCapacityTimeConstant
    idfObject.setDouble(Coil_Cooling_DX_CurveFit_OperatingModeFields::LatentCapacityTimeConstant, modelObject.latentCapacityTimeConstant());

    // NominalTimeforCondensateRemovaltoBegin
    idfObject.setDouble(Coil_Cooling_DX_CurveFit_OperatingModeFields::NominalTimeforCondensateRemovaltoBegin,
                        modelObject.nominalTimeforCondensateRemovaltoBegin());

    // ApplyLatentDegradationtoSpeedsGreaterthan1
    if (modelObject.applyLatentDegradationtoSpeedsGreaterthan1()) {
      idfObject.setString(Coil_Cooling_DX_CurveFit_OperatingModeFields::ApplyLatentDegradationtoSpeedsGreaterthan1, "Yes");
    } else {
      idfObject.setString(Coil_Cooling_DX_CurveFit_OperatingModeFields::ApplyLatentDegradationtoSpeedsGreaterthan1, "No");
    }

    // CondenserType
    idfObject.setString(Coil_Cooling_DX_CurveFit_OperatingModeFields::CondenserType, modelObject.condenserType());

    // NominalEvaporativeCondenserPumpPower
    value = modelObject.nominalEvaporativeCondenserPumpPower();
    if (value) {
      idfObject.setDouble(Coil_Cooling_DX_CurveFit_OperatingModeFields::NominalEvaporativeCondenserPumpPower, value.get());
    } else {
      idfObject.setString(Coil_Cooling_DX_CurveFit_OperatingModeFields::NominalEvaporativeCondenserPumpPower, "Autosize");
    }

    // NominalSpeedNumber
    idfObject.setInt(Coil_Cooling_DX_CurveFit_OperatingModeFields::NominalSpeedNumber, modelObject.nominalSpeedNumber());

    // SpeedxName
    for (auto speed : modelObject.speeds()) {
      if (auto _s = translateAndMapModelObject(speed)) {
        auto eg = idfObject.pushExtensibleGroup();
        eg.setString(Coil_Cooling_DX_CurveFit_OperatingModeExtensibleFields::SpeedName, _s->nameString());
      } else {
        LOG(Warn, modelObject.briefDescription() << " cannot translate speed " << _s->briefDescription());
      }
    }

    return boost::optional<IdfObject>(idfObject);
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
