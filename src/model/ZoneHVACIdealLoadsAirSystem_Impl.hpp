/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEHVACIDEALLOADSAIRSYSTEM_IMPL_HPP
#define MODEL_ZONEHVACIDEALLOADSAIRSYSTEM_IMPL_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class DesignSpecificationOutdoorAir;

  namespace detail {

    /** ZoneHVACIdealLoadsAirSystem_Impl is a ZoneHVACComponent_Impl that is the implementation class for ZoneHVACIdealLoadsAirSystem.*/
    class MODEL_API ZoneHVACIdealLoadsAirSystem_Impl : public ZoneHVACComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ZoneHVACIdealLoadsAirSystem_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ZoneHVACIdealLoadsAirSystem_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ZoneHVACIdealLoadsAirSystem_Impl(const ZoneHVACIdealLoadsAirSystem_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ZoneHVACIdealLoadsAirSystem_Impl() override = default;

      //@}

      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<Schedule> availabilitySchedule() const;

      double maximumHeatingSupplyAirTemperature() const;

      bool isMaximumHeatingSupplyAirTemperatureDefaulted() const;

      double minimumCoolingSupplyAirTemperature() const;

      bool isMinimumCoolingSupplyAirTemperatureDefaulted() const;

      double maximumHeatingSupplyAirHumidityRatio() const;

      bool isMaximumHeatingSupplyAirHumidityRatioDefaulted() const;

      double minimumCoolingSupplyAirHumidityRatio() const;

      bool isMinimumCoolingSupplyAirHumidityRatioDefaulted() const;

      std::string heatingLimit() const;

      bool isHeatingLimitDefaulted() const;

      boost::optional<double> maximumHeatingAirFlowRate() const;

      bool isMaximumHeatingAirFlowRateAutosized() const;

      boost::optional<double> maximumSensibleHeatingCapacity() const;

      bool isMaximumSensibleHeatingCapacityAutosized() const;

      std::string coolingLimit() const;

      bool isCoolingLimitDefaulted() const;

      boost::optional<double> maximumCoolingAirFlowRate() const;

      bool isMaximumCoolingAirFlowRateAutosized() const;

      boost::optional<double> maximumTotalCoolingCapacity() const;

      bool isMaximumTotalCoolingCapacityAutosized() const;

      boost::optional<Schedule> heatingAvailabilitySchedule() const;

      boost::optional<Schedule> coolingAvailabilitySchedule() const;

      std::string dehumidificationControlType() const;

      bool isDehumidificationControlTypeDefaulted() const;

      double coolingSensibleHeatRatio() const;

      bool isCoolingSensibleHeatRatioDefaulted() const;

      std::string humidificationControlType() const;

      bool isHumidificationControlTypeDefaulted() const;

      boost::optional<DesignSpecificationOutdoorAir> designSpecificationOutdoorAirObject() const;

      std::string demandControlledVentilationType() const;

      bool isDemandControlledVentilationTypeDefaulted() const;

      std::string outdoorAirEconomizerType() const;

      bool isOutdoorAirEconomizerTypeDefaulted() const;

      std::string heatRecoveryType() const;

      bool isHeatRecoveryTypeDefaulted() const;

      double sensibleHeatRecoveryEffectiveness() const;

      bool isSensibleHeatRecoveryEffectivenessDefaulted() const;

      double latentHeatRecoveryEffectiveness() const;

      bool isLatentHeatRecoveryEffectivenessDefaulted() const;

      boost::optional<double> autosizedMaximumHeatingAirFlowRate() const;

      boost::optional<double> autosizedMaximumSensibleHeatingCapacity() const;

      boost::optional<double> autosizedMaximumCoolingAirFlowRate() const;

      boost::optional<double> autosizedMaximumTotalCoolingCapacity() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& availabilitySchedule);

      void resetAvailabilitySchedule();

      bool setMaximumHeatingSupplyAirTemperature(double maximumHeatingSupplyAirTemperature);

      void resetMaximumHeatingSupplyAirTemperature();

      bool setMinimumCoolingSupplyAirTemperature(double minimumCoolingSupplyAirTemperature);

      void resetMinimumCoolingSupplyAirTemperature();

      bool setMaximumHeatingSupplyAirHumidityRatio(double maximumHeatingSupplyAirHumidityRatio);

      void resetMaximumHeatingSupplyAirHumidityRatio();

      bool setMinimumCoolingSupplyAirHumidityRatio(double minimumCoolingSupplyAirHumidityRatio);

      void resetMinimumCoolingSupplyAirHumidityRatio();

      bool setHeatingLimit(const std::string& heatingLimit);

      void resetHeatingLimit();

      bool setMaximumHeatingAirFlowRate(boost::optional<double> maximumHeatingAirFlowRate);

      void resetMaximumHeatingAirFlowRate();

      void autosizeMaximumHeatingAirFlowRate();

      bool setMaximumSensibleHeatingCapacity(boost::optional<double> maximumSensibleHeatingCapacity);

      void resetMaximumSensibleHeatingCapacity();

      void autosizeMaximumSensibleHeatingCapacity();

      bool setCoolingLimit(const std::string& coolingLimit);

      void resetCoolingLimit();

      bool setMaximumCoolingAirFlowRate(boost::optional<double> maximumCoolingAirFlowRate);

      void resetMaximumCoolingAirFlowRate();

      void autosizeMaximumCoolingAirFlowRate();

      bool setMaximumTotalCoolingCapacity(boost::optional<double> maximumTotalCoolingCapacity);

      void resetMaximumTotalCoolingCapacity();

      void autosizeMaximumTotalCoolingCapacity();

      bool setHeatingAvailabilitySchedule(Schedule& heatingAvailabilitySchedule);

      void resetHeatingAvailabilitySchedule();

      bool setCoolingAvailabilitySchedule(Schedule& coolingAvailabilitySchedule);

      void resetCoolingAvailabilitySchedule();

      bool setDehumidificationControlType(const std::string& dehumidificationControlType);

      void resetDehumidificationControlType();

      bool setCoolingSensibleHeatRatio(double coolingSensibleHeatRatio);

      void resetCoolingSensibleHeatRatio();

      bool setHumidificationControlType(const std::string& humidificationControlType);

      void resetHumidificationControlType();

      bool setDesignSpecificationOutdoorAirObject(const boost::optional<DesignSpecificationOutdoorAir>& designSpecificationOutdoorAirObject);

      void resetDesignSpecificationOutdoorAirObject();

      bool setDemandControlledVentilationType(const std::string& demandControlledVentilationType);

      void resetDemandControlledVentilationType();

      bool setOutdoorAirEconomizerType(const std::string& outdoorAirEconomizerType);

      void resetOutdoorAirEconomizerType();

      bool setHeatRecoveryType(const std::string& heatRecoveryType);

      void resetHeatRecoveryType();

      bool setSensibleHeatRecoveryEffectiveness(double sensibleHeatRecoveryEffectiveness);

      void resetSensibleHeatRecoveryEffectiveness();

      bool setLatentHeatRecoveryEffectiveness(double latentHeatRecoveryEffectiveness);

      void resetLatentHeatRecoveryEffectiveness();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ZoneHVACIdealLoadsAirSystem");

      std::vector<std::string> heatingLimitValues() const;
      std::vector<std::string> coolingLimitValues() const;
      std::vector<std::string> dehumidificationControlTypeValues() const;
      std::vector<std::string> humidificationControlTypeValues() const;
      std::vector<std::string> demandControlledVentilationTypeValues() const;
      std::vector<std::string> outdoorAirEconomizerTypeValues() const;
      std::vector<std::string> heatRecoveryTypeValues() const;
      boost::optional<ModelObject> availabilityScheduleAsModelObject() const;
      boost::optional<ModelObject> heatingAvailabilityScheduleAsModelObject() const;
      boost::optional<ModelObject> coolingAvailabilityScheduleAsModelObject() const;
      boost::optional<ModelObject> designSpecificationOutdoorAirObjectAsModelObject() const;

      bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setHeatingAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setCoolingAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setDesignSpecificationOutdoorAirObjectAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACIDEALLOADSAIRSYSTEM_IMPL_HPP
