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

#ifndef MODEL_HEADEREDPUMPSVARIABLESPEED_IMPL_HPP
#define MODEL_HEADEREDPUMPSVARIABLESPEED_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class ThermalZone;

  namespace detail {

    /** HeaderedPumpsVariableSpeed_Impl is a StraightComponent_Impl that is the implementation class for HeaderedPumpsVariableSpeed.*/
    class MODEL_API HeaderedPumpsVariableSpeed_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      HeaderedPumpsVariableSpeed_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      HeaderedPumpsVariableSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      HeaderedPumpsVariableSpeed_Impl(const HeaderedPumpsVariableSpeed_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~HeaderedPumpsVariableSpeed_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<double> totalRatedFlowRate() const;

      bool isTotalRatedFlowRateAutosized() const;

      int numberofPumpsinBank() const;

      std::string flowSequencingControlScheme() const;

      double ratedPumpHead() const;

      boost::optional<double> ratedPowerConsumption() const;

      bool isRatedPowerConsumptionAutosized() const;

      double motorEfficiency() const;

      double fractionofMotorInefficienciestoFluidStream() const;

      double coefficient1ofthePartLoadPerformanceCurve() const;

      double coefficient2ofthePartLoadPerformanceCurve() const;

      double coefficient3ofthePartLoadPerformanceCurve() const;

      double coefficient4ofthePartLoadPerformanceCurve() const;

      double minimumFlowRateFraction() const;

      std::string pumpControlType() const;

      boost::optional<Schedule> pumpFlowRateSchedule() const;

      boost::optional<ThermalZone> thermalZone() const;

      double skinLossRadiativeFraction() const;

      std::string designPowerSizingMethod() const;

      double designElectricPowerPerUnitFlowRate() const;

      double designShaftPowerPerUnitFlowRatePerUnitHead() const;

      boost::optional<double> autosizedTotalRatedFlowRate() const;

      boost::optional<double> autosizedRatedPowerConsumption() const;

      std::string endUseSubcategory() const;

      //@}
      /** @name Setters */
      //@{

      bool setTotalRatedFlowRate(boost::optional<double> totalRatedFlowRate);

      void autosizeTotalRatedFlowRate();

      bool setNumberofPumpsinBank(int numberofPumpsinBank);

      bool setFlowSequencingControlScheme(const std::string& flowSequencingControlScheme);

      bool setRatedPumpHead(double ratedPumpHead);

      bool setRatedPowerConsumption(boost::optional<double> ratedPowerConsumption);

      void autosizeRatedPowerConsumption();

      bool setMotorEfficiency(double motorEfficiency);

      bool setFractionofMotorInefficienciestoFluidStream(double fractionofMotorInefficienciestoFluidStream);

      bool setCoefficient1ofthePartLoadPerformanceCurve(double coefficient1ofthePartLoadPerformanceCurve);

      bool setCoefficient2ofthePartLoadPerformanceCurve(double coefficient2ofthePartLoadPerformanceCurve);

      bool setCoefficient3ofthePartLoadPerformanceCurve(double coefficient3ofthePartLoadPerformanceCurve);

      bool setCoefficient4ofthePartLoadPerformanceCurve(double coefficient4ofthePartLoadPerformanceCurve);

      bool setMinimumFlowRateFraction(double minimumFlowRateFraction);

      bool setPumpControlType(const std::string& pumpControlType);

      bool setPumpFlowRateSchedule(Schedule& schedule);

      void resetPumpFlowRateSchedule();

      bool setThermalZone(const boost::optional<ThermalZone>& thermalZone);

      void resetThermalZone();

      bool setSkinLossRadiativeFraction(double skinLossRadiativeFraction);

      bool setDesignPowerSizingMethod(const std::string& designPowerSizingMethod);

      bool setDesignElectricPowerPerUnitFlowRate(double designElectricPowerPerUnitFlowRate);

      bool setDesignShaftPowerPerUnitFlowRatePerUnitHead(double designShaftPowerPerUnitFlowRatePerUnitHead);

      bool setEndUseSubcategory(const std::string& endUseSubcategory);

      //@}
      /** @name Other */
      //@{

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      bool addToNode(Node& node) override;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.HeaderedPumpsVariableSpeed");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_HEADEREDPUMPSVARIABLESPEED_IMPL_HPP
