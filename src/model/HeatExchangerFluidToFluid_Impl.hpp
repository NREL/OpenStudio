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

#ifndef MODEL_HEATEXCHANGERFLUIDTOFLUID_IMPL_HPP
#define MODEL_HEATEXCHANGERFLUIDTOFLUID_IMPL_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent_Impl.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class Node;

  namespace detail {

    /** HeatExchangerFluidToFluid_Impl is a WaterToWaterComponent_Impl that is the implementation class for HeatExchangerFluidToFluid.*/
    class MODEL_API HeatExchangerFluidToFluid_Impl : public WaterToWaterComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      HeatExchangerFluidToFluid_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      HeatExchangerFluidToFluid_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      HeatExchangerFluidToFluid_Impl(const HeatExchangerFluidToFluid_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~HeatExchangerFluidToFluid_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual unsigned supplyInletPort() const override;

      virtual unsigned supplyOutletPort() const override;

      virtual unsigned demandInletPort() const override;

      virtual unsigned demandOutletPort() const override;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<Schedule> availabilitySchedule() const;

      boost::optional<double> loopDemandSideDesignFlowRate() const;

      bool isLoopDemandSideDesignFlowRateAutosized() const;

      boost::optional<double> loopSupplySideDesignFlowRate() const;

      bool isLoopSupplySideDesignFlowRateAutosized() const;

      std::string heatExchangeModelType() const;

      bool isHeatExchangeModelTypeDefaulted() const;

      boost::optional<double> heatExchangerUFactorTimesAreaValue() const;

      bool isHeatExchangerUFactorTimesAreaValueAutosized() const;

      std::string controlType() const;

      bool isControlTypeDefaulted() const;

      double minimumTemperatureDifferencetoActivateHeatExchanger() const;

      bool isMinimumTemperatureDifferencetoActivateHeatExchangerDefaulted() const;

      std::string heatTransferMeteringEndUseType() const;

      bool isHeatTransferMeteringEndUseTypeDefaulted() const;

      boost::optional<Node> componentOverrideLoopSupplySideInletNode() const;

      boost::optional<Node> componentOverrideLoopDemandSideInletNode() const;

      std::string componentOverrideCoolingControlTemperatureMode() const;

      bool isComponentOverrideCoolingControlTemperatureModeDefaulted() const;

      double sizingFactor() const;

      bool isSizingFactorDefaulted() const;

      boost::optional<double> operationMinimumTemperatureLimit() const;

      boost::optional<double> operationMaximumTemperatureLimit() const;

      boost::optional<double> autosizedLoopDemandSideDesignFlowRate() const;

      boost::optional<double> autosizedLoopSupplySideDesignFlowRate() const;

      boost::optional<double> autosizedHeatExchangerUFactorTimesAreaValue() const;

      //@}
      /** @name Setters */
      //@{

      // TODO: Check argument type. From object lists, some candidates are: Schedule.
      bool setAvailabilitySchedule(Schedule& schedule);

      void resetAvailabilitySchedule();

      bool setLoopDemandSideDesignFlowRate(boost::optional<double> loopDemandSideDesignFlowRate);

      void autosizeLoopDemandSideDesignFlowRate();

      bool setLoopSupplySideDesignFlowRate(boost::optional<double> loopSupplySideDesignFlowRate);

      void autosizeLoopSupplySideDesignFlowRate();

      bool setHeatExchangeModelType(const std::string& heatExchangeModelType);

      void resetHeatExchangeModelType();

      bool setHeatExchangerUFactorTimesAreaValue(boost::optional<double> heatExchangerUFactorTimesAreaValue);

      void autosizeHeatExchangerUFactorTimesAreaValue();

      bool setControlType(const std::string& controlType);

      void resetControlType();

      bool setMinimumTemperatureDifferencetoActivateHeatExchanger(double minimumTemperatureDifferencetoActivateHeatExchanger);

      void resetMinimumTemperatureDifferencetoActivateHeatExchanger();

      bool setHeatTransferMeteringEndUseType(const std::string& heatTransferMeteringEndUseType);

      void resetHeatTransferMeteringEndUseType();

      bool setComponentOverrideLoopSupplySideInletNode(const boost::optional<Node>& node);

      void resetComponentOverrideLoopSupplySideInletNode();

      bool setComponentOverrideLoopDemandSideInletNode(const boost::optional<Node>& node);

      void resetComponentOverrideLoopDemandSideInletNode();

      bool setComponentOverrideCoolingControlTemperatureMode(const std::string& componentOverrideCoolingControlTemperatureMode);

      void resetComponentOverrideCoolingControlTemperatureMode();

      bool setSizingFactor(double sizingFactor);

      void resetSizingFactor();

      bool setOperationMinimumTemperatureLimit(boost::optional<double> operationMinimumTemperatureLimit);

      void resetOperationMinimumTemperatureLimit();

      bool setOperationMaximumTemperatureLimit(boost::optional<double> operationMaximumTemperatureLimit);

      void resetOperationMaximumTemperatureLimit();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.HeatExchangerFluidToFluid");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_HEATEXCHANGERFLUIDTOFLUID_IMPL_HPP
