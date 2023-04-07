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

#ifndef MODEL_COILCOOLINGWATER_IMPL_HPP
#define MODEL_COILCOOLINGWATER_IMPL_HPP

#include "WaterToAirComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;

  namespace detail {

    class MODEL_API CoilCoolingWater_Impl : public WaterToAirComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilCoolingWater_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilCoolingWater_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilCoolingWater_Impl(const CoilCoolingWater_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilCoolingWater_Impl() = default;

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

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}

      Schedule availabilitySchedule() const;

      bool setAvailabilitySchedule(Schedule& schedule);

      boost::optional<double> designWaterFlowRate();

      bool setDesignWaterFlowRate(double value);

      bool isDesignWaterFlowRateAutosized();

      void autosizeDesignWaterFlowRate();

      boost::optional<double> designAirFlowRate();

      bool setDesignAirFlowRate(double value);

      bool isDesignAirFlowRateAutosized();

      void autosizeDesignAirFlowRate();

      boost::optional<double> designInletWaterTemperature();

      bool setDesignInletWaterTemperature(double value);

      bool isDesignInletWaterTemperatureAutosized();

      void autosizeDesignInletWaterTemperature();

      boost::optional<double> designInletAirTemperature();

      bool setDesignInletAirTemperature(double value);

      bool isDesignInletAirTemperatureAutosized();

      void autosizeDesignInletAirTemperature();

      boost::optional<double> designOutletAirTemperature();

      bool setDesignOutletAirTemperature(double value);

      bool isDesignOutletAirTemperatureAutosized();

      void autosizeDesignOutletAirTemperature();

      boost::optional<double> designInletAirHumidityRatio();

      bool setDesignInletAirHumidityRatio(double value);

      bool isDesignInletAirHumidityRatioAutosized();

      void autosizeDesignInletAirHumidityRatio();

      boost::optional<double> designOutletAirHumidityRatio();

      bool setDesignOutletAirHumidityRatio(double value);

      bool isDesignOutletAirHumidityRatioAutosized();

      void autosizeDesignOutletAirHumidityRatio();

      std::string typeOfAnalysis();

      bool setTypeOfAnalysis(const std::string& value);

      std::string heatExchangerConfiguration();

      bool setHeatExchangerConfiguration(const std::string& value);

      AirflowNetworkEquivalentDuct getAirflowNetworkEquivalentDuct(double length, double diameter);

      boost::optional<AirflowNetworkEquivalentDuct> airflowNetworkEquivalentDuct() const;

      boost::optional<double> autosizedDesignWaterFlowRate() const;

      boost::optional<double> autosizedDesignAirFlowRate() const;

      boost::optional<double> autosizedDesignInletWaterTemperature() const;

      boost::optional<double> autosizedDesignInletAirTemperature() const;

      boost::optional<double> autosizedDesignOutletAirTemperature() const;

      boost::optional<double> autosizedDesignInletAirHumidityRatio() const;

      boost::optional<double> autosizedDesignOutletAirHumidityRatio() const;

      // Not part of the applySizingValues
      boost::optional<double> autosizedDesignCoilLoad() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

     private:
      REGISTER_LOGGER("openstudio.model.CoilCoolingWater");

      boost::optional<ModelObject> availabilityScheduleAsModelObject() const;

      bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_COILCOOLINGWATER_IMPL_HPP
