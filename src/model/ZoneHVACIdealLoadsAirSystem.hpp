/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEHVACIDEALLOADSAIRSYSTEM_HPP
#define MODEL_ZONEHVACIDEALLOADSAIRSYSTEM_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class DesignSpecificationOutdoorAir;

  namespace detail {

    class ZoneHVACIdealLoadsAirSystem_Impl;

  }  // namespace detail

  /** ZoneHVACIdealLoadsAirSystem is a ZoneHVACComponent that wraps the OpenStudio IDD
 *  object 'OS:ZoneHVAC:IdealLoadsAirSystem'. */
  class MODEL_API ZoneHVACIdealLoadsAirSystem : public ZoneHVACComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ZoneHVACIdealLoadsAirSystem(const Model& model);

    virtual ~ZoneHVACIdealLoadsAirSystem() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ZoneHVACIdealLoadsAirSystem(const ZoneHVACIdealLoadsAirSystem& other) = default;
    ZoneHVACIdealLoadsAirSystem(ZoneHVACIdealLoadsAirSystem&& other) = default;
    ZoneHVACIdealLoadsAirSystem& operator=(const ZoneHVACIdealLoadsAirSystem&) = default;
    ZoneHVACIdealLoadsAirSystem& operator=(ZoneHVACIdealLoadsAirSystem&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> heatingLimitValues();

    static std::vector<std::string> coolingLimitValues();

    static std::vector<std::string> dehumidificationControlTypeValues();

    static std::vector<std::string> humidificationControlTypeValues();

    static std::vector<std::string> demandControlledVentilationTypeValues();

    static std::vector<std::string> outdoorAirEconomizerTypeValues();

    static std::vector<std::string> heatRecoveryTypeValues();

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

    bool setMaximumHeatingAirFlowRate(double maximumHeatingAirFlowRate);

    void resetMaximumHeatingAirFlowRate();

    void autosizeMaximumHeatingAirFlowRate();

    bool setMaximumSensibleHeatingCapacity(double maximumSensibleHeatingCapacity);

    void resetMaximumSensibleHeatingCapacity();

    void autosizeMaximumSensibleHeatingCapacity();

    bool setCoolingLimit(const std::string& coolingLimit);

    void resetCoolingLimit();

    bool setMaximumCoolingAirFlowRate(double maximumCoolingAirFlowRate);

    void resetMaximumCoolingAirFlowRate();

    void autosizeMaximumCoolingAirFlowRate();

    bool setMaximumTotalCoolingCapacity(double maximumTotalCoolingCapacity);

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

    bool setDesignSpecificationOutdoorAirObject(const DesignSpecificationOutdoorAir& designSpecificationOutdoorAirObject);

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

    boost::optional<double> autosizedMaximumHeatingAirFlowRate() const;

    boost::optional<double> autosizedMaximumSensibleHeatingCapacity() const;

    boost::optional<double> autosizedMaximumCoolingAirFlowRate() const;

    boost::optional<double> autosizedMaximumTotalCoolingCapacity() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::ZoneHVACIdealLoadsAirSystem_Impl;

    explicit ZoneHVACIdealLoadsAirSystem(std::shared_ptr<detail::ZoneHVACIdealLoadsAirSystem_Impl> impl);

    friend class detail::ZoneHVACIdealLoadsAirSystem_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ZoneHVACIdealLoadsAirSystem");
  };

  /** \relates ZoneHVACIdealLoadsAirSystem*/
  using OptionalZoneHVACIdealLoadsAirSystem = boost::optional<ZoneHVACIdealLoadsAirSystem>;

  /** \relates ZoneHVACIdealLoadsAirSystem*/
  using ZoneHVACIdealLoadsAirSystemVector = std::vector<ZoneHVACIdealLoadsAirSystem>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACIDEALLOADSAIRSYSTEM_HPP
