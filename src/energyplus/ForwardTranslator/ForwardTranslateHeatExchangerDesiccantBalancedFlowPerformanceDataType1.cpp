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

#include "../../model/HeatExchangerDesiccantBalancedFlowPerformanceDataType1.hpp"

#include <utilities/idd/HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1_FieldEnums.hxx>
// #include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateHeatExchangerDesiccantBalancedFlowPerformanceDataType1(
    model::HeatExchangerDesiccantBalancedFlowPerformanceDataType1& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> d;

    IdfObject idfObject(openstudio::IddObjectType::HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1);

    m_idfObjects.push_back(idfObject);

    // Name
    idfObject.setString(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::Name, modelObject.name().get());

    // NominalAirFlowRate
    if (modelObject.isNominalAirFlowRateAutosized()) {
      idfObject.setString(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::NominalAirFlowRate, "Autosize");
    } else {
      d = modelObject.nominalAirFlowRate();
      if (d) {
        idfObject.setDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::NominalAirFlowRate, d.get());
      }
    }

    // NominalAirFaceVelocity
    if (modelObject.isNominalAirFaceVelocityAutosized()) {
      idfObject.setString(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::NominalAirFaceVelocity, "Autosize");
    } else {
      d = modelObject.nominalAirFaceVelocity();
      if (d) {
        idfObject.setDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::NominalAirFaceVelocity, d.get());
      }
    }

    // NominalElectricPower
    d = modelObject.nominalElectricPower();
    if (d) {
      idfObject.setDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::NominalElectricPower, *d);
    }

    // TemperatureEquationCoefficient1
    d = modelObject.temperatureEquationCoefficient1();
    if (d) {
      idfObject.setDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::TemperatureEquationCoefficient1, *d);
    }

    // TemperatureEquationCoefficient2
    d = modelObject.temperatureEquationCoefficient2();
    if (d) {
      idfObject.setDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::TemperatureEquationCoefficient2, *d);
    }

    // TemperatureEquationCoefficient3
    d = modelObject.temperatureEquationCoefficient3();
    if (d) {
      idfObject.setDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::TemperatureEquationCoefficient3, *d);
    }

    // TemperatureEquationCoefficient4
    d = modelObject.temperatureEquationCoefficient4();
    if (d) {
      idfObject.setDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::TemperatureEquationCoefficient4, *d);
    }

    // TemperatureEquationCoefficient5
    d = modelObject.temperatureEquationCoefficient5();
    if (d) {
      idfObject.setDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::TemperatureEquationCoefficient5, *d);
    }

    // TemperatureEquationCoefficient6
    d = modelObject.temperatureEquationCoefficient6();
    if (d) {
      idfObject.setDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::TemperatureEquationCoefficient6, *d);
    }

    // TemperatureEquationCoefficient7
    d = modelObject.temperatureEquationCoefficient7();
    if (d) {
      idfObject.setDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::TemperatureEquationCoefficient7, *d);
    }

    // TemperatureEquationCoefficient8
    d = modelObject.temperatureEquationCoefficient8();
    if (d) {
      idfObject.setDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::TemperatureEquationCoefficient8, *d);
    }

    // MinimumRegenerationInletAirHumidityRatioforTemperatureEquation
    d = modelObject.minimumRegenerationInletAirHumidityRatioforTemperatureEquation();
    if (d) {
      idfObject.setDouble(
        HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationInletAirHumidityRatioforTemperatureEquation, *d);
    }

    // MaximumRegenerationInletAirHumidityRatioforTemperatureEquation
    d = modelObject.maximumRegenerationInletAirHumidityRatioforTemperatureEquation();
    if (d) {
      idfObject.setDouble(
        HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationInletAirHumidityRatioforTemperatureEquation, *d);
    }

    // MinimumRegenerationInletAirTemperatureforTemperatureEquation
    d = modelObject.minimumRegenerationInletAirTemperatureforTemperatureEquation();
    if (d) {
      idfObject.setDouble(
        HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationInletAirTemperatureforTemperatureEquation, *d);
    }

    // MaximumRegenerationInletAirTemperatureforTemperatureEquation
    d = modelObject.maximumRegenerationInletAirTemperatureforTemperatureEquation();
    if (d) {
      idfObject.setDouble(
        HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationInletAirTemperatureforTemperatureEquation, *d);
    }

    // MinimumProcessInletAirHumidityRatioforTemperatureEquation
    d = modelObject.minimumProcessInletAirHumidityRatioforTemperatureEquation();
    if (d) {
      idfObject.setDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumProcessInletAirHumidityRatioforTemperatureEquation,
                          *d);
    }

    // MaximumProcessInletAirHumidityRatioforTemperatureEquation
    d = modelObject.maximumProcessInletAirHumidityRatioforTemperatureEquation();
    if (d) {
      idfObject.setDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumProcessInletAirHumidityRatioforTemperatureEquation,
                          *d);
    }

    // MinimumProcessInletAirTemperatureforTemperatureEquation
    d = modelObject.minimumProcessInletAirTemperatureforTemperatureEquation();
    if (d) {
      idfObject.setDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumProcessInletAirTemperatureforTemperatureEquation,
                          *d);
    }

    // MaximumProcessInletAirTemperatureforTemperatureEquation
    d = modelObject.maximumProcessInletAirTemperatureforTemperatureEquation();
    if (d) {
      idfObject.setDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumProcessInletAirTemperatureforTemperatureEquation,
                          *d);
    }

    // MinimumRegenerationAirVelocityforTemperatureEquation
    d = modelObject.minimumRegenerationAirVelocityforTemperatureEquation();
    if (d) {
      idfObject.setDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationAirVelocityforTemperatureEquation, *d);
    }

    // MaximumRegenerationAirVelocityforTemperatureEquation
    d = modelObject.maximumRegenerationAirVelocityforTemperatureEquation();
    if (d) {
      idfObject.setDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationAirVelocityforTemperatureEquation, *d);
    }

    // MinimumRegenerationOutletAirTemperatureforTemperatureEquation
    d = modelObject.minimumRegenerationOutletAirTemperatureforTemperatureEquation();
    if (d) {
      idfObject.setDouble(
        HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationOutletAirTemperatureforTemperatureEquation, *d);
    }

    // MaximumRegenerationOutletAirTemperatureforTemperatureEquation
    d = modelObject.maximumRegenerationOutletAirTemperatureforTemperatureEquation();
    if (d) {
      idfObject.setDouble(
        HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationOutletAirTemperatureforTemperatureEquation, *d);
    }

    // MinimumRegenerationInletAirRelativeHumidityforTemperatureEquation
    d = modelObject.minimumRegenerationInletAirRelativeHumidityforTemperatureEquation();
    if (d) {
      idfObject.setDouble(
        HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationInletAirRelativeHumidityforTemperatureEquation, *d);
    }

    // MaximumRegenerationInletAirRelativeHumidityforTemperatureEquation
    d = modelObject.maximumRegenerationInletAirRelativeHumidityforTemperatureEquation();
    if (d) {
      idfObject.setDouble(
        HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationInletAirRelativeHumidityforTemperatureEquation, *d);
    }

    // MinimumProcessInletAirRelativeHumidityforTemperatureEquation
    d = modelObject.minimumProcessInletAirRelativeHumidityforTemperatureEquation();
    if (d) {
      idfObject.setDouble(
        HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumProcessInletAirRelativeHumidityforTemperatureEquation, *d);
    }

    // MaximumProcessInletAirRelativeHumidityforTemperatureEquation
    d = modelObject.maximumProcessInletAirRelativeHumidityforTemperatureEquation();
    if (d) {
      idfObject.setDouble(
        HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumProcessInletAirRelativeHumidityforTemperatureEquation, *d);
    }

    // HumidityRatioEquationCoefficient1
    d = modelObject.humidityRatioEquationCoefficient1();
    if (d) {
      idfObject.setDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::HumidityRatioEquationCoefficient1, *d);
    }

    // HumidityRatioEquationCoefficient2
    d = modelObject.humidityRatioEquationCoefficient2();
    if (d) {
      idfObject.setDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::HumidityRatioEquationCoefficient2, *d);
    }

    // HumidityRatioEquationCoefficient3
    d = modelObject.humidityRatioEquationCoefficient3();
    if (d) {
      idfObject.setDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::HumidityRatioEquationCoefficient3, *d);
    }

    // HumidityRatioEquationCoefficient4
    d = modelObject.humidityRatioEquationCoefficient4();
    if (d) {
      idfObject.setDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::HumidityRatioEquationCoefficient4, *d);
    }

    // HumidityRatioEquationCoefficient5
    d = modelObject.humidityRatioEquationCoefficient5();
    if (d) {
      idfObject.setDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::HumidityRatioEquationCoefficient5, *d);
    }

    // HumidityRatioEquationCoefficient6
    d = modelObject.humidityRatioEquationCoefficient6();
    if (d) {
      idfObject.setDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::HumidityRatioEquationCoefficient6, *d);
    }

    // HumidityRatioEquationCoefficient7
    d = modelObject.humidityRatioEquationCoefficient7();
    if (d) {
      idfObject.setDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::HumidityRatioEquationCoefficient7, *d);
    }

    // HumidityRatioEquationCoefficient8
    d = modelObject.humidityRatioEquationCoefficient8();
    if (d) {
      idfObject.setDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::HumidityRatioEquationCoefficient8, *d);
    }

    // MinimumRegenerationInletAirHumidityRatioforHumidityRatioEquation
    d = modelObject.minimumRegenerationInletAirHumidityRatioforHumidityRatioEquation();
    if (d) {
      idfObject.setDouble(
        HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationInletAirHumidityRatioforHumidityRatioEquation, *d);
    }

    // MaximumRegenerationInletAirHumidityRatioforHumidityRatioEquation
    d = modelObject.maximumRegenerationInletAirHumidityRatioforHumidityRatioEquation();
    if (d) {
      idfObject.setDouble(
        HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationInletAirHumidityRatioforHumidityRatioEquation, *d);
    }

    // MinimumRegenerationInletAirTemperatureforHumidityRatioEquation
    d = modelObject.minimumRegenerationInletAirTemperatureforHumidityRatioEquation();
    if (d) {
      idfObject.setDouble(
        HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationInletAirTemperatureforHumidityRatioEquation, *d);
    }

    // MaximumRegenerationInletAirTemperatureforHumidityRatioEquation
    d = modelObject.maximumRegenerationInletAirTemperatureforHumidityRatioEquation();
    if (d) {
      idfObject.setDouble(
        HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationInletAirTemperatureforHumidityRatioEquation, *d);
    }

    // MinimumProcessInletAirHumidityRatioforHumidityRatioEquation
    d = modelObject.minimumProcessInletAirHumidityRatioforHumidityRatioEquation();
    if (d) {
      idfObject.setDouble(
        HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumProcessInletAirHumidityRatioforHumidityRatioEquation, *d);
    }

    // MaximumProcessInletAirHumidityRatioforHumidityRatioEquation
    d = modelObject.maximumProcessInletAirHumidityRatioforHumidityRatioEquation();
    if (d) {
      idfObject.setDouble(
        HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumProcessInletAirHumidityRatioforHumidityRatioEquation, *d);
    }

    // MinimumProcessInletAirTemperatureforHumidityRatioEquation
    d = modelObject.minimumProcessInletAirTemperatureforHumidityRatioEquation();
    if (d) {
      idfObject.setDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumProcessInletAirTemperatureforHumidityRatioEquation,
                          *d);
    }

    // MaximumProcessInletAirTemperatureforHumidityRatioEquation
    d = modelObject.maximumProcessInletAirTemperatureforHumidityRatioEquation();
    if (d) {
      idfObject.setDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumProcessInletAirTemperatureforHumidityRatioEquation,
                          *d);
    }

    // MinimumRegenerationAirVelocityforHumidityRatioEquation
    d = modelObject.minimumRegenerationAirVelocityforHumidityRatioEquation();
    if (d) {
      idfObject.setDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationAirVelocityforHumidityRatioEquation,
                          *d);
    }

    // MaximumRegenerationAirVelocityforHumidityRatioEquation
    d = modelObject.maximumRegenerationAirVelocityforHumidityRatioEquation();
    if (d) {
      idfObject.setDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationAirVelocityforHumidityRatioEquation,
                          *d);
    }

    // MinimumRegenerationOutletAirHumidityRatioforHumidityRatioEquation
    d = modelObject.minimumRegenerationOutletAirHumidityRatioforHumidityRatioEquation();
    if (d) {
      idfObject.setDouble(
        HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationOutletAirHumidityRatioforHumidityRatioEquation, *d);
    }

    // MaximumRegenerationOutletAirHumidityRatioforHumidityRatioEquation
    d = modelObject.maximumRegenerationOutletAirHumidityRatioforHumidityRatioEquation();
    if (d) {
      idfObject.setDouble(
        HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationOutletAirHumidityRatioforHumidityRatioEquation, *d);
    }

    // MinimumRegenerationInletAirRelativeHumidityforHumidityRatioEquation
    d = modelObject.minimumRegenerationInletAirRelativeHumidityforHumidityRatioEquation();
    if (d) {
      idfObject.setDouble(
        HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationInletAirRelativeHumidityforHumidityRatioEquation, *d);
    }

    // MaximumRegenerationInletAirRelativeHumidityforHumidityRatioEquation
    d = modelObject.maximumRegenerationInletAirRelativeHumidityforHumidityRatioEquation();
    if (d) {
      idfObject.setDouble(
        HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationInletAirRelativeHumidityforHumidityRatioEquation, *d);
    }

    // MinimumProcessInletAirRelativeHumidityforHumidityRatioEquation
    d = modelObject.minimumProcessInletAirRelativeHumidityforHumidityRatioEquation();
    if (d) {
      idfObject.setDouble(
        HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumProcessInletAirRelativeHumidityforHumidityRatioEquation, *d);
    }

    // MaximumProcessInletAirRelativeHumidityforHumidityRatioEquation
    d = modelObject.maximumProcessInletAirRelativeHumidityforHumidityRatioEquation();
    if (d) {
      idfObject.setDouble(
        HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumProcessInletAirRelativeHumidityforHumidityRatioEquation, *d);
    }

    return boost::optional<IdfObject>(idfObject);
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
