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

#ifndef MODEL_HEATEXCHANGERDESICCANTBALANCEDFLOWPERFORMANCEDATATYPE1_IMPL_HPP
#define MODEL_HEATEXCHANGERDESICCANTBALANCEDFLOWPERFORMANCEDATATYPE1_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ResourceObject_Impl.hpp"

namespace openstudio {
namespace model {

  class HeatExchangerDesiccantBalancedFlow;

  namespace detail {

    /** HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl is a ResourceObject_Impl that is the implementation class for HeatExchangerDesiccantBalancedFlowPerformanceDataType1.*/
    class MODEL_API HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl : public ResourceObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                  bool keepHandle);

      HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl(const HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl& other,
                                                                  Model_Impl* model, bool keepHandle);

      virtual ~HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual ModelObject clone(Model model) const override;

      // If this object is still used by at least one HeatExchangerDesiccantBalancedFlow, refuse to remove to avoid putting the HeatExchangerDesiccantBalancedFlow(s) in a broken state
      virtual std::vector<IdfObject> remove() override;

      //@}
      /** @name Getters */
      //@{

      std::vector<HeatExchangerDesiccantBalancedFlow> heatExchangerDesiccantBalancedFlows() const;

      bool isNominalAirFlowRateAutosized() const;

      boost::optional<double> nominalAirFlowRate() const;

      bool isNominalAirFaceVelocityAutosized() const;

      boost::optional<double> nominalAirFaceVelocity() const;

      double nominalElectricPower() const;

      double temperatureEquationCoefficient1() const;

      double temperatureEquationCoefficient2() const;

      double temperatureEquationCoefficient3() const;

      double temperatureEquationCoefficient4() const;

      double temperatureEquationCoefficient5() const;

      double temperatureEquationCoefficient6() const;

      double temperatureEquationCoefficient7() const;

      double temperatureEquationCoefficient8() const;

      double minimumRegenerationInletAirHumidityRatioforTemperatureEquation() const;

      double maximumRegenerationInletAirHumidityRatioforTemperatureEquation() const;

      double minimumRegenerationInletAirTemperatureforTemperatureEquation() const;

      double maximumRegenerationInletAirTemperatureforTemperatureEquation() const;

      double minimumProcessInletAirHumidityRatioforTemperatureEquation() const;

      double maximumProcessInletAirHumidityRatioforTemperatureEquation() const;

      double minimumProcessInletAirTemperatureforTemperatureEquation() const;

      double maximumProcessInletAirTemperatureforTemperatureEquation() const;

      double minimumRegenerationAirVelocityforTemperatureEquation() const;

      double maximumRegenerationAirVelocityforTemperatureEquation() const;

      double minimumRegenerationOutletAirTemperatureforTemperatureEquation() const;

      double maximumRegenerationOutletAirTemperatureforTemperatureEquation() const;

      double minimumRegenerationInletAirRelativeHumidityforTemperatureEquation() const;

      double maximumRegenerationInletAirRelativeHumidityforTemperatureEquation() const;

      double minimumProcessInletAirRelativeHumidityforTemperatureEquation() const;

      double maximumProcessInletAirRelativeHumidityforTemperatureEquation() const;

      double humidityRatioEquationCoefficient1() const;

      double humidityRatioEquationCoefficient2() const;

      double humidityRatioEquationCoefficient3() const;

      double humidityRatioEquationCoefficient4() const;

      double humidityRatioEquationCoefficient5() const;

      double humidityRatioEquationCoefficient6() const;

      double humidityRatioEquationCoefficient7() const;

      double humidityRatioEquationCoefficient8() const;

      double minimumRegenerationInletAirHumidityRatioforHumidityRatioEquation() const;

      double maximumRegenerationInletAirHumidityRatioforHumidityRatioEquation() const;

      double minimumRegenerationInletAirTemperatureforHumidityRatioEquation() const;

      double maximumRegenerationInletAirTemperatureforHumidityRatioEquation() const;

      double minimumProcessInletAirHumidityRatioforHumidityRatioEquation() const;

      double maximumProcessInletAirHumidityRatioforHumidityRatioEquation() const;

      double minimumProcessInletAirTemperatureforHumidityRatioEquation() const;

      double maximumProcessInletAirTemperatureforHumidityRatioEquation() const;

      double minimumRegenerationAirVelocityforHumidityRatioEquation() const;

      double maximumRegenerationAirVelocityforHumidityRatioEquation() const;

      double minimumRegenerationOutletAirHumidityRatioforHumidityRatioEquation() const;

      double maximumRegenerationOutletAirHumidityRatioforHumidityRatioEquation() const;

      double minimumRegenerationInletAirRelativeHumidityforHumidityRatioEquation() const;

      double maximumRegenerationInletAirRelativeHumidityforHumidityRatioEquation() const;

      double minimumProcessInletAirRelativeHumidityforHumidityRatioEquation() const;

      double maximumProcessInletAirRelativeHumidityforHumidityRatioEquation() const;

      //@}
      /** @name Setters */
      //@{

      void autosizeNominalAirFlowRate();

      bool setNominalAirFlowRate(double nominalAirFlowRate);

      void autosizeNominalAirFaceVelocity();

      bool setNominalAirFaceVelocity(double nominalAirFaceVelocity);

      bool setNominalElectricPower(double nominalElectricPower);

      bool setTemperatureEquationCoefficient1(double temperatureEquationCoefficient1);

      bool setTemperatureEquationCoefficient2(double temperatureEquationCoefficient2);

      bool setTemperatureEquationCoefficient3(double temperatureEquationCoefficient3);

      bool setTemperatureEquationCoefficient4(double temperatureEquationCoefficient4);

      bool setTemperatureEquationCoefficient5(double temperatureEquationCoefficient5);

      bool setTemperatureEquationCoefficient6(double temperatureEquationCoefficient6);

      bool setTemperatureEquationCoefficient7(double temperatureEquationCoefficient7);

      bool setTemperatureEquationCoefficient8(double temperatureEquationCoefficient8);

      bool setMinimumRegenerationInletAirHumidityRatioforTemperatureEquation(double minimumRegenerationInletAirHumidityRatioforTemperatureEquation);

      bool setMaximumRegenerationInletAirHumidityRatioforTemperatureEquation(double maximumRegenerationInletAirHumidityRatioforTemperatureEquation);

      bool setMinimumRegenerationInletAirTemperatureforTemperatureEquation(double minimumRegenerationInletAirTemperatureforTemperatureEquation);

      bool setMaximumRegenerationInletAirTemperatureforTemperatureEquation(double maximumRegenerationInletAirTemperatureforTemperatureEquation);

      bool setMinimumProcessInletAirHumidityRatioforTemperatureEquation(double minimumProcessInletAirHumidityRatioforTemperatureEquation);

      bool setMaximumProcessInletAirHumidityRatioforTemperatureEquation(double maximumProcessInletAirHumidityRatioforTemperatureEquation);

      bool setMinimumProcessInletAirTemperatureforTemperatureEquation(double minimumProcessInletAirTemperatureforTemperatureEquation);

      bool setMaximumProcessInletAirTemperatureforTemperatureEquation(double maximumProcessInletAirTemperatureforTemperatureEquation);

      bool setMinimumRegenerationAirVelocityforTemperatureEquation(double minimumRegenerationAirVelocityforTemperatureEquation);

      bool setMaximumRegenerationAirVelocityforTemperatureEquation(double maximumRegenerationAirVelocityforTemperatureEquation);

      bool setMinimumRegenerationOutletAirTemperatureforTemperatureEquation(double minimumRegenerationOutletAirTemperatureforTemperatureEquation);

      bool setMaximumRegenerationOutletAirTemperatureforTemperatureEquation(double maximumRegenerationOutletAirTemperatureforTemperatureEquation);

      bool setMinimumRegenerationInletAirRelativeHumidityforTemperatureEquation(
        double minimumRegenerationInletAirRelativeHumidityforTemperatureEquation);

      bool setMaximumRegenerationInletAirRelativeHumidityforTemperatureEquation(
        double maximumRegenerationInletAirRelativeHumidityforTemperatureEquation);

      bool setMinimumProcessInletAirRelativeHumidityforTemperatureEquation(double minimumProcessInletAirRelativeHumidityforTemperatureEquation);

      bool setMaximumProcessInletAirRelativeHumidityforTemperatureEquation(double maximumProcessInletAirRelativeHumidityforTemperatureEquation);

      bool setHumidityRatioEquationCoefficient1(double humidityRatioEquationCoefficient1);

      bool setHumidityRatioEquationCoefficient2(double humidityRatioEquationCoefficient2);

      bool setHumidityRatioEquationCoefficient3(double humidityRatioEquationCoefficient3);

      bool setHumidityRatioEquationCoefficient4(double humidityRatioEquationCoefficient4);

      bool setHumidityRatioEquationCoefficient5(double humidityRatioEquationCoefficient5);

      bool setHumidityRatioEquationCoefficient6(double humidityRatioEquationCoefficient6);

      bool setHumidityRatioEquationCoefficient7(double humidityRatioEquationCoefficient7);

      bool setHumidityRatioEquationCoefficient8(double humidityRatioEquationCoefficient8);

      bool
        setMinimumRegenerationInletAirHumidityRatioforHumidityRatioEquation(double minimumRegenerationInletAirHumidityRatioforHumidityRatioEquation);

      bool
        setMaximumRegenerationInletAirHumidityRatioforHumidityRatioEquation(double maximumRegenerationInletAirHumidityRatioforHumidityRatioEquation);

      bool setMinimumRegenerationInletAirTemperatureforHumidityRatioEquation(double minimumRegenerationInletAirTemperatureforHumidityRatioEquation);

      bool setMaximumRegenerationInletAirTemperatureforHumidityRatioEquation(double maximumRegenerationInletAirTemperatureforHumidityRatioEquation);

      bool setMinimumProcessInletAirHumidityRatioforHumidityRatioEquation(double minimumProcessInletAirHumidityRatioforHumidityRatioEquation);

      bool setMaximumProcessInletAirHumidityRatioforHumidityRatioEquation(double maximumProcessInletAirHumidityRatioforHumidityRatioEquation);

      bool setMinimumProcessInletAirTemperatureforHumidityRatioEquation(double minimumProcessInletAirTemperatureforHumidityRatioEquation);

      bool setMaximumProcessInletAirTemperatureforHumidityRatioEquation(double maximumProcessInletAirTemperatureforHumidityRatioEquation);

      bool setMinimumRegenerationAirVelocityforHumidityRatioEquation(double minimumRegenerationAirVelocityforHumidityRatioEquation);

      bool setMaximumRegenerationAirVelocityforHumidityRatioEquation(double maximumRegenerationAirVelocityforHumidityRatioEquation);

      bool setMinimumRegenerationOutletAirHumidityRatioforHumidityRatioEquation(
        double minimumRegenerationOutletAirHumidityRatioforHumidityRatioEquation);

      bool setMaximumRegenerationOutletAirHumidityRatioforHumidityRatioEquation(
        double maximumRegenerationOutletAirHumidityRatioforHumidityRatioEquation);

      bool setMinimumRegenerationInletAirRelativeHumidityforHumidityRatioEquation(
        double minimumRegenerationInletAirRelativeHumidityforHumidityRatioEquation);

      bool setMaximumRegenerationInletAirRelativeHumidityforHumidityRatioEquation(
        double maximumRegenerationInletAirRelativeHumidityforHumidityRatioEquation);

      bool setMinimumProcessInletAirRelativeHumidityforHumidityRatioEquation(double minimumProcessInletAirRelativeHumidityforHumidityRatioEquation);

      bool setMaximumProcessInletAirRelativeHumidityforHumidityRatioEquation(double maximumProcessInletAirRelativeHumidityforHumidityRatioEquation);

      //@}
      /** @name Other */
      //@{

      boost::optional<double> autosizedNominalAirFlowRate();

      boost::optional<double> autosizedNominalAirFaceVelocity();

      void autosize();

      void applySizingValues();

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.HeatExchangerDesiccantBalancedFlowPerformanceDataType1");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_HEATEXCHANGERDESICCANTBALANCEDFLOWPERFORMANCEDATATYPE1_IMPL_HPP
