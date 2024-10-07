/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

      virtual ~CoilCoolingWater_Impl() override = default;

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
