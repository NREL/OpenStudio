/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEHVACUNITVENTILATOR_HPP
#define MODEL_ZONEHVACUNITVENTILATOR_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class HVACComponent;

  namespace detail {

    class ZoneHVACUnitVentilator_Impl;

  }  // namespace detail

  /** ZoneHVACUnitVentilator is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:UnitVentilator'. */
  class MODEL_API ZoneHVACUnitVentilator : public ZoneHVACComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ZoneHVACUnitVentilator(const Model& model);

    explicit ZoneHVACUnitVentilator(const Model& model, const HVACComponent& supplyAirFan);

    virtual ~ZoneHVACUnitVentilator() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ZoneHVACUnitVentilator(const ZoneHVACUnitVentilator& other) = default;
    ZoneHVACUnitVentilator(ZoneHVACUnitVentilator&& other) = default;
    ZoneHVACUnitVentilator& operator=(const ZoneHVACUnitVentilator&) = default;
    ZoneHVACUnitVentilator& operator=(ZoneHVACUnitVentilator&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> outdoorAirControlTypeValues();

    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    boost::optional<double> maximumSupplyAirFlowRate() const;

    bool isMaximumSupplyAirFlowRateAutosized() const;

    std::string outdoorAirControlType() const;

    boost::optional<double> minimumOutdoorAirFlowRate() const;

    bool isMinimumOutdoorAirFlowRateAutosized() const;

    Schedule minimumOutdoorAirSchedule() const;

    boost::optional<double> maximumOutdoorAirFlowRate() const;

    bool isMaximumOutdoorAirFlowRateAutosized() const;

    Schedule maximumOutdoorAirFractionorTemperatureSchedule() const;

    HVACComponent supplyAirFan() const;

    boost::optional<Schedule> supplyAirFanOperatingModeSchedule() const;

    boost::optional<HVACComponent> heatingCoil() const;

    double heatingConvergenceTolerance() const;

    boost::optional<HVACComponent> coolingCoil() const;

    double coolingConvergenceTolerance() const;

    // boost::optional<ModelObject> availabilityManagerList() const;

    // boost::optional<ModelObject> designSpecificationZoneHVACSizingObject() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setMaximumSupplyAirFlowRate(double maximumSupplyAirFlowRate);

    void autosizeMaximumSupplyAirFlowRate();

    bool setOutdoorAirControlType(const std::string& outdoorAirControlType);

    bool setMinimumOutdoorAirFlowRate(double minimumOutdoorAirFlowRate);

    void autosizeMinimumOutdoorAirFlowRate();

    bool setMinimumOutdoorAirSchedule(Schedule& schedule);

    bool setMaximumOutdoorAirFlowRate(double maximumOutdoorAirFlowRate);

    void autosizeMaximumOutdoorAirFlowRate();

    bool setMaximumOutdoorAirFractionorTemperatureSchedule(Schedule& schedule);

    bool setSupplyAirFan(const HVACComponent& supplyAirFan);

    bool setSupplyAirFanOperatingModeSchedule(Schedule& schedule);

    void resetSupplyAirFanOperatingModeSchedule();

    bool setHeatingCoil(const HVACComponent& heatingCoil);

    void resetHeatingCoil();

    bool setHeatingConvergenceTolerance(double heatingConvergenceTolerance);

    bool setCoolingCoil(const HVACComponent& coolingCoil);

    void resetCoolingCoil();

    bool setCoolingConvergenceTolerance(double coolingConvergenceTolerance);

    // bool setAvailabilityManagerList(const ModelObject& systemAvailabilityManagerLists);

    // void resetAvailabilityManagerList();

    // bool setDesignSpecificationZoneHVACSizingObject(const ModelObject& designSpecificationZoneHVACSizingName);

    // void resetDesignSpecificationZoneHVACSizingObject();

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedMaximumSupplyAirFlowRate() const;

    boost::optional<double> autosizedMinimumOutdoorAirFlowRate() const;

    boost::optional<double> autosizedMaximumOutdoorAirFlowRate() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::ZoneHVACUnitVentilator_Impl;

    explicit ZoneHVACUnitVentilator(std::shared_ptr<detail::ZoneHVACUnitVentilator_Impl> impl);

    friend class detail::ZoneHVACUnitVentilator_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ZoneHVACUnitVentilator");
  };

  /** \relates ZoneHVACUnitVentilator*/
  using OptionalZoneHVACUnitVentilator = boost::optional<ZoneHVACUnitVentilator>;

  /** \relates ZoneHVACUnitVentilator*/
  using ZoneHVACUnitVentilatorVector = std::vector<ZoneHVACUnitVentilator>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACUNITVENTILATOR_HPP
