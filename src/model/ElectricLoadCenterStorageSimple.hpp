/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ELECTRICLOADCENTERSTORAGESIMPLE_HPP
#define MODEL_ELECTRICLOADCENTERSTORAGESIMPLE_HPP

#include "ModelAPI.hpp"
#include "ElectricalStorage.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class ThermalZone;

  namespace detail {

    class ElectricLoadCenterStorageSimple_Impl;

  }  // namespace detail

  /** ElectricLoadCenterStorageSimple is a ElectricalStorage that wraps the OpenStudio IDD object 'OS:ElectricLoadCenter:Storage:Simple'. */
  class MODEL_API ElectricLoadCenterStorageSimple : public ElectricalStorage
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ElectricLoadCenterStorageSimple(const Model& model);

    virtual ~ElectricLoadCenterStorageSimple() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ElectricLoadCenterStorageSimple(const ElectricLoadCenterStorageSimple& other) = default;
    ElectricLoadCenterStorageSimple(ElectricLoadCenterStorageSimple&& other) = default;
    ElectricLoadCenterStorageSimple& operator=(const ElectricLoadCenterStorageSimple&) = default;
    ElectricLoadCenterStorageSimple& operator=(ElectricLoadCenterStorageSimple&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    // Convenience method to return the electricalLoadCenter on which it's assigned (optional)
    // In parent
    // boost::optional<ElectricLoadCenterDistribution> electricLoadCenterDistribution() const;

    Schedule availabilitySchedule() const;
    bool isAvailabilityScheduleDefaulted() const;

    // TODO: Included in parent class, shouldn't need to define it here...
    //virtual boost::optional<ThermalZone> thermalZone() const override;

    double radiativeFractionforZoneHeatGains() const;
    bool isRadiativeFractionforZoneHeatGainsDefaulted() const;

    double nominalEnergeticEfficiencyforCharging() const;
    bool isNominalEnergeticEfficiencyforChargingDefaulted() const;

    double nominalDischargingEnergeticEfficiency() const;
    bool isNominalDischargingEnergeticEfficiencyDefaulted() const;

    double maximumStorageCapacity() const;

    double maximumPowerforDischarging() const;

    double maximumPowerforCharging() const;

    double initialStateofCharge() const;
    bool isInitialStateofChargeDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);
    void resetAvailabilitySchedule();

    // TODO: Included in parent class, shouldn't need to define it...
    //virtual bool setThermalZone(ThermalZone& zone) override;
    //virtual void resetThermalZone() override;

    bool setRadiativeFractionforZoneHeatGains(double radiativeFractionforZoneHeatGains);
    void resetRadiativeFractionforZoneHeatGains();

    bool setNominalEnergeticEfficiencyforCharging(double nominalEnergeticEfficiencyforCharging);
    void resetNominalEnergeticEfficiencyforCharging();

    // TODO: I've requested an .IDD change in EnergyPlus, to make this "Nominal Energetic Efficiency for Discharging"
    // TODO: https://github.com/NREL/EnergyPlus/issues/5730
    bool setNominalDischargingEnergeticEfficiency(double nominalDischargingEnergeticEfficiency);
    void resetNominalDischargingEnergeticEfficiency();

    bool setMaximumStorageCapacity(double maximumStorageCapacity);

    bool setMaximumPowerforDischarging(double maximumPowerforDischarging);

    bool setMaximumPowerforCharging(double maximumPowerforCharging);

    bool setInitialStateofCharge(double initialStateofCharge);
    void resetInitialStateofCharge();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::ElectricLoadCenterStorageSimple_Impl;

    explicit ElectricLoadCenterStorageSimple(std::shared_ptr<detail::ElectricLoadCenterStorageSimple_Impl> impl);

    friend class detail::ElectricLoadCenterStorageSimple_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ElectricLoadCenterStorageSimple");
  };

  /** \relates ElectricLoadCenterStorageSimple*/
  using OptionalElectricLoadCenterStorageSimple = boost::optional<ElectricLoadCenterStorageSimple>;

  /** \relates ElectricLoadCenterStorageSimple*/
  using ElectricLoadCenterStorageSimpleVector = std::vector<ElectricLoadCenterStorageSimple>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ELECTRICLOADCENTERSTORAGESIMPLE_HPP
