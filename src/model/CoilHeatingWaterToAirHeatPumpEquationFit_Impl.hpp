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

#ifndef MODEL_COILHEATINGWATERTOAIRHEATPUMPEQUATIONFIT_IMPL_HPP
#define MODEL_COILHEATINGWATERTOAIRHEATPUMPEQUATIONFIT_IMPL_HPP

#include "ModelAPI.hpp"
#include "WaterToAirComponent_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** CoilHeatingWaterToAirHeatPumpEquationFit_Impl is a WaterToAirComponent_Impl that is the implementation class for CoilHeatingWaterToAirHeatPumpEquationFit.*/
    class MODEL_API CoilHeatingWaterToAirHeatPumpEquationFit_Impl : public WaterToAirComponent_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      CoilHeatingWaterToAirHeatPumpEquationFit_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilHeatingWaterToAirHeatPumpEquationFit_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilHeatingWaterToAirHeatPumpEquationFit_Impl(const CoilHeatingWaterToAirHeatPumpEquationFit_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilHeatingWaterToAirHeatPumpEquationFit_Impl() {}

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual unsigned airInletPort() const override;

      virtual unsigned airOutletPort() const override;

      virtual unsigned waterInletPort() const override;

      virtual unsigned waterOutletPort() const override;

      virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<double> ratedAirFlowRate() const;

      bool isRatedAirFlowRateDefaulted() const;

      bool isRatedAirFlowRateAutosized() const;

      boost::optional<double> ratedWaterFlowRate() const;

      bool isRatedWaterFlowRateDefaulted() const;

      bool isRatedWaterFlowRateAutosized() const;

      boost::optional<double> ratedHeatingCapacity() const;

      bool isRatedHeatingCapacityDefaulted() const;

      bool isRatedHeatingCapacityAutosized() const;

      double ratedHeatingCoefficientofPerformance() const;

      bool isRatedHeatingCoefficientofPerformanceDefaulted() const;

      double heatingCapacityCoefficient1() const;

      double heatingCapacityCoefficient2() const;

      double heatingCapacityCoefficient3() const;

      double heatingCapacityCoefficient4() const;

      double heatingCapacityCoefficient5() const;

      double heatingPowerConsumptionCoefficient1() const;

      double heatingPowerConsumptionCoefficient2() const;

      double heatingPowerConsumptionCoefficient3() const;

      double heatingPowerConsumptionCoefficient4() const;

      double heatingPowerConsumptionCoefficient5() const;

      boost::optional<double> autosizedRatedAirFlowRate() const;

      boost::optional<double> autosizedRatedWaterFlowRate() const;

      boost::optional<double> autosizedRatedHeatingCapacity() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      //@}
      /** @name Setters */
      //@{

      bool setRatedAirFlowRate(boost::optional<double> ratedAirFlowRate);

      bool setRatedAirFlowRate(double ratedAirFlowRate);

      void resetRatedAirFlowRate();

      void autosizeRatedAirFlowRate();

      bool setRatedWaterFlowRate(boost::optional<double> ratedWaterFlowRate);

      bool setRatedWaterFlowRate(double ratedWaterFlowRate);

      void resetRatedWaterFlowRate();

      void autosizeRatedWaterFlowRate();

      bool setRatedHeatingCapacity(boost::optional<double> ratedHeatingCapacity);

      bool setRatedHeatingCapacity(double ratedHeatingCapacity);

      void resetRatedHeatingCapacity();

      void autosizeRatedHeatingCapacity();

      bool setRatedHeatingCoefficientofPerformance(double ratedHeatingCoefficientofPerformance);

      void resetRatedHeatingCoefficientofPerformance();

      bool setHeatingCapacityCoefficient1(double heatingCapacityCoefficient1);

      bool setHeatingCapacityCoefficient2(double heatingCapacityCoefficient2);

      bool setHeatingCapacityCoefficient3(double heatingCapacityCoefficient3);

      bool setHeatingCapacityCoefficient4(double heatingCapacityCoefficient4);

      bool setHeatingCapacityCoefficient5(double heatingCapacityCoefficient5);

      bool setHeatingPowerConsumptionCoefficient1(double heatingPowerConsumptionCoefficient1);

      bool setHeatingPowerConsumptionCoefficient2(double heatingPowerConsumptionCoefficient2);

      bool setHeatingPowerConsumptionCoefficient3(double heatingPowerConsumptionCoefficient3);

      bool setHeatingPowerConsumptionCoefficient4(double heatingPowerConsumptionCoefficient4);

      bool setHeatingPowerConsumptionCoefficient5(double heatingPowerConsumptionCoefficient5);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilHeatingWaterToAirHeatPumpEquationFit");

      virtual boost::optional<HVACComponent> containingHVACComponent() const override;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGWATERTOAIRHEATPUMPEQUATIONFIT_IMPL_HPP
