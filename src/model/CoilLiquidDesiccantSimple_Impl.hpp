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

#ifndef MODEL_COILLIQUIDDESICCANTSIMPLE_IMPL_HPP
#define MODEL_COILLIQUIDDESICCANTSIMPLE_IMPL_HPP

#include "WaterToAirComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;

  namespace detail {

    class MODEL_API CoilLiquidDesiccantSimple_Impl : public WaterToAirComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilLiquidDesiccantSimple_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilLiquidDesiccantSimple_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilLiquidDesiccantSimple_Impl(const CoilLiquidDesiccantSimple_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilLiquidDesiccantSimple_Impl();

      //@}
      /** @name Virtual Methods */
      //@{

      virtual std::vector<openstudio::IdfObject> remove() override;

      virtual bool removeFromPlantLoop() override;

      virtual ModelObject clone(Model model) const override;

      virtual IddObjectType iddObjectType() const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual unsigned airInletPort() const override;

      virtual unsigned airOutletPort() const override;

      virtual unsigned waterInletPort() const override;

      virtual unsigned waterOutletPort() const override;

      virtual bool addToNode(Node& node) override;

      virtual boost::optional<HVACComponent> containingHVACComponent() const override;

      virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;

      virtual std::vector<ModelObject> children() const override;

      //@}
      /** @name Getters */
      //@{

      Schedule availabilitySchedule() const;

      boost::optional<double> designWaterFlowRate();

      bool isDesignWaterFlowRateAutosized();

      boost::optional<double> designAirFlowRate();

      bool isDesignAirFlowRateAutosized();

      boost::optional<double> designInletWaterTemperature();

      bool isDesignInletWaterTemperatureAutosized();

      boost::optional<double> designInletAirTemperature();

      bool isDesignInletAirTemperatureAutosized();

      boost::optional<double> designOutletAirTemperature();

      bool isDesignOutletAirTemperatureAutosized();

      boost::optional<double> designInletAirHumidityRatio();

      bool isDesignInletAirHumidityRatioAutosized();

      boost::optional<double> designOutletAirHumidityRatio();

      bool isDesignOutletAirHumidityRatioAutosized();

      boost::optional<double> designInletSolutionConcentration();

      bool isDesignInletSolutionConcentrationAutosized();

      boost::optional<double> designFanPowerPerUnitMassFlowRate();

      bool isDesignFanPowerPerUnitMassFlowRateAutosized();

      boost::optional<double> outdoorAirFlowRates();

      bool isOutdoorAirFlowRatesAutosized();

      boost::optional<double> designPumpPower();

      bool isDesignPumpPowerAutosized();

      boost::optional<double> designEffectivenessAtNormalCondition();

      bool isDesignEffectivenessAtNormalConditionAutosized();

      std::string typeOfOperationMode();

      bool isTypeOfOperationModeDefaulted();

      std::string airSource();

      bool isAirSourceDefaulted();

      std::string material();

      bool isMaterialDefaulted();

      // boost::optional<WaterStorageTank> condensateCollectionWaterStorageTank() const;

      boost::optional<double> designLiquidDesiccantConcentrationDifference();

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setDesignWaterFlowRate(double designWaterFlowRate);

      void autosizeDesignWaterFlowRate();

      bool setDesignAirFlowRate(double designAirFlowRate);

      void autosizeDesignAirFlowRate();

      bool setDesignInletWaterTemperature(double designInletWaterTemperature);

      void autosizeDesignInletWaterTemperature();

      bool setDesignInletAirTemperature(double designInletAirTemperature);

      void autosizeDesignInletAirTemperature();

      bool setDesignOutletAirTemperature(double designOutletAirTemperature);

      void autosizeDesignOutletAirTemperature();

      bool setDesignInletAirHumidityRatio(double designInletAirHumidityRatio);

      void autosizeDesignInletAirHumidityRatio();

      bool setDesignOutletAirHumidityRatio(double designOutletAirHumidityRatio);

      void autosizeDesignOutletAirHumidityRatio();

      bool setDesignInletSolutionConcentration(double designInletSolutionConcentration);

      void autosizeDesignInletSolutionConcentration();

      bool setDesignFanPowerPerUnitMassFlowRate(double designFanPowerPerUnitMassFlowRate);

      void autosizeDesignFanPowerPerUnitMassFlowRate();

      bool setOutdoorAirFlowRates(double outdoorAirFlowRates);

      void autosizeOutdoorAirFlowRates();

      bool setDesignPumpPower(double designPumpPower);

      void autosizeDesignPumpPower();

      bool setDesignEffectivenessAtNormalCondition(double designEffectivenessAtNormalCondition);

      void autosizeDesignEffectivenessAtNormalCondition();

      bool setTypeOfOperationMode(std::string typeOfOperationMode);

      void resetTypeOfOperationMode();

      bool setAirSource(std::string airSource);

      void resetAirSource();

      bool setMaterial(std::string material);

      void resetMaterial();

      // bool setCondensateCollectionWaterStorageTank(const boost::optional<WaterStorageTank>& waterStorageTank);

      // void resetCondensateCollectionWaterStorageTank();

      bool setDesignLiquidDesiccantConcentrationDifference(double designLiquidDesiccantConcentrationDifference);

      void resetDesignLiquidDesiccantConcentrationDifference();

      //@}
      /** @name Other */
      //@{

      AirflowNetworkEquivalentDuct getAirflowNetworkEquivalentDuct(double length, double diameter);

      boost::optional<AirflowNetworkEquivalentDuct> airflowNetworkEquivalentDuct() const;

      boost::optional<double> autosizedDesignWaterFlowRate() const;

      boost::optional<double> autosizedDesignAirFlowRate() const;

      boost::optional<double> autosizedDesignInletWaterTemperature() const;

      boost::optional<double> autosizedDesignInletAirTemperature() const;

      boost::optional<double> autosizedDesignOutletAirTemperature() const;

      boost::optional<double> autosizedDesignInletAirHumidityRatio() const;

      boost::optional<double> autosizedDesignOutletAirHumidityRatio() const;

      boost::optional<double> autosizedDesignInletSolutionConcentration() const;

      boost::optional<double> autosizedDesignFanPowerPerUnitMassFlowRate() const;

      boost::optional<double> autosizedOutdoorAirFlowRates() const;

      boost::optional<double> autosizedDesignPumpPower() const;

      boost::optional<double> autosizedDesignEffectivenessAtNormalCondition() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

     private:
      REGISTER_LOGGER("openstudio.model.CoilLiquidDesiccantSimple");

      boost::optional<ModelObject> availabilityScheduleAsModelObject() const;

      bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_COILLIQUIDDESICCANTSIMPLE_IMPL_HPP
