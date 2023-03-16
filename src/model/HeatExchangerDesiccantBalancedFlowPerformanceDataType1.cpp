/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "HeatExchangerDesiccantBalancedFlowPerformanceDataType1.hpp"
#include "HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "HeatExchangerDesiccantBalancedFlow.hpp"
#include "HeatExchangerDesiccantBalancedFlow_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1_FieldEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl(
      const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == HeatExchangerDesiccantBalancedFlowPerformanceDataType1::iddObjectType());
    }

    HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == HeatExchangerDesiccantBalancedFlowPerformanceDataType1::iddObjectType());
    }

    HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl(
      const HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl& other, Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::outputVariableNames() const {
      static std::vector<std::string> result;
      return result;
    }

    IddObjectType HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::iddObjectType() const {
      return HeatExchangerDesiccantBalancedFlowPerformanceDataType1::iddObjectType();
    }

    ModelObject HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::clone(Model model) const {
      // clone the operating modes is already handle in ModelObject_Impl::clone since they are ResourceObjects
      // We don't do ParentObject_Impl::clone since it'll also CLONE the children...
      return ModelObject_Impl::clone(model);
    }

    std::vector<IdfObject> HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::remove() {
      if (!heatExchangerDesiccantBalancedFlows().empty()) {
        LOG(Warn, "Cannot remove object because it is used by at least one heatExchangerDesiccantBalancedFlow as a required field");
        return {};
      }
      return ResourceObject_Impl::remove();
    }

    std::vector<HeatExchangerDesiccantBalancedFlow>
      HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::heatExchangerDesiccantBalancedFlows() const {
      return getObject<ModelObject>().getModelObjectSources<HeatExchangerDesiccantBalancedFlow>(HeatExchangerDesiccantBalancedFlow::iddObjectType());
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::isNominalAirFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::NominalAirFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::nominalAirFlowRate() const {
      return getDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::NominalAirFlowRate, true);
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::isNominalAirFaceVelocityAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::NominalAirFaceVelocity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::nominalAirFaceVelocity() const {
      return getDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::NominalAirFaceVelocity, true);
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::nominalElectricPower() const {
      boost::optional<double> value = getDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::NominalElectricPower, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::temperatureEquationCoefficient1() const {
      boost::optional<double> value =
        getDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::TemperatureEquationCoefficient1, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::temperatureEquationCoefficient2() const {
      boost::optional<double> value =
        getDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::TemperatureEquationCoefficient2, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::temperatureEquationCoefficient3() const {
      boost::optional<double> value =
        getDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::TemperatureEquationCoefficient3, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::temperatureEquationCoefficient4() const {
      boost::optional<double> value =
        getDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::TemperatureEquationCoefficient4, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::temperatureEquationCoefficient5() const {
      boost::optional<double> value =
        getDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::TemperatureEquationCoefficient5, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::temperatureEquationCoefficient6() const {
      boost::optional<double> value =
        getDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::TemperatureEquationCoefficient6, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::temperatureEquationCoefficient7() const {
      boost::optional<double> value =
        getDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::TemperatureEquationCoefficient7, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::temperatureEquationCoefficient8() const {
      boost::optional<double> value =
        getDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::TemperatureEquationCoefficient8, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::minimumRegenerationInletAirHumidityRatioforTemperatureEquation() const {
      boost::optional<double> value = getDouble(
        OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationInletAirHumidityRatioforTemperatureEquation, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::maximumRegenerationInletAirHumidityRatioforTemperatureEquation() const {
      boost::optional<double> value = getDouble(
        OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationInletAirHumidityRatioforTemperatureEquation, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::minimumRegenerationInletAirTemperatureforTemperatureEquation() const {
      boost::optional<double> value = getDouble(
        OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationInletAirTemperatureforTemperatureEquation, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::maximumRegenerationInletAirTemperatureforTemperatureEquation() const {
      boost::optional<double> value = getDouble(
        OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationInletAirTemperatureforTemperatureEquation, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::minimumProcessInletAirHumidityRatioforTemperatureEquation() const {
      boost::optional<double> value = getDouble(
        OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumProcessInletAirHumidityRatioforTemperatureEquation, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::maximumProcessInletAirHumidityRatioforTemperatureEquation() const {
      boost::optional<double> value = getDouble(
        OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumProcessInletAirHumidityRatioforTemperatureEquation, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::minimumProcessInletAirTemperatureforTemperatureEquation() const {
      boost::optional<double> value =
        getDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumProcessInletAirTemperatureforTemperatureEquation, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::maximumProcessInletAirTemperatureforTemperatureEquation() const {
      boost::optional<double> value =
        getDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumProcessInletAirTemperatureforTemperatureEquation, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::minimumRegenerationAirVelocityforTemperatureEquation() const {
      boost::optional<double> value =
        getDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationAirVelocityforTemperatureEquation, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::maximumRegenerationAirVelocityforTemperatureEquation() const {
      boost::optional<double> value =
        getDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationAirVelocityforTemperatureEquation, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::minimumRegenerationOutletAirTemperatureforTemperatureEquation() const {
      boost::optional<double> value = getDouble(
        OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationOutletAirTemperatureforTemperatureEquation, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::maximumRegenerationOutletAirTemperatureforTemperatureEquation() const {
      boost::optional<double> value = getDouble(
        OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationOutletAirTemperatureforTemperatureEquation, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::minimumRegenerationInletAirRelativeHumidityforTemperatureEquation() const {
      boost::optional<double> value = getDouble(
        OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationInletAirRelativeHumidityforTemperatureEquation, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::maximumRegenerationInletAirRelativeHumidityforTemperatureEquation() const {
      boost::optional<double> value = getDouble(
        OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationInletAirRelativeHumidityforTemperatureEquation, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::minimumProcessInletAirRelativeHumidityforTemperatureEquation() const {
      boost::optional<double> value = getDouble(
        OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumProcessInletAirRelativeHumidityforTemperatureEquation, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::maximumProcessInletAirRelativeHumidityforTemperatureEquation() const {
      boost::optional<double> value = getDouble(
        OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumProcessInletAirRelativeHumidityforTemperatureEquation, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::humidityRatioEquationCoefficient1() const {
      boost::optional<double> value =
        getDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::HumidityRatioEquationCoefficient1, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::humidityRatioEquationCoefficient2() const {
      boost::optional<double> value =
        getDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::HumidityRatioEquationCoefficient2, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::humidityRatioEquationCoefficient3() const {
      boost::optional<double> value =
        getDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::HumidityRatioEquationCoefficient3, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::humidityRatioEquationCoefficient4() const {
      boost::optional<double> value =
        getDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::HumidityRatioEquationCoefficient4, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::humidityRatioEquationCoefficient5() const {
      boost::optional<double> value =
        getDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::HumidityRatioEquationCoefficient5, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::humidityRatioEquationCoefficient6() const {
      boost::optional<double> value =
        getDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::HumidityRatioEquationCoefficient6, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::humidityRatioEquationCoefficient7() const {
      boost::optional<double> value =
        getDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::HumidityRatioEquationCoefficient7, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::humidityRatioEquationCoefficient8() const {
      boost::optional<double> value =
        getDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::HumidityRatioEquationCoefficient8, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::minimumRegenerationInletAirHumidityRatioforHumidityRatioEquation() const {
      boost::optional<double> value = getDouble(
        OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationInletAirHumidityRatioforHumidityRatioEquation, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::maximumRegenerationInletAirHumidityRatioforHumidityRatioEquation() const {
      boost::optional<double> value = getDouble(
        OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationInletAirHumidityRatioforHumidityRatioEquation, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::minimumRegenerationInletAirTemperatureforHumidityRatioEquation() const {
      boost::optional<double> value = getDouble(
        OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationInletAirTemperatureforHumidityRatioEquation, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::maximumRegenerationInletAirTemperatureforHumidityRatioEquation() const {
      boost::optional<double> value = getDouble(
        OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationInletAirTemperatureforHumidityRatioEquation, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::minimumProcessInletAirHumidityRatioforHumidityRatioEquation() const {
      boost::optional<double> value = getDouble(
        OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumProcessInletAirHumidityRatioforHumidityRatioEquation, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::maximumProcessInletAirHumidityRatioforHumidityRatioEquation() const {
      boost::optional<double> value = getDouble(
        OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumProcessInletAirHumidityRatioforHumidityRatioEquation, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::minimumProcessInletAirTemperatureforHumidityRatioEquation() const {
      boost::optional<double> value = getDouble(
        OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumProcessInletAirTemperatureforHumidityRatioEquation, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::maximumProcessInletAirTemperatureforHumidityRatioEquation() const {
      boost::optional<double> value = getDouble(
        OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumProcessInletAirTemperatureforHumidityRatioEquation, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::minimumRegenerationAirVelocityforHumidityRatioEquation() const {
      boost::optional<double> value =
        getDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationAirVelocityforHumidityRatioEquation, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::maximumRegenerationAirVelocityforHumidityRatioEquation() const {
      boost::optional<double> value =
        getDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationAirVelocityforHumidityRatioEquation, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::minimumRegenerationOutletAirHumidityRatioforHumidityRatioEquation() const {
      boost::optional<double> value = getDouble(
        OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationOutletAirHumidityRatioforHumidityRatioEquation, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::maximumRegenerationOutletAirHumidityRatioforHumidityRatioEquation() const {
      boost::optional<double> value = getDouble(
        OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationOutletAirHumidityRatioforHumidityRatioEquation, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::minimumRegenerationInletAirRelativeHumidityforHumidityRatioEquation() const {
      boost::optional<double> value = getDouble(
        OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationInletAirRelativeHumidityforHumidityRatioEquation,
        true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::maximumRegenerationInletAirRelativeHumidityforHumidityRatioEquation() const {
      boost::optional<double> value = getDouble(
        OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationInletAirRelativeHumidityforHumidityRatioEquation,
        true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::minimumProcessInletAirRelativeHumidityforHumidityRatioEquation() const {
      boost::optional<double> value = getDouble(
        OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumProcessInletAirRelativeHumidityforHumidityRatioEquation, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::maximumProcessInletAirRelativeHumidityforHumidityRatioEquation() const {
      boost::optional<double> value = getDouble(
        OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumProcessInletAirRelativeHumidityforHumidityRatioEquation, true);
      OS_ASSERT(value);
      return value.get();
    }

    void HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::autosizeNominalAirFlowRate() {
      bool result = setString(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::NominalAirFlowRate, "autosize");
      OS_ASSERT(result);
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setNominalAirFlowRate(double nominalAirFlowRate) {
      bool result = setDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::NominalAirFlowRate, nominalAirFlowRate);
      return result;
    }

    void HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::autosizeNominalAirFaceVelocity() {
      bool result = setString(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::NominalAirFaceVelocity, "autosize");
      OS_ASSERT(result);
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setNominalAirFaceVelocity(double nominalAirFaceVelocity) {
      bool result = setDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::NominalAirFaceVelocity, nominalAirFaceVelocity);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setNominalElectricPower(double nominalElectricPower) {
      bool result = setDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::NominalElectricPower, nominalElectricPower);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setTemperatureEquationCoefficient1(double temperatureEquationCoefficient1) {
      bool result = setDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::TemperatureEquationCoefficient1,
                              temperatureEquationCoefficient1);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setTemperatureEquationCoefficient2(double temperatureEquationCoefficient2) {
      bool result = setDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::TemperatureEquationCoefficient2,
                              temperatureEquationCoefficient2);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setTemperatureEquationCoefficient3(double temperatureEquationCoefficient3) {
      bool result = setDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::TemperatureEquationCoefficient3,
                              temperatureEquationCoefficient3);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setTemperatureEquationCoefficient4(double temperatureEquationCoefficient4) {
      bool result = setDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::TemperatureEquationCoefficient4,
                              temperatureEquationCoefficient4);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setTemperatureEquationCoefficient5(double temperatureEquationCoefficient5) {
      bool result = setDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::TemperatureEquationCoefficient5,
                              temperatureEquationCoefficient5);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setTemperatureEquationCoefficient6(double temperatureEquationCoefficient6) {
      bool result = setDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::TemperatureEquationCoefficient6,
                              temperatureEquationCoefficient6);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setTemperatureEquationCoefficient7(double temperatureEquationCoefficient7) {
      bool result = setDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::TemperatureEquationCoefficient7,
                              temperatureEquationCoefficient7);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setTemperatureEquationCoefficient8(double temperatureEquationCoefficient8) {
      bool result = setDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::TemperatureEquationCoefficient8,
                              temperatureEquationCoefficient8);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setMinimumRegenerationInletAirHumidityRatioforTemperatureEquation(
      double minimumRegenerationInletAirHumidityRatioforTemperatureEquation) {
      bool result =
        setDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationInletAirHumidityRatioforTemperatureEquation,
                  minimumRegenerationInletAirHumidityRatioforTemperatureEquation);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setMaximumRegenerationInletAirHumidityRatioforTemperatureEquation(
      double maximumRegenerationInletAirHumidityRatioforTemperatureEquation) {
      bool result =
        setDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationInletAirHumidityRatioforTemperatureEquation,
                  maximumRegenerationInletAirHumidityRatioforTemperatureEquation);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setMinimumRegenerationInletAirTemperatureforTemperatureEquation(
      double minimumRegenerationInletAirTemperatureforTemperatureEquation) {
      bool result =
        setDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationInletAirTemperatureforTemperatureEquation,
                  minimumRegenerationInletAirTemperatureforTemperatureEquation);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setMaximumRegenerationInletAirTemperatureforTemperatureEquation(
      double maximumRegenerationInletAirTemperatureforTemperatureEquation) {
      bool result =
        setDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationInletAirTemperatureforTemperatureEquation,
                  maximumRegenerationInletAirTemperatureforTemperatureEquation);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setMinimumProcessInletAirHumidityRatioforTemperatureEquation(
      double minimumProcessInletAirHumidityRatioforTemperatureEquation) {
      bool result =
        setDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumProcessInletAirHumidityRatioforTemperatureEquation,
                  minimumProcessInletAirHumidityRatioforTemperatureEquation);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setMaximumProcessInletAirHumidityRatioforTemperatureEquation(
      double maximumProcessInletAirHumidityRatioforTemperatureEquation) {
      bool result =
        setDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumProcessInletAirHumidityRatioforTemperatureEquation,
                  maximumProcessInletAirHumidityRatioforTemperatureEquation);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setMinimumProcessInletAirTemperatureforTemperatureEquation(
      double minimumProcessInletAirTemperatureforTemperatureEquation) {
      bool result =
        setDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumProcessInletAirTemperatureforTemperatureEquation,
                  minimumProcessInletAirTemperatureforTemperatureEquation);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setMaximumProcessInletAirTemperatureforTemperatureEquation(
      double maximumProcessInletAirTemperatureforTemperatureEquation) {
      bool result =
        setDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumProcessInletAirTemperatureforTemperatureEquation,
                  maximumProcessInletAirTemperatureforTemperatureEquation);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setMinimumRegenerationAirVelocityforTemperatureEquation(
      double minimumRegenerationAirVelocityforTemperatureEquation) {
      bool result =
        setDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationAirVelocityforTemperatureEquation,
                  minimumRegenerationAirVelocityforTemperatureEquation);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setMaximumRegenerationAirVelocityforTemperatureEquation(
      double maximumRegenerationAirVelocityforTemperatureEquation) {
      bool result =
        setDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationAirVelocityforTemperatureEquation,
                  maximumRegenerationAirVelocityforTemperatureEquation);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setMinimumRegenerationOutletAirTemperatureforTemperatureEquation(
      double minimumRegenerationOutletAirTemperatureforTemperatureEquation) {
      bool result =
        setDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationOutletAirTemperatureforTemperatureEquation,
                  minimumRegenerationOutletAirTemperatureforTemperatureEquation);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setMaximumRegenerationOutletAirTemperatureforTemperatureEquation(
      double maximumRegenerationOutletAirTemperatureforTemperatureEquation) {
      bool result =
        setDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationOutletAirTemperatureforTemperatureEquation,
                  maximumRegenerationOutletAirTemperatureforTemperatureEquation);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setMinimumRegenerationInletAirRelativeHumidityforTemperatureEquation(
      double minimumRegenerationInletAirRelativeHumidityforTemperatureEquation) {
      bool result = setDouble(
        OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationInletAirRelativeHumidityforTemperatureEquation,
        minimumRegenerationInletAirRelativeHumidityforTemperatureEquation);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setMaximumRegenerationInletAirRelativeHumidityforTemperatureEquation(
      double maximumRegenerationInletAirRelativeHumidityforTemperatureEquation) {
      bool result = setDouble(
        OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationInletAirRelativeHumidityforTemperatureEquation,
        maximumRegenerationInletAirRelativeHumidityforTemperatureEquation);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setMinimumProcessInletAirRelativeHumidityforTemperatureEquation(
      double minimumProcessInletAirRelativeHumidityforTemperatureEquation) {
      bool result =
        setDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumProcessInletAirRelativeHumidityforTemperatureEquation,
                  minimumProcessInletAirRelativeHumidityforTemperatureEquation);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setMaximumProcessInletAirRelativeHumidityforTemperatureEquation(
      double maximumProcessInletAirRelativeHumidityforTemperatureEquation) {
      bool result =
        setDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumProcessInletAirRelativeHumidityforTemperatureEquation,
                  maximumProcessInletAirRelativeHumidityforTemperatureEquation);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setHumidityRatioEquationCoefficient1(double humidityRatioEquationCoefficient1) {
      bool result = setDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::HumidityRatioEquationCoefficient1,
                              humidityRatioEquationCoefficient1);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setHumidityRatioEquationCoefficient2(double humidityRatioEquationCoefficient2) {
      bool result = setDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::HumidityRatioEquationCoefficient2,
                              humidityRatioEquationCoefficient2);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setHumidityRatioEquationCoefficient3(double humidityRatioEquationCoefficient3) {
      bool result = setDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::HumidityRatioEquationCoefficient3,
                              humidityRatioEquationCoefficient3);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setHumidityRatioEquationCoefficient4(double humidityRatioEquationCoefficient4) {
      bool result = setDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::HumidityRatioEquationCoefficient4,
                              humidityRatioEquationCoefficient4);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setHumidityRatioEquationCoefficient5(double humidityRatioEquationCoefficient5) {
      bool result = setDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::HumidityRatioEquationCoefficient5,
                              humidityRatioEquationCoefficient5);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setHumidityRatioEquationCoefficient6(double humidityRatioEquationCoefficient6) {
      bool result = setDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::HumidityRatioEquationCoefficient6,
                              humidityRatioEquationCoefficient6);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setHumidityRatioEquationCoefficient7(double humidityRatioEquationCoefficient7) {
      bool result = setDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::HumidityRatioEquationCoefficient7,
                              humidityRatioEquationCoefficient7);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setHumidityRatioEquationCoefficient8(double humidityRatioEquationCoefficient8) {
      bool result = setDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::HumidityRatioEquationCoefficient8,
                              humidityRatioEquationCoefficient8);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setMinimumRegenerationInletAirHumidityRatioforHumidityRatioEquation(
      double minimumRegenerationInletAirHumidityRatioforHumidityRatioEquation) {
      bool result = setDouble(
        OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationInletAirHumidityRatioforHumidityRatioEquation,
        minimumRegenerationInletAirHumidityRatioforHumidityRatioEquation);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setMaximumRegenerationInletAirHumidityRatioforHumidityRatioEquation(
      double maximumRegenerationInletAirHumidityRatioforHumidityRatioEquation) {
      bool result = setDouble(
        OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationInletAirHumidityRatioforHumidityRatioEquation,
        maximumRegenerationInletAirHumidityRatioforHumidityRatioEquation);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setMinimumRegenerationInletAirTemperatureforHumidityRatioEquation(
      double minimumRegenerationInletAirTemperatureforHumidityRatioEquation) {
      bool result =
        setDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationInletAirTemperatureforHumidityRatioEquation,
                  minimumRegenerationInletAirTemperatureforHumidityRatioEquation);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setMaximumRegenerationInletAirTemperatureforHumidityRatioEquation(
      double maximumRegenerationInletAirTemperatureforHumidityRatioEquation) {
      bool result =
        setDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationInletAirTemperatureforHumidityRatioEquation,
                  maximumRegenerationInletAirTemperatureforHumidityRatioEquation);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setMinimumProcessInletAirHumidityRatioforHumidityRatioEquation(
      double minimumProcessInletAirHumidityRatioforHumidityRatioEquation) {
      bool result =
        setDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumProcessInletAirHumidityRatioforHumidityRatioEquation,
                  minimumProcessInletAirHumidityRatioforHumidityRatioEquation);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setMaximumProcessInletAirHumidityRatioforHumidityRatioEquation(
      double maximumProcessInletAirHumidityRatioforHumidityRatioEquation) {
      bool result =
        setDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumProcessInletAirHumidityRatioforHumidityRatioEquation,
                  maximumProcessInletAirHumidityRatioforHumidityRatioEquation);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setMinimumProcessInletAirTemperatureforHumidityRatioEquation(
      double minimumProcessInletAirTemperatureforHumidityRatioEquation) {
      bool result =
        setDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumProcessInletAirTemperatureforHumidityRatioEquation,
                  minimumProcessInletAirTemperatureforHumidityRatioEquation);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setMaximumProcessInletAirTemperatureforHumidityRatioEquation(
      double maximumProcessInletAirTemperatureforHumidityRatioEquation) {
      bool result =
        setDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumProcessInletAirTemperatureforHumidityRatioEquation,
                  maximumProcessInletAirTemperatureforHumidityRatioEquation);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setMinimumRegenerationAirVelocityforHumidityRatioEquation(
      double minimumRegenerationAirVelocityforHumidityRatioEquation) {
      bool result =
        setDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationAirVelocityforHumidityRatioEquation,
                  minimumRegenerationAirVelocityforHumidityRatioEquation);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setMaximumRegenerationAirVelocityforHumidityRatioEquation(
      double maximumRegenerationAirVelocityforHumidityRatioEquation) {
      bool result =
        setDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationAirVelocityforHumidityRatioEquation,
                  maximumRegenerationAirVelocityforHumidityRatioEquation);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setMinimumRegenerationOutletAirHumidityRatioforHumidityRatioEquation(
      double minimumRegenerationOutletAirHumidityRatioforHumidityRatioEquation) {
      bool result = setDouble(
        OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationOutletAirHumidityRatioforHumidityRatioEquation,
        minimumRegenerationOutletAirHumidityRatioforHumidityRatioEquation);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setMaximumRegenerationOutletAirHumidityRatioforHumidityRatioEquation(
      double maximumRegenerationOutletAirHumidityRatioforHumidityRatioEquation) {
      bool result = setDouble(
        OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationOutletAirHumidityRatioforHumidityRatioEquation,
        maximumRegenerationOutletAirHumidityRatioforHumidityRatioEquation);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setMinimumRegenerationInletAirRelativeHumidityforHumidityRatioEquation(
      double minimumRegenerationInletAirRelativeHumidityforHumidityRatioEquation) {
      bool result = setDouble(
        OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationInletAirRelativeHumidityforHumidityRatioEquation,
        minimumRegenerationInletAirRelativeHumidityforHumidityRatioEquation);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setMaximumRegenerationInletAirRelativeHumidityforHumidityRatioEquation(
      double maximumRegenerationInletAirRelativeHumidityforHumidityRatioEquation) {
      bool result = setDouble(
        OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationInletAirRelativeHumidityforHumidityRatioEquation,
        maximumRegenerationInletAirRelativeHumidityforHumidityRatioEquation);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setMinimumProcessInletAirRelativeHumidityforHumidityRatioEquation(
      double minimumProcessInletAirRelativeHumidityforHumidityRatioEquation) {
      bool result =
        setDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumProcessInletAirRelativeHumidityforHumidityRatioEquation,
                  minimumProcessInletAirRelativeHumidityforHumidityRatioEquation);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::setMaximumProcessInletAirRelativeHumidityforHumidityRatioEquation(
      double maximumProcessInletAirRelativeHumidityforHumidityRatioEquation) {
      bool result =
        setDouble(OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumProcessInletAirRelativeHumidityforHumidityRatioEquation,
                  maximumProcessInletAirRelativeHumidityforHumidityRatioEquation);
      return result;
    }

    boost::optional<double> HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::autosizedNominalAirFlowRate() {
      return getAutosizedValue("Design Size Nominal Air Flow Rate", "m3/s");
    }

    boost::optional<double> HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::autosizedNominalAirFaceVelocity() {
      return getAutosizedValue("Design Size Nominal Air Face Velocity", "m/s");
    }

    void HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::autosize() {
      autosizeNominalAirFlowRate();
      autosizeNominalAirFaceVelocity();
    }

    void HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedNominalAirFlowRate();
      if (val) {
        setNominalAirFlowRate(val.get());
      }

      val = autosizedNominalAirFaceVelocity();
      if (val) {
        setNominalAirFaceVelocity(val.get());
      }
    }

  }  // namespace detail

  HeatExchangerDesiccantBalancedFlowPerformanceDataType1::HeatExchangerDesiccantBalancedFlowPerformanceDataType1(const Model& model)
    : ResourceObject(HeatExchangerDesiccantBalancedFlowPerformanceDataType1::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>());

    autosizeNominalAirFlowRate();
    autosizeNominalAirFaceVelocity();
    setNominalElectricPower(0);
    // FurnaceWithDXSystem_CoolingHXAssisted.idf
    setTemperatureEquationCoefficient1(-7.18302E+00);
    setTemperatureEquationCoefficient2(-1.84967E+02);
    setTemperatureEquationCoefficient3(1.00051E+00);
    setTemperatureEquationCoefficient4(1.16033E+04);
    setTemperatureEquationCoefficient5(-5.07550E+01);
    setTemperatureEquationCoefficient6(-1.68467E-02);
    setTemperatureEquationCoefficient7(5.82213E+01);
    setTemperatureEquationCoefficient8(5.98863E-01);
    setMinimumRegenerationInletAirHumidityRatioforTemperatureEquation(0.007143);
    setMaximumRegenerationInletAirHumidityRatioforTemperatureEquation(0.024286);
    setMinimumRegenerationInletAirTemperatureforTemperatureEquation(17.83333);
    setMaximumRegenerationInletAirTemperatureforTemperatureEquation(48.88889);
    setMinimumProcessInletAirHumidityRatioforTemperatureEquation(0.005000);
    setMaximumProcessInletAirHumidityRatioforTemperatureEquation(0.015714);
    setMinimumProcessInletAirTemperatureforTemperatureEquation(4.583333);
    setMaximumProcessInletAirTemperatureforTemperatureEquation(21.83333);
    setMinimumRegenerationAirVelocityforTemperatureEquation(2.286);
    setMaximumRegenerationAirVelocityforTemperatureEquation(4.826);
    setMinimumRegenerationOutletAirTemperatureforTemperatureEquation(16.66667);
    setMaximumRegenerationOutletAirTemperatureforTemperatureEquation(46.11111);
    setMinimumRegenerationInletAirRelativeHumidityforTemperatureEquation(10.0);
    setMaximumRegenerationInletAirRelativeHumidityforTemperatureEquation(100.0);
    setMinimumProcessInletAirRelativeHumidityforTemperatureEquation(80.0);
    setMaximumProcessInletAirRelativeHumidityforTemperatureEquation(100.0);
    setHumidityRatioEquationCoefficient1(3.13878E-03);
    setHumidityRatioEquationCoefficient2(1.09689E+00);
    setHumidityRatioEquationCoefficient3(-2.63341E-05);
    setHumidityRatioEquationCoefficient4(-6.33885E+00);
    setHumidityRatioEquationCoefficient5(9.38196E-03);
    setHumidityRatioEquationCoefficient6(5.21186E-05);
    setHumidityRatioEquationCoefficient7(6.70354E-02);
    setHumidityRatioEquationCoefficient8(-1.60823E-04);
    setMinimumRegenerationInletAirHumidityRatioforHumidityRatioEquation(0.007143);
    setMaximumRegenerationInletAirHumidityRatioforHumidityRatioEquation(0.024286);
    setMinimumRegenerationInletAirTemperatureforHumidityRatioEquation(17.83333);
    setMaximumRegenerationInletAirTemperatureforHumidityRatioEquation(48.88889);
    setMinimumProcessInletAirHumidityRatioforHumidityRatioEquation(0.005000);
    setMaximumProcessInletAirHumidityRatioforHumidityRatioEquation(0.015714);
    setMinimumProcessInletAirTemperatureforHumidityRatioEquation(4.583333);
    setMaximumProcessInletAirTemperatureforHumidityRatioEquation(21.83333);
    setMinimumRegenerationAirVelocityforHumidityRatioEquation(2.286);
    setMaximumRegenerationAirVelocityforHumidityRatioEquation(4.826);
    setMinimumRegenerationOutletAirHumidityRatioforHumidityRatioEquation(0.007811);
    setMaximumRegenerationOutletAirHumidityRatioforHumidityRatioEquation(0.026707);
    setMinimumRegenerationInletAirRelativeHumidityforHumidityRatioEquation(10.0);
    setMaximumRegenerationInletAirRelativeHumidityforHumidityRatioEquation(100.0);
    setMinimumProcessInletAirRelativeHumidityforHumidityRatioEquation(80.0);
    setMaximumProcessInletAirRelativeHumidityforHumidityRatioEquation(100.0);
  }

  IddObjectType HeatExchangerDesiccantBalancedFlowPerformanceDataType1::iddObjectType() {
    return {IddObjectType::OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1};
  }

  std::vector<HeatExchangerDesiccantBalancedFlow>
    HeatExchangerDesiccantBalancedFlowPerformanceDataType1::heatExchangerDesiccantBalancedFlows() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->heatExchangerDesiccantBalancedFlows();
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::isNominalAirFlowRateAutosized() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->isNominalAirFlowRateAutosized();
  }

  boost::optional<double> HeatExchangerDesiccantBalancedFlowPerformanceDataType1::nominalAirFlowRate() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->nominalAirFlowRate();
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::isNominalAirFaceVelocityAutosized() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->isNominalAirFlowRateAutosized();
  }

  boost::optional<double> HeatExchangerDesiccantBalancedFlowPerformanceDataType1::nominalAirFaceVelocity() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->nominalAirFaceVelocity();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::nominalElectricPower() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->nominalElectricPower();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::temperatureEquationCoefficient1() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->temperatureEquationCoefficient1();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::temperatureEquationCoefficient2() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->temperatureEquationCoefficient2();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::temperatureEquationCoefficient3() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->temperatureEquationCoefficient3();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::temperatureEquationCoefficient4() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->temperatureEquationCoefficient4();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::temperatureEquationCoefficient5() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->temperatureEquationCoefficient5();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::temperatureEquationCoefficient6() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->temperatureEquationCoefficient6();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::temperatureEquationCoefficient7() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->temperatureEquationCoefficient7();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::temperatureEquationCoefficient8() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->temperatureEquationCoefficient8();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::minimumRegenerationInletAirHumidityRatioforTemperatureEquation() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->minimumRegenerationInletAirHumidityRatioforTemperatureEquation();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::maximumRegenerationInletAirHumidityRatioforTemperatureEquation() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->maximumRegenerationInletAirHumidityRatioforTemperatureEquation();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::minimumRegenerationInletAirTemperatureforTemperatureEquation() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->minimumRegenerationInletAirTemperatureforTemperatureEquation();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::maximumRegenerationInletAirTemperatureforTemperatureEquation() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->maximumRegenerationInletAirTemperatureforTemperatureEquation();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::minimumProcessInletAirHumidityRatioforTemperatureEquation() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->minimumProcessInletAirHumidityRatioforTemperatureEquation();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::maximumProcessInletAirHumidityRatioforTemperatureEquation() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->maximumProcessInletAirHumidityRatioforTemperatureEquation();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::minimumProcessInletAirTemperatureforTemperatureEquation() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->minimumProcessInletAirTemperatureforTemperatureEquation();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::maximumProcessInletAirTemperatureforTemperatureEquation() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->maximumProcessInletAirTemperatureforTemperatureEquation();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::minimumRegenerationAirVelocityforTemperatureEquation() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->minimumRegenerationAirVelocityforTemperatureEquation();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::maximumRegenerationAirVelocityforTemperatureEquation() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->maximumRegenerationAirVelocityforTemperatureEquation();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::minimumRegenerationOutletAirTemperatureforTemperatureEquation() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->minimumRegenerationOutletAirTemperatureforTemperatureEquation();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::maximumRegenerationOutletAirTemperatureforTemperatureEquation() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->maximumRegenerationOutletAirTemperatureforTemperatureEquation();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::minimumRegenerationInletAirRelativeHumidityforTemperatureEquation() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->minimumRegenerationInletAirRelativeHumidityforTemperatureEquation();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::maximumRegenerationInletAirRelativeHumidityforTemperatureEquation() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->maximumRegenerationInletAirRelativeHumidityforTemperatureEquation();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::minimumProcessInletAirRelativeHumidityforTemperatureEquation() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->minimumProcessInletAirRelativeHumidityforTemperatureEquation();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::maximumProcessInletAirRelativeHumidityforTemperatureEquation() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->maximumProcessInletAirRelativeHumidityforTemperatureEquation();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::humidityRatioEquationCoefficient1() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->humidityRatioEquationCoefficient1();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::humidityRatioEquationCoefficient2() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->humidityRatioEquationCoefficient2();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::humidityRatioEquationCoefficient3() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->humidityRatioEquationCoefficient3();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::humidityRatioEquationCoefficient4() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->humidityRatioEquationCoefficient4();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::humidityRatioEquationCoefficient5() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->humidityRatioEquationCoefficient5();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::humidityRatioEquationCoefficient6() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->humidityRatioEquationCoefficient6();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::humidityRatioEquationCoefficient7() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->humidityRatioEquationCoefficient7();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::humidityRatioEquationCoefficient8() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->humidityRatioEquationCoefficient8();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::minimumRegenerationInletAirHumidityRatioforHumidityRatioEquation() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->minimumRegenerationInletAirHumidityRatioforHumidityRatioEquation();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::maximumRegenerationInletAirHumidityRatioforHumidityRatioEquation() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->maximumRegenerationInletAirHumidityRatioforHumidityRatioEquation();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::minimumRegenerationInletAirTemperatureforHumidityRatioEquation() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->minimumRegenerationInletAirTemperatureforHumidityRatioEquation();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::maximumRegenerationInletAirTemperatureforHumidityRatioEquation() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->maximumRegenerationInletAirTemperatureforHumidityRatioEquation();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::minimumProcessInletAirHumidityRatioforHumidityRatioEquation() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->minimumProcessInletAirHumidityRatioforHumidityRatioEquation();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::maximumProcessInletAirHumidityRatioforHumidityRatioEquation() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->maximumProcessInletAirHumidityRatioforHumidityRatioEquation();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::minimumProcessInletAirTemperatureforHumidityRatioEquation() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->minimumProcessInletAirTemperatureforHumidityRatioEquation();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::maximumProcessInletAirTemperatureforHumidityRatioEquation() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->maximumProcessInletAirTemperatureforHumidityRatioEquation();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::minimumRegenerationAirVelocityforHumidityRatioEquation() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->minimumRegenerationAirVelocityforHumidityRatioEquation();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::maximumRegenerationAirVelocityforHumidityRatioEquation() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->maximumRegenerationAirVelocityforHumidityRatioEquation();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::minimumRegenerationOutletAirHumidityRatioforHumidityRatioEquation() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->minimumRegenerationOutletAirHumidityRatioforHumidityRatioEquation();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::maximumRegenerationOutletAirHumidityRatioforHumidityRatioEquation() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->maximumRegenerationOutletAirHumidityRatioforHumidityRatioEquation();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::minimumRegenerationInletAirRelativeHumidityforHumidityRatioEquation() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->minimumRegenerationInletAirRelativeHumidityforHumidityRatioEquation();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::maximumRegenerationInletAirRelativeHumidityforHumidityRatioEquation() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->maximumRegenerationInletAirRelativeHumidityforHumidityRatioEquation();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::minimumProcessInletAirRelativeHumidityforHumidityRatioEquation() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->minimumProcessInletAirRelativeHumidityforHumidityRatioEquation();
  }

  double HeatExchangerDesiccantBalancedFlowPerformanceDataType1::maximumProcessInletAirRelativeHumidityforHumidityRatioEquation() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->maximumProcessInletAirRelativeHumidityforHumidityRatioEquation();
  }

  void HeatExchangerDesiccantBalancedFlowPerformanceDataType1::autosizeNominalAirFlowRate() {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->autosizeNominalAirFlowRate();
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setNominalAirFlowRate(double nominalAirFlowRate) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->setNominalAirFlowRate(nominalAirFlowRate);
  }

  void HeatExchangerDesiccantBalancedFlowPerformanceDataType1::autosizeNominalAirFaceVelocity() {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->autosizeNominalAirFaceVelocity();
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setNominalAirFaceVelocity(double nominalAirFaceVelocity) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->setNominalAirFaceVelocity(nominalAirFaceVelocity);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setNominalElectricPower(double nominalElectricPower) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->setNominalElectricPower(nominalElectricPower);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setTemperatureEquationCoefficient1(double temperatureEquationCoefficient1) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->setTemperatureEquationCoefficient1(
      temperatureEquationCoefficient1);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setTemperatureEquationCoefficient2(double temperatureEquationCoefficient2) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->setTemperatureEquationCoefficient2(
      temperatureEquationCoefficient2);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setTemperatureEquationCoefficient3(double temperatureEquationCoefficient3) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->setTemperatureEquationCoefficient3(
      temperatureEquationCoefficient3);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setTemperatureEquationCoefficient4(double temperatureEquationCoefficient4) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->setTemperatureEquationCoefficient4(
      temperatureEquationCoefficient4);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setTemperatureEquationCoefficient5(double temperatureEquationCoefficient5) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->setTemperatureEquationCoefficient5(
      temperatureEquationCoefficient5);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setTemperatureEquationCoefficient6(double temperatureEquationCoefficient6) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->setTemperatureEquationCoefficient6(
      temperatureEquationCoefficient6);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setTemperatureEquationCoefficient7(double temperatureEquationCoefficient7) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->setTemperatureEquationCoefficient7(
      temperatureEquationCoefficient7);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setTemperatureEquationCoefficient8(double temperatureEquationCoefficient8) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->setTemperatureEquationCoefficient8(
      temperatureEquationCoefficient8);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setMinimumRegenerationInletAirHumidityRatioforTemperatureEquation(
    double minimumRegenerationInletAirHumidityRatioforTemperatureEquation) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->setMinimumRegenerationInletAirHumidityRatioforTemperatureEquation(minimumRegenerationInletAirHumidityRatioforTemperatureEquation);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setMaximumRegenerationInletAirHumidityRatioforTemperatureEquation(
    double maximumRegenerationInletAirHumidityRatioforTemperatureEquation) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->setMaximumRegenerationInletAirHumidityRatioforTemperatureEquation(maximumRegenerationInletAirHumidityRatioforTemperatureEquation);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setMinimumRegenerationInletAirTemperatureforTemperatureEquation(
    double minimumRegenerationInletAirTemperatureforTemperatureEquation) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->setMinimumRegenerationInletAirTemperatureforTemperatureEquation(minimumRegenerationInletAirTemperatureforTemperatureEquation);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setMaximumRegenerationInletAirTemperatureforTemperatureEquation(
    double maximumRegenerationInletAirTemperatureforTemperatureEquation) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->setMaximumRegenerationInletAirTemperatureforTemperatureEquation(maximumRegenerationInletAirTemperatureforTemperatureEquation);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setMinimumProcessInletAirHumidityRatioforTemperatureEquation(
    double minimumProcessInletAirHumidityRatioforTemperatureEquation) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->setMinimumProcessInletAirHumidityRatioforTemperatureEquation(minimumProcessInletAirHumidityRatioforTemperatureEquation);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setMaximumProcessInletAirHumidityRatioforTemperatureEquation(
    double maximumProcessInletAirHumidityRatioforTemperatureEquation) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->setMaximumProcessInletAirHumidityRatioforTemperatureEquation(maximumProcessInletAirHumidityRatioforTemperatureEquation);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setMinimumProcessInletAirTemperatureforTemperatureEquation(
    double minimumProcessInletAirTemperatureforTemperatureEquation) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->setMinimumProcessInletAirTemperatureforTemperatureEquation(
      minimumProcessInletAirTemperatureforTemperatureEquation);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setMaximumProcessInletAirTemperatureforTemperatureEquation(
    double maximumProcessInletAirTemperatureforTemperatureEquation) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->setMaximumProcessInletAirTemperatureforTemperatureEquation(
      maximumProcessInletAirTemperatureforTemperatureEquation);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setMinimumRegenerationAirVelocityforTemperatureEquation(
    double minimumRegenerationAirVelocityforTemperatureEquation) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->setMinimumRegenerationAirVelocityforTemperatureEquation(
      minimumRegenerationAirVelocityforTemperatureEquation);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setMaximumRegenerationAirVelocityforTemperatureEquation(
    double maximumRegenerationAirVelocityforTemperatureEquation) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->setMaximumRegenerationAirVelocityforTemperatureEquation(
      maximumRegenerationAirVelocityforTemperatureEquation);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setMinimumRegenerationOutletAirTemperatureforTemperatureEquation(
    double minimumRegenerationOutletAirTemperatureforTemperatureEquation) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->setMinimumRegenerationOutletAirTemperatureforTemperatureEquation(minimumRegenerationOutletAirTemperatureforTemperatureEquation);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setMaximumRegenerationOutletAirTemperatureforTemperatureEquation(
    double maximumRegenerationOutletAirTemperatureforTemperatureEquation) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->setMaximumRegenerationOutletAirTemperatureforTemperatureEquation(maximumRegenerationOutletAirTemperatureforTemperatureEquation);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setMinimumRegenerationInletAirRelativeHumidityforTemperatureEquation(
    double minimumRegenerationInletAirRelativeHumidityforTemperatureEquation) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->setMinimumRegenerationInletAirRelativeHumidityforTemperatureEquation(minimumRegenerationInletAirRelativeHumidityforTemperatureEquation);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setMaximumRegenerationInletAirRelativeHumidityforTemperatureEquation(
    double maximumRegenerationInletAirRelativeHumidityforTemperatureEquation) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->setMaximumRegenerationInletAirRelativeHumidityforTemperatureEquation(maximumRegenerationInletAirRelativeHumidityforTemperatureEquation);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setMinimumProcessInletAirRelativeHumidityforTemperatureEquation(
    double minimumProcessInletAirRelativeHumidityforTemperatureEquation) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->setMinimumProcessInletAirRelativeHumidityforTemperatureEquation(minimumProcessInletAirRelativeHumidityforTemperatureEquation);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setMaximumProcessInletAirRelativeHumidityforTemperatureEquation(
    double maximumProcessInletAirRelativeHumidityforTemperatureEquation) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->setMaximumProcessInletAirRelativeHumidityforTemperatureEquation(maximumProcessInletAirRelativeHumidityforTemperatureEquation);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setHumidityRatioEquationCoefficient1(double humidityRatioEquationCoefficient1) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->setHumidityRatioEquationCoefficient1(
      humidityRatioEquationCoefficient1);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setHumidityRatioEquationCoefficient2(double humidityRatioEquationCoefficient2) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->setHumidityRatioEquationCoefficient2(
      humidityRatioEquationCoefficient2);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setHumidityRatioEquationCoefficient3(double humidityRatioEquationCoefficient3) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->setHumidityRatioEquationCoefficient3(
      humidityRatioEquationCoefficient3);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setHumidityRatioEquationCoefficient4(double humidityRatioEquationCoefficient4) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->setHumidityRatioEquationCoefficient4(
      humidityRatioEquationCoefficient4);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setHumidityRatioEquationCoefficient5(double humidityRatioEquationCoefficient5) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->setHumidityRatioEquationCoefficient5(
      humidityRatioEquationCoefficient5);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setHumidityRatioEquationCoefficient6(double humidityRatioEquationCoefficient6) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->setHumidityRatioEquationCoefficient6(
      humidityRatioEquationCoefficient6);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setHumidityRatioEquationCoefficient7(double humidityRatioEquationCoefficient7) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->setHumidityRatioEquationCoefficient7(
      humidityRatioEquationCoefficient7);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setHumidityRatioEquationCoefficient8(double humidityRatioEquationCoefficient8) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->setHumidityRatioEquationCoefficient8(
      humidityRatioEquationCoefficient8);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setMinimumRegenerationInletAirHumidityRatioforHumidityRatioEquation(
    double minimumRegenerationInletAirHumidityRatioforHumidityRatioEquation) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->setMinimumRegenerationInletAirHumidityRatioforHumidityRatioEquation(minimumRegenerationInletAirHumidityRatioforHumidityRatioEquation);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setMaximumRegenerationInletAirHumidityRatioforHumidityRatioEquation(
    double maximumRegenerationInletAirHumidityRatioforHumidityRatioEquation) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->setMaximumRegenerationInletAirHumidityRatioforHumidityRatioEquation(maximumRegenerationInletAirHumidityRatioforHumidityRatioEquation);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setMinimumRegenerationInletAirTemperatureforHumidityRatioEquation(
    double minimumRegenerationInletAirTemperatureforHumidityRatioEquation) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->setMinimumRegenerationInletAirTemperatureforHumidityRatioEquation(minimumRegenerationInletAirTemperatureforHumidityRatioEquation);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setMaximumRegenerationInletAirTemperatureforHumidityRatioEquation(
    double maximumRegenerationInletAirTemperatureforHumidityRatioEquation) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->setMaximumRegenerationInletAirTemperatureforHumidityRatioEquation(maximumRegenerationInletAirTemperatureforHumidityRatioEquation);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setMinimumProcessInletAirHumidityRatioforHumidityRatioEquation(
    double minimumProcessInletAirHumidityRatioforHumidityRatioEquation) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->setMinimumProcessInletAirHumidityRatioforHumidityRatioEquation(minimumProcessInletAirHumidityRatioforHumidityRatioEquation);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setMaximumProcessInletAirHumidityRatioforHumidityRatioEquation(
    double maximumProcessInletAirHumidityRatioforHumidityRatioEquation) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->setMaximumProcessInletAirHumidityRatioforHumidityRatioEquation(maximumProcessInletAirHumidityRatioforHumidityRatioEquation);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setMinimumProcessInletAirTemperatureforHumidityRatioEquation(
    double minimumProcessInletAirTemperatureforHumidityRatioEquation) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->setMinimumProcessInletAirTemperatureforHumidityRatioEquation(minimumProcessInletAirTemperatureforHumidityRatioEquation);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setMaximumProcessInletAirTemperatureforHumidityRatioEquation(
    double maximumProcessInletAirTemperatureforHumidityRatioEquation) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->setMaximumProcessInletAirTemperatureforHumidityRatioEquation(maximumProcessInletAirTemperatureforHumidityRatioEquation);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setMinimumRegenerationAirVelocityforHumidityRatioEquation(
    double minimumRegenerationAirVelocityforHumidityRatioEquation) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->setMinimumRegenerationAirVelocityforHumidityRatioEquation(
      minimumRegenerationAirVelocityforHumidityRatioEquation);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setMaximumRegenerationAirVelocityforHumidityRatioEquation(
    double maximumRegenerationAirVelocityforHumidityRatioEquation) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->setMaximumRegenerationAirVelocityforHumidityRatioEquation(
      maximumRegenerationAirVelocityforHumidityRatioEquation);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setMinimumRegenerationOutletAirHumidityRatioforHumidityRatioEquation(
    double minimumRegenerationOutletAirHumidityRatioforHumidityRatioEquation) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->setMinimumRegenerationOutletAirHumidityRatioforHumidityRatioEquation(minimumRegenerationOutletAirHumidityRatioforHumidityRatioEquation);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setMaximumRegenerationOutletAirHumidityRatioforHumidityRatioEquation(
    double maximumRegenerationOutletAirHumidityRatioforHumidityRatioEquation) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->setMaximumRegenerationOutletAirHumidityRatioforHumidityRatioEquation(maximumRegenerationOutletAirHumidityRatioforHumidityRatioEquation);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setMinimumRegenerationInletAirRelativeHumidityforHumidityRatioEquation(
    double minimumRegenerationInletAirRelativeHumidityforHumidityRatioEquation) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->setMinimumRegenerationInletAirRelativeHumidityforHumidityRatioEquation(minimumRegenerationInletAirRelativeHumidityforHumidityRatioEquation);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setMaximumRegenerationInletAirRelativeHumidityforHumidityRatioEquation(
    double maximumRegenerationInletAirRelativeHumidityforHumidityRatioEquation) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->setMaximumRegenerationInletAirRelativeHumidityforHumidityRatioEquation(maximumRegenerationInletAirRelativeHumidityforHumidityRatioEquation);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setMinimumProcessInletAirRelativeHumidityforHumidityRatioEquation(
    double minimumProcessInletAirRelativeHumidityforHumidityRatioEquation) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->setMinimumProcessInletAirRelativeHumidityforHumidityRatioEquation(minimumProcessInletAirRelativeHumidityforHumidityRatioEquation);
  }

  bool HeatExchangerDesiccantBalancedFlowPerformanceDataType1::setMaximumProcessInletAirRelativeHumidityforHumidityRatioEquation(
    double maximumProcessInletAirRelativeHumidityforHumidityRatioEquation) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()
      ->setMaximumProcessInletAirRelativeHumidityforHumidityRatioEquation(maximumProcessInletAirRelativeHumidityforHumidityRatioEquation);
  }

  boost::optional<double> HeatExchangerDesiccantBalancedFlowPerformanceDataType1::autosizedNominalAirFlowRate() {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->autosizedNominalAirFlowRate();
  }

  boost::optional<double> HeatExchangerDesiccantBalancedFlowPerformanceDataType1::autosizedNominalAirFaceVelocity() {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->autosizedNominalAirFaceVelocity();
  }

  void HeatExchangerDesiccantBalancedFlowPerformanceDataType1::autosize() {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->autosize();
  }

  void HeatExchangerDesiccantBalancedFlowPerformanceDataType1::applySizingValues() {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl>()->applySizingValues();
  }

  /// @cond
  HeatExchangerDesiccantBalancedFlowPerformanceDataType1::HeatExchangerDesiccantBalancedFlowPerformanceDataType1(
    std::shared_ptr<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl> impl)
    : ResourceObject(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
