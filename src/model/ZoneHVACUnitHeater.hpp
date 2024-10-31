/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEHVACUNITHEATER_HPP
#define MODEL_ZONEHVACUNITHEATER_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class HVACComponent;

  namespace detail {

    class ZoneHVACUnitHeater_Impl;

  }  // namespace detail

  /** ZoneHVACUnitHeater is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:UnitHeater'. */

  class MODEL_API ZoneHVACUnitHeater : public ZoneHVACComponent
  {
   public:
    /** @name Constructors and Destructors */

    ZoneHVACUnitHeater(const Model& model, Schedule& availabilitySchedule, HVACComponent& supplyAirFan, HVACComponent& heatingCoil);

    virtual ~ZoneHVACUnitHeater() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ZoneHVACUnitHeater(const ZoneHVACUnitHeater& other) = default;
    ZoneHVACUnitHeater(ZoneHVACUnitHeater&& other) = default;
    ZoneHVACUnitHeater& operator=(const ZoneHVACUnitHeater&) = default;
    ZoneHVACUnitHeater& operator=(ZoneHVACUnitHeater&&) = default;

    static IddObjectType iddObjectType();

    static std::vector<std::string> fanControlTypeValues();

    /** @name Getters */

    Schedule availabilitySchedule() const;

    HVACComponent supplyAirFan() const;

    boost::optional<double> maximumSupplyAirFlowRate() const;

    bool isMaximumSupplyAirFlowRateAutosized() const;

    /** In EnergyPlus 8.2.0 and above this property maps to the EnergyPlus field "Supply Air Fan Operation During No Heating" **/
    std::string fanControlType() const;

    HVACComponent heatingCoil() const;

    boost::optional<double> maximumHotWaterFlowRate() const;

    bool isMaximumHotWaterFlowRateAutosized() const;

    double minimumHotWaterFlowRate() const;

    bool isMinimumHotWaterFlowRateDefaulted() const;

    double heatingConvergenceTolerance() const;

    bool isHeatingConvergenceToleranceDefaulted() const;

    /** @name Setters */

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setSupplyAirFan(const HVACComponent& fan);

    bool setMaximumSupplyAirFlowRate(double maximumSupplyAirFlowRate);

    void autosizeMaximumSupplyAirFlowRate();

    bool setFanControlType(const std::string& fanControlType);

    bool setHeatingCoil(const HVACComponent& heatingCoil);

    bool setMaximumHotWaterFlowRate(double maximumHotWaterFlowRate);

    void resetMaximumHotWaterFlowRate();

    void autosizeMaximumHotWaterFlowRate();

    bool setMinimumHotWaterFlowRate(double minimumHotWaterFlowRate);

    void resetMinimumHotWaterFlowRate();

    bool setHeatingConvergenceTolerance(double heatingConvergenceTolerance);

    void resetHeatingConvergenceTolerance();

    /** @name Other */

    boost::optional<double> autosizedMaximumSupplyAirFlowRate() const;

    boost::optional<double> autosizedMaximumHotWaterFlowRate() const;

   protected:
    /// @cond
    using ImplType = detail::ZoneHVACUnitHeater_Impl;

    explicit ZoneHVACUnitHeater(std::shared_ptr<detail::ZoneHVACUnitHeater_Impl> impl);

    friend class detail::ZoneHVACUnitHeater_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond

   private:
    REGISTER_LOGGER("openstudio.model.ZoneHVACUnitHeater");
  };

  /** \relates ZoneHVACUnitHeater*/
  using OptionalZoneHVACUnitHeater = boost::optional<ZoneHVACUnitHeater>;

  /** \relates ZoneHVACUnitHeater*/
  using ZoneHVACUnitHeaterVector = std::vector<ZoneHVACUnitHeater>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACUNITHEATER_HPP
