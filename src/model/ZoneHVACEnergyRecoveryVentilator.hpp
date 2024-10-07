/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEHVACENERGYRECOVERYVENTILATOR_HPP
#define MODEL_ZONEHVACENERGYRECOVERYVENTILATOR_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class HVACComponent;
  class ZoneHVACEnergyRecoveryVentilatorController;

  namespace detail {

    class ZoneHVACEnergyRecoveryVentilator_Impl;

  }  // namespace detail

  /** ZoneHVACEnergyRecoveryVentilator is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:EnergyRecoveryVentilator'. */
  class MODEL_API ZoneHVACEnergyRecoveryVentilator : public ZoneHVACComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ZoneHVACEnergyRecoveryVentilator(const Model& model);

    explicit ZoneHVACEnergyRecoveryVentilator(const Model& model, const HVACComponent& heatExchanger, const HVACComponent& supplyAirFan,
                                              const HVACComponent& exhaustAirFan);

    virtual ~ZoneHVACEnergyRecoveryVentilator() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ZoneHVACEnergyRecoveryVentilator(const ZoneHVACEnergyRecoveryVentilator& other) = default;
    ZoneHVACEnergyRecoveryVentilator(ZoneHVACEnergyRecoveryVentilator&& other) = default;
    ZoneHVACEnergyRecoveryVentilator& operator=(const ZoneHVACEnergyRecoveryVentilator&) = default;
    ZoneHVACEnergyRecoveryVentilator& operator=(ZoneHVACEnergyRecoveryVentilator&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    HVACComponent heatExchanger() const;

    boost::optional<double> supplyAirFlowRate() const;

    bool isSupplyAirFlowRateAutosized() const;

    boost::optional<double> exhaustAirFlowRate() const;

    bool isExhaustAirFlowRateAutosized() const;

    HVACComponent supplyAirFan() const;

    HVACComponent exhaustAirFan() const;

    boost::optional<ZoneHVACEnergyRecoveryVentilatorController> controller() const;

    double ventilationRateperUnitFloorArea() const;

    double ventilationRateperOccupant() const;

    // boost::optional<ModelObject> availabilityManagerList() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setHeatExchanger(const HVACComponent& hxAirToAirSensibleAndLatent);

    bool setSupplyAirFlowRate(double supplyAirFlowRate);

    void autosizeSupplyAirFlowRate();

    bool setExhaustAirFlowRate(double exhaustAirFlowRate);

    void autosizeExhaustAirFlowRate();

    bool setSupplyAirFan(const HVACComponent& supplyAirFan);

    bool setExhaustAirFan(const HVACComponent& exhaustAirFan);

    bool setController(const ZoneHVACEnergyRecoveryVentilatorController& zoneHVACEnergyRecoveryVentilatorController);

    void resetController();

    bool setVentilationRateperUnitFloorArea(double ventilationRateperUnitFloorArea);

    bool setVentilationRateperOccupant(double ventilationRateperOccupant);

    // bool setAvailabilityManagerList(const ModelObject& systemAvailabilityManagerLists);

    // void resetAvailabilityManagerList();

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedSupplyAirFlowRate() const;

    boost::optional<double> autosizedExhaustAirFlowRate() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::ZoneHVACEnergyRecoveryVentilator_Impl;

    explicit ZoneHVACEnergyRecoveryVentilator(std::shared_ptr<detail::ZoneHVACEnergyRecoveryVentilator_Impl> impl);

    friend class detail::ZoneHVACEnergyRecoveryVentilator_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ZoneHVACEnergyRecoveryVentilator");
  };

  /** \relates ZoneHVACEnergyRecoveryVentilator*/
  using OptionalZoneHVACEnergyRecoveryVentilator = boost::optional<ZoneHVACEnergyRecoveryVentilator>;

  /** \relates ZoneHVACEnergyRecoveryVentilator*/
  using ZoneHVACEnergyRecoveryVentilatorVector = std::vector<ZoneHVACEnergyRecoveryVentilator>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACENERGYRECOVERYVENTILATOR_HPP
