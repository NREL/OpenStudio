/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONECONTROLTHERMOSTATSTAGEDDUALSETPOINT_HPP
#define MODEL_ZONECONTROLTHERMOSTATSTAGEDDUALSETPOINT_HPP

#include "ModelAPI.hpp"
#include "Thermostat.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  namespace detail {

    class ZoneControlThermostatStagedDualSetpoint_Impl;

  }  // namespace detail

  /** ZoneControlThermostatStagedDualSetpoint is a ModelObject that wraps the OpenStudio IDD object 'OS:ZoneControl:Thermostat:StagedDualSetpoint'. */
  class MODEL_API ZoneControlThermostatStagedDualSetpoint : public Thermostat
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ZoneControlThermostatStagedDualSetpoint(const Model& model);

    virtual ~ZoneControlThermostatStagedDualSetpoint() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ZoneControlThermostatStagedDualSetpoint(const ZoneControlThermostatStagedDualSetpoint& other) = default;
    ZoneControlThermostatStagedDualSetpoint(ZoneControlThermostatStagedDualSetpoint&& other) = default;
    ZoneControlThermostatStagedDualSetpoint& operator=(const ZoneControlThermostatStagedDualSetpoint&) = default;
    ZoneControlThermostatStagedDualSetpoint& operator=(ZoneControlThermostatStagedDualSetpoint&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    int numberofHeatingStages() const;

    boost::optional<Schedule> heatingTemperatureSetpointSchedule() const;

    double heatingThrottlingTemperatureRange() const;

    double stage1HeatingTemperatureOffset() const;

    double stage2HeatingTemperatureOffset() const;

    double stage3HeatingTemperatureOffset() const;

    double stage4HeatingTemperatureOffset() const;

    int numberofCoolingStages() const;

    boost::optional<Schedule> coolingTemperatureSetpointBaseSchedule() const;

    double coolingThrottlingTemperatureRange() const;

    double stage1CoolingTemperatureOffset() const;

    double stage2CoolingTemperatureOffset() const;

    double stage3CoolingTemperatureOffset() const;

    double stage4CoolingTemperatureOffset() const;

    //@}
    /** @name Setters */
    //@{

    bool setNumberofHeatingStages(int numberofHeatingStages);

    bool setHeatingTemperatureSetpointSchedule(Schedule& schedule);

    void resetHeatingTemperatureSetpointSchedule();

    bool setHeatingThrottlingTemperatureRange(double heatingThrottlingTemperatureRange);

    bool setStage1HeatingTemperatureOffset(double stage1HeatingTemperatureOffset);

    bool setStage2HeatingTemperatureOffset(double stage2HeatingTemperatureOffset);

    bool setStage3HeatingTemperatureOffset(double stage3HeatingTemperatureOffset);

    bool setStage4HeatingTemperatureOffset(double stage4HeatingTemperatureOffset);

    bool setNumberofCoolingStages(int numberofCoolingStages);

    bool setCoolingTemperatureSetpointBaseSchedule(Schedule& schedule);

    void resetCoolingTemperatureSetpointBaseSchedule();

    bool setCoolingThrottlingTemperatureRange(double coolingThrottlingTemperatureRange);

    bool setStage1CoolingTemperatureOffset(double stage1CoolingTemperatureOffset);

    bool setStage2CoolingTemperatureOffset(double stage2CoolingTemperatureOffset);

    bool setStage3CoolingTemperatureOffset(double stage3CoolingTemperatureOffset);

    bool setStage4CoolingTemperatureOffset(double stage4CoolingTemperatureOffset);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::ZoneControlThermostatStagedDualSetpoint_Impl;

    explicit ZoneControlThermostatStagedDualSetpoint(std::shared_ptr<detail::ZoneControlThermostatStagedDualSetpoint_Impl> impl);

    friend class detail::ZoneControlThermostatStagedDualSetpoint_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ZoneControlThermostatStagedDualSetpoint");
  };

  /** \relates ZoneControlThermostatStagedDualSetpoint*/
  using OptionalZoneControlThermostatStagedDualSetpoint = boost::optional<ZoneControlThermostatStagedDualSetpoint>;

  /** \relates ZoneControlThermostatStagedDualSetpoint*/
  using ZoneControlThermostatStagedDualSetpointVector = std::vector<ZoneControlThermostatStagedDualSetpoint>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONECONTROLTHERMOSTATSTAGEDDUALSETPOINT_HPP
