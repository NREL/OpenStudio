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
#include "../../model/CoilPerformanceDXCooling.hpp"
#include "../../model/CoilPerformanceDXCooling_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/CoilPerformance_DX_Cooling_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCoilPerformanceDXCooling(CoilPerformanceDXCooling& modelObject) {
    IdfObject idfObject(IddObjectType::CoilPerformance_DX_Cooling);
    m_idfObjects.push_back(idfObject);

    // Name
    if (auto s = modelObject.name()) {
      idfObject.setName(*s);
    }

    // GrossRatedTotalCoolingCapacity
    if (modelObject.isGrossRatedTotalCoolingCapacityAutosized()) {
      idfObject.setString(CoilPerformance_DX_CoolingFields::GrossRatedTotalCoolingCapacity, "Autosize");
    } else if (auto value = modelObject.grossRatedTotalCoolingCapacity()) {
      idfObject.setDouble(CoilPerformance_DX_CoolingFields::GrossRatedTotalCoolingCapacity, value.get());
    }

    // GrossRatedSensibleHeatRatio
    if (modelObject.isGrossRatedSensibleHeatRatioAutosized()) {
      idfObject.setString(CoilPerformance_DX_CoolingFields::GrossRatedSensibleHeatRatio, "Autosize");
    } else if (auto value = modelObject.grossRatedSensibleHeatRatio()) {
      idfObject.setDouble(CoilPerformance_DX_CoolingFields::GrossRatedSensibleHeatRatio, value.get());
    }

    // GrossRatedCoolingCOP
    {
      auto value = modelObject.grossRatedCoolingCOP();
      idfObject.setDouble(CoilPerformance_DX_CoolingFields::GrossRatedCoolingCOP, value);
    }

    // RatedAirFlowRate
    if (modelObject.isRatedAirFlowRateAutosized()) {
      idfObject.setString(CoilPerformance_DX_CoolingFields::RatedAirFlowRate, "Autosize");
    } else if (auto value = modelObject.ratedAirFlowRate()) {
      idfObject.setDouble(CoilPerformance_DX_CoolingFields::RatedAirFlowRate, value.get());
    }

    // FractionofAirFlowBypassedAroundCoil
    {
      auto value = modelObject.fractionofAirFlowBypassedAroundCoil();
      idfObject.setDouble(CoilPerformance_DX_CoolingFields::FractionofAirFlowBypassedAroundCoil, value);
    }

    // TotalCoolingCapacityFunctionofTemperatureCurveName
    {
      auto curve = modelObject.totalCoolingCapacityFunctionofTemperatureCurve();
      if (auto idf = translateAndMapModelObject(curve)) {
        idfObject.setString(CoilPerformance_DX_CoolingFields::TotalCoolingCapacityFunctionofTemperatureCurveName, idf->name().get());
      }
    }

    // TotalCoolingCapacityFunctionofFlowFractionCurveName
    {
      auto curve = modelObject.totalCoolingCapacityFunctionofFlowFractionCurve();
      if (auto idf = translateAndMapModelObject(curve)) {
        idfObject.setString(CoilPerformance_DX_CoolingFields::TotalCoolingCapacityFunctionofFlowFractionCurveName, idf->name().get());
      }
    }

    // EnergyInputRatioFunctionofTemperatureCurve
    {
      auto curve = modelObject.energyInputRatioFunctionofTemperatureCurve();
      if (auto idf = translateAndMapModelObject(curve)) {
        idfObject.setString(CoilPerformance_DX_CoolingFields::EnergyInputRatioFunctionofTemperatureCurveName, idf->name().get());
      }
    }

    // EnergyInputRatioFunctionofFlowFractionCurve
    {
      auto curve = modelObject.energyInputRatioFunctionofFlowFractionCurve();
      if (auto idf = translateAndMapModelObject(curve)) {
        idfObject.setString(CoilPerformance_DX_CoolingFields::EnergyInputRatioFunctionofFlowFractionCurveName, idf->name().get());
      }
    }

    // PartLoadFractionCorrelationCurveName
    {
      auto curve = modelObject.partLoadFractionCorrelationCurve();
      if (auto idf = translateAndMapModelObject(curve)) {
        idfObject.setString(CoilPerformance_DX_CoolingFields::PartLoadFractionCorrelationCurveName, idf->name().get());
      }
    }

    // NominalTimeforCondensateRemovaltoBegin
    {
      auto value = modelObject.nominalTimeforCondensateRemovaltoBegin();
      idfObject.setDouble(CoilPerformance_DX_CoolingFields::NominalTimeforCondensateRemovaltoBegin, value);
    }

    // RatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity
    {
      auto value = modelObject.ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity();
      idfObject.setDouble(CoilPerformance_DX_CoolingFields::RatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity, value);
    }

    // MaximumCyclingRate
    {
      auto value = modelObject.maximumCyclingRate();
      idfObject.setDouble(CoilPerformance_DX_CoolingFields::MaximumCyclingRate, value);
    }

    // LatentCapacityTimeConstant
    {
      auto value = modelObject.latentCapacityTimeConstant();
      idfObject.setDouble(CoilPerformance_DX_CoolingFields::LatentCapacityTimeConstant, value);
    }

    // CondenserAirInletNode
    if (auto node = modelObject.condenserAirInletNode()) {
      idfObject.setString(CoilPerformance_DX_CoolingFields::CondenserAirInletNodeName, node->name().get());
    }

    // CondenserType
    {
      auto value = modelObject.condenserType();
      idfObject.setString(CoilPerformance_DX_CoolingFields::CondenserType, value);
    }

    // EvaporativeCondenserEffectiveness
    {
      auto value = modelObject.evaporativeCondenserEffectiveness();
      idfObject.setDouble(CoilPerformance_DX_CoolingFields::EvaporativeCondenserEffectiveness, value);
    }

    // EvaporativeCondenserAirFlowRate
    if (modelObject.isEvaporativeCondenserAirFlowRateAutosized()) {
      idfObject.setString(CoilPerformance_DX_CoolingFields::EvaporativeCondenserAirFlowRate, "Autosize");
    } else if (auto value = modelObject.evaporativeCondenserAirFlowRate()) {
      idfObject.setDouble(CoilPerformance_DX_CoolingFields::EvaporativeCondenserAirFlowRate, value.get());
    }

    // EvaporativeCondenserPumpRatedPowerConsumption
    if (modelObject.isEvaporativeCondenserPumpRatedPowerConsumptionAutosized()) {
      idfObject.setString(CoilPerformance_DX_CoolingFields::EvaporativeCondenserPumpRatedPowerConsumption, "Autosize");
    } else if (auto value = modelObject.evaporativeCondenserPumpRatedPowerConsumption()) {
      idfObject.setDouble(CoilPerformance_DX_CoolingFields::EvaporativeCondenserPumpRatedPowerConsumption, value.get());
    }

    // SensibleHeatRatioFunctionofTemperatureCurveName
    if (auto curve = modelObject.sensibleHeatRatioFunctionofTemperatureCurve()) {
      if (auto idf = translateAndMapModelObject(curve.get())) {
        idfObject.setString(CoilPerformance_DX_CoolingFields::SensibleHeatRatioFunctionofTemperatureCurveName, idf->name().get());
      }
    }

    // SensibleHeatRatioFunctionofFlowFractionCurveName
    if (auto curve = modelObject.sensibleHeatRatioFunctionofFlowFractionCurve()) {
      if (auto idf = translateAndMapModelObject(curve.get())) {
        idfObject.setString(CoilPerformance_DX_CoolingFields::SensibleHeatRatioFunctionofFlowFractionCurveName, idf->name().get());
      }
    }

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
