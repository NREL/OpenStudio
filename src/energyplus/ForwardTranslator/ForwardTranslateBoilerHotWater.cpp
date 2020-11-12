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
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"
#include "../../model/BoilerHotWater.hpp"
#include "../../model/BoilerHotWater_Impl.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/Boiler_HotWater_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateBoilerHotWater(BoilerHotWater& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;

    IdfObject idfObject(IddObjectType::Boiler_HotWater);

    m_idfObjects.push_back(idfObject);

    // Name

    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }

    // FuelType

    if ((s = modelObject.fuelType())) {
      idfObject.setString(Boiler_HotWaterFields::FuelType, s.get());
    }

    // NominalCapacity

    if (modelObject.isNominalCapacityAutosized()) {
      idfObject.setString(Boiler_HotWaterFields::NominalCapacity, "Autosize");
    } else if ((value = modelObject.nominalCapacity())) {
      idfObject.setDouble(Boiler_HotWaterFields::NominalCapacity, value.get());
    }

    // NominalThermalEfficiency

    if ((value = modelObject.nominalThermalEfficiency())) {
      idfObject.setDouble(Boiler_HotWaterFields::NominalThermalEfficiency, value.get());
    }

    // EfficiencyCurveTemperatureEvaluationVariable

    if ((s = modelObject.efficiencyCurveTemperatureEvaluationVariable())) {
      idfObject.setString(Boiler_HotWaterFields::EfficiencyCurveTemperatureEvaluationVariable, s.get());
    }

    // Normalized Boiler Efficiency Curve

    if (auto value = modelObject.efficiencyCurveTemperatureEvaluationVariable()) {
      if (!value->empty()) {
        if (auto curve = modelObject.normalizedBoilerEfficiencyCurve()) {
          if (auto _curve = translateAndMapModelObject(curve.get())) {
            idfObject.setString(Boiler_HotWaterFields::NormalizedBoilerEfficiencyCurveName, _curve->name().get());
          }
        }
      }
    }

    // DesignWaterFlowRate

    if (modelObject.isDesignWaterFlowRateAutosized()) {
      idfObject.setString(Boiler_HotWaterFields::DesignWaterFlowRate, "Autosize");
    } else if ((value = modelObject.designWaterFlowRate())) {
      idfObject.setDouble(Boiler_HotWaterFields::DesignWaterFlowRate, value.get());
    }

    // MinimumPartLoadRatio

    if ((value = modelObject.minimumPartLoadRatio())) {
      idfObject.setDouble(Boiler_HotWaterFields::MinimumPartLoadRatio, value.get());
    }

    // MaximumPartLoadRatio

    if ((value = modelObject.maximumPartLoadRatio())) {
      idfObject.setDouble(Boiler_HotWaterFields::MaximumPartLoadRatio, value.get());
    }

    // OptimumPartLoadRatio

    if ((value = modelObject.optimumPartLoadRatio())) {
      idfObject.setDouble(Boiler_HotWaterFields::OptimumPartLoadRatio, value.get());
    }

    // BoilerWaterInletNodeName

    if (boost::optional<ModelObject> mo = modelObject.inletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Boiler_HotWaterFields::BoilerWaterInletNodeName, node->name().get());
      }
    }

    // BoilerWaterOutletNodeName

    if (boost::optional<ModelObject> mo = modelObject.outletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Boiler_HotWaterFields::BoilerWaterOutletNodeName, node->name().get());
      }
    }

    // WaterOutletUpperTemperatureLimit

    if ((value = modelObject.waterOutletUpperTemperatureLimit())) {
      idfObject.setDouble(Boiler_HotWaterFields::WaterOutletUpperTemperatureLimit, value.get());
    }

    // BoilerFlowMode

    if ((s = modelObject.boilerFlowMode())) {
      idfObject.setString(Boiler_HotWaterFields::BoilerFlowMode, s.get());
    }

    // ParasiticElectricLoad

    if ((value = modelObject.parasiticElectricLoad())) {
      idfObject.setDouble(Boiler_HotWaterFields::ParasiticElectricLoad, value.get());
    }

    // SizingFactor

    if ((value = modelObject.sizingFactor())) {
      idfObject.setDouble(Boiler_HotWaterFields::SizingFactor, value.get());
    }

    // End Use Subcategory
    if ((s = modelObject.endUseSubcategory())) {
      idfObject.setString(Boiler_HotWaterFields::EndUseSubcategory, s.get());
    }

    return boost::optional<IdfObject>(idfObject);
  }

  //OPENSTUDIO_ENUM( Boiler_HotWaterFields,
  //  ((NormalizedBoilerEfficiencyCurveName)(Normalized Boiler Efficiency Curve Name))

}  // namespace energyplus

}  // namespace openstudio
