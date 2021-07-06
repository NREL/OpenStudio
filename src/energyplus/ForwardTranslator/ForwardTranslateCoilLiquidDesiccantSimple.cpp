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
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/CoilLiquidDesiccantSimple.hpp"
#include "../../model/CoilLiquidDesiccantSimple_Impl.hpp"

#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/Coil_LiquidDesiccant_Simple_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCoilLiquidDesiccantSimple(CoilLiquidDesiccantSimple& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;

    IdfObject idfObject(IddObjectType::Coil_LiquidDesiccant_Simple);

    m_idfObjects.push_back(idfObject);

    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }

    Schedule sched = modelObject.availabilitySchedule();
    boost::optional<IdfObject> _sched = translateAndMapModelObject(sched);
    if (_sched) {
      idfObject.setString(Coil_LiquidDesiccant_SimpleFields::AvailabilityScheduleName, _sched->name().get());
    }

    // DesignWaterFlowRate

    if (modelObject.isDesignWaterFlowRateAutosized()) {
      idfObject.setString(Coil_LiquidDesiccant_SimpleFields::DesignWaterFlowRate, "Autosize");
    } else if ((value = modelObject.designWaterFlowRate())) {
      idfObject.setDouble(Coil_LiquidDesiccant_SimpleFields::DesignWaterFlowRate, value.get());
    }

    // DesignAirFlowRate

    if (modelObject.isDesignAirFlowRateAutosized()) {
      idfObject.setString(Coil_LiquidDesiccant_SimpleFields::DesignAirFlowRate, "Autosize");
    } else if ((value = modelObject.designAirFlowRate())) {
      idfObject.setDouble(Coil_LiquidDesiccant_SimpleFields::DesignAirFlowRate, value.get());
    }

    // DesignInletWaterTemperature

    if (modelObject.isDesignInletWaterTemperatureAutosized()) {
      idfObject.setString(Coil_LiquidDesiccant_SimpleFields::DesignInletWaterTemperature, "Autosize");
    } else if ((value = modelObject.designInletWaterTemperature())) {
      idfObject.setDouble(Coil_LiquidDesiccant_SimpleFields::DesignInletWaterTemperature, value.get());
    }

    // DesignInletAirTemperature

    if (modelObject.isDesignInletAirTemperatureAutosized()) {
      idfObject.setString(Coil_LiquidDesiccant_SimpleFields::DesignInletAirTemperature, "Autosize");
    } else if ((value = modelObject.designInletAirTemperature())) {
      idfObject.setDouble(Coil_LiquidDesiccant_SimpleFields::DesignInletAirTemperature, value.get());
    }

    // DesignOutletAirTemperature

    if (modelObject.isDesignOutletAirTemperatureAutosized()) {
      idfObject.setString(Coil_LiquidDesiccant_SimpleFields::DesignOutletAirTemperature, "Autosize");
    } else if ((value = modelObject.designOutletAirTemperature())) {
      idfObject.setDouble(Coil_LiquidDesiccant_SimpleFields::DesignOutletAirTemperature, value.get());
    }

    // DesignInletAirHumidityRatio

    if (modelObject.isDesignInletAirHumidityRatioAutosized()) {
      idfObject.setString(Coil_LiquidDesiccant_SimpleFields::DesignInletAirHumidityRatio, "Autosize");
    } else if ((value = modelObject.designInletAirHumidityRatio())) {
      idfObject.setDouble(Coil_LiquidDesiccant_SimpleFields::DesignInletAirHumidityRatio, value.get());
    }

    // DesignOutletAirHumidityRatio

    if (modelObject.isDesignOutletAirHumidityRatioAutosized()) {
      idfObject.setString(Coil_LiquidDesiccant_SimpleFields::DesignOutletAirHumidityRatio, "Autosize");
    } else if ((value = modelObject.designOutletAirHumidityRatio())) {
      idfObject.setDouble(Coil_LiquidDesiccant_SimpleFields::DesignOutletAirHumidityRatio, value.get());
    }

    // DesignInletSolutionConcentration

    if (modelObject.isDesignInletSolutionConcentrationAutosized()) {
      idfObject.setString(Coil_LiquidDesiccant_SimpleFields::DesignInletSolutionConcentration, "Autosize");
    } else if ((value = modelObject.designInletSolutionConcentration())) {
      idfObject.setDouble(Coil_LiquidDesiccant_SimpleFields::DesignInletSolutionConcentration, value.get());
    }

    // DesignFanPowerPerUnitMassFlowRate

    if (modelObject.isDesignFanPowerPerUnitMassFlowRateAutosized()) {
      idfObject.setString(Coil_LiquidDesiccant_SimpleFields::DesignFanPowerPerUnitMassFlowRate, "Autosize");
    } else if ((value = modelObject.designFanPowerPerUnitMassFlowRate())) {
      idfObject.setDouble(Coil_LiquidDesiccant_SimpleFields::DesignFanPowerPerUnitMassFlowRate, value.get());
    }

    // OutdoorAirFlowRates

    if (modelObject.isOutdoorAirFlowRatesAutosized()) {
      idfObject.setString(Coil_LiquidDesiccant_SimpleFields::OutdoorAirFlowRates, "Autosize");
    } else if ((value = modelObject.outdoorAirFlowRates())) {
      idfObject.setDouble(Coil_LiquidDesiccant_SimpleFields::OutdoorAirFlowRates, value.get());
    }

    // DesignPumpPower

    if (modelObject.isDesignPumpPowerAutosized()) {
      idfObject.setString(Coil_LiquidDesiccant_SimpleFields::DesignPumpPower, "Autosize");
    } else if ((value = modelObject.designPumpPower())) {
      idfObject.setDouble(Coil_LiquidDesiccant_SimpleFields::DesignPumpPower, value.get());
    }

    // DesignEffectivenessAtNormalCondition

    if (modelObject.isDesignEffectivenessAtNormalConditionAutosized()) {
      idfObject.setString(Coil_LiquidDesiccant_SimpleFields::DesignEffectivenessAtNormalCondition, "Autosize");
    } else if ((value = modelObject.designEffectivenessAtNormalCondition())) {
      idfObject.setDouble(Coil_LiquidDesiccant_SimpleFields::DesignEffectivenessAtNormalCondition, value.get());
    }

    // WaterInletNodeName

    if (boost::optional<ModelObject> mo = modelObject.waterInletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Coil_LiquidDesiccant_SimpleFields::WaterInletNodeName, node->name().get());
      }
    }

    // WaterOutletNodeName

    if (boost::optional<ModelObject> mo = modelObject.waterOutletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Coil_LiquidDesiccant_SimpleFields::WaterOutletNodeName, node->name().get());
      }
    }

    // AirInletNodeName

    if (boost::optional<ModelObject> mo = modelObject.airInletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Coil_LiquidDesiccant_SimpleFields::AirInletNodeName, node->name().get());
      }
    }

    // AirOutletNodeName

    if (boost::optional<ModelObject> mo = modelObject.airOutletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Coil_LiquidDesiccant_SimpleFields::AirOutletNodeName, node->name().get());
      }
    }

    // TypeOfOperationMode

    s = modelObject.typeOfOperationMode();
    if (s) {
      idfObject.setString(Coil_LiquidDesiccant_SimpleFields::TypeOfOperationMode, s.get());
    }

    // AirSource

    s = modelObject.airSource();
    if (s) {
      idfObject.setString(Coil_LiquidDesiccant_SimpleFields::AirSource, s.get());
    }

    // Material

    s = modelObject.material();
    if (s) {
      idfObject.setString(Coil_LiquidDesiccant_SimpleFields::Material, s.get());
    }

    // DesignLiquidDesiccantConcentrationDifference

    if ((value = modelObject.designLiquidDesiccantConcentrationDifference())) {
      idfObject.setDouble(Coil_LiquidDesiccant_SimpleFields::DesignLiquidDesiccantConcentrationDifference, value.get());
    }

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
