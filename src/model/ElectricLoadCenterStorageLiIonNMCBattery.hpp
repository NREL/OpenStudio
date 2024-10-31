/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ELECTRICLOADCENTERSTORAGELIIONNMCBATTERY_HPP
#define MODEL_ELECTRICLOADCENTERSTORAGELIIONNMCBATTERY_HPP

#include "ModelAPI.hpp"
#include "ElectricalStorage.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class ThermalZone;

  namespace detail {

    class ElectricLoadCenterStorageLiIonNMCBattery_Impl;

  }  // namespace detail

  /** ElectricLoadCenterStorageLiIonNMCBattery is a ElectricalStorage that wraps the OpenStudio IDD object 'OS:ElectricLoadCenter:Storage:LiIonNMCBattery'. */
  class MODEL_API ElectricLoadCenterStorageLiIonNMCBattery : public ElectricalStorage
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ElectricLoadCenterStorageLiIonNMCBattery(const Model& model);

    explicit ElectricLoadCenterStorageLiIonNMCBattery(const Model& model, const int numberofCellsinSeries, const int numberofStringsinParallel,
                                                      const double batteryMass, const double batterySurfaceArea);

    virtual ~ElectricLoadCenterStorageLiIonNMCBattery() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ElectricLoadCenterStorageLiIonNMCBattery(const ElectricLoadCenterStorageLiIonNMCBattery& other) = default;
    ElectricLoadCenterStorageLiIonNMCBattery(ElectricLoadCenterStorageLiIonNMCBattery&& other) = default;
    ElectricLoadCenterStorageLiIonNMCBattery& operator=(const ElectricLoadCenterStorageLiIonNMCBattery&) = default;
    ElectricLoadCenterStorageLiIonNMCBattery& operator=(ElectricLoadCenterStorageLiIonNMCBattery&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> lifetimeModelValues();

    /** @name Getters */
    //@{

    // Convenience method to return the electricalLoadCenter on which it's assigned (optional)
    // In parent
    // boost::optional<ElectricLoadCenterDistribution> electricLoadCenterDistribution() const;

    Schedule availabilitySchedule() const;

    // Included in parent class, shouldn't need to define it here...
    //virtual boost::optional<ThermalZone> thermalZone() const override;

    double radiativeFraction() const;

    std::string lifetimeModel() const;

    int numberofCellsinSeries() const;

    int numberofStringsinParallel() const;

    double initialFractionalStateofCharge() const;

    double dctoDCChargingEfficiency() const;

    double batteryMass() const;

    double batterySurfaceArea() const;

    double batterySpecificHeatCapacity() const;

    double heatTransferCoefficientBetweenBatteryandAmbient() const;

    double fullyChargedCellVoltage() const;

    double cellVoltageatEndofExponentialZone() const;

    double cellVoltageatEndofNominalZone() const;

    double defaultNominalCellVoltage() const;

    double fullyChargedCellCapacity() const;

    double fractionofCellCapacityRemovedattheEndofExponentialZone() const;

    double fractionofCellCapacityRemovedattheEndofNominalZone() const;

    double chargeRateatWhichVoltagevsCapacityCurveWasGenerated() const;

    double batteryCellInternalElectricalResistance() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    // Included in parent class, shouldn't need to define it...
    //virtual bool setThermalZone(ThermalZone& zone) override;
    //virtual void resetThermalZone() override;

    bool setRadiativeFraction(double radiativeFraction);

    bool setLifetimeModel(const std::string& lifetimeModel);

    bool setNumberofCellsinSeries(int numberofCellsinSeries);

    bool setNumberofStringsinParallel(int numberofStringsinParallel);

    bool setInitialFractionalStateofCharge(double initialFractionalStateofCharge);

    bool setDCtoDCChargingEfficiency(double dctoDCChargingEfficiency);

    bool setBatteryMass(double batteryMass);

    bool setBatterySurfaceArea(double batterySurfaceArea);

    bool setBatterySpecificHeatCapacity(double batterySpecificHeatCapacity);

    bool setHeatTransferCoefficientBetweenBatteryandAmbient(double heatTransferCoefficientBetweenBatteryandAmbient);

    bool setFullyChargedCellVoltage(double fullyChargedCellCapacity);

    bool setCellVoltageatEndofExponentialZone(double cellVoltageatEndofExponentialZone);

    bool setCellVoltageatEndofNominalZone(double cellVoltageatEndofNominalZone);

    bool setDefaultNominalCellVoltage(double defaultNominalCellVoltage);

    bool setFullyChargedCellCapacity(double fullyChargedCellCapacity);

    bool setFractionofCellCapacityRemovedattheEndofExponentialZone(double fractionofCellCapacityRemovedattheEndofExponentialZone);

    bool setFractionofCellCapacityRemovedattheEndofNominalZone(double fractionofCellCapacityRemovedattheEndofNominalZone);

    bool setChargeRateatWhichVoltagevsCapacityCurveWasGenerated(double chargeRateatWhichVoltagevsCapacityCurveWasGenerated);

    bool setBatteryCellInternalElectricalResistance(double batteryCellInternalElectricalResistance);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl;

    explicit ElectricLoadCenterStorageLiIonNMCBattery(std::shared_ptr<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl> impl);

    friend class detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ElectricLoadCenterStorageLiIonNMCBattery");
  };

  /** \relates ElectricLoadCenterStorageLiIonNMCBattery*/
  using OptionalElectricLoadCenterStorageLiIonNMCBattery = boost::optional<ElectricLoadCenterStorageLiIonNMCBattery>;

  /** \relates ElectricLoadCenterStorageLiIonNMCBattery*/
  using ElectricLoadCenterStorageLiIonNMCBatteryVector = std::vector<ElectricLoadCenterStorageLiIonNMCBattery>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ELECTRICLOADCENTERSTORAGELIIONNMCBATTERY_HPP
