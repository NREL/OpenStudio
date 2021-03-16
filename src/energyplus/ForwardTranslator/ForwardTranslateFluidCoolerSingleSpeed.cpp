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
#include "../../model/FluidCoolerSingleSpeed.hpp"
#include "../../model/FluidCoolerSingleSpeed_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/FluidCooler_SingleSpeed_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateFluidCoolerSingleSpeed(FluidCoolerSingleSpeed& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::FluidCooler_SingleSpeed);

    m_idfObjects.push_back(idfObject);

    if (auto name = modelObject.name()) {
      idfObject.setName(name.get());
    }

    if (auto node = modelObject.inletModelObject()) {
      idfObject.setString(FluidCooler_SingleSpeedFields::WaterInletNodeName, node->name().get());
    }

    if (auto node = modelObject.outletModelObject()) {
      idfObject.setString(FluidCooler_SingleSpeedFields::WaterOutletNodeName, node->name().get());
    }

    {
      auto value = modelObject.performanceInputMethod();
      idfObject.setString(FluidCooler_SingleSpeedFields::PerformanceInputMethod, value);
    }

    if (istringEqual("UFactorTimesAreaAndDesignWaterFlowRate", modelObject.performanceInputMethod())) {
      if (modelObject.isDesignAirFlowRateUfactorTimesAreaValueAutosized()) {
        idfObject.setString(FluidCooler_SingleSpeedFields::DesignAirFlowRateUfactorTimesAreaValue, "Autosize");
      } else if (auto value = modelObject.designAirFlowRateUfactorTimesAreaValue()) {
        idfObject.setDouble(FluidCooler_SingleSpeedFields::DesignAirFlowRateUfactorTimesAreaValue, value.get());
      }
    }

    if (istringEqual("NominalCapacity", modelObject.performanceInputMethod())) {
      auto value = modelObject.nominalCapacity();
      idfObject.setDouble(FluidCooler_SingleSpeedFields::NominalCapacity, value);
    }

    {
      auto value = modelObject.designEnteringWaterTemperature();
      idfObject.setDouble(FluidCooler_SingleSpeedFields::DesignEnteringWaterTemperature, value);
    }

    {
      auto value = modelObject.designEnteringAirTemperature();
      idfObject.setDouble(FluidCooler_SingleSpeedFields::DesignEnteringAirTemperature, value);
    }

    {
      auto value = modelObject.designEnteringAirWetbulbTemperature();
      idfObject.setDouble(FluidCooler_SingleSpeedFields::DesignEnteringAirWetbulbTemperature, value);
    }

    if (modelObject.isDesignWaterFlowRateAutosized()) {
      idfObject.setString(FluidCooler_SingleSpeedFields::DesignWaterFlowRate, "Autosize");
    } else if (auto value = modelObject.designWaterFlowRate()) {
      idfObject.setDouble(FluidCooler_SingleSpeedFields::DesignWaterFlowRate, value.get());
    }

    if (modelObject.isDesignAirFlowRateAutosized()) {
      idfObject.setString(FluidCooler_SingleSpeedFields::DesignAirFlowRate, "Autosize");
    } else if (auto value = modelObject.designAirFlowRate()) {
      idfObject.setDouble(FluidCooler_SingleSpeedFields::DesignAirFlowRate, value.get());
    }

    if (modelObject.isDesignAirFlowRateFanPowerAutosized()) {
      idfObject.setString(FluidCooler_SingleSpeedFields::DesignAirFlowRateFanPower, "Autosize");
    } else if (auto value = modelObject.designAirFlowRateFanPower()) {
      idfObject.setDouble(FluidCooler_SingleSpeedFields::DesignAirFlowRateFanPower, value.get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
