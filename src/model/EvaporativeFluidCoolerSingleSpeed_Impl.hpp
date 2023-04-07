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

#ifndef MODEL_EVAPORATIVEFLUIDCOOLERSINGLESPEED_IMPL_HPP
#define MODEL_EVAPORATIVEFLUIDCOOLERSINGLESPEED_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  //class WaterStorageTank;

  namespace detail {

    /** EvaporativeFluidCoolerSingleSpeed_Impl is a StraightComponent_Impl that is the implementation class for EvaporativeFluidCoolerSingleSpeed.*/
    class MODEL_API EvaporativeFluidCoolerSingleSpeed_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      EvaporativeFluidCoolerSingleSpeed_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      EvaporativeFluidCoolerSingleSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      EvaporativeFluidCoolerSingleSpeed_Impl(const EvaporativeFluidCoolerSingleSpeed_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~EvaporativeFluidCoolerSingleSpeed_Impl() = default;

      //@}

      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual bool addToNode(Node& node) override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{
      boost::optional<double> designAirFlowRate() const;

      bool isDesignAirFlowRateAutosized() const;

      boost::optional<double> fanPoweratDesignAirFlowRate() const;

      bool isFanPoweratDesignAirFlowRateAutosized() const;

      double designSprayWaterFlowRate() const;

      std::string performanceInputMethod() const;

      boost::optional<double> standardDesignCapacity() const;

      boost::optional<double> ufactorTimesAreaValueatDesignAirFlowRate() const;

      bool isUfactorTimesAreaValueatDesignAirFlowRateAutosized() const;

      boost::optional<double> designWaterFlowRate() const;

      bool isDesignWaterFlowRateAutosized() const;

      boost::optional<double> userSpecifiedDesignCapacity() const;

      boost::optional<double> designEnteringWaterTemperature() const;

      boost::optional<double> designEnteringAirTemperature() const;

      boost::optional<double> designEnteringAirWetbulbTemperature() const;

      std::string capacityControl() const;

      bool isCapacityControlDefaulted() const;

      double sizingFactor() const;

      bool isSizingFactorDefaulted() const;

      std::string evaporationLossMode() const;

      bool isEvaporationLossModeDefaulted() const;

      boost::optional<double> evaporationLossFactor() const;

      double driftLossPercent() const;

      bool isDriftLossPercentDefaulted() const;

      std::string blowdownCalculationMode() const;

      bool isBlowdownCalculationModeDefaulted() const;

      double blowdownConcentrationRatio() const;

      bool isBlowdownConcentrationRatioDefaulted() const;

      boost::optional<Schedule> blowdownMakeupWaterUsageSchedule() const;

      boost::optional<double> autosizedDesignAirFlowRate() const;

      boost::optional<double> autosizedFanPoweratDesignAirFlowRate() const;

      boost::optional<double> autosizedUfactorTimesAreaValueatDesignAirFlowRate() const;

      boost::optional<double> autosizedDesignWaterFlowRate() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      // boost::optional<WaterStorageTank> supplyWaterStorageTank() const;
      // A11; \field Supply Water Storage Tank Name

      //@}
      /** @name Setters */
      //@{

      bool setDesignAirFlowRate(boost::optional<double> designAirFlowRate);

      void autosizeDesignAirFlowRate();

      bool setFanPoweratDesignAirFlowRate(boost::optional<double> fanPoweratDesignAirFlowRate);

      void autosizeFanPoweratDesignAirFlowRate();

      bool setDesignSprayWaterFlowRate(double designSprayWaterFlowRate);

      bool setPerformanceInputMethod(const std::string& performanceInputMethod);

      void resetOutdoorAirInletNode();

      bool setStandardDesignCapacity(boost::optional<double> standardDesignCapacity);

      void resetStandardDesignCapacity();

      bool setUfactorTimesAreaValueatDesignAirFlowRate(boost::optional<double> ufactorTimesAreaValueatDesignAirFlowRate);

      void resetUfactorTimesAreaValueatDesignAirFlowRate();

      void autosizeUfactorTimesAreaValueatDesignAirFlowRate();

      bool setDesignWaterFlowRate(boost::optional<double> designWaterFlowRate);

      void resetDesignWaterFlowRate();

      void autosizeDesignWaterFlowRate();

      bool setUserSpecifiedDesignCapacity(boost::optional<double> userSpecifiedDesignCapacity);

      void resetUserSpecifiedDesignCapacity();

      bool setDesignEnteringWaterTemperature(boost::optional<double> designEnteringWaterTemperature);

      void resetDesignEnteringWaterTemperature();

      bool setDesignEnteringAirTemperature(boost::optional<double> designEnteringAirTemperature);

      void resetDesignEnteringAirTemperature();

      bool setDesignEnteringAirWetbulbTemperature(boost::optional<double> designEnteringAirWetbulbTemperature);

      void resetDesignEnteringAirWetbulbTemperature();

      bool setCapacityControl(const std::string& capacityControl);

      void resetCapacityControl();

      bool setSizingFactor(double sizingFactor);

      void resetSizingFactor();

      bool setEvaporationLossMode(const std::string& evaporationLossMode);

      void resetEvaporationLossMode();

      bool setEvaporationLossFactor(boost::optional<double> evaporationLossFactor);

      void resetEvaporationLossFactor();

      bool setDriftLossPercent(double driftLossPercent);

      void resetDriftLossPercent();

      bool setBlowdownCalculationMode(const std::string& blowdownCalculationMode);

      void resetBlowdownCalculationMode();

      bool setBlowdownConcentrationRatio(double blowdownConcentrationRatio);

      void resetBlowdownConcentrationRatio();

      bool setBlowdownMakeupWaterUsageSchedule(Schedule& schedule);

      void resetBlowdownMakeupWaterUsageSchedule();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.EvaporativeFluidCoolerSingleSpeed");

      std::vector<std::string> performanceInputMethodValues() const;
      std::vector<std::string> capacityControlValues() const;
      std::vector<std::string> evaporationLossModeValues() const;
      std::vector<std::string> blowdownCalculationModeValues() const;
      boost::optional<ModelObject> blowdownMakeupWaterUsageScheduleAsModelObject() const;
      bool setBlowdownMakeupWaterUsageScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EVAPORATIVEFLUIDCOOLERSINGLESPEED_IMPL_HPP
